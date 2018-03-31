= AWSのネームサーバ（Route53）を使ってみよう

@<chapref>{domain}で自分のドメインを買って、@<chapref>{whatIsDns}ではDNSの仕組みを学びました。
@<chapref>{aws}ではAWSのRoute53（ルートフィフティースリー）というDNSサービスで自分のドメインのゾーンを作ってみましょう。

//pagebreak

== AWSとは

AWSとはアマゾン ウェブ サービス（Amazon Web Services）の略で、欲しいものをぽちっとなすると翌日には届くあのアマゾンがやっているクラウドです。

アマゾンがやっているクラウドと言われても、そもそもクラウドってなんだろう？という方もいますよね。クラウドとは「ブラウザ上でスペックを選んでぽちぽちっとするだけで誰でもすぐにウェブサーバやDBサーバを立てたりネームサーバを使ったりできるサービス」@<fn>{cloud}のことです。AWSは1秒単位の従量課金なので例えば3分使ったら3分ぶんのお金しかかかりません。

//footnote[cloud][クラウドについて真面目に説明するともう1冊本ができそうなので、ここでは正確さよりも分かりやすさを優先したゆるい説明にしています。]

今回はAWSの中でもRoute53というDNSのサービスを使用します。クラウドといえばGoogleのGoogle Cloud PlatformやMicrosoftのAzure（アジュール）、さくらインターネットのクラウドやGMOクラウドなどAWS以外にもたくさんあります。ですが現状のシェアトップはダントツでAWS@<fn>{cloudShare}です。AWSなら何か困って検索したときに出てくる情報も多いので、今回はAWSでドメインのゾーンを作成します。

//footnote[cloudShare][2017年時点、AWSはシェア全体の34%を占めクラウド界のトップを独走中です。]

== AWSアカウント作成

先ずはAWSのアカウントを作りますので次の2つを用意してください。

 * クレジットカード
 * 通話可能な携帯電話（電話番号認証で使用するため）

なおAWSを初めて使用する場合、利用料が1年分無料@<fn>{awsFree}となります。

//footnote[awsFree][無料利用枠の範囲が決まっており、何をどれだけ使っても無料という訳ではありませんので注意してください。たとえばこの後利用するRoute53というDNSのサービスは、1つのドメインにつき50セント/月かかります。詳細は@<href>{https://aws.amazon.com/jp/free/}を確認してください。]

「AWS 無料」（@<img>{searchAwsFree}）で検索して上から2つめの「AWS クラウド無料利用枠 | AWS - Amazon AWS」@<fn>{awsFreeUrl}をクリックします。

//image[searchAwsFree][「AWS 無料」で検索][scale=0.8]{
//}

//footnote[awsFreeUrl][@<href>{https://aws.amazon.com/jp/free/}]

「AWSクラウド無料利用枠」（@<img>{startAwsForFree}）のページを開いたら、中央にある「まずは無料で始める」をクリックします。

//image[startAwsForFree][「まずは無料で始める」をクリック][scale=0.8]{
//}

AWSアカウントの作成画面（@<img>{signupAws}）で次の4つを入力したら、「続行」をクリックします。後で分からなくならないように、何を登録したのかはメモしておいてください。

 * Eメールアドレス
 * パスワード
 * パスワードの確認
 * AWSアカウント名

//image[signupAws][メールアドレスとパスワードとAWSアカウント名を記入して「続行」を押す][scale=0.8]{
//}

連絡先情報（@<img>{registUserData01}・@<img>{registUserData02}）はすべて英語表記で登録します。今回は個人利用ですのでアカウントの種類は「パーソナル」を選択してください。住所と電話番号を記入したら、「AWSカスタマーアグリーメントの同意」にチェックを入れて「アカウントを作成して続行」を押します。

//image[registUserData01][連絡先情報は英語で登録][scale=0.8]{
//}

//image[registUserData02][「AWSカスタマーアグリーメントの同意」にチェックを入れて「アカウントを作成して続行」を押す][scale=0.8]{
//}

前述のとおり、AWSを初めて使用する場合は利用料が1年分無料なのですが、クレジットカードは登録しておく必要があります。支払情報（@<img>{paymentInfo01}・@<img>{paymentInfo02}）にクレジットカード情報を記入して「連絡先住所を使用する」を選択したら「セキュアな送信」を押します。

//image[paymentInfo01][クレジットカード情報を記入][scale=0.8]{
//}

//image[paymentInfo02][「請求先住所を選択して「セキュアな送信」を押す][scale=0.8]{
//}

電話による確認（@<img>{phoneCall}）で電話番号（例：090-1234-5678）とセキュリティチェックのCaptchaを入力したら、電話を手元に用意した状態で「すぐに連絡を受ける」を押します。

//image[phoneCall][電話番号とCaptchaを入力したら「すぐに連絡を受ける」を押す][scale=0.8]{
//}

このとき「エラー：お支払情報に問題があります」（@<img>{paymentError}）と表示されて、何度試しても電話確認に進めない場合があります。AWSでは初回登録時に「1ドル認証」と呼ばれる認証方法でクレジットカードが決済可能かをチェックしているのですが、クレジットカードによってはこの1ドル認証を不審な決済と判断して通さないため、それによってエラーが発生することがあるようです。その場合は別のクレジットカードで試すか、AWSのチャットサポートで問い合わせてみてください。

//image[paymentError][クレジットカードに起因するエラー][scale=0.8]{
//}

「すぐに連絡を受ける」を押すとすぐに通知不可能で電話（日本語の自動音声）がかかってくるので、パソコン側で表示されている4ケタの番号（@<img>{enterNumber}）を電話でプッシュしてください。

//image[enterNumber][パソコンで表示された4ケタの番号を電話でプッシュ][scale=0.8]{
//}

確認完了して電話が切れたら、パソコン側で「本人確認が終了しました」（@<img>{userCheckEnd}）と表示されるので「続行」を押します。

//image[userCheckEnd][「本人確認が終了しました」と表示されたら「続行」を押す][scale=0.8]{
//}

サポートプランの選択（@<img>{supportPlan}）はベーシックプランがよいので、いちばん左の「無料」を押します。

//image[supportPlan][サポートは無料のベーシックプランを選択][scale=0.8]{
//}

「アマゾン ウェブ サービスへようこそ」（@<img>{welcomeToAws}）と表示されたらAWSのアカウント作成は完了です。おめでとうございます！「コンソールにサインイン」ボタンを押して次のステップへ進みましょう。

//image[welcomeToAws][AWSのアカウント作成完了][scale=0.8]{
//}

=== AWSのマネジメントコンソールにログイン

