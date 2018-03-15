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

//footnote[bukichi][スプラトゥーンに出てくる武器屋の店主。ブキの話になると超早口で果てしなく解説するブキマニア。語尾が「でし！」でオタク感があふれていて素晴らしく可愛い。]

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

大昔はnslookupを叩くと「nslookupは非推奨だし、将来的には廃止されるから今後はdigやhostを使ってね@<fn>{nslookupIsDeprecated}」という警告メッセージが都度出ていたので、その頃を知っている人は「nslookupっていずれなくなるんでしょ？」という認識かと思いますが、実際はBIND@<fn>{bind}9.9.0a3が公開されたタイミングでnslookupからこの警告メッセージは消え、リリースノートには「nslookupを非推奨として扱うのはもうやめるね。非推奨の警告も消したよ@<fn>{noLongerDeprecated}」と書かれているので、nslookupが非推奨だの廃止だのという話は一旦なくなったようです。

//footnote[nslookupIsDeprecated][Note: nslookup is deprecated and may be removed from future releases. Consider using the `dig' or `host' programs instead.と表示されていました。]
//footnote[bind][BINDはフルリゾルバとネームサーバ両方の機能を持つDNSサーバ。ISC（Internet Software Consortium）によって開発が行われている。多くのDNSサーバでBINDが採用されているが「夏のBIND脆弱性祭り」などと揶揄されるほど脆弱性の注意喚起とそれに伴うアップデート推奨が多い。]
//footnote[noLongerDeprecated][nslookup is no longer to be treated as deprecated. Remove "deprecated" warning message.]

何もトラブルが起きておらず、単純に名前解決した結果を知りたいだけであればhostやnslookupでも事足ります。ですがトラブル発生時の調査手段としてhostやnslookupを使おうとすると、必要な情報が不足していたり調べてきた結果を下手に加工して出力したりするため、どちらもあまり使いやすいコマンドとは言えません。これを機に今後はdigを使っていきましょう！

== Whoisを叩いてドメインやIPの持ち主を調べよう

whoisコマンドを使うと@<chapref>{domain}で説明したように「ドメインを所有している組織や担当者の連絡先」といったWhois情報を調べることができます。しかもドメインの持ち主だけでなく、IPアドレスの持ち主を調べることもできます。

//cmd{
ドメインの持ち主を調べる
$ whois ドメイン

IPアドレスの持ち主を調べる
$ whois IPアドレス
//}

== digを叩いてリソースレコードを確認してみよう

=== Aレコード

たとえば「www.example.jpは203.0.113.222」や「mx.example.comは203.0.113.22」のように、ドメインとIPアドレスを紐づけているのがAレコードです。あなたが「このサイトはどこにあるんだろう？」と思ったら、次のようにdigコマンドを叩いてみましょう。@<fn>{prompt}するとそのドメインに紐づくIPアドレスが表示されるはずです。

//cmd{
$ dig ドメイン名 a +short
//}

//footnote[prompt][一番左の「$」はサーバにログインしたときにユーザ名やサーバの名前が表示される「プロンプト」というものを表しています。この「$」は実際は入力しなくていいので、digから右を入力してください。]

=== 【ドリル】ウェブサーバはどこにある？

==== 問題

あなたは「mojimo-manga」（モジモマンガ）というサイト（@<href>{https://mojimo.jp/manga/}）の運用担当になりました。@<fn>{exampleSite}（@<img>{mojimoManga}）ところが前任者が急に辞めてしまって引継ぎ資料も見当たらず、このサイトがどこのウェブサーバで動いているのかちっとも分かりません。ウェブサーバはどこのクラウドサービスを使っているのでしょうか？

//footnote[exampleSite][もちろん例えです。本著及び著者は例えに使用させていただいたサイトと何の関係もありません。]

//image[mojimoManga][mojimo-manga - あのマンガの、あのアニメの、あのフォントが使える！][scale=0.75]{
//}

 * A. さくらインターネット
 * B. AWS
 * C. IDCFクラウド

==== 解答

正解はBです。

先ず「dig ドメイン名 a +short」でドメインからIPを引いてみましょう。サイトのURLが@<href>{https://mojimo.jp/manga/}なので、確認すべきドメインはmojimo.jpです。

//cmd{
$ dig mojimo.jp a +short
46.51.255.231
54.248.233.139
//}

IPだけではまだどこのクラウドサービスなのか分かりません。さらにwhoisコマンドでこのIPの持ち主を調べてみましょう。@<fn>{twoIp}「$whois 46.51.255.231」を叩いてみると「descr: Amazon Web Services, Elastic Compute Cloud, EC2」と出ますので、このIPはAWSのEC2のものであることが分かります。これでサイトがAWSのEC2上で動いていることが分かりました。

//footnote[twoIp][IPアドレスは2つありますがどちらでも構いません。]

//cmd{
$ whois 46.51.255.231
（中略）
netname:        AMAZON-NRT
descr:          The activity you have detected originates from a dynamic hosting environment. For fastest response, please submit abuse reports at http://aws-portal.amazon.com/gp/aws/html-forms-controller/contactus/AWSAbuse For more information regarding EC2 see: http://ec2.amazonaws.com/ All reports MUST include: * src IP * dest IP (your IP) * dest port * Accurate date/timestamp and timezone of activity * Intensity/frequency (short log extracts) * Your contact details (phone and email) Without these we will be unable to identify the correct owner of the IP address at that point in time.
descr:          Amazon Web Services, Elastic Compute Cloud, EC2
country:        JP
admin-c:        ADSI2-RIPE
tech-c:         ADSI2-RIPE
status:         ASSIGNED PA
mnt-by:         MNT-ADSI
mnt-by:         NN20150720
created:        2017-11-07T20:48:28Z
last-modified:  2017-11-07T20:49:00Z
source:         RIPE
（後略）
//}

===[column] 【コラム】digのオプションは略せる

digコマンドには「+short」や「+norecurse」のようなクエリオプションがありますが、それぞれ後ろの何文字かを省略して「+shor」や「+norec」でも実行可能です。

こんな「コマンドオプションって略せるの？！」のようなdigの細かい仕様や仕組みをもっと詳しく知りたい！という方は、JPRSが公開している「初心者のためのDNS運用入門 - トラブル事例とその解決のポイント -」@<fn>{dnsBeginner}という資料がお勧めです。

//footnote[dnsBeginner][@<href>{https://dnsops.jp/event/20140626/dns-beginners-guide2014-mizuno.pdf}]

=== MXレコード

「〇〇@example.co.jp」というメールアドレス宛てにメールを送ったらこのメールサーバで受信します、という設定をしているのがMXレコードです。「このメールアドレスってメールはどこで受信してるんだっけ？」と思ったら、次のようにdigコマンドを叩いてみましょう

//cmd{
$ dig ドメイン名 mx +short
//}

たとえば任天堂の窓口であるinfo@nintendo.co.jpにメールを送ったら、どこのメールサーバが受信するのか確認してみましょう。

//cmd{
$ dig nintendo.co.jp mx +short
10 nintendo-co-jp.mail.protection.outlook.com.
//}

「nintendo-co-jp.mail.protection.outlook.com」というのがメール受信サーバであることが分かりました。先頭の10はプリファレンス値といって「メールサーバが複数台ある場合の優先度」を表しています。MXレコードは複数設定できるため、プリファレンス値が10のメールサーバを複数台用意して負荷を分散したり、プリファレンス値が10のメールサーバが落ちていたら代わりにプリファレンス値が20のメールサーバで受信する、というように冗長性を高めたりできます。

MXレコードから察するに任天堂はOffice 365@<fn>{office365}を使っているようです。さらに「nintendo-co-jp.mail.protection.outlook.com」のAレコードを引くと、最終的にはメール受信サーバのIPまでたどり着きます。

//footnote[office365][Microsoftの法人向けサービス。メールやグループウェア、Microsoft Officeなどを利用できる。]

//cmd{
$ dig nintendo-co-jp.mail.protection.outlook.com a +short
23.103.139.138
23.103.139.170
//}

== 【トラブル】test@test.co.jpを使って情報漏洩

あなたは広告代理店B社のプロデューサーで、A社の新製品である花粉症用マスクの先行体験キャンペーンを企画・担当しています。

キャンペーンサイトのフォームから新製品の先行体験キャンペーンに申し込むと、お客様には「応募を受け付けました」というメールが届きます。このメールにはお客様がフォームで記入された住所や電話番号などが書いてあります。

この受付完了メールですが、実は送信元メールアドレス（From）を何にするか実装が始まった時点では仕様が決まっていませんでした。そのため送信元メールアドレスは、ディレクターが仕様書に取り合えずのつもりで書いておいたinfo@test.co.jpというアドレスで実装されてしまい、テストでも誰も気づかないまま本番リリースを迎えてしまいました。

キャンペーンが始まり、花粉症のCさんは早速フォームから申し込みました。しかしCさんは最近引っ越したばかりだったので、うっかり古い住所をフォームで送ってしまいました。「応募を受け付けました」メールを見て住所が間違っていることに気づいたCさんは、すぐにメールの返信でinfo@test.co.jp宛てに「入力した住所が間違っていました。正しい住所は〇〇なので訂正したいです」という問い合わせを送りました。

しかしいつまで経っても返事が来ません。Cさんがキャンペーン事務局にクレームの電話を入れてエンジニアのDさんが調査をした結果、送信元メールアドレスがA社ではなくinfo@test.co.jpになっていた問題が発覚しました。果たしてCさんがinfo@test.co.jp宛てに送ったメールはどこへ行ってしまったのでしょう？

プロデューサーのあなたもエンジニアのDさんと一緒にこの問題を調査してみましょう。メール受信サーバを知りたいときは「dig ドメイン名 mx +short」です。

//cmd{
$ dig test.co.jp mx +short
10 mail10.heteml.jp.
//}

〇〇@test.co.jp宛てのメールを受信しているメールサーバはmail10.heteml.jpであることが分かりました。mail10.heteml.jpについてウェブで検索してみたところ、どうやらGMOペパボがやっているヘテムル@<fn>{heteml}というクラウドサービスのメールサーバのようです。

//footnote[heteml][@<href>{https://heteml.jp/}]

〇〇@test.co.jpに送ったメールはヘテムルで受信しているというところまで分かりましたが、そもそもtest.co.jpというドメインは誰の持ち物なのでしょうか？今度はwhoisでドメインの持ち主を調べてみましょう。

//cmd{
$ whois test.co.jp
[Querying whois.jprs.jp]
[whois.jprs.jp]
[ JPRS database provides information on network administration. Its use is    ]
[ restricted to network administration purposes. For further information,     ]
[ use 'whois -h whois.jprs.jp help'. To suppress Japanese output, add'/e'     ]
[ at the end of command, e.g. 'whois -h whois.jprs.jp xxx/e'.                 ]

Domain Information:
a. [Domain Name]                TEST.CO.JP
g. [Organization]               EDUCATIONAL ASSESSMENT INSTITUTE LIMITED COMPANY
l. [Organization Type]          Limited Company
m. [Administrative Contact]     TY2813JP
n. [Technical Contact]          KT071JP
p. [Name Server]                dns0.heteml.jp
p. [Name Server]                dns1.heteml.jp
s. [Signing Key]
[State]                         Connected (2019/02/28)
[Registered Date]               2000/02/14
[Connected Date]                2000/02/16
[Last Update]                   2018/03/01 01:05:12 (JST)
//}

Whoisで調べてみるとこのドメインは「EDUCATIONAL ASSESSMENT INSTITUTE LIMITED COMPANY」という名前の有限会社の持ち物だよ、という情報が出てきました。プロデューサーのあなたとエンジニアのDさんはなんだか嫌な予感がして、ブラウザで@<href>{http://test.co.jp/}（@<img>{testCoJp}）を開いてみました。すると有限会社教育評価研究所という会社のウェブサイトが表示されました。

//image[testCoJp][教育評価研究所][scale=0.75]{
//}

なんと花粉症のCさんが返信で送ったお問い合わせメールは、クライアントのA社とも広告代理店のB社とも何の関係もない、この有限会社教育評価研究所という会社に飛んでしまったようです。「エンドユーザの個人情報漏洩だ、やってしまった・・・」とあなたとDさんは頭を抱えました。

=== 自分のものでないドメインを使うべきでない理由

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

== whoisコマンドの使い方
