== よくあるエラーと解決策

本書を通じてアプリケーション開発を進める中で、様々なエラーに遭遇するかもしれません。ここでは、特にNext.jsとSupabaseを使った開発でよく見られるエラーとその解決策をまとめました。問題に直面した際の参考にしてください。

=== 1. 環境変数が読み込まれない

**現象:**
`process.env.NEXT_PUBLIC_SUPABASE_URL`などが`undefined`になる、またはVercelにデプロイ後にAPIキー関連のエラーが発生する。

**考えられる原因と解決策:**

 * **`.env.local`ファイルの記述ミス**: キー名や値に誤りがないか、余分なスペースなどがないか確認してください。
 * **開発サーバーの再起動忘れ**: `.env.local`ファイルを変更した後は、必ず開発サーバーを再起動（`npm run dev`を一度停止して再実行）してください。
 * **`NEXT_PUBLIC_`プレフィックスの有無**: クライアントサイドで利用したい環境変数には、必ず`NEXT_PUBLIC_`プレフィックスが必要です。サーバーサイドでのみ利用する場合は不要です。
 * **Vercelでの環境変数設定漏れ**: Vercelにデプロイする際は、Vercelのダッシュボードで環境変数を設定する必要があります。ローカルの`.env.local`はVercelにはデプロイされません。

=== 2. Supabaseの認証エラー

**現象:**
サインアップやログインができない、または「AuthApiError: Email rate limit exceeded」などのエラーが表示される。

**考えられる原因と解決策:**

 * **メールアドレスの確認**: サインアップ時にSupabaseから送信される確認メールのリンクをクリックしていない可能性があります。迷惑メールフォルダも確認してください。
 * **SupabaseのURL設定ミス**: Supabaseダッシュボードの「Authentication」>「URL Configuration」で設定した「Site URL」が、アプリケーションの正しいURL（ローカル開発時は`http://localhost:3000`、本番環境ではVercelのURL）になっているか確認してください。
 * **メール送信制限**: 短時間に大量のサインアップを試行すると、Supabaseのメール送信制限に引っかかることがあります。しばらく時間をおいてから再度試すか、Supabaseのダッシュボードで設定を確認してください。

=== 3. RLS（Row Level Security）関連のエラー

**現象:**
データベースからデータが取得できない、または更新・削除ができない。特に、ログインしているはずなのに自分のデータにアクセスできない。

**考えられる原因と解決策:**

 * **RLSポリシーの記述ミス**: Supabaseダッシュボードの「Table Editor」から対象テーブルを選択し、「Policies」タブでポリシーの記述に誤りがないか確認してください。特に`auth.uid() = user_id`のような条件式が正しいか注意深く見てください。
 * **RLSの有効化忘れ**: テーブルに対してRLSが有効（`Enable Row Level Security`がON）になっているか確認してください。
 * **`createServerComponentClient`または`createPagesBrowserClient`の誤用**: Server Componentsでは`createServerComponentClient`、Client Componentsでは`createPagesBrowserClient`を正しく使用しているか確認してください。特にServer Actions内で`createPagesBrowserClient`を使うと、認証情報が正しく渡らずRLSが機能しないことがあります。

=== 4. YouTube API関連のエラー

**現象:**
動画検索ができない、または「API key not valid」などのエラーが表示される。

**考えられる原因と解決策:**

 * **APIキーの記述ミス**: `.env.local`やVercelの環境変数に設定した`YOUTUBE_API_KEY`が正しいか確認してください。
 * **APIの有効化忘れ**: Google Cloud Platformで「YouTube Data API v3」が有効になっているか確認してください。
 * **APIキーの制限設定ミス**: Google Cloud Platformの認証情報で設定したAPIキーの制限（HTTPリファラーなど）が、アプリケーションのURL（ローカル開発時は`http://localhost:3000/*`、本番環境ではVercelのURL`https://your-app.vercel.app/*`）を許可しているか確認してください。

=== 5. `"use client"`ディレクティブの誤用

**現象:**
`useState`や`useEffect`を使っているのにエラーになる、または`window is not defined`のようなエラーが表示される。

**考えられる原因と解決策:**

 * **`"use client"`の記述忘れ**: `useState`や`useEffect`などのReactフックや、ブラウザAPI（`window`, `localStorage`など）を使用するコンポーネントは、必ずファイルの先頭に`"use client"`を記述してClient Componentとして宣言する必要があります。
 * **Server ComponentでブラウザAPIにアクセス**: `"use client"`を記述していないServer Component内で、誤ってブラウザAPIにアクセスしようとしていないか確認してください。

エラーメッセージは、問題解決のための重要な手がかりです。エラーが発生したら、まずはメッセージをよく読み、どこで何が起きているのかを理解しようと努めましょう。そして、本書の該当箇所や公式ドキュメントを参照しながら、一つずつ解決していくことが大切です。
