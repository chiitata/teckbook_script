== Supabase Authの概要

この章では、アプリケーションにユーザー認証機能を導入します。誰がどの動画をお気に入りに登録したのかを管理するためには、「ユーザー」という概念が不可欠です。Supabaseが提供する認証サービス「Supabase Auth」を利用することで、高機能な認証システムを驚くほど簡単に構築できます。

=== Supabase Authとは？

Supabase Authは、メールアドレスとパスワードによる認証はもちろん、GoogleやGitHubといったソーシャルログイン（OAuth）にも対応した、フル機能の認証サービスです。

Supabase Authを利用すると、以下のような機能がすぐに使えるようになります。

 * **サインアップ（ユーザー登録）**
 * **ログイン・ログアウト**
 * **パスワードリセット**
 * **ソーシャルログイン（Google, GitHub, etc.）**
 * **セキュアなセッション管理（JWT）**

これらの機能を自前で実装するのは非常に大変ですが、Supabaseを使えば数行のコードで実現できてしまいます。

=== 認証とデータベースの連携

Supabase Authの大きな特徴は、認証システムがデータベースと密に連携している点です。

ユーザーがサインアップすると、Supabaseの`auth.users`という特別なテーブルにそのユーザーの情報が自動的に保存されます。この`auth.users`テーブルには、ユーザーIDやメールアドレスなどが格納されます。

#image[supabase-auth-users][auth.usersテーブル]{
#}

さらに、このユーザー情報と、自分たちが作成するテーブル（例えば、ユーザーのプロフィール情報を保存する`profiles`テーブル）を連携させることができます。

そして、第6章で詳しく解説するRLS（Row Level Security）というPostgreSQLの機能と組み合わせることで、「自分のデータは自分しか操作できない」といった、きめ細やかなアクセスコントロールをデータベースレベルで実現できます。これがSupabase Authの最も強力な点です。

この章では、まずSupabase Authを使って基本的なサインアップ・ログイン機能を実装し、ユーザーが自分の情報を登録できるようにするところまでを目指します。
== データベースの準備: `profiles`テーブルの作成

Supabase Authが管理する`auth.users`テーブルには、メールアドレスなどの認証に必要な最低限の情報しか含まれていません。アプリケーションで利用するユーザー名やアバター画像といった追加情報を保存するために、`profiles`という新しいテーブルを自分たちで作成します。

=== テーブルの設計

`profiles`テーブルには、以下のカラム（列）を持たせることにします。

 * **`id`**: ユーザーを一意に識別するためのID。`auth.users`テーブルの`id`と連携させるため、`UUID`型とし、主キーに設定します。
 * **`username`**: ユーザー名。`text`型とします。
 * **`avatar_url`**: アバター画像のURL。`text`型とします。
 * **`updated_at`**: レコードの最終更新日時。`timestamp with time zone`型とします。

`id`カラムは、`auth.users`テーブルの`id`カラムを外部キーとして参照するように設定します。これにより、`auth.users`に存在しないユーザーのプロフィールが作られることを防ぎ、データの整合性を保つことができます。

=== SQLエディタによるテーブル作成

Supabaseのダッシュボードには、SQLを直接実行できる「SQL Editor」という機能があります。これを使って`profiles`テーブルを作成しましょう。

 1. Supabaseプロジェクトのダッシュボードで、左側メニューから「SQL Editor」を選択します。
 2. 「New query」をクリックします。
 3. 以下のSQLクエリをエディタに貼り付けます。

```sql
-- profilesテーブルを作成
create table public.profiles (
  id uuid not null primary key,
  username text,
  avatar_url text,
  updated_at timestamp with time zone,
  constraint profiles_id_fkey foreign key (id) references auth.users (id) on delete cascade
);

-- profilesテーブルへのアクセス権限を設定
alter table public.profiles enable row level security;

create policy "Public profiles are viewable by everyone." on public.profiles
  for select using (true);

create policy "Users can insert their own profile." on public.profiles
  for insert with check (auth.uid() = id);

create policy "Users can update own profile." on public.profiles
  for update using (auth.uid() = id);
```

 4. 「RUN」ボタンをクリックしてクエリを実行します。

#image[supabase-sql-editor][SQL Editorでのテーブル作成]{
#}

=== SQLクエリの解説

実行したSQLクエリは、大きく2つの部分に分かれています。

**1. テーブル作成 (`create table`)**

`public.profiles`という名前でテーブルを作成しています。`id`カラムを主キー（`primary key`）に設定し、`auth.users`テーブルの`id`を外部キー（`foreign key`）として参照するように制約（`constraint`）を設けています。`on delete cascade`は、`auth.users`からユーザーが削除された場合に、関連する`profiles`のレコードも自動的に削除するための設定です。

**2. アクセス権限の設定（RLSポリシー）**

ここがSupabaseの強力な部分です。`alter table ... enable row level security`で、`profiles`テーブルに対してRLS（Row Level Security）を有効にしています。

