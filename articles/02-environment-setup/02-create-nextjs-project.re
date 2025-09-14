== Next.jsプロジェクトの作成

開発ツールの準備が整ったので、いよいよNext.jsのプロジェクトを作成します。プロジェクトの雛形は、公式が提供している`create-next-app`というコマンドラインツールを使うことで、対話的に簡単に作成できます。

ターミナルを開き、プロジェクトを作成したいディレクトリに移動して、以下のコマンドを実行してください。

```sh
$ npx create-next-app@latest
```

コマンドを実行すると、いくつか質問されます。本書では、以下のように回答してプロジェクトを作成します。

```
What is your project named? my-youtube-app
Would you like to use TypeScript? Yes
Would you like to use ESLint? Yes
Would you like to use Tailwind CSS? Yes
Would you like to use `src/` directory? Yes
Would you like to use App Router? (recommended) Yes
Would you like to customize the default import alias? No
```

それぞれの質問項目について解説します。

 * **What is your project named?**: プロジェクト名です。ここでは `my-youtube-app` とします。
 * **Would you like to use TypeScript?**: TypeScriptを使用するかどうか。本書では型安全な開発を行うため `Yes` を選択します。
 * **Would you like to use ESLint?**: コードの静的解析ツールESLintを使用するかどうか。コードの品質を保つために `Yes` を選択します。
 * **Would you like to use Tailwind CSS?**: CSSフレームワークのTailwind CSSを使用するかどうか。効率的なスタイリングのため `Yes` を選択します。
 * **Would you like to use `src/` directory?**: ソースコードを `src` ディレクトリにまとめるかどうか。`Yes` を選択すると、コンポーネントなどのソースコードが `src` ディレクトリ以下に配置されます。
 * **Would you like to use App Router?**: App Routerを使用するかどうか。Next.js 13以降の推奨機能であり、本書の核となる機能なので `Yes` を選択します。
 * **Would you like to customize the default import alias?**: モジュールのインポートパスのエイリアス（別名）をカスタマイズするかどうか。デフォルト設定のままで問題ないため `No` を選択します。

全ての質問に回答すると、`my-youtube-app` という名前のディレクトリが作成され、その中にNext.jsプロジェクトのファイルが一式生成されます。


作成されたプロジェクトのディレクトリに移動し、開発サーバーを起動してみましょう。

```sh
$ cd my-youtube-app
$ npm run dev
```

ターミナルに以下のような表示が出れば成功です。

```
- ready started server on 0.0.0.0:3000, url: http://localhost:3000
```

Webブラウザで `http://localhost:3000` にアクセスしてみてください。Next.jsのウェルカムページが表示されるはずです。

#image[nextjs-welcome][Next.jsのウェルカムページ]{}

これで、Next.jsアプリケーションを開発する準備が整いました。一度 `Ctrl + C` で開発サーバーを停止してください。
