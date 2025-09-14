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