RLSを有効にすると、デフォルトでは誰もこのテーブルのデータにアクセスできなくなります。データにアクセスするためには、「ポリシー（Policy）」を定義する必要があります。

 * **`Public profiles are viewable by everyone.`**: `select`（読み取り）操作は、誰でも（`using (true)`）許可する、というポリシーです。
 * **`Users can insert their own profile.`**: `insert`（書き込み）操作は、これから書き込もうとしているデータの`id`と、現在ログインしているユーザーのID（`auth.uid()`）が一致する場合にのみ許可する、というポリシーです。
 * **`Users can update own profile.`**: `update`（更新）操作も同様に、更新しようとしているデータの`id`が、ログイン中のユーザーIDと一致する場合にのみ許可します。

これにより、「自分のプロフィールは自分しか作成・更新できないが、誰でも見ることはできる」というセキュリティルールを、データベースレベルで強制することができます。

これで、ユーザー情報を格納するための器が準備できました。次のセクションでは、実際にユーザーがサインアップ・ログインできるUIを実装していきます。
== サインアップとログイン機能の実装

データベースの準備ができたので、ユーザーが実際にサインアップ（ユーザー登録）とログインを行えるUIを実装します。Supabaseは、認証機能のUIを簡単に実装するためのライブラリ`@supabase/auth-ui-react`を提供しており、これを利用します。

=== 必要なライブラリのインストール

まずは、SupabaseのJavaScriptクライアントと、認証UIライブラリをインストールします。

```sh
$ npm install @supabase/supabase-js @supabase/auth-ui-react @supabase/auth-helpers-nextjs
```

 * **`@supabase/supabase-js`**: Supabase APIと通信するための基本的なクライアントライブラリです。
 * **`@supabase/auth-ui-react`**: 定義済みの認証UIコンポーネントを提供します。
 * **`@supabase/auth-helpers-nextjs`**: Next.js環境でSupabaseの認証を扱うためのヘルパー関数を提供します。

=== Supabaseクライアントの作成

Next.jsアプリケーションのどこからでもSupabaseクライアントを呼び出せるように、クライアントを作成するファイルを用意します。`src/lib`ディレクトリに`supabase.ts`というファイルを作成してください。

```ts
# src/lib/supabase.ts
import { createPagesBrowserClient } from '@supabase/auth-helpers-nextjs'

export const supabase = createPagesBrowserClient()
```

`@supabase/auth-helpers-nextjs`が提供する`createPagesBrowserClient`は、ブラウザ環境で動作するSupabaseクライアントを作成するための関数です。環境変数（`NEXT_PUBLIC_SUPABASE_URL`と`NEXT_PUBLIC_SUPABASE_ANON_KEY`）を自動的に読み込んでクライアントを初期化してくれます。

=== ログインページの作成

次に、ログインフォームを表示するためのページを作成します。`src/app`ディレクトリに`login`というフォルダを作成し、その中に`page.tsx`を作成してください。

```tsx
# src/app/login/page.tsx
"use client"

import { supabase } from "@/lib/supabase"
import { Auth } from "@supabase/auth-ui-react"
import { ThemeSupa } from "@supabase/auth-ui-shared"

const LoginPage = () => {
  return (
    <div className="max-w-md mx-auto mt-8">
      <Auth
        supabaseClient={supabase}
        appearance={{ theme: ThemeSupa }}
        providers={['google', 'github']}
        localization={{
          variables: {
            sign_in: {
              email_label: 'メールアドレス',
              password_label: 'パスワード',
              button_label: 'ログイン',
              # ... 他のラベルも日本語化可能
            },
            sign_up: {
              email_label: 'メールアドレス',
              password_label: 'パスワード',
              button_label: '新規登録',
            },
          },
        }}
      />
    </div>
  )
}

export default LoginPage
```

`@supabase/auth-ui-react`から`Auth`コンポーネントをインポートし、`supabaseClient` propsに先ほど作成したSupabaseクライアントを渡すだけで、高機能な認証フォームが完成します。

 * **`appearance`**: `ThemeSupa`を渡すことで、Supabase公式のデザインテーマを適用できます。
 * **`providers`**: メールアドレス・パスワード認証に加えて、ソーシャルログインを有効にする場合に指定します。ここではGoogleとGitHubを有効にしています。（別途Supabaseのダッシュボードでの設定が必要です）
 * **`localization`**: UIの文言をカスタマイズします。ここでは一部を日本語化しています。


=== ヘッダーにリンクを追加

作成したログインページへのリンクを、共通ヘッダーに追加しましょう。`src/components/Header.tsx`を修正します。

```tsx
# src/components/Header.tsx
import Link from 'next/link'

const Header = () => {
  return (
    <header className="bg-gray-800 p-4">
      <nav className="container mx-auto flex justify-between items-center">
        {/* ... */}
        <ul className="flex space-x-4">
          <li>
            <Link href="/" className="text-gray-300 hover:text-white">
              Home
            </Link>
          </li>
          <li>
            <Link href="/login" className="text-gray-300 hover:text-white">
              Login
            </Link>
          </li>
        </ul>
      </nav>
    </header>
  )
}

export default Header
```

