= DNSの仕組み

ドメインを買ってウェブサーバにコンテンツを載せたらそれだけでサイトが見られるでしょうか？

いいえ、ドメインとウェブサーバがあってもそれぞれ存在しているだけではサイトは見られません。ドメインとウェブサーバを紐づける必要があります。でもサーバはともかくとしてドメインは手で触れるようなものではないので、この2つをリアルに紐で結ぶことはできません。ドメインとサーバを紐づけるには一体どうしたらいいのでしょう？

そこで出てくるのがDNSです。この章ではDNSの仕組みをしっかり学んでいきましょう。

== DNSとは

DNSサーバのDNSはDomain Name Systemの略で、日本語に直訳すると「ドメイン名の管理システム」といった感じです。一口にDNSサーバと言っても、その実態は「ネームサーバ」と「フルリゾルバ」の2つに分かれています。異なる働きをする2つのサーバがどちらも「DNSサーバ」と呼ばれていることが、DNSの分かりにくさの一因だと思います。具体的な仕組みの解説に入る前にきちんとこの2つを整理しておきましょう。

=== ネームサーバ

ネームサーバは「電話帳」のような役割を果たします。

電話帳には名前とそれに紐づく電話番号が書いてありますが、ネームサーバにはドメインとそれに紐づくIPアドレスが登録されています。

「DNSコンテンツサーバ」「権威DNSサーバ」と呼ばれることもありますが、本著では統一してネームサーバと呼びます。

=== フルリゾルバ

フルリゾルバは「秘書」のような役割を果たします。

フルリゾルバに「このドメインに紐づくIPアドレスが知りたいの」と言うと、あちこちのネームサーバに聞きまわってIPアドレスを調べてきて教えてくれます。しかも一度調べると一定期間はそのドメインとIPアドレスの紐づけを記憶（キャッシュ）するため、もう1回同じことを聞くと今度はすぐに教えてくれます。「DNSキャッシュサーバ」「フルサービスリゾルバ」と呼ばれることもありますが、本著では統一してフルリゾルバと呼びます。

あなたがブラウザでウェブサイトを見るときは必ずこのフルリゾルバにドメインの名前解決を頼んでいます。「でもそんなもの使う設定をした記憶ないけど・・・」と思われるかも知れませんが、会社のオフィスなら情シスが、家庭なら契約しているISP@<fn>{isp}がフルリゾルバを用意していて、恐らくDHCPサーバから「このフルリゾルバを使ってね」と自動で割り当てられているので意識していないだけです。

//footnote[isp][インターネットサービスプロバイダの略。インターネットへの接続サービスを提供する電気通信事業者のこと。]

情シスやISPがそれぞれのネットワーク内で提供しているフルリゾルバの他に、Google Public DNS@<fn>{googlePublicDns}のようにだれでも無料で使えるオープンリゾルバというものがあります。2013年8月にOCN@<fn>{ocn}のフルリゾルバが死んでOCNユーザがインターネットに接続できなくなる@<fn>{incident}障害が発生しました。そのとき「DNSサーバの設定を8.8.8.8にすれば直るよ」という情報がTwitterで出回りました（@<img>{twitter8888}）。

//footnote[ocn][NTTコミュニケーションズが運営する日本最大級のインターネットサービスプロバイダ。]
//footnote[incident][ドメインからIPを引く名前解決ができないことでウェブサーバのIPが分からず、サイトが見られなくなったりゲームやアプリなどのサービスが使えなくなったりした、ということです。]

//image[twitter8888][DNSサーバを8.8.8.8にするとつながるというツイート][scale=0.75]{
//}

これは使用するフルリゾルバを故障中のOCNのものからGoogle Public DNSに変更したことで名前解決が出来るようになってサイトも見られるようになった、ということです。

//footnote[googlePublicDns][@<href>{https://developers.google.com/speed/public-dns/}]

フルリゾルバはもちろんサーバでも使っています。Linuxサーバなら/etc/resolv.confというファイルを見てみましょう。resolv.confのnameserverという項目で指定されているのがそのサーバのフルリゾルバです。

//cmd{
$ cat /etc/resolv.conf
options timeout:2 attempts:5
nameserver 172.31.0.2
//}

== お名前.comのページが表示されるまで

@<chapref>{domain}の最後に、買ったばかりのドメインをブラウザで開いて見たところ、お名前.comの「このドメインは、お名前.comで取得されています。」（@<img>{onamaeCm}）というページが表示されました。

//image[onamaeCm][http://自分のドメイン/を開くとお名前の広告が出る][scale=0.75]{
//}

ブラウザでhttp://自分のドメイン/を開いたときに、このページが表示されるまでの流れは次（@<img>{afterBuyingDomain}）のようになっていました。

 1. フルリゾルバにstartdns.funに紐づくIPを聞きに行く
 1. フルリゾルバは自分の中にstartdns.funに紐づくIPアドレスのキャッシュがないことを確認
 1. フルリゾルバはルートネームサーバにstartdns.funに紐づくIPアドレスを聞きに行く
 1. ルートネームサーバは「.funのネームサーバはa.nic.funだよ」と教えてくれる
 1. フルリゾルバはa.nic.funにstartdns.funに紐づくIPアドレスを聞きに行く
 1. a.nic.funは「startdns.funのネームサーバはdns1.onamae.comだよ」と教えてくれる
 1. フルリゾルバはdns1.onamae.comにstartdns.funに紐づくIPアドレスを聞きに行く
 1. dns1.onamae.comは「startdns.funに紐づくIPアドレスは150.95.255.38だよ」と教えてくれる
 1. フルリゾルバが「startdns.funに紐づくIPアドレスは150.95.255.38です」と教えてくれる
 1. 150.95.255.38（ウェブサーバ）にstartdns.funのページをリクエスト
 1. 150.95.255.38（ウェブサーバ）がページを返してくれる

//image[afterBuyingDomain][ページが表示されるまでのフルリゾルバとネームサーバの仕組み][scale=0.75]{
※この図は仮。デザイナーに描いてもらう。
//}

ブラウザのアドレスバーに自分のドメインを入れてEnterを押してからページが表示されるまでの一瞬の間に、なんとこれだけの名前解決が行われていたのです。

== ゾーンと委任

前述のようにDNSは「1つのネームサーバが何もかも知っている」という集中管理ではなく、「いくつかのネームサーバに聞けば答えにたどり着く」という管理権限の分散されて仕組みになっています。先ほどの名前解決を会社に例えると次のようになります。

 1. 自分の秘書に「A部門のBさんの携帯番号知ってる？」と聞く
 1. 秘書はBさんの携帯番号を知らなかった
 1. 秘書は社長に「A部門のBさんの携帯番号を知りたいんです」と聞きに行く
 1. 社長が「A部門のことはA部長に聞いて」と答える
 1. 秘書はA部長に「A部門のBさんの携帯番号を知りたいんです」と聞きに行く
 1. A部長は「A部門のBさんのことはマネージャのCさんに聞いて」と答える
 1. 秘書はマネージャのCさんに「A部門のBさんの携帯番号を知りたいんです」と聞きに行く
 1. マネージャのCさんが「A部門のBさんの携帯番号は090-〇〇〇〇-〇〇〇〇だよ」と教えてくれる
 1. 秘書が「A部門のBさんの携帯番号は090-〇〇〇〇-〇〇〇〇です」と教えてくれる
 1. 090-〇〇〇〇-〇〇〇〇に電話をかける
 1. Bさんが「もしもし」と応答する

秘書がフルリゾルバで、社長やA部長やマネージャのCさんがネームサーバ、Bさんがウェブサーバに当てはまります。

== リソースレコード

ネームサーバのお腹の中にある電話帳は管理しやすいように「startdns.funの電話帳」「example.comの電話帳」のようにドメインごとに分かれています。この一冊一冊の電話帳が管理している範囲をゾーンと呼びます。

そしてこのゾーンの中にある「ドメインとIPアドレスの紐づけ」ひとつひとつのことをリソースレコードと呼びます。たとえばstartdns.funのゾーンの中には「startdns.funとそれに紐づくIPアドレス」や「www.startdns.funとそれに紐づくIPアドレス」、「staging.startdns.funとそれに紐づくIPアドレス」のようにたくさんのリソースレコードを書くことができます。

先ほどの会社の例で言うと、A部長はA部門というゾーンを管理していて、マネージャのCさんはBさんというゾーンを管理してます。そしてCさんが管理するBさんというゾーンの中には「Bさんの携帯番号は090-〇〇〇〇-〇〇〇〇」や「Bさんの自宅番号は03-〇〇〇〇-〇〇〇〇」といったリソースレコードがあるのです。

リソースレコードには次（@<table>{resourceRecord}）のようにAレコードやMXレコードといった種類があり、それぞれ書き方も決められています。

//table[resourceRecord][リソースレコードの種類]{
リソースレコードのタイプ	値の意味
------------------------------------
Aレコード	ドメインに紐づくIPアドレス（例：ウェブサーバ）
NSレコード	ドメインのゾーンを管理するネームサーバ
MXレコード	ドメインに紐づくメール受信サーバ
TXT（SPF）	このドメインのメール送信元サーバ
SOA	ドメインのゾーンの管理情報
CNAME	このドメインの別名でリソースレコードの参照先
//}

それぞれどういうときに使うのかは@<chapref>{aws}や@<chapref>{dig}で具体例を見て、手を動かしながら確認していきましょう。