== 環境変数の設定

前のセクションで取得したSupabaseとYouTubeのAPIキーは、非常に重要な情報であり、GitHubなどのパブリックなリポジトリに直接書き込むべきではありません。これらの機密情報を安全に管理するために、Next.jsの「環境変数」という仕組みを利用します。

=== .env.localファイルの作成

Next.jsプロジェクトのルートディレクトリ（`package.json`がある場所）に、`.env.local`という名前のファイルを作成します。このファイルに記述された内容は、ローカルの開発環境でのみ読み込まれ、Gitの管理対象から自動的に除外されるため、安全に機密情報を扱うことができます。

VS Codeでプロジェクトを開き、ルートディレクトリに`.env.local`ファイルを作成してください。

```sh
# .env.local

# Supabase
NEXT_PUBLIC_SUPABASE_URL=あなたのSupabaseプロジェクトURL
NEXT_PUBLIC_SUPABASE_ANON_KEY=あなたのSupabase anon publicキー

# YouTube
YOUTUBE_API_KEY=あなたのYouTube APIキー
```

このファイルに、前のセクションで取得した以下の3つの値を設定します。

 * `NEXT_PUBLIC_SUPABASE_URL`: SupabaseのプロジェクトURLに置き換えます。
 * `NEXT_PUBLIC_SUPABASE_ANON_KEY`: Supabaseの`anon` `public`キーに置き換えます。
 * `YOUTUBE_API_KEY`: YouTubeのAPIキーに置き換えます。

=== 環境変数のプレフィックスについて

ここで、Supabaseの環境変数名についている `NEXT_PUBLIC_` というプレフィックスに注目してください。

Next.jsでは、セキュリティ上の理由から、デフォルトではサーバーサイドでしか環境変数を読み込むことができません。しかし、`NEXT_PUBLIC_` というプレフィックスを付けた環境変数は、特別にクライアントサイド（ブラウザ）のコードからもアクセスできるようになります。

SupabaseのURLとanonキーは、ブラウザからSupabaseのAPIを呼び出す際に必要となるため、このプレフィックスを付けています。一方で、YouTubeのAPIキーはサーバーサイドからのみ利用するため、プレフィックスは付けていません。

このように、環境変数がどこから利用されるのかを意識してプレフィックスを使い分けることが、セキュアなアプリケーションを開発する上で非常に重要です。

`.env.local` ファイルを保存したら、一度開発サーバーを再起動してください。環境変数の変更を反映させるには、再起動が必要です。

```sh
$ npm run dev
```

これで、第2章「開発環境の準備」は完了です。アプリケーションを開発するための土台がすべて整いました。次の章から、いよいよUIの構築に取り掛かります。
