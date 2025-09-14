== API通信の準備

この章では、アプリケーションの中核機能であるYouTube動画検索を実装します。その第一歩として、YouTube Data API v3と通信するための準備を整えましょう。

=== Google API Client Libraryの導入

YouTube APIは、HTTPリクエストを送信することで利用できますが、より簡単に扱うためにGoogleが公式に提供しているクライアントライブラリを利用します。

ターミナルで以下のコマンドを実行し、`googleapis`をプロジェクトにインストールしてください。

```sh
$ npm install googleapis
```

このライブラリを使うことで、TypeScriptの型定義の恩恵を受けながら、少ないコード量でAPIを呼び出すことができます。

=== YouTube APIクライアントの初期化

次に、APIを呼び出すためのクライアントを初期化するコードを記述します。`src`ディレクトリに`lib`という名前のディレクトリを作成し、その中に`youtube.ts`というファイルを作成してください。

```
src/
└── lib/
    └── youtube.ts
```

`src/lib/youtube.ts`に、以下のコードを記述します。

```ts
# src/lib/youtube.ts
import { google } from 'googleapis'

export const youtube = google.youtube({
  version: 'v3',
  auth: process.env.YOUTUBE_API_KEY,
})
```

ここでは、インストールした`googleapis`から`google`オブジェクトをインポートし、`google.youtube()`メソッドを使ってYouTube APIクライアントを作成しています。

`version`にはAPIのバージョンである`v3`を指定します。

`auth`には、APIリクエストの認証に使うAPIキーを指定します。ここでは、第2章で`.env.local`に設定した環境変数`YOUTUBE_API_KEY`を`process.env.YOUTUBE_API_KEY`として読み込んでいます。

`process.env`はNode.jsで環境変数を参照するためのオブジェクトです。`YOUTUBE_API_KEY`には`NEXT_PUBLIC_`プレフィックスが付いていないため、このコードはサーバーサイド（Server ComponentsやServer Actions）でのみ実行可能であることに注意してください。クライアントサイドでこのファイルをインポートしようとするとエラーになります。

作成した`youtube`オブジェクトを`export`することで、他のファイルからこのクライアントを再利用できるようになります。

これで、YouTube APIと通信するための準備が整いました。次のセクションでは、このクライアントを使って、実際に動画を検索するためのフォームと、サーバーサイドのロジックを実装していきます。
