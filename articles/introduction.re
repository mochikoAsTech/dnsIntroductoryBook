= はじめに

//flushright{

2018年4月
mochikoAsTech

//}

この本を手に取ってくださったあなた、はじめまして。
「DNSをはじめよう」の筆者、mochikoAsTechです。

DNSは好きですか？筆者はDNSが大好きです！なぜか分からないけど、DNS絡みの障害が起きたニュースを見てはわくわくして勝手に調査してしまうし、DNSのことで困っている人がいると解説したくてうずうずします。

普段、システムの開発や運用をされているWebアプリケーションエンジニアを料理人に例えると、インフラエンジニアは台所そのものを作るのが仕事です。料理人は料理を作るのは得意でいつも台所にいますが、台所そのものの作り方まで詳しいか？というと、必ずしもそうとは言えません。（もちろん家の建て方から、料理から、テーブルコーディネイトから、後片付けまで全部出来るフルスタックなエンジニアもいますが、みんながみんなそうではなく、分担しあって頑張っていますよね）

台所の造りに詳しくなくても料理は作れます。ですが台所の造りに詳しいと、困ったときに「自分でなんとかできる範囲」が広がります。例えばシンクの蛇口から水漏れして台所が水浸しになっても、水道管や蛇口のことをある程度知っていれば、先ずは元栓を閉める応急処置をしたり、あるいはパッキンを自分で交換して直すこともできます。修理を頼んだ場合でも、修理業者の説明が理解できるので金額の妥当性もちゃんと判断できます。

DNSは水道管のように地味なインフラで、蛇口を捻れば水が出るのは当たり前なのと同じように、ドメイン名を聞けばIPアドレスが返ってくるのは当たり前で意識されることすらありません。ですがひとたびDNSで問題があれば、サイトは見られなくなり、メールは送れなくなり、水が止まったのと同じくらい影響範囲の大きい障害となります。

筆者はインフラエンジニアを経験した後、現在のWebアプリケーションエンジニアにジョブチェンジしたので、インフラの知識が土台としてあったことでその上にアプリケーションの知識が載せやすく、この順番で学んでおいてよかったな、と思う場面が多々ありました。

「DNSをはじめよう」は、そんなDNSが好きで仕方ない著者が、普段開発や運用をしているエンジニアに「DNSのことを知っておくと自分でなんとか出来る範囲が広がるよ」と伝えたくて書いた一冊です。

== 想定する読者層

この本は、こんな人に向けて書かれています。

  * これからシステムやプログラミングを学ぼうと思っている新人
  * ウェブ系で開発をしているアプリケーションエンジニア
  * 「インフラよく分からないコンプレックス」がある人
  * ドメイン買ったりするけどDNSはあんまり分かっていない人
  * 自分のブログやポートフォリオサイトを独自ドメインで作ってみたい人
  * AWSやRoute53という単語に興味がある人

== この本の特徴

この本は実際にドメインを購入し、手を動かして試しながら学べるので理解しやすく、インフラ初心者でも安心して読み進められる内容です。

実際にありがちなトラブルをとり上げて、

  * こんな障害が起きたら原因はどう調べたらいいのか？
  * 問題をどう解決したらいいのか？
  * どうしたら事前に避けられるのか？

を解説するとともに、何度もコマンドをたたいて反復学習するためのドリルもついています。

== この本のゴール

この本を読み終わると、あなたはこのような状態になっています。

  * ドメインを買うときは何に注意してどこでどう買ったらいいか分かっている
  * Whois情報に何を登録すべきか分かっている
  * 問題が起きたときに黒い画面（ターミナル）でdigコマンドやwhoisコマンドを駆使して原因を調査できる
  * サイト移管時に「DNS浸透待ちで8～24時間くらいは切り替わりません」みたいなことを言わなくなる

== 免責事項

本著に記載されている内容は執筆者の所属する組織の公式見解ではありません。

また本著はできるだけ正確を期すように努めましたが、執筆者が内容を保証するものではありません。よって本著の記載内容に基づいて読者が行った行為、及び読者が被った損害について執筆者は何ら責任を負うものではありません。

不正確あるいは誤認と思われる箇所がありましたら、電子版については適宜改訂を行いますので執筆者までお知らせいただけますと幸いです。