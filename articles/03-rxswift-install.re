= RxSwiftの導入

== 導入要件

RxSwiftリポジトリより引用（2018年8月31日現在）

  * Xcode 9.0
  * Swift 4
  * Swift 3.x （rxswift-3.0 ブランチを指定)
  * Swift 2.3 （rxswift-2.0 ブランチを指定)

== 導入方法

RxSwiftの導入方法はCocoaPodsやCarthage、SwiftPackageManager等いくつかありますが、ここでは１番簡単でよく使われる（著者の観測範囲）CocoaPodsでの導入方法を紹介します。

CocoaPodsとは、iOS/Mac向けのアプリを開発する際のライブラリ管理をしてくれるツールのことで、これを使うと外部ライブラリが簡単に導入できます@<br>{}
CocoaPodsを導入するにはRubyが端末にインストールされてる必要があります。（Macではデフォルトで入っているのであまり気にしなくてもよいですが）

次のコマンドでCocoaPodsを導入できます

//cmd{
gem install cocoapods
gem install -v 1.5.3 cocoapods = バージョンを本書と同じにしたい場合はコッチ
//}

これでCocoaPodsを端末に導入することができました。

次に、CocoaPodsを用いて、プロジェクトに外部ライブラリを導入してみます。@<br>{}
大まかな流れは次のとおりです。

  1. Podfileというファイルを作成
  2. Podfileに導入したいライブラリを定義
  3. コマンドラインで pod install と入力

では、実際にやってみましょう。

//cmd{
# プロジェクトのルートディレクトリで実行
vi Podfile
//}

//listnum[podfile-install][Podfile][ruby]{
# Podfile
use_frameworks!

target 'YOUR_TARGET_NAME' do
    pod 'RxSwift',    '~> 4.0'
    pod 'RxCocoa',    '~> 4.0'
end
//}

@<code>{YOUR_TARGET_NAME} は各自のプロジェクト名に置き換えてください

//cmd{
# プロジェクトのルートディレクトリで実行
pod install
//}

@<code>{Pod installation complete!} というメッセージが出力されたら導入成功です！

もし導入できていなさそうな出力であれば、書き方やtypoをもう一度確認してみてください。

==== Tips: Podfile

PodfileはRubyのまったく同じ構文で定義されています。@<br>{}
そのため、シンタックスハイライトにRubyを指定してあげることで、正しく表示してくれます。

Podfileでは、導入するライブラリのバージョンを指定することができ、本書でもバージョンを固定しています。@<br>{}
しかし、基本的にはバージョンを固定せず、ライブラリを定期的にバージョンアップさせることが推奨されています。@<br>{}
プロダクション環境で使う場合には、Podfileは次のように定義しましょう。

//emlist[]{
  pod 'RxSwift'
  pod 'RxCocoa'
//}
