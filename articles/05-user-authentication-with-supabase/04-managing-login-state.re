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