@<href>{https://aws.amazon.com/}の「コンソールへサインイン」を押すと、マネジメントコンソールへのログイン画面が表示されます。（@<img>{signInMailAddress}）先ほど登録したAWSアカウントのEメールアドレスを入力して「次へ」を押します。

//image[signInMailAddress][AWSのアカウントのEメールアドレスを入力][scale=0.8]{
//}

続いてルートユーザーサインインの画面（@<img>{rootUserSignIn}）でパスワードを入力したら「サインイン」を押します。

//image[rootUserSignIn][パスワードを入力してサインイン][scale=0.8]{
//}

ログインするとマネジメントコンソールというAWSの管理画面（@<img>{managementConsole}）が表示されます。

//image[managementConsole][マネジメントコンソール（AWSの管理画面）][scale=0.8]{
//}

== お名前.comで買ったドメインのネームサーバをRoute53に変更

=== Route53でホストゾーンを作成

AWSのマネジメントコンソールで、左上の「サービス」を押すとAWSのサービス一覧（@<img>{serviceRoute53}）が表示されます。「ネットワーキング ＆ コンテンツ配信」というグループにあるRoute53を選択してください。

//image[serviceRoute53][サービス一覧からRoute53を選択][scale=0.8]{
//}

Route53を押すといくつかメニューが表示される（@<img>{route53Top}）ので、いちばん左側にあるDNS managementの「Get started now」を押してください。

//image[route53Top][DNS managementの「Get started now」を押す][scale=0.8]{
//}

Route53というネームサーバの中に自分のドメインのゾーンを作りたいので、ホストゾーンの画面（@<img>{hostedZones}）が表示されたら「Create Hosted Zone」押してください。

//image[hostedZones][「Create Hosted Zone」を押す][scale=0.8]{
//}

まだゾーンがひとつもないため「You have no hosted zones」と表示（@<img>{youHaveNoHostedZones}）されます。再び「Create Hosted Zone」ボタンを押してください。

//image[youHaveNoHostedZones][「You have no hosted zones」と表示されたら再び「Create Hosted Zone」を押す][scale=0.8]{
//}

ホストゾーンの作成画面（@<img>{createHostedZones}）を開いたらDomain Nameのところに先ほどお名前.comで買った自分のドメインを書きます。私はstartdns.funというドメインを買ったので、Domain Nameのところにstartdns.funと書きました。

TypeはPublic Hosted Zoneを選択（@<table>{hostedZones}）します。「Public Hosted Zone」にしておけば外から「このドメインに紐づいてるIPはなに？」と聞かれたときに「IPはこれだよ」と返事をします。ここを「Private Hosted Zone for Amazon VPC」にしてしまうと、AWS内での名前解決しか出来なくなり外から聞かれても何も答えてくれなくなってしまいますので、必ず「Public Hosted Zone」にしておいてください。入力完了したら「Create」を押します。

//table[hostedZones][ホストゾーン作成時の設定]{
項目	入力するもの
------------------------------------
Domain Name	お名前.comで買った自分のドメイン
Comment	何も入力しない
Type	Public Hosted Zone
//}

//image[createHostedZones][お名前.comで買った自分のドメインを書く][scale=0.8]{
//}

これでRoute53というネームサーバの中にゾーンが出来て、ゾーンの中にドメインのネームサーバを示すNSレコード（@<img>{nsRecord}）と管理情報を示すSOAレコードというリソースレコードも出来ました。

//image[nsRecord][ゾーンとその中にリソースレコードが出来た][scale=0.8]{
//}

=== 自分のドメインのネームサーバが何か確認

ではこれで自分のドメインのネームサーバがRoute53になったのか、ちょっと確認してみましょう。

ブラウザの別タブで「dig」を検索（@<img>{searchDig}）して、一番上の「nslookup(dig)テスト【DNSサーバ接続確認】」@<fn>{cmanUrl}というページを開きます。

//footnote[cmanUrl][@<href>{https://www.cman.jp/network/support/nslookup.html}]

//image[searchDig][「dig」で検索][scale=0.8]{
//}

このサイトでは自分のドメインのネームサーバが今何になっているのか、現状を確認できます。では「nslookup(dig)テスト【DNSサーバ接続確認】」のページ（@<img>{digStartDnsFunByCman}）で「ホスト名(FQDN)を指定してください」という欄に自分のドメインを入力して、オプションで「NS：正式なネームサーバ」を選択したら「dig実行」を押してください。

//image[digStartDnsFunByCman][自分のドメインを入力してネームサーバを調べる][scale=0.8]{
//}

ちょっと下にスクロールして確認結果のANSWER SECTIONというところを見てください。次のように表示されましたか？

//quote{
;; ANSWER SECTION:

startdns.fun.       300 IN NS   dns1.onamae.com.

startdns.fun.       300 IN NS   dns2.onamae.com.
//}

ANSWER SECTIONは名前のとおり「startdns.funのNSレコードを教えて！」という質問に対する答えです。NSレコードは「このドメインのゾーンはこのネームサーバにあるよ」という意味なので、「startdns.funというドメインのゾーンはお名前.comのネームサーバの中にあるよ」と言っています。

なんということでしょう。AWSのRoute53というネームサーバの中にゾーンを作ったのに、ドメインのネームサーバはまだお名前.comのままになっています。これは、次のような流れになっているからです。

 1. フルリゾルバにstartdns.funのネームサーバ（NSレコード）を聞きに行く
 1. フルリゾルバは自分の中にstartdns.funに紐づくネームサーバのキャッシュがないことを確認
 1. フルリゾルバはルートネームサーバにstartdns.funのネームサーバを聞きに行く
 1. ルートネームサーバは「.funのネームサーバはa.nic.funだよ」と教えてくれる
 1. フルリゾルバはa.nic.funにstartdns.funのネームサーバを聞きに行く
 1. @<b>{a.nic.funは「startdns.funのネームサーバはdns1.onamae.comだよ」と教えてくれる}
 1. フルリゾルバはdns1.onamae.comにstartdns.funのネームサーバ（NSレコード）を聞きに行く
 1. dns1.onamae.comは「startdns.funのネームサーバはdns1.onamae.comとdns2.onamae.comだよ」と教えてくれる
 1. フルリゾルバが「startdns.funのネームサーバはdns1.onamae.comとdns2.onamae.comです」と教えてくれる

//image[whichOneIsMyNameServer][自分のドメインを入力してネームサーバを調べる][scale=0.8]{
//}

お名前.comのネームサーバとRoute53のネームサーバ、どちらにもstartdns.funのゾーンがあるのですが、上位ネームサーバのa.nic.funで「startdns.funのネームサーバはdns1.onamae.com」という設定になっているので、Route53のネームサーバには流れてこないのです。

そもそもお名前.comでドメインを買うと、デフォルトの設定でネームサーバには次の2つが設定されています。

 * dns1.onamae.com.
 * dns2.onamae.com.

そのためネームサーバをRoute53に変更したければ、Route53でゾーンを用意するだけでなくお名前.comの管理画面で「ゾーンはお名前.comにある」という設定を「ゾーンはRoute53にある」という設定に書き換えなければいけないのです。

先ほどのマネジメントコンソールに戻って（@<img>{nsRecord}）NSレコードのValueを見てみましょう。次のような4つのネームサーバが表示されていると思います。@<fn>{route53NameServer}この4つは後ほど必要になりますのでパソコンのメモ帳に書き留めておいてください。

 * ns-943.awsdns-53.net.
 * ns-1605.awsdns-08.co.uk.
 * ns-1072.awsdns-06.org.
 * ns-177.awsdns-22.com.

//footnote[route53NameServer][数字やTLDは人によって異なります。]

//image[nsRecord][NSレコードのValueに書かれたネームサーバをメモしておく][scale=0.8]{
//}

=== ネームサーバをお名前.comからRoute53に変更

それではドメインのネームサーバをRoute53に変更する設定を行いましょう。ブラウザの別タブでお名前.comのトップページ@<fn>{onamaeUrl}を開いて右上の「ドメインNavi ログイン」をクリックしてください。（@<img>{onamaeTopPageAgain}）

//footnote[onamaeUrl][@<href>{https://www.onamae.com/}]

//image[onamaeTopPageAgain][右上の「ドメインNavi ログイン」をクリック][scale=0.8]{
//}

ログイン画面（@<img>{domainNaviLogin}）が表示されたら、ドメインを買ったときに発行されたお名前ID@<fn>{forgetOnamaeId}とパスワードを入力し、「ログイン」を押して管理画面のドメインNaviにログインします。

//footnote[forgetOnamaeId][もしお名前IDを忘れてしまったらドメイン登録したときに届くメールに記載されています。]

//image[domainNaviLogin][お名前IDとパスワードを入れてドメインNaviにログイン][scale=0.8]{
//}

ログインするとドメインNaviのトップページではなく「ドメイン契約更新」の画面（@<img>{continueContract}）が表示されます。@<fn>{abTestAgain}

//footnote[abTestAgain][ABテストの影響でログインするお名前IDによってドメインNaviの見た目が異なるかもしれません。見た目に関わらず管理画面でやりたいことは同じで「ドメインのネームサーバを、デフォルトのお名前.comからRoute53に変更したい」だけです。]

//image[continueContract][ドメインNavi ドメイン契約更新][scale=0.8]{
//}

「ドメイン契約更新」の画面が表示されるのは、ドメインNaviがログインするたびに「来年の更新をしませんか？」と聞いてくる仕様だからです。繰り返しますが有効期限は大分先ですので無視して先へ進みましょう。先ほど買ったばかりのドメインの設定変更がしたいので、上部のメニューで左から2つ目の「ドメイン」をクリックしてください。

他のページへ移動しようとすると「更新手続きをお忘れではございませんか？（中略）更新が必要なドメインはお早めにお手続きされることをお勧めいたします。」と警告が出ますが（@<img>{contractRemind}）、前述のとおり有効期限は1年も先です。問題ありませんので「このページを離れる」を押してください。

//image[contractRemind][ドメインNavi 更新アラート][scale=0.8]{
//}

ドメイン一覧（@<img>{domainNaviDomainList}）には自分が買ったドメイン（私だったらstartdns.fun）があります。買ったばかりのドメインはネームサーバが「初期設定」になっているので、そこをクリックして「ネームサーバー設定」の画面に進みます。

//image[domainNaviDomainList][ドメインNavi ドメイン一覧][scale=0.8]{
//}

ネームサーバー設定の画面（@<img>{nameServerSetting01}）を開いたら、少し下の「2.ネームサーバーの選択」までスクロールしてください。

//image[nameServerSetting01][ドメインNavi ネームサーバー設定][scale=0.8]{
//}

変更前（@<img>{nameServerSetting02}）は「お名前.comのネームサーバー」になっているので「その他のネームサーバー」というラジオボタンを選択（@<img>{nameServerSetting03}）します。「ネームサーバー情報を入力」と書いてあるところに、さきほどメモしたRoute53のネームサーバを1行ずつ書いてください。初めは入力欄が3つしかないですが＋を押すと追加できます。4つとも入力出来たら「確認」を押します。

//image[nameServerSetting02][2.ネームサーバーの選択 変更前][scale=0.8]{
//}

//image[nameServerSetting03][2.ネームサーバーの選択 変更後][scale=0.8]{
//}

確認画面（@<img>{confirmNameServerSetting}）が出てくるので、ネームサーバ情報がRoute53のネームサーバになっていることを確認して「OK」を押します。

//image[confirmNameServerSetting][ネームサーバの設定確認][scale=0.8]{
//}

「完了しました。」（@<img>{nameServerSettingFinish}）と表示されたらネームサーバの設定変更は完了です。ネームサーバの設定変更が完了すると、少し経ってから「[お名前.com]ネームサーバー情報変更 完了通知」という件名のメールが届きます。

//image[nameServerSettingFinish][ネームサーバの設定変更完了][scale=0.8]{
//}

=== TTLが過ぎるまではネームサーバが切り替わらない

ところで今、ネームサーバをお名前.comからRoute53に変更しましたが、古い設定のTTLが過ぎるまでフルリゾルバにはキャッシュが残っています。

TTLとはTime To Liveの略でキャッシュ保持時間のことです。先ほど「nslookup(dig)テスト【DNSサーバ接続確認】」でNSレコードを確認したとき、ANSWER SECTIONに表示されていた「300」というのがTTLの値です。

//quote{
;; ANSWER SECTION:

startdns.fun.       300 IN NS   dns1.onamae.com.

startdns.fun.       300 IN NS   dns2.onamae.com.
//}

TTLに300と書かれているので、最大で300秒＝5分待てばネームサーバはRoute53に切り替わるはずです。再び「nslookup(dig)テスト【DNSサーバ接続確認】」で「ホスト名(FQDN)を指定してください」という欄に自分のドメインを入力（@<img>{digStartDnsFunByCman}）して、オプションで「NS：正式なネームサーバ」を選択したら「dig実行」を押してください。

//image[digStartDnsFunByCman][再び自分のドメインのネームサーバを調べる][scale=0.8]{
//}

ちょっと下にスクロールして確認結果のANSWER SECTIONというところを見てください。次のように表示されていればネームサーバはちゃんとRoute53へ切り替わっています。

//quote{
;; ANSWER SECTION:

startdns.fun. 3599 IN NS ns-1072.awsdns-06.org.

startdns.fun. 3599 IN NS ns-1605.awsdns-08.co.uk.

startdns.fun. 3599 IN NS ns-177.awsdns-22.com.

startdns.fun. 3599 IN NS ns-943.awsdns-53.net.
//}

よくDNSの変更は「浸透に時間がかかる」と言われますが、これはTTLに指定された時間が過ぎるまではフルリゾルバに古いリソースレコードのキャッシュが残っているため、新しいリソースレコードの情報が反映されない、ということです。人によって異なるフルリゾルバを使っているため、たとえばサイトリニューアルでAレコードを書き換えたときに、Web制作会社のA社は新しいサイトが見られるがクライアントのB社ではまだ古いサイトが表示される、といった現象が起こります。

@<comment>{※ここにフルリゾルバがキャッシュのTTL過ぎるまでは腹の中のキャッシュ情報を返す図をいれたい。}

このように「浸透」はただ闇雲に待つ必要はありません。事前にTTLを把握しておけば、最大でどれだけ待てば新しいリソースレコードの情報に切り替わるのか分かりますし、事前にTTLを短くしておくことで反映にかかる時間を短くすることもできます。逆に当面は書き換える予定がないリソースレコードについては、TTLを長くしておけば都度ネームサーバまで聞きに来なくてよいのでキャッシュヒット率が上がりますし、万が一ネームサーバが死んでしまったときもしばらくはキャッシュでしのげるので影響範囲を狭めることができます。

どうしてもキャッシュを今すぐ消したい！という場合は、もし情シスが自社のフルリゾルバを管理しているのであれば「フルリゾルバのキャッシュをクリアして！」と頼めばキャッシュがなくなって、ネームサーバへ改めてリソースレコードを問い合わせに行ってくれます。

=== 【ドリル】ネームサーバを変えること≠レジストラを変えること

==== 問題

あなたは今、ドメインのネームサーバをデフォルト設定のお名前.comから、Route53のネームサーバに変更しました。1年後に「ドメインがもうすぐ有効期限を迎えるので更新しましょう！」と連絡してくるのはどちらですか？

 * A. AWSのRoute53
 * B. お名前.com

//raw[|latex|\begin{reviewimage}\begin{flushright}\includegraphics[width=0.5\maxwidth\]{./images/answerColumnShort.png}\end{flushright}\end{reviewimage}]

==== 解答

正解はBです。今行った作業はドメインのネームサーバを、デフォルト設定のお名前.comからAWSのRoute53というネームサーバに変更しただけです。レジストラ（ドメインを購入するお店）をお名前.comからAWSのRoute53に変更する「レジストラ移管」を行ったわけではないので、1年後に更新の連絡をしてくるのもお名前.comです。

Route53のRegistered domainsというメニューからドメインを購入することもできますがお名前.comに比べると値段が高いので、Route53のネームサーバを使いたいだけであれば、本著のようにお名前.comで買ってRoute53のネームサーバを使う方がお勧めです。