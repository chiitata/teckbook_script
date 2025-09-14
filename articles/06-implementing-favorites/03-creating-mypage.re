== マイページの作成

お気に入りに登録した動画を一覧で確認できる「マイページ」を作成します。このページは、ログインしているユーザーだけがアクセスできるように設定します。

=== Server Componentでのデータ取得

マイページでは、サーバーサイドでSupabaseからお気に入り動画のリストを取得します。`src/app`に`mypage`ディレクトリを作成し、その中に`page.tsx`を作成してください。

```tsx
# src/app/mypage/page.tsx
import { createServerComponentClient } from "@supabase/auth-helpers-nextjs"
import { cookies } from "next/headers"
import { redirect } from "next/navigation"

export default async function MyPage() {
  const supabase = createServerComponentClient({ cookies })

  # ログインユーザーのセッションを取得
  const { data: { session } } = await supabase.auth.getSession()

  # 未ログイン場合はログインページにリダイレクト
  if (!session) {
    redirect("/login")
  }

  # favoritesテーブルからデータを取得
  const { data: favorites, error } = await supabase
    .from("favorites")
    .select("*")
    .order("created_at", { ascending: false })

  if (error) {
    console.error("Error fetching favorites:", error)
  }

  return (
    <div className="container mx-auto p-4">
      <h1 className="text-2xl font-bold mb-4">お気に入り一覧</h1>
      {favorites && favorites.length > 0 ? (
        <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-4">
          {favorites.map((fav) => (
            <div key={fav.video_id} className="border rounded-lg p-4 text-left">
              <img
                src={fav.thumbnail_url}
                alt={fav.video_title}
                className="w-full rounded-md mb-2"
              />
              <h3 className="font-bold">{fav.video_title}</h3>
              {/* ここにお気に入り解除ボタンを後で追加 */}
            </div>
          ))}
        </div>
      ) : (
        <p>お気に入りに登録された動画はありません。</p>
      )}
    </div>
  )
}
```

**コードの解説:**

1.  **`async function MyPage()`**: このページコンポーネントは、`async`関数として定義されています。これにより、コンポーネント内で`await`を使って非同期処理（データ取得など）を行うことができます。これはServer Componentsの大きな特徴です。
2.  **`createServerComponentClient`**: Server Components内でSupabaseクライアントを初期化するための関数です。
3.  **`supabase.auth.getSession()`**: 現在のユーザーセッションを取得します。`session`がなければ未ログインと判断し、`next/navigation`の`redirect`関数を使って`/login`ページに強制的に遷移させます。これにより、このページがログインユーザー専用になります。
4.  **`supabase.from("favorites").select("*")`**: `favorites`テーブルからデータを取得します。RLSポリシー（`auth.uid() = user_id`）が有効になっているため、Supabaseは自動的に現在ログインしているユーザーのお気に入りデータのみを返してくれます。これがRLSの強力な点です。
5.  取得した`favorites`データを`map`で展開し、一覧表示しています。

=== ホームページのお気に入りボタンの完成

マイページでお気に入り一覧が取得できたので、このロジックを応用して、ホームページの検索結果に表示するお気に入りボタンの状態を正しく表示させましょう。

ホームページ（`src/app/page.tsx`）もServer Componentに変更し、サーバーサイドで事前にお気に入り情報を取得するようにします。

```tsx
# src/app/page.tsx (抜粋)
import { createServerComponentClient } from "@supabase/auth-helpers-nextjs"
import { cookies } from "next/headers"
# ... 他のコンポーネント

export default async function HomePage() {
  const supabase = createServerComponentClient({ cookies })
  const { data: { session } } = await supabase.auth.getSession()

  # お気に入り動画IDのリストを取得
  const { data: favorites } = await supabase.from("favorites").select("video_id")
  const favoritedVideoIds = favorites?.map(fav => fav.video_id) || []

  return (
    <div>
      {/* ... 検索フォームなど */}
      <SearchResultList session={session} favoritedVideoIds={favoritedVideoIds} />
    </div>
  )
}
```

このように、ページコンポーネントをServer Componentとし、データ取得ロジックをサーバーサイドに集約します。そして、取得したデータ（`session`やお気に入りIDリスト）を、UI表示責務を持つClient Component（`SearchResultList`など）にpropsとして渡すのが、App Routerにおける基本的な設計パターンです。

（※ `SearchResultList`コンポーネントへのリファクタリングは、本書の範囲を超えるため、ここでは概念的な説明に留めます。）

これで、お気に入り機能の主要な部分が完成しました。次のセクションでは、この機能を支える重要なセキュリティ設定であるRLSについて、改めて詳しく解説します。