これで、ヘッダーの「Login」リンクからログインページにアクセスできるようになりました。`http://localhost:3000/login`にアクセスして、サインアップやログインが試せることを確認してください。

#image[supabase-auth-ui][Supabase Auth UIが表示されたログインページ]{
#}

メールアドレスとパスワードで新規登録を行うと、Supabaseから確認メールが送信されます。メール内のリンクをクリックして認証を完了させると、ログインできるようになります。

次のセクションでは、このログイン状態をアプリケーション全体で管理し、UIに反映させる方法を学びます。
== ログイン状態の管理とUIの動的変更

ログイン機能は実装できましたが、現状ではログインしてもUIに変化がありません。このセクションでは、ユーザーのログイン状態をアプリケーション全体で把握し、「ログイン中はログアウトボタンを表示する」といった動的なUIの変更を実装します。

=== 認証状態の変更を検知する

Supabaseクライアントは、`onAuthStateChange`というイベントリスナーを提供しており、これを使うことでサインインやサインアウトといった認証状態の変化をリアルタイムに検知できます。

この仕組みを利用して、ログイン状態を管理するカスタムフックを作成しましょう。`src/hooks`というディレクトリを作成し、その中に`useAuth.ts`ファイルを作成します。

```ts
# src/hooks/useAuth.ts
"use client"

import { useState, useEffect } from 'react'
import { supabase } from '@/lib/supabase'
import type { Session } from '@supabase/supabase-js'

export const useAuth = () => {
  const [session, setSession] = useState<Session | null>(null)

  useEffect(() => {
    # 初回レンダリング時に現在のセッションを取得
    supabase.auth.getSession().then(({ data: { session } }) => {
      setSession(session)
    })

    # 認証状態の変化を監視
    const { data: authListener } = supabase.auth.onAuthStateChange(
      (_event, session) => {
        setSession(session)
      }
    )

    # クリーンアップ関数
    return () => {
      authListener.subscription.unsubscribe()
    }
  }, [])

  return { session }
}
```

この`useAuth`フックは、以下の処理を行っています。

1.  `useState`で`session`というstateを定義します。`session`オブジェクトには、ログインしているユーザーの情報などが含まれます。ログインしていない場合は`null`です。
2.  `useEffect`を使って、コンポーネントのマウント時に一度だけ処理を実行します。
3.  `supabase.auth.getSession()`で現在のセッション情報を取得し、stateを更新します。
4.  `supabase.auth.onAuthStateChange`で認証状態の変化を監視し、変化があれば（ログイン、ログアウトなど）、その都度`session` stateを更新します。
5.  `useEffect`のクリーンアップ関数で、コンポーネントがアンマウントされるときにイベントリスナーを解除（`unsubscribe`）します。これにより、メモリリークを防ぎます。

このフックを使うことで、どのコンポーネントからでも簡単に現在のログイン状態（`session`）を取得できるようになります。

=== ヘッダーの動的変更

作成した`useAuth`フックを使って、ヘッダーの表示をログイン状態に応じて切り替えてみましょう。`src/components/Header.tsx`を修正します。

```tsx
# src/components/Header.tsx
"use client" // フックを使うためClient Componentに変更

import Link from 'next/link'
import { useAuth } from '@/hooks/useAuth'
import { supabase } from '@/lib/supabase'

const Header = () => {
  const { session } = useAuth()

  const handleLogout = async () => {
    await supabase.auth.signOut()
  }

  return (
    <header className="bg-gray-800 p-4">
      <nav className="container mx-auto flex justify-between items-center">
        {/* ... */}
        <ul className="flex items-center space-x-4">
          <li>
            <Link href="/" className="text-gray-300 hover:text-white">
              Home
            </Link>
          </li>
          {session ? (
            <>
              <li>
                <Link href="/mypage" className="text-gray-300 hover:text-white">
                  マイページ
                </Link>
              </li>
              <li>
                <button onClick={handleLogout} className="text-gray-300 hover:text-white">
                  ログアウト
                </button>
              </li>
            </>
          ) : (
            <li>
              <Link href="/login" className="text-gray-300 hover:text-white">
                ログイン
              </Link>
            </li>
          )}
        </ul>
      </nav>
    </header>
  )
}

export default Header
```

**主な変更点:**

1.  `useAuth`フックを使うため、ファイルの先頭に`"use client"`を追加します。
2.  `useAuth()`を呼び出して`session`オブジェクトを取得します。
3.  `supabase.auth.signOut()`を実行する`handleLogout`関数を定義します。
4.  JSXの部分で、`session`が存在するかどうかで条件分岐させます。
    *   `session`があれば（ログイン中）、「マイページ」へのリンクと「ログアウト」ボタンを表示します。
    *   `session`がなければ（ログアウト中）、「ログイン」リンクを表示します。

これで、ログイン状態に応じてヘッダーの表示が切り替わるようになりました。ログインページでログインやログアウトを試すと、ヘッダーの表示がリアルタイムに変化することを確認してください。

これで第5章は完了です。Supabaseを使った認証機能の基本をマスターしました。次の章では、いよいよ本書のメイン機能である「お気に入り機能」を実装します。
