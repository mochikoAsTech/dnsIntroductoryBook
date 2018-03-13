=  digとwhoisを叩いて学ぶDNS

性能向上のための格言に「推測するな計測せよ」というものがありますが、障害発生時にも同じことが言えます。DNS絡みのトラブルが起きたとき、意図した動作をしないとき、調べ方さえ知っていれば原因を調査できますし「恐らくDNSの浸透が原因じゃないかと・・・48時間ほど待てば多分・・・」みたいなエンジニアらしからぬ回答をしなくて済みます。

本章ではdigコマンドとwhoisコマンドを実際に叩いて色んなことを調べながら、DNSの仕組みを学んでいきましょう。

== digとwhoisのインストール

CentOSやAmazonLinuxなどのサーバ環境がある人は、そこで次のyumコマンドを叩けばインストールできます。

//cmd{
digコマンドのインストール
# @<b>{yum -y install bind-utils}

whoisコマンドのインストール
# @<b>{yum install -y jwhois}
//}

=== Linuxのサーバ環境がない場合

パソコンはWindowsだしコマンドを叩けるような環境はない・・・という方は、Route53を使うために折角AWSでアカウントを作ったのでEC2@<fn>{ec2}でインスタンス@<fn>{instance}を立ててみるのがお勧めです。サーバスペックごとにインスタンスタイプという区分があるのですが、t2.microというインスタンスタイプならアカウント作成から12か月間は毎月750時間まで無料で使えます。@<fn>{750h}

//footnote[ec2][Elastic Compute Cloudの略。AWSにはいろいろなサービスがありますがEC2はいわゆるサーバのことです。]
//footnote[instance][AWSではサーバのことをインスタンスを呼びます。]
//footnote[750h][月に750時間まで無料とありますが24時間×31日で744時間なので要は12か月間ずっと無料ということです。]

== nslookupはもう卒業！digコマンドの便利な使い方

ドメインに紐づくIPを調べたいとき、あるいはIPに紐づくドメインを調べたいときはdigコマンド@<fn>{dig}を使用します。

//footnote[dig][digはdomain information groperの略です。ちなみにGoogleに翻訳してもらったら「ドメイン情報痴漢」でした。gropeは手さぐりするという意味なので、ドメイン情報を手探りして調べてきてくれるということです。余談ですがpingのgも同じgroperなのでdigが痴漢ならpingも痴漢です。]

