== Tailwind CSSによるスタイリング

本書では、UIのスタイリングにTailwind CSSを利用します。Tailwind CSSは「ユーティリティファースト」を掲げるCSSフレームワークで、予め用意された小さなクラス（ユーティリティクラス）をHTML（JSX）に直接記述していくことで、デザインを構築します。

=== ユーティリティファーストとは？

従来のCSS開発では、`MyButton`のようなカスタムクラスを定義し、それに対してCSSファイルでスタイルを適用するのが一般的でした。

```css
/* styles.css */
.my-button {
  background-color: blue;
  color: white;
  padding: 0.5rem 1rem;
  border-radius: 0.25rem;
}
```

```tsx
# Button.tsx
<button className="my-button">Click me</button>
```

一方、Tailwind CSSでは、スタイルの役割を持つクラスを直接適用します。

```tsx
# Button.tsx
<button className="bg-blue-500 text-white py-2 px-4 rounded">
  Click me
</button>
```

`bg-blue-500`は背景色、`text-white`は文字色、`py-2`は上下のpadding、`px-4`は左右のpadding、`rounded`は角丸を意味します。このように、CSSを直接書くことなく、クラス名を組み合わせるだけでデザインを表現できるのがTailwind CSSの大きな特徴です。

最初はクラス名が長くなりがちで戸惑うかもしれませんが、CSSファイルとJSXファイルを行き来する必要がなくなり、コンポーネント内でデザインが完結するため、開発効率が大きく向上します。

=== ホームページのスタイリング

それでは、Tailwind CSSを使って、現在のホームページ（`src/app/page.tsx`）の見た目を整えてみましょう。`create-next-app`で生成された初期のコンテンツを削除し、アプリケーションの入り口としてふさわしいシンプルな内容に書き換えます。

`src/app/page.tsx` を開き、以下のように修正してください。

```tsx
# src/app/page.tsx
export default function Home() {
  return (
    <div className="text-center">
      <h1 className="text-4xl font-bold mb-4">YouTubeお気に入りアプリ</h1>
      <p className="text-lg text-gray-600 mb-8">
        好きなYouTube動画を検索して、自分だけのリストを作成しよう。
      </p>

      {/* この下に動画検索フォームを配置していきます */}
    </div>
  )
}
```

ここでは、`text-center`で要素を中央揃えにし、`text-4xl`や`font-bold`で見出しの文字サイズや太さを調整しています。`mb-4`は`margin-bottom`を意味し、要素の下側に余白を設けています。

開発サーバーを起動して `http://localhost:3000` を確認すると、ヘッダーの下に、中央揃えされたシンプルなタイトルと説明文が表示されるはずです。

#image[home-page-styled][スタイリングされたホームページ]{}

このように、本書ではTailwind CSSのユーティリティクラスを使いながら、各コンポーネントのスタイリングを行っていきます。クラス名に慣れないうちは、Tailwind CSSの公式サイトのドキュメントを参照しながら進めると良いでしょう。

 * [Tailwind CSS Documentation](https://tailwindcss.com/docs)

これで第3章は完了です。Next.jsの基本的な概念とスタイリングの方法を学びました。次の章から、いよいよ動画検索機能の実装に入ります。
