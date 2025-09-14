== Server Actionsの活用と状態管理

前のセクションでは、Server Actionsを使ってAPIからデータを取得し、表示することに成功しました。しかし、実際のアプリケーションでは、データ取得中にローディング状態を示したり、エラーが発生した場合にユーザーにフィードバックしたりする必要があります。

このセクションでは、Server Actionsをより実践的に活用するために、ローディングとエラーの状態管理を実装します。

=== ローディング状態の管理

APIからのデータ取得には時間がかかる場合があります。その間、ユーザーに何も表示されないと、アプリケーションが停止しているかのような印象を与えてしまいます。`useState`を使って、検索中であるという状態を管理しましょう。

`src/app/page.tsx`を以下のように修正します。

```tsx
# src/app/page.tsx
"use client"

import { useState } from "react"
# ... (他のimport文)

export default function Home() {
  const [videos, setVideos] = useState<youtube_v3.Schema$SearchResult[]>([])
  const [loading, setLoading] = useState(false) // loading stateを追加

  const handleSearch = async (query: string) => {
    setLoading(true) // 検索開始時にloadingをtrueに
    const results = await searchVideos(query)
    setVideos(results)
    setLoading(false) // 検索終了時にloadingをfalseに
  }

  return (
    <div className="text-center">
      {/* ... */}
      <SearchForm onSearch={handleSearch} />

      {loading ? (
        <p>検索中...</p>
      ) : (
        <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-4 mt-8">
          # ... video mapping ...
        </div>
      )}
    </div>
  )
}
```

`loading`という新しいstateを導入し、`handleSearch`が実行される直前に`true`、実行が終わった後に`false`に設定します。そして、JSXの部分で`loading`が`true`の間は「検索中...」というテキストを表示し、`false`のときに検索結果を表示するように分岐させています。

これで、検索ボタンを押してから結果が表示されるまでの間に、ローディング中であることがユーザーに伝わるようになりました。

=== エラーハンドリング

次に、Server Actionでエラーが発生した場合のハンドリングです。例えば、APIキーが間違っている場合や、APIの利用制限を超えてしまった場合などが考えられます。

まずは、`src/app/actions.ts`の`searchVideos`関数を修正し、エラー情報を返すようにします。

```ts
# src/app/actions.ts
"use server"

import { youtube } from "@/lib/youtube"

# 関数の戻り値の型を定義
interface SearchResult {
  items?: youtube_v3.Schema$SearchResult[]
  error?: string
}

export const searchVideos = async (query: string): Promise<SearchResult> => {
  // ...
  try {
    const response = await youtube.search.list({ /* ... */ })
    return { items: response.data.items || [] }
  } catch (error) {
    console.error("Error searching videos:", error)
    return { error: "動画の検索中にエラーが発生しました。" }
  }
}
```

戻り値の型`SearchResult`を定義し、成功した場合は`items`プロパティに、失敗した場合は`error`プロパティにエラーメッセージを入れて返すように変更しました。

次に、この変更に合わせて`src/app/page.tsx`も修正します。

```tsx
# src/app/page.tsx
"use client"

import { useState } from "react"

export default function Home() {
  const [videos, setVideos] = useState<youtube_v3.Schema$SearchResult[]>([])
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null) // error stateを追加

  const handleSearch = async (query: string) => {
    setLoading(true)
    setError(null) // 検索開始時にエラーをリセット
    const result = await searchVideos(query)

    if (result.error) {
      setError(result.error)
    } else {
      setVideos(result.items || [])
    }
    setLoading(false)
  }

  return (
    <div className="text-center">
      {/* ... */}
      <SearchForm onSearch={handleSearch} />

      {loading && <p>検索中...</p>}
      {error && <p className="text-red-500">{error}</p>}

      {!loading && !error && (
        <div className="grid ...">
          # ... video mapping ...
        </div>
      )}
    </div>
  )
}
```

`error`というstateを追加し、Server Actionからエラーが返ってきた場合にそのメッセージをstateに保存します。JSXでは、`error`が存在する場合に、その内容を赤い文字で表示するようにしています。

このように、Server Actionsは単にサーバーの関数を呼び出すだけでなく、その戻り値を使ってクライアント側で柔軟に状態を管理することができます。これにより、ユーザーに対してより親切なUIを提供できるようになります。

これで第4章は完了です。アプリケーションのコアとなる動画検索機能を実装できました。次の章では、ユーザー認証機能を追加していきます。
