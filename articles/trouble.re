= ドメイントラブルシューティング
== 【トラブル】test@test.co.jpを使って情報漏洩
=== 例示用ドメインを使おう
== 【トラブル】.devで終わるテスト環境のサイトが見られなくなった
=== 自分のものでないドメインを使うべきでない理由
== 【トラブル】他社にサイト運用を移管するがドメインの管理者が分からない
=== サイト移管のAtoZ
=== 事前にTTLを短くして無駄な「浸透待ち」をなくす
== 【トラブル】サーバから送った受付完了メールが迷惑メール扱いで届かない
=== 送信ドメイン認証のためSPFレコードを設定
=== 送信元IPのPTR（逆引き）レコードも設定
=== PTRで設定したホスト名のAレコードを設定
=== バウンスメール受信のためにMXレコードを設定
== 【トラブル】急ぎでDNS設定を依頼したが設定後もサイトが見られない
=== ネガティブキャッシュ
=== キャッシュ残り時間の調べ方
== 【トラブル】証明書が発行できない
=== CAAレコードは親のドメインも要確認
== 【トラブル】AWSで突然ドメイン名が引けなくなった
=== Amazon DNSのレートリミット問題
== 【トラブル】サブドメインが登録できない
=== ドメインにはアンダーバーが使えない
=== *を使うと便利
