== 動画データの取得と表示

検索フォームのUIができたので、次はいよいよサーバーサイドでYouTube APIを呼び出し、取得したデータを表示する部分を実装します。ここでは、Next.jsのServer Actionsという機能を使います。

=== Server Actionsとは？

Server Actionsは、クライアントからのリクエストをトリガーに、サーバーサイドで直接実行される非同期関数です。これにより、APIルートを別途作成することなく、フォームの送信などの処理をコンポーネント内で完結させることができます。

Server Actionsは、Server Components内で定義するか、`"use server"`ディレクティブをファイルの先頭に記述することで作成できます。

=== 検索アクションの作成

まずは、動画を検索するためのServer Actionを作成します。`src/app`ディレクトリに`actions.ts`というファイルを作成してください。

```ts
# src/app/actions.ts
"use server"

import { youtube } from "@/lib/youtube"

export const searchVideos = async (query: string) => {
  if (!query) {
    return []
  }

  try {
    const response = await youtube.search.list({
      part: ["snippet"],
      q: query,
      type: ["video"],
      maxResults: 10,
    })

    return response.data.items || []
  } catch (error) {
    console.error("Error searching videos:", error)
    # エラーハンドリングをここに追加
    return []
  }
}
```

ファイルの先頭に`"use server"`を記述し、このファイル内の関数がServer Actionsであることを宣言します。

`searchVideos`関数は、検索キーワード`query`を引数に取ります。`@/lib/youtube.ts`で作成したYouTube APIクライアントを使い、`youtube.search.list`メソッドを呼び出しています。

 * `part: ["snippet"]`：動画の基本的な情報（タイトル、説明、サムネイルなど）を取得します。
 * `q: query`：検索キーワードを指定します。
 * `type: ["video"]`：検索対象を動画に限定します。
 * `maxResults: 10`：最大10件の結果を取得します。

APIから取得した検索結果（`response.data.items`）を返し、エラーが発生した場合はコンソールに出力して空の配列を返すようにしています。

=== 検索結果の表示

次に、このServer Actionを呼び出し、結果を表示する部分を実装します。ホームページ`src/app/page.tsx`を、状態を持つコンポーネントにする必要があるため、`"use client"`を使ってClient Componentに変更します。

```tsx
# src/app/page.tsx
"use client"

import { useState } from "react"
import { searchVideos } from "./actions"
import type { youtube_v3 } from "googleapis"

export default function Home() {
  const [videos, setVideos] = useState<youtube_v3.Schema$SearchResult[]>([])

  const handleSearch = async (query: string) => {
    const results = await searchVideos(query)
    setVideos(results)
  }

  return (
    <div className="text-center">
      {/* ... h1とpタグは変更なし ... */}

      {/* SearchFormコンポーネントにhandleSearchを渡す */}
      <SearchForm onSearch={handleSearch} />

      {/* 検索結果の表示 */}
      <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-4 mt-8">
        {videos.map((video) => (
          <div key={video.id?.videoId} className="border rounded-lg p-4 text-left">
            <img
              src={video.snippet?.thumbnails?.medium?.url || ""}
              alt={video.snippet?.title || ""}
              className="w-full rounded-md mb-2"
            />
            <h3 className="font-bold">{video.snippet?.title}</h3>
          </div>
        ))}
      </div>
    </div>
  )
}
```

**主な変更点:**

1.  `"use client"`を追加し、Client Componentに変更します。
2.  `useState`で検索結果の動画リスト（`videos`）を管理します。
3.  `searchVideos`アクションをインポートし、それを呼び出す`handleSearch`関数を定義します。取得した結果は`setVideos`でstateに保存します。
4.  `SearchForm`コンポーネントに、`onSearch`というpropsで`handleSearch`関数を渡すように変更します（`SearchForm`側も後で修正します）。
5.  `videos`配列を`map`でループし、各動画のサムネイルとタイトルを表示します。スタイリングにはTailwind CSSのグリッドレイアウトを使用しています。


=== SearchFormの修正

最後に、`SearchForm`が`onSearch`を受け取って実行できるように修正します。

```tsx
# src/components/SearchForm.tsx
"use client"

import { useState } from "react"

# onSearch propsの型を定義
interface SearchFormProps {
  onSearch: (query: string) => void
}

const SearchForm = ({ onSearch }: SearchFormProps) => {
  const [query, setQuery] = useState("")

  const handleSubmit = (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault()
    onSearch(query) // 親から渡されたonSearchを実行
  }

  # ...フォームのJSXは変更なし
}

export default SearchForm
```

`onSearch`という名前で、文字列を引数に取る関数をpropsとして受け取るように型定義し、フォーム送信時にその関数を実行するように変更しました。

これで、検索フォームに入力して検索ボタンを押すと、Server Actionが実行され、取得した動画データが画面に表示されるようになりました。実際に試してみてください。
