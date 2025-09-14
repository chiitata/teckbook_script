== データベースの準備: `profiles`テーブルの作成

Supabase Authが管理する`auth.users`テーブルには、メールアドレスなどの認証に必要な最低限の情報しか含まれていません。アプリケーションで利用するユーザー名やアバター画像といった追加情報を保存するために、`profiles`という新しいテーブルを自分たちで作成します。

=== テーブルの設計

`profiles`テーブルには、以下のカラム（列）を持たせることにします。

 * **`id`**: ユーザーを一意に識別するためのID。`auth.users`テーブルの`id`と連携させるため、`UUID`型とし、主キーに設定します。
 * **`username`**: ユーザー名。`text`型とします。
 * **`avatar_url`**: アバター画像のURL。`text`型とします。
 * **`updated_at`**: レコードの最終更新日時。`timestamp with time zone`型とします。

`id`カラムは、`auth.users`テーブルの`id`カラムを外部キーとして参照するように設定します。これにより、`auth.users`に存在しないユーザーのプロフィールが作られることを防ぎ、データの整合性を保つことができます。

=== SQLエディタによるテーブル作成

Supabaseのダッシュボードには、SQLを直接実行できる「SQL Editor」という機能があります。これを使って`profiles`テーブルを作成しましょう。

 1. Supabaseプロジェクトのダッシュボードで、左側メニューから「SQL Editor」を選択します。
 2. 「New query」をクリックします。
 3. 以下のSQLクエリをエディタに貼り付けます。

```sql
-- profilesテーブルを作成
create table public.profiles (
  id uuid not null primary key,
  username text,
  avatar_url text,
  updated_at timestamp with time zone,
  constraint profiles_id_fkey foreign key (id) references auth.users (id) on delete cascade
);

-- profilesテーブルへのアクセス権限を設定
alter table public.profiles enable row level security;

create policy "Public profiles are viewable by everyone." on public.profiles
  for select using (true);

create policy "Users can insert their own profile." on public.profiles
  for insert with check (auth.uid() = id);

create policy "Users can update own profile." on public.profiles
  for update using (auth.uid() = id);
```

 4. 「RUN」ボタンをクリックしてクエリを実行します。

#image[supabase-sql-editor][SQL Editorでのテーブル作成]{
#}

=== SQLクエリの解説

実行したSQLクエリは、大きく2つの部分に分かれています。

**1. テーブル作成 (`create table`)**

`public.profiles`という名前でテーブルを作成しています。`id`カラムを主キー（`primary key`）に設定し、`auth.users`テーブルの`id`を外部キー（`foreign key`）として参照するように制約（`constraint`）を設けています。`on delete cascade`は、`auth.users`からユーザーが削除された場合に、関連する`profiles`のレコードも自動的に削除するための設定です。

**2. アクセス権限の設定（RLSポリシー）**

ここがSupabaseの強力な部分です。`alter table ... enable row level security`で、`profiles`テーブルに対してRLS（Row Level Security）を有効にしています。

RLSを有効にすると、デフォルトでは誰もこのテーブルのデータにアクセスできなくなります。データにアクセスするためには、「ポリシー（Policy）」を定義する必要があります。

 * **`Public profiles are viewable by everyone.`**: `select`（読み取り）操作は、誰でも（`using (true)`）許可する、というポリシーです。
 * **`Users can insert their own profile.`**: `insert`（書き込み）操作は、これから書き込もうとしているデータの`id`と、現在ログインしているユーザーのID（`auth.uid()`）が一致する場合にのみ許可する、というポリシーです。
 * **`Users can update own profile.`**: `update`（更新）操作も同様に、更新しようとしているデータの`id`が、ログイン中のユーザーIDと一致する場合にのみ許可します。

これにより、「自分のプロフィールは自分しか作成・更新できないが、誰でも見ることはできる」というセキュリティルールを、データベースレベルで強制することができます。

これで、ユーザー情報を格納するための器が準備できました。次のセクションでは、実際にユーザーがサインアップ・ログインできるUIを実装していきます。
