== デプロイ後の必須設定

Vercelへのデプロイは完了しましたが、アプリケーションが本番環境で正しく機能するためには、SupabaseとGoogle Cloud Platform側でいくつかの重要な設定を変更する必要があります。

=== Supabase Auth設定の更新

現在のSupabaseプロジェクトは、ユーザーがサインアップした後の確認メールなどに記載されるURLが、ローカル開発用の`http://localhost:3000`になっています。これを、Vercelによって割り当てられた本番URLに変更する必要があります。

 1. Supabaseのプロジェクトダッシュボードにアクセスします。
 2. 左側メニューから「Authentication」>「URL Configuration」を選択します。
 3. 「Site URL」の項目に、VercelでデプロイされたアプリケーションのURL（例: `https://my-youtube-app-xxxx.vercel.app`）を入力します。
 4. 「Save」ボタンをクリックして保存します。

#image[supabase-url-config][SupabaseのURL設定画面]{
#}

この設定により、Supabaseが送信するメール（パスワードリセット、メールアドレス確認など）に含まれるリンクが、本番環境の正しいURLを指すようになります。

=== YouTube APIキーの制限設定の更新

同様に、YouTube APIキーも、現在は`http://localhost:3000`からのリクエストしか許可していません。本番URLからのリクエストを許可するように設定を追加します。

 1. [Google Cloud Platformコンソール](https://console.cloud.google.com/)にアクセスします。
 2. 「APIとサービス」>「認証情報」ページに移動し、使用しているAPIキーを選択します。
 3. 「キーを制限」セクションの「ウェブサイトの制限」で「追加」をクリックします。
 4. VercelのURLにワイルドカードを追加した形式（例: `https://my-youtube-app-xxxx.vercel.app/*`）を入力します。
 5. 「保存」をクリックします。

#image[gcp-api-key-prod][本番URLを追加したAPIキーの制限設定]{
#}

これで、ローカル環境と本番環境の両方から安全にAPIを呼び出すことができるようになりました。

=== カスタムドメイン（補足）

Vercelでは、`*.vercel.app`というドメインだけでなく、独自に取得したドメイン（例: `my-youtube-app.com`）をアプリケーションに設定することも非常に簡単です。

Vercelのプロジェクトダッシュボードから「Settings」>「Domains」と進み、所有しているドメイン名を追加し、画面の指示に従ってDNS設定を行うことで、よりプロフェッショナルなURLでアプリケーションを公開できます。興味がある方はぜひ試してみてください。

これらの設定を終えて、初めてデプロイ作業が完了したと言えます。次のセクションでは、公開されたアプリケーションの最終的な動作確認を行います。
