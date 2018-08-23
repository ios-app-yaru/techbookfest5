# おおまかなセクション
- RxSwift入門
  - 対象読者
  - 必須知識
  - 想定環境
  - ReactiveExtensionとは 概要
  - RxSwiftとは　概要
  - RxCocoaとは　概要
  - どこが便利なの？
  - どこで使えるの？
  - どう書くの？
- サンプルアプリのパターン比較
  - ゴリゴリパターン
  - delegateパターン
  - KVOパターン
  - RxSwiftパターン

# 書かない

- RxSwiftの基本的な知識
  - 公式ドキュメントが１番わかりやすい

# 文章構成

##### ここまでメモ #####

# この本で書くこと

# 対象読者
本書は次の読者を対象として作成しています。 

- プログラミング歴1年以上（種類問わず）
- Swift による iOS アプリの開発経験が少しだけある（3ヶ月〜1年未満）
- RxSwiftライブラリを使った開発をしたことが全くない・ほんの少しだけある

# 必須知識
- Swiftの基本的な言語仕様
  - if, for, switch, enum, class, struct
- Xcode の基本的な操作
- よく使われる UIKitの仕様
  - UILabel UITextView UITableView UICollectionView

# 推奨知識
- mapやfilterなどの高階関数の扱い
- 設計パターン
  - MVVMアーキテクチャ
- デザインパターン
  - delegate パターン
  - KVO パターン
  - オブザーバパターン

# 想定環境
- OSX High Sierra
- Xcode 9.4
- Swift 4.1
- cocoapods 1.5.3

# 覚えておきたい用語と１行概要

- Reactive Extensions
  - GoFのデザインパターンの１つ、 `オブザーバパターン` を表したインターフェース
- RxSwift
  - ReactiveExtensionsのSwift版のライブラリ
- RxCocoa
  - UIKitでRxを使えるように様々なクラスをextensionで定義してくれているライブラリ
- オブザーバパターン
  - プログラム内のオブジェクトのイベント( 事象 )を他のオブジェクトへ通知する処理で使われるデザインパターンの一種


# RxSwiftって何？

RxSwift とはReactive ExtensionsのSwift版です
Reactive Extensionsについては後述しますが非同期操作とイベント/データストリームの実装を容易にできるライブラリのことを指します

# Reactive Extensionって何？

Reactive Extensionとは、`Reactive Programming` を実現するための`デザイン`とその`実装`ができる`ライブラリ`のことを指します。
元々は `Microsoft` が研究して開発した `.NET用のライブラリ` でしたが、これがとても有用な概念だったため `JavaScript` や `Java`, `Swift` など、垣根を越えて様々な言語に移植されています。

本書では RxSwift について解説しますが世の中には `RxJava`, `RxJS`, `RxScala` など様々なライブラリがあります。
どのライブラリも概念はおおまかな考え方は一緒です。概念だけでも１度覚えておくと他の言語でもすぐに扱えるようになるためこの機会にぜひ覚えてみましょう！



# どこが便利なの？

# どこで使えるの？

# どう書くの？

# 目次

# 初めてのRxSwift


