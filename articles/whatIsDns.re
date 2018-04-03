= DNSの仕組み

ドメインを買ってウェブサーバにコンテンツを載せたらそれだけでサイトが見られるでしょうか？

いいえ、ドメインとウェブサーバがそれぞれ存在しているだけではサイトは見られません。ドメインとウェブサーバを紐づける必要があります。でもサーバはともかくとしてドメインは手で触れるようなものではないので、この2つをリアルに紐で結ぶことはできません。ドメインとサーバを紐づけるには一体どうしたらいいのでしょう？

そこで出てくるのがDNSです。この章ではDNSの仕組みをしっかり学んでいきましょう。

//pagebreak

== DNSとは

DNSサーバのDNSはDomain Name Systemの略で、日本語に直訳すると「ドメイン名の管理システム」といったところです。一口にDNSサーバと言っても、その実態は「ネームサーバ」と「フルリゾルバ」の2つに分かれています。異なる働きをする2つのサーバがどちらも「DNSサーバ」と呼ばれていることが、DNSを分かりにくくしている一因だと筆者は思います。DNSの仕組みの解説に入る前にきちんとこの2つを整理しておきましょう。

=== ネームサーバ

ネームサーバは「電話帳」のような役割を果たします。

電話帳には名前とそれに紐づく電話番号が書いてありますが、ネームサーバにはドメインとそれに紐づくIPアドレスが登録されています。

ネームサーバは「DNSコンテンツサーバ」「権威DNSサーバ」と呼ばれることもありますが、本著では統一してネームサーバと呼びます。

=== フルリゾルバ

フルリゾルバは「秘書」のような役割を果たします。

フルリゾルバに「このドメインに紐づくIPアドレスが知りたいの」と言うと、あちこちのネームサーバに聞きまわってIPアドレスを調べてきて教えてくれます。しかも一度調べると一定期間はそのドメインとIPアドレスの紐づけを記憶（キャッシュ）するため、もう1回同じことを聞くと今度はすぐに教えてくれます。

フルリゾルバは「DNSキャッシュサーバ」「フルサービスリゾルバ」と呼ばれることもありますが、本著では統一してフルリゾルバと呼びます。

あなたがブラウザでウェブサイトを見るときは必ずこのフルリゾルバにドメインの名前解決を頼んでいます。「でもそんなもの使う設定をした記憶ないけど・・・」と思われるかも知れませんが、会社のオフィスなら情シスが、家庭なら契約しているISP@<fn>{isp}がフルリゾルバを用意していて、自動で「このフルリゾルバを使ってね」と割り当てられているので意識していないだけです。

//footnote[isp][インターネットサービスプロバイダの略。インターネットへの接続サービスを提供する電気通信事業者のこと。]

情シスやISPがそれぞれのネットワーク内で提供しているフルリゾルバの他に、Google Public DNS@<fn>{googlePublicDns}やCloudflareの1.1.1.1@<fn>{cloudflare}のようにだれでも無料で使えるオープンリゾルバというものがあります。2013年8月にOCN@<fn>{ocn}のフルリゾルバが死んでOCNユーザがインターネットに接続できなくなる@<fn>{incident}障害が発生しました。そのとき「DNSサーバの設定を8.8.8.8にすれば直るよ」という情報がTwitterで出回りました（@<img>{twitter8888}）。

//footnote[googlePublicDns][@<href>{https://developers.google.com/speed/public-dns/}]
//footnote[cloudflare][@<href>{https://1.1.1.1/ja-jp/}]
//footnote[ocn][NTTコミュニケーションズが運営する日本最大級のインターネットサービスプロバイダ。]
//footnote[incident][ドメインからIPを引く名前解決ができないことでウェブサーバのIPが分からず、サイトが見られなくなったりゲームやアプリなどのサービスが使えなくなったりした、ということです。]

//image[twitter8888][DNSサーバを8.8.8.8にするとつながるというツイート][scale=0.8]{
//}

これは使用するフルリゾルバを故障中のOCNのものからGoogle Public DNSに変更したことで名前解決が出来るようになってサイトも見られるようになった、ということです。


フルリゾルバはもちろんサーバでも使っています。Linuxサーバなら/etc/resolv.confというファイルを見てみましょう。resolv.confのnameserverという項目で指定されているのがそのサーバのフルリゾルバです。

//cmd{
$ cat /etc/resolv.conf
options timeout:2 attempts:5
nameserver 172.31.0.2
//}

== お名前.comのページが表示されるまで

@<chapref>{domain}の最後で、自分が買ったドメインをブラウザで開いてみたところ、お名前.comの「このドメインは、お名前.comで取得されています。」（@<img>{onamaeCm}）というページが表示されました。

//image[onamaeCm][http://自分のドメイン/を開くとお名前.comの広告が出る][scale=0.8]{
//}

ブラウザでhttp://自分のドメイン/を開いたときに、このページが表示されるまでの流れは次（@<img>{afterBuyingDomain}）のようになっていました。

//image[afterBuyingDomain][ページが表示されるまでのフルリゾルバとネームサーバの仕組み][scale=0.8]{
//}

ブラウザのアドレスバーに自分のドメインを入れてEnterを押してからページが表示されるまでの一瞬の間に、なんとこれだけの名前解決が行われていたのです。

== ゾーンと委任

このようにDNSは「1つのネームサーバが何もかも知っている」という集中管理ではなく、「いくつかのネームサーバに聞けば答えにたどり着く」という管理権限の分散された仕組みになっています。先ほどの名前解決を会社に例えると次のようになります。

//image[secretaryLikeNameServer][Bさんに電話をかけるまでの流れ][scale=0.8]{
//}

秘書がフルリゾルバで、社長やA部長やマネージャのCさんがネームサーバ、Bさんがウェブサーバに当てはまります。

社長がA部門のことはA部長に任せていたように、ルートネームサーバは.funのことはa.nic.funに任せていました。このとき「A部門」や「.fun」、「Bさん」や「startdns.fun」のような範囲をゾーンと呼びます。

A部長は自分が任されているA部門の中で、BさんについてはCマネージャに管理を任せていました。同様にa.nic.funというネームサーバは、自分が任されている.funというゾーンの中で、.funのサブドメインにあたるstartdns.funというゾーンについてはdns1.onamae.comに任せていました。

このように自身が任されているゾーンを分割して、その一部のゾーンを他のネームサーバに任せることを委任と呼びます。

つまり社長がA部門というゾーンをA部長に委任していたように、ルートネームサーバは.funというゾーンをa.nic.funに委任していた、ということです。ゾーンを委任されているネームサーバは、そのドメインについて権威を持つので、サブドメインを作ったり、任されたゾーンをさらに分割して他のネームサーバに委任したりできます。

== リソースレコード

ネームサーバのお腹の中にある電話帳は管理しやすいように「startdns.funの電話帳」「example.comの電話帳」のようにドメインごとに分かれています。この一冊一冊の電話帳が管理している範囲を前述のとおりゾーンと呼びます。

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

それぞれのリソースレコードをどういうときに使うのか？については@<chapref>{aws}や@<chapref>{dig}で具体例を見て、手を動かしながら確認していきましょう。
