== 検索フォームの作成

ユーザーが動画を検索するためのキーワードを入力できるUIを作成します。このフォームは、ホームページ（`/`）に配置することにします。

=== SearchFormコンポーネントの作成

まずは、検索フォームのコンポーネントを作成します。`src/components`ディレクトリに`SearchForm.tsx`というファイルを作成してください。

```tsx
# src/components/SearchForm.tsx
"use client"

import { useState } from "react"

const SearchForm = () => {
  const [query, setQuery] = useState("")

  const handleSubmit = (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault()
    # ここに検索処理を実装します
    console.log(query)
  }

  return (
    <form onSubmit={handleSubmit} className="mb-8">
      <div className="flex justify-center">
        <input
          type="text"
          value={query}
          onChange={(e) => setQuery(e.target.value)}
          placeholder="キーワードを入力..."
          className="w-full max-w-md px-4 py-2 border border-gray-300 rounded-l-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
        <button
          type="submit"
          className="px-6 py-2 bg-blue-500 text-white font-semibold rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500"
        >
          検索
        </button>
      </div>
    </form>
  )
}

export default SearchForm
```

このコンポーネントでは、`useState`を使って入力されたキーワード（`query`）を管理しています。`useState`はReactフックなので、このコンポーネントはClient Componentである必要があります。そのため、ファイルの先頭に`"use client"`を記述しています。

フォームが送信（`onSubmit`）されたときの処理は`handleSubmit`関数に記述しますが、現時点では入力されたキーワードをコンソールに出力するだけにしておきます。実際の検索処理は、次のセクションでServer Actionsを使って実装します。

=== ホームページへの配置

作成した`SearchForm`コンポーネントを、ホームページ（`src/app/page.tsx`）に配置します。

```tsx
# src/app/page.tsx
import SearchForm from "@/components/SearchForm"

export default function Home() {
  return (
    <div className="text-center">
      <h1 className="text-4xl font-bold mb-4">YouTubeお気に入りアプリ</h1>
      <p className="text-lg text-gray-600 mb-8">
        好きなYouTube動画を検索して、自分だけのリストを作成しよう。
      </p>

      <SearchForm />
    </div>
  )
}
```

`SearchForm`コンポーネントをインポートし、説明文の下に配置します。

開発サーバーを起動して`http://localhost:3000`にアクセスすると、検索フォームが表示されるはずです。キーワードを入力して「検索」ボタンをクリックすると、ブラウザの開発者ツールのコンソールに入力した文字が出力されることを確認してください。

#image[search-form-ui][検索フォームが表示されたホームページ]{}

これで、検索機能のUI部分は完成です。次のセクションでは、このフォームから入力されたキーワードを使って、実際にYouTube APIを呼び出す処理を実装します。
