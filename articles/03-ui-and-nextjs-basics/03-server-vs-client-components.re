== Server Components vs Client Components

App Routerを使いこなす上で最も重要な概念が、Server ComponentsとClient Componentsです。この2種類のコンポーネントの違いを理解し、適切に使い分けることが、パフォーマンスと開発体験の向上に繋がります。

=== Server Components: デフォルトのコンポーネント

App Routerでは、**デフォルトで全てのコンポーネントがServer Components**として扱われます。Server Componentsは、その名の通り、サーバーサイドでのみレンダリングされるコンポーネントです。

**Server Componentsの特徴:**

 * **サーバーサイドでのみ実行**: ビルド時またはリクエスト時にサーバーでレンダリングされ、HTMLとしてブラウザに送信されます。クライアントサイドのJavaScriptバンドルに含まれないため、ブラウザがダウンロードするJavaScriptの量を削減できます。
 * **バックエンドリソースへの直接アクセス**: サーバーサイドで実行されるため、データベースやファイルシステム、外部APIなどへ直接アクセスできます。これにより、APIルートを作成する手間なく、安全にデータを取得できます。
 * **状態(State)やライフサイクルが持てない**: `useState`や`useEffect`といったReactフックや、`onClick`のようなイベントハンドラを使用することはできません。インタラクティブな機能は扱えません。

先ほど作成した`Header`コンポーネントや、`layout.tsx`、`page.tsx`は、何も指定していないため、全てServer Componentsです。

=== Client Components: インタラクティブなUIを実現

ユーザーの操作に応じて動的に変化するような、インタラクティブなUIを作成するには、Client Componentsを使用します。Client Componentsは、サーバーで事前レンダリングされた後、クライアントサイド（ブラウザ）でJavaScriptによって「ハイドレーション」され、インタラクティブになります。

コンポーネントをClient Componentにするには、ファイルの先頭に `"use client"` というディレクティブ（指示）を記述します。

**Client Componentsの特徴:**

 * **インタラクティブな機能**: `useState`, `useEffect`といったフックや、`onClick`などのイベントリスナーを使用できます。
 * **ブラウザAPIへのアクセス**: `window`や`localStorage`といったブラウザの機能にアクセスできます。
 * **ファイルの先頭に`"use client"`が必要**: この記述により、Next.jsはこのコンポーネントをClient Componentとして扱います。

例えば、ボタンをクリックしたらカウンターが増えるようなコンポーネントは、Client Componentで作成する必要があります。

```tsx
# src/components/Counter.tsx
"use client" // Client Componentであることを宣言

import { useState } from 'react'

const Counter = () => {
  const [count, setCount] = useState(0)

  return (
    <div>
      <p>Count: {count}</p>
      <button onClick={() => setCount(count + 1)}>Increment</button>
    </div>
  )
}

export default Counter
```

=== どちらを使うべきか？

Next.jsの公式ドキュメントでは、以下のような指針が示されています。

> **可能な限りServer Componentsを使い、インタラクティブ性が必要な部分のみをClient Componentsにする。**

まずは全てをServer Componentsとして考え、状態管理やイベントハンドラ、ブラウザAPIが必要になったコンポーネントだけを`"use client"`でClient Componentに切り替えていく、というアプローチが基本です。

また、Server Componentsの中にClient Componentsをインポートして配置することはできますが、その逆（Client Componentsの中にServer Componentsをインポートする）はできません（ただし、childrenとして渡すことは可能です）。

この原則を理解しておくことで、パフォーマンスが高く、かつメンテナンスしやすいアプリケーションを構築できます。今後の章で、この2つのコンポーネントを実際に使い分けながら開発を進めていきます。
