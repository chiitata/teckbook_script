== Supabase CLIの導入とローカル開発

本書では、Supabaseのダッシュボードを使ってデータベースのテーブル作成やRLSの設定を行ってきました。しかし、チーム開発やCI/CD（継続的インテグレーション/継続的デプロイメント）を考慮すると、これらの設定をコードとして管理し、ローカル環境でSupabaseのサービスを動かすことが重要になります。

Supabaseは、そのためのツールとして「Supabase CLI」を提供しています。

=== Supabase CLIとは？

Supabase CLIは、Supabaseプロジェクトをコマンドラインから操作するためのツールです。主な機能は以下の通りです。

 * **ローカル開発環境の構築**: Dockerを使って、ローカルマシン上にSupabaseの全サービス（PostgreSQL, Auth, Storageなど）を起動できます。
 * **マイグレーション管理**: データベーススキーマの変更履歴をコードとして管理し、適用できます。
 * **環境の同期**: ローカルの変更をリモートのSupabaseプロジェクトに同期したり、その逆を行ったりできます。
 * **Edge Functionsのデプロイ**: Edge Functionsをコマンドラインからデプロイできます。

=== Supabase CLIのインストール

Supabase CLIは、npmを使って簡単にインストールできます。

```sh
$ npm install -g supabase
```

インストール後、バージョンを確認してみましょう。

```sh
$ supabase -v
supabase version 1.x.x # 表示されるバージョンは実行時期によって異なります
```

=== ローカル開発環境の構築

Supabase CLIを使って、ローカルでSupabaseのサービスを起動してみましょう。Next.jsプロジェクトのルートディレクトリ（`my-youtube-app`）で、以下のコマンドを実行します。

 1. **Supabaseプロジェクトの初期化**: 
    ```sh
    $ supabase init
    ```
    これにより、`supabase`というディレクトリが作成され、その中にマイグレーションファイルなどが格納されます。

 2. **Supabaseプロジェクトのリンク**: 
    ```sh
    $ supabase link --project-ref your-project-id
    ```
    `your-project-id`には、SupabaseダッシュボードのURL（例: `https://app.supabase.com/project/your-project-id`）から取得できるプロジェクトIDを指定します。これにより、ローカルのCLIとリモートのSupabaseプロジェクトが紐付けられます。

 3. **ローカルサービスの起動**: 
    ```sh
    $ supabase start
    ```
    このコマンドを実行すると、Dockerを使ってローカルマシン上にSupabaseのサービス群が起動します。起動が完了すると、ローカルのSupabaseのURLやAPIキーが表示されます。

    ローカルで起動したSupabaseを使うには、Next.jsプロジェクトの`.env.local`ファイルを、表示されたローカルのURLとキーに書き換える必要があります。

    ```sh
    # .env.local
    NEXT_PUBLIC_SUPABASE_URL=http://localhost:54321
    NEXT_PUBLIC_SUPABASE_ANON_KEY=eyJ... # ローカルのanonキー
    ```

これで、インターネットに接続していなくても、ローカルでSupabaseの全機能を使った開発が可能になります。データベースのスキーマ変更も、`supabase migration new`コマンドでマイグレーションファイルを作成し、`supabase migrate deploy`で適用するといった、より堅牢な開発フローを構築できます。

=== さらなるステップアップ

Supabase CLIは、大規模なアプリケーション開発やチーム開発において、非常に強力なツールとなります。本書では基本的な使い方に留めましたが、公式ドキュメントにはさらに多くの機能や活用方法が紹介されています。

 * [Supabase CLI Documentation](https://supabase.com/docs/guides/cli)

ローカル開発環境の構築やマイグレーション管理は、開発の効率性と安定性を高める上で非常に重要です。ぜひこの機会にSupabase CLIを深く学び、あなたの開発ワークフローに取り入れてみてください。
