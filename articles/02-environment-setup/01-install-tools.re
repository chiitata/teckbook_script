== 必要なツールのインストール

アプリケーション開発を始める前に、必要なツールをコンピュータにインストールしましょう。本書では、以下のツールを使用します。

 * **Node.js**: JavaScriptをサーバーサイドで実行するための環境です。Next.jsはNode.js上で動作します。
 * **npm または yarn**: Node.jsに付属するパッケージ管理ツールです。ライブラリのインストールなどに使用します。
 * **Visual Studio Code**: 高機能なテキストエディタです。多くのエンジニアに利用されています。

=== Node.jsのインストール

Node.jsは、そのバージョンを管理しやすくするために、`nvm`（Node Version Manager）や`Volta`といったツールを使ってインストールすることをおすすめします。ここでは`Volta`を使ったインストール方法を紹介します。

Voltaは、プロジェクトごとにNode.jsのバージョンを自動で切り替えてくれる便利なツールです。

**macOS / Linux の場合**

ターミナルを開き、以下のコマンドを実行してVoltaをインストールします。

```sh
$ curl https://get.volta.sh | bash
```

インストール後、ターミナルを再起動するか、以下のコマンドを実行してVoltaを有効化します。

```sh
$ source ~/.bash_profile # or ~/.zshrc etc.
```

次に、Voltaを使ってNode.jsのLTS（Long Term Support）版をインストールします。

```sh
$ volta install node
```

**Windows の場合**

公式サイトからインストーラーをダウンロードして実行してください。

 * https://volta.sh/

インストール後、PowerShellやコマンドプロンプトで同様に `volta install node` を実行します。

インストールが完了したら、バージョンを確認してみましょう。

```sh
$ node -v
v18.18.0 # 表示されるバージョンは実行時期によって異なります

$ npm -v
9.8.1
```

このようにバージョンが表示されれば、Node.jsとnpmのインストールは成功です。

=== Visual Studio Codeのインストール

Visual Studio Code（VS Code）は、Microsoftが開発している無料のコードエディタです。まだインストールしていない場合は、公式サイトからダウンロードしてインストールしてください。

 * https://code.visualstudio.com/

VS Codeには、開発を効率化するための拡張機能が豊富に用意されています。本書を進める上で、特に以下の拡張機能をインストールしておくことをお勧めします。

 * **Japanese Language Pack for Visual Studio Code**: VS CodeのUIを日本語化します。
 * **Prettier - Code formatter**: コードを自動で整形してくれます。
 * **Tailwind CSS IntelliSense**: Tailwind CSSのクラス名を補完してくれます。

拡張機能は、VS Codeの左側にあるアクティビティバーの四角いアイコンから検索・インストールできます。

これで、開発に必要なツールの準備が整いました。次のセクションでは、いよいよNext.jsプロジェクトを作成します。
