== お気に入り登録処理の実装

データベースの準備ができたので、次はお気に入り登録の具体的な処理を実装します。検索結果として表示された動画に「お気に入り」ボタンを設置し、クリックされたら`favorites`テーブルにデータを保存する流れを作ります。

=== お気に入り追加・削除のServer Actions

まず、`favorites`テーブルを操作するためのServer Actionsを`src/app/actions.ts`に追記します。

```ts
# src/app/actions.ts
"use server"

import { createServerActionClient } from "@supabase/auth-helpers-nextjs"
import { cookies } from "next/headers"
# ... (searchVideosなど)

# お気に入り追加
export const addFavorite = async (video: youtube_v3.Schema$SearchResult) => {
  const supabase = createServerActionClient({ cookies })
  const { data: { user } } = await supabase.auth.getUser()

  if (!user) {
    return { error: "ログインが必要です。" }
  }

  const { error } = await supabase.from("favorites").insert({
    user_id: user.id,
    video_id: video.id?.videoId,
    video_title: video.snippet?.title,
    thumbnail_url: video.snippet?.thumbnails?.medium?.url,
  })

  if (error) {
    console.error(error)
    return { error: "お気に入り登録に失敗しました。" }
  }

  return { success: true }
}

# お気に入り削除
export const removeFavorite = async (videoId: string) => {
  const supabase = createServerActionClient({ cookies })
  const { data: { user } } = await supabase.auth.getUser()

  if (!user) {
    return { error: "ログインが必要です。" }
  }

  const { error } = await supabase
    .from("favorites")
    .delete()
    .match({ user_id: user.id, video_id: videoId })

  if (error) {
    console.error(error)
    return { error: "お気に入り解除に失敗しました。" }
  }

  return { success: true }
}
```

**コードの解説:**

 * **`createServerActionClient`**: Server Actions内でSupabaseクライアントを初期化するための関数です。`cookies`を渡すことで、サーバー側で現在のユーザーの認証状態を正しく認識できます。
 * **`supabase.auth.getUser()`**: 現在ログインしているユーザーの情報を取得します。ユーザーが存在しない場合はエラーを返します。
 * **`supabase.from("favorites").insert(...)`**: `favorites`テーブルに新しいレコードを挿入します。`user_id`や動画の情報をオブジェクトとして渡します。
 * **`supabase.from("favorites").delete().match(...)`**: `favorites`テーブルからレコードを削除します。`match`で、`user_id`と`video_id`が一致するレコードのみを削除対象としています。

=== お気に入りボタンコンポーネントの作成

次に、お気に入りボタン自体をコンポーネントとして切り出します。このボタンは、自分がその動画を既にお気に入り登録しているかどうかを知る必要があります。

`src/components`に`FavoriteButton.tsx`を作成します。

```tsx
# src/components/FavoriteButton.tsx
"use client"

import { useState } from "react"
import { addFavorite, removeFavorite } from "@/app/actions"
import type { youtube_v3 } from "googleapis"

interface FavoriteButtonProps {
  video: youtube_v3.Schema$SearchResult
  isFavorited: boolean
}

const FavoriteButton = ({ video, isFavorited }: FavoriteButtonProps) => {
  const [favorited, setFavorited] = useState(isFavorited)
  const [loading, setLoading] = useState(false)

  const handleClick = async () => {
    setLoading(true)
    if (favorited) {
      await removeFavorite(video.id!.videoId!)
      setFavorited(false)
    } else {
      await addFavorite(video)
      setFavorited(true)
    }
    setLoading(false)
  }

  return (
    <button
      onClick={handleClick}
      disabled={loading}
      className={`w-full mt-2 px-4 py-2 text-white font-semibold rounded ${favorited ? "bg-red-500 hover:bg-red-600" : "bg-blue-500 hover:bg-blue-600"}`}>
      {loading ? "処理中..." : favorited ? "お気に入りから削除" : "お気に入りに追加"}
    </button>
  )
}

export default FavoriteButton
```

このコンポーネントは、動画情報（`video`）と、その動画が既にお気に入り済みかどうかの真偽値（`isFavorited`）をpropsとして受け取ります。
ボタンがクリックされたら、`isFavorited`の状態に応じて`addFavorite`または`removeFavorite`アクションを呼び出し、自身の状態を更新します。

=== ホームページへの組み込み

最後に、この`FavoriteButton`をホームページに組み込みます。そのためには、ホームページ側で「どの動画がお気に入り済みか」という情報を持っている必要があります。

この処理は少し複雑になるため、次章の「マイページ」の実装と合わせて解説します。現段階では、まずはお気に入り登録のロジックが完成したことを理解してください。

次のセクションでは、お気に入りに登録した動画を一覧表示する「マイページ」を作成します。