たとえば本書を頒布する技術書典4のURLは@<href>{https://techbookfest.org/event/tbf04}ですので、このサイトがどこのウェブサーバに乗っているのか知りたかったらdigで引数に「techbookfest.org」というドメインを渡してやれば調べられます。早速digを叩いてみましょう。

//cmd{
$ @<b>{dig techbookfest.org a}

; <<>> DiG 9.8.2rc1-RedHat-9.8.2-0.62.rc1.el6_9.5 <<>> techbookfest.org a
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 48654
;; flags: qr rd ra; QUERY: 1, ANSWER: 4, AUTHORITY: 4, ADDITIONAL: 8

@<b>{;; QUESTION SECTION:}
;techbookfest.org.              IN      A

@<b>{;; ANSWER SECTION:}
techbookfest.org.       300     IN      A       216.239.32.21
techbookfest.org.       300     IN      A       216.239.34.21
techbookfest.org.       300     IN      A       216.239.36.21
techbookfest.org.       300     IN      A       216.239.38.21

@<b>{;; AUTHORITY SECTION:}
techbookfest.org.       75009   IN      NS      ns-cloud-b1.googledomains.com.
techbookfest.org.       75009   IN      NS      ns-cloud-b4.googledomains.com.
techbookfest.org.       75009   IN      NS      ns-cloud-b2.googledomains.com.
techbookfest.org.       75009   IN      NS      ns-cloud-b3.googledomains.com.

@<b>{;; ADDITIONAL SECTION:}
ns-cloud-b1.googledomains.com. 334209 IN A      216.239.32.107
ns-cloud-b1.googledomains.com. 334209 IN AAAA   2001:4860:4802:32::6b
ns-cloud-b2.googledomains.com. 334209 IN A      216.239.34.107
ns-cloud-b2.googledomains.com. 334209 IN AAAA   2001:4860:4802:34::6b
ns-cloud-b3.googledomains.com. 334209 IN A      216.239.36.107
ns-cloud-b3.googledomains.com. 334209 IN AAAA   2001:4860:4802:36::6b
ns-cloud-b4.googledomains.com. 334209 IN A      216.239.38.107
ns-cloud-b4.googledomains.com. 334209 IN AAAA   2001:4860:4802:38::6b

;; Query time: 148 msec
;; SERVER: 127.0.0.1#53(127.0.0.1)
;; WHEN: Sun Mar 11 11:51:13 2018
;; MSG SIZE  rcvd: 395
//}

ただtechbookfest.orgに紐づくIPアドレスが知りたかっただけなのにものすごくいっぱい出てきました。digはまるでブキチ@<fn>{bukichi}のようなコマンドなのでちょっと聞いただけで「調べてきた結果を教えまし！ここのネームサーバに聞いたところこのドメインに紐づくIPはこれとこれとこれとこれなのでし！ちなみにこれを教えてくれたネームサーバの名前はこれでし、IPはこっちなのでし！ちなみにフルリゾルバはこれで調査には148msecかかったのでし！」と調査の過程や付加情報まで含めて全部教えてくれます。

//footnote[bukichi][スプラトゥーンに出てくる武器屋の店主。ブキの話になると超早口で果てしなく解説するブキマニア。オタク感があふれていて素晴らしく可愛い。]

いいから簡潔に「techbookfest.orgに紐づくIPアドレス」だけを教えろ！と思ったら、+shortというオプションを付けましょう。+shortさえつければdigはごく簡潔に答えてくれます。

//cmd{
$ dig techbookfest.org a +short
216.239.34.21
216.239.36.21
216.239.38.21
216.239.32.21
//}

=== hostやnslookupじゃダメですか？

ちなみにhostコマンドやnslookupコマンドでも同じように調べることができます。

//cmd{
$ host -t a techbookfest.org
techbookfest.org has address 216.239.36.21
techbookfest.org has address 216.239.38.21
techbookfest.org has address 216.239.32.21
techbookfest.org has address 216.239.34.21
//}

//cmd{
$ nslookup
> set type=a
> techbookfest.org
Server:         172.31.0.2
Address:        172.31.0.2#53

Non-authoritative answer:
Name:   techbookfest.org
Address: 216.239.36.21
Name:   techbookfest.org
Address: 216.239.38.21
Name:   techbookfest.org
Address: 216.239.32.21
Name:   techbookfest.org
Address: 216.239.34.21
> exit
//}

大昔はnslookupを叩くと「nslookupは非推奨だし、将来的には廃止されるから今後はdigやhostを使ってね@<fn>{nslookupIsDeprecated}」という警告メッセージが都度出ていたので、その頃を知っている人は「nslookupっていずれなくなるんでしょ？」認識かと思いますが、実際はBIND@<fn>{bind}9.9.0a3が公開されたタイミングでnslookupからこの警告メッセージは消え、リリースノートには「nslookupを非推奨として扱うのはもうやめるね。非推奨の警告も消したよ@<fn>{noLongerDeprecated}」と書かれているので、非推奨だの廃止だのという話は結局のところなくなったようです。

//footnote[nslookupIsDeprecated][Note: nslookup is deprecated and may be removed from future releases. Consider using the `dig' or `host' programs instead.と表示されていました。]
//footnote[bind][BINDはフルリゾルバとネームサーバ両方の機能を持つDNSサーバ。ISC（Internet Software Consortium）によって開発が行われている。多くのDNSサーバでBINDが採用されているが「夏のBIND脆弱性祭り」などと揶揄されるほど脆弱性の注意喚起とそれに伴うアップデート推奨が多い。]
//footnote[noLongerDeprecated][nslookup is no longer to be treated as deprecated. Remove "deprecated" warning message.]

何もトラブルが起きておらず、単純に名前解決した結果を知りたいだけであればhostやnslookupでも事足ります。ですがトラブル発生時の調査手段としてhostやnslookupを使おうとすると、必要な情報が不足していたり調べてきた結果を下手に加工して出力したりするため、あまり使いやすいコマンドとは言えません。これを機にdigを使ってみましょう！

=== オプションは略せる

== whoisコマンドの使い方

== 叩いて確認リソースレコード

=== Aレコード

==== 【トラブル】パンフレットと商品パッケージでwwwありなしが混在してしまった

=== MXレコード

==== MX設定しないとAにメールが届く

=== NSレコード

==== グルーレコード

=== SPFレコード（TXTレコード）

=== PTRレコード

=== CNAMEレコード

==== ZONE APEXはCNAME使えない

== Route53のAliasならZONE APEXでも設定可能

=== AliasはCNAMEと違って名前解決が1回

== オープンリゾルバ

=== Google Public DNS

=== OpenDNS

==== 【トラブル】NXDOMAINの代わりに広告が返ってくる
