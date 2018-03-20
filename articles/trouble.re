= ドメイントラブルシューティング

ここからはドメインやDNSでありがちなトラブルとその解決方法、あるいは未然に防ぐための対策をご紹介していきます。

== 【トラブル】URLはwwwありなしどっち？

よくあるトラブルとしてサイトのドメインをwwwありかなしかはっきり決めておかなかったことで、商品パッケージではwwwなし、店頭で配るチラシにはwwwありのURLを記載してしまい、wwwありとなしのURLが混在してしまう・・・という問題があります。

え、別にwwwあるかないかくらい些細なことじゃないの？何かだめなの？と思われるかも知れませんがいけません。

そもそもドメインをwww.example.co.jpのように頭（第4レベルドメイン）から最後（トップレベルドメイン）までしっかり全部書いたものをFQDN@<fn>{fqdn}と呼びます。

 * www : 第4レベルドメイン
 * example : 第3レベルドメイン
 * co : 第2レベルドメイン
 * jp : トップレベルドメイン

//footnote[fqdn][Fully Qualified Domain Nameの略で、日本語にすると完全修飾ドメイン名。正確を期するのであればFQDNにはTLDであるjpまでではなく、さらにルートを表す.（ドット）も含むべきですが、ここではTLDまで表記されていればFQDNとします。余談ですがFQDNに対して、wwwやwww.exampleのように一部を抜き出した表記は相対ドメイン名、あるいは不完全なドメイン名と呼びます。]

startdns.funとwww.example.co.jpは全く別のFQDNです。それと同じようにwww.example.co.jpとexample.co.jpも全く別のFQDNです。

名前が途中まで一緒だとなんだか近しいような気がしてしまいますが、北海道と奄美大島が全く別の場所であるのと同様に、地名が似ていても奄美大島と大島も全然別の場所@<fn>{oshima}ですよね。

//footnote[oshima][大島は伊豆諸島にあり東京から船で30分、対して奄美大島は沖縄の近くにある。]

たとえば実際に用意したウェブサイトはwww.example.co.jpなのに、LINEプッシュで「今すぐexample.co.jpにアクセス！」と告知してしまったとします。これはイベント会場が奄美大島にあるのに「大島（東京）でイベントやります！」と告知してしまったようなものなので、LINEを見たユーザがURLをクリックしてexample.co.jpにアクセスしてもこのままではサイトは表示されません。

気の利いたサイト制作会社であれば、依頼側が何も言わなくてもドメインやリダイレクトの設定をしておいてくれて、example.co.jpを訪れたユーザを取りこぼさずに、www.example.co.jpにリダイレクトさせておいてくれるかも知れませんがそうでなければ阿鼻叫喚です。（「リダイレクトの設定」というのは、大島に「奄美大島への自動転送装置」を設置しておくようなものだと思ってください）

また「うちのサイトはwwwありとなしのどちらでアクセスしても同じ画面が出るよ！リダイレクトはされないけどね！」という場合もやはり問題です。検索エンジンはFQDNが異なるサイトを全く別のサイトとして認識するため、wwwありのサイトとなしのサイトでドメインとしての価値が分散されてしまい、結果として検索結果で上位に表示されにくくなります。

サイトを作るときは、最初にFQDNをwwwありなのかなしなのかはっきり決めましょう。もし既にばらばらの状態でサイトが公開されてしまっていたら、今からでも「メインをwwwありにする」のように決めて、片方にちゃんとアクセスを寄せるようにしましょう。wwwありでもなしでも正直どっちでもいい、という場合は、前述の「ZONE APEXではCNAMEが使えない」問題があるため、将来CDNを使いたくなった時に弊害がないようwwwありにしておくのがお勧めです。

== 【トラブル】.devで終わるテストサイトが見られなくなった

2017年12月、「.dev」で終わるドメインのサイトをChromeで開くと強制的にHTTPからHTTPSにリダイレクトされるようになり、あちこちで「テストサイトが見られなくなった！」「開発環境が急に使えなくなった」という悲鳴が上がりました。

もともとgTLDは.comや.netなどの22種類しかなく、.devというTLDは存在しなかったため、IT系の各社が組織内のネットワーク（開発環境とか）でオレオレTLDとして勝手に.devを使っていた、という背景があります。しかしgTLDの種類が段々増えてきてとうとう.devというTLDもできてしまい、色んなところで「やばい名前衝突(Name Collision)する・・・」という話題が出たのが2014年ごろのことでした。

ちなみにICANNWiki@<fn>{icannWiki}によると、この.devは当初AmazonとGoogleで「俺がレジストリになる！」「いいや、俺がなる！」と奪い合ってたけど、.youと.talkをAmazonに譲って、代わりに.devをGoogleがゲット、みたいな経緯があったようです。詳しくは@<chapref>{domain}で説明していますが、レジストリとはそのTLDの唯一の卸元ですので、あちこちで「Googleが.devを買った」と言われているのは、この「Googleが.devのレジストリになったこと」を指しているものと思われます。

//footnote[icannWiki][@<href>{https://icannwiki.org/.dev}]

そして今回Googleが「Chromeで.devを開くと強制的にHTTPSにリダイレクトする」という暴挙に出たのは「Googleは2014年にdevのレジストリ（生産元）になった。でもまだ.devのドメインは一般発売していない。ということは今.devを使っている人は全員、ドメイン買わずに勝手に使ってる人なので迷惑が掛かっても知ったことではない。じゃあ.devはHTTPSに強制リダイレクトさせるね」ということなんだと推察しています。

.devだけなく.appというTLDも同様のようですので、これらのドメインがもうHTTPSでしか使えないということは、Googleは.devを.comや.netのように普通に売るのではなく、何か特別な用途で使わせるつもりなのでは？と思われます。

このトラブルについては「.devが使えなくなったからとりあえず.testにした！解決！」のようなブログ記事も見ますが、なんで.testにしたら直ったのか？.testでいいならたとえば.dev2でもいいのか？という疑問は浮かばないでしょうか？

ざっくり言うと、.testはいいけど.dev2はだめです。社内ネットワークにしてもテスト用のメールアドレスにしても@<chapref>{dig}で書いた通り例示やテストで使っていいドメインが用意されているので、基本的にはそれを使いましょう。「自分の持ち物でないドメイン」を勝手に使うのは、今回のようなトラブルの元なので控えましょう。

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
