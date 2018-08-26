# おおまかなセクション
- はじめに
  - 対象読者
  - 必須知識
  - 推奨知識
  - 想定環境
  - お問い合わせ先
  - 免責事項
- 目次
- iOSアプリ開発とSwift
- RxSwift入門
  - 覚えておきたい用語と１業概要
  - RxSwiftって何？
  - ReactiveExtensionって何？
    - 思想
    - 歴史
  - iOSアプリ開発とSwift
  - RxSwiftとは　概要
  - RxSwiftの特徴
  - RxSwiftで何が解決できる？
  - 導入事例
- RxSwiftの導入方法
  - 導入要件
- 基本的な書き方
  - Hello world
- 応用
- サンプルアプリのパターン比較
  - ゴリゴリパターン
  - delegateパターン
  - KVOパターン
  - RxSwiftパターン
- 次のステップ
  - 学習方法
  - コミュニティ
- 様々なRxSwift系ライブラリ
  - RxOptional
  - RxWebkit
  - RxDataSources

# 書かない


# メリデメ
## メリット
- 時間経過に関する処理をシンプルに書ける
- ViewControllerの呼び出し側で処理が書ける
- 非同期処理のコールバック地獄をシンプルに書ける
- コード全体が一貫する
- まとまった流れが見やすい
- 差分がわかりやすい
- スレッドを買えやすい

## デメリット
- デバッグしにくい
- 学習コストが高い
- 一度errorが発生すると止まってしまう
  - UIとバインドするような時は止まってしまうと困るので、errorが流れないものを使う
- 簡単な処理で使うと長くなりがち

# 文章構成

##### ここまでメモ #####

# はじめに

## 対象読者
本書は次の読者を対象として作成しています。 

- プログラミング歴1年以上（種類問わず）
- Swift による iOS アプリの開発経験が少しだけある（3ヶ月〜1年未満）
- RxSwiftライブラリを使った開発をしたことが全くない・ほんの少しだけある

## 必須知識
- Swiftの基本的な言語仕様
  - if, for, switch, enum, class, struct
- Xcode の基本的な操作
- よく使われる UIKitの仕様
  - UILabel UITextView UITableView UICollectionView

## 推奨知識

- mapやfilterなどの高階関数の扱い
- 設計パターン
  - MVVMアーキテクチャ
- デザインパターン
  - delegate パターン
  - KVO パターン
  - オブザーバパターン

## 想定環境
- OSX High Sierra
- Xcode 9.4
- Swift 4.1
- cocoapods 1.5.3

## お問い合わせ先

- Twitter
  - @k0uhashi

## 免責事項

# 目次


# iOSアプリ開発とSwift

iOSのアプリ開発において、いまではほぼSwift一択の状況ではないでしょうか？
Swiftが登場したころからStoryboardの機能も充実し、UIと処理の分けてさらに書きやすくなりました。
Objective-Cを使っていたころよりもアプリ開発が楽になり、アプリ開発初心者でもかなりとっつきやすくなったのがわかります。

ですが、かなりとっつきやすくなったと言ってもまだ問題はいくつかあります。
例えば、「非同期処理が実装しにくい、読みにくい」「通信処理の成功・失敗の制御」「DelegateやaddTarget, IBAction等、動作するところと処理が離れている」などあります。
これを解決するのが、RxSwiftです。

では具体的にどう解決できるのか簡単なサンプルを例に出しながら解説します。

# RxSwift入門

## 覚えておきたい用語と１行概要

- Reactive Extensions
  - GoFのデザインパターンの１つ、 `オブザーバパターン` を表したインターフェース
- RxSwift
  - ReactiveExtensionsのSwift版のライブラリ
- RxCocoa
  - UIKitでRxを使えるように様々なクラスをextensionで定義してくれているライブラリ
- オブザーバパターン
  - プログラム内のオブジェクトのイベント（事象）を他のオブジェクトへ通知する処理で使われるデザインパターンの一種


## RxSwiftって何？

RxSwift とはReactive ExtensionsのSwift版です
Reactive Extensionsについては後述しますが非同期操作とイベント/データストリームの実装を容易にできるライブラリのことを指します

## Reactive Extensionって何？

### 思想
Reactive Extensionとは、`Reactive Programming` を実現するための`デザイン`とその`実装`ができる`ライブラリ`のことを指します。

### 歴史
元々は `Microsoft` が研究して開発した `.NET用のライブラリ` でしたが、これがとても有用な概念だったため `JavaScript` や `Java`, `Swift` など、垣根を越えて様々な言語に移植されています。

本書では RxSwift について解説しますが世の中には `RxJava`, `RxJS`, `RxScala` など様々なライブラリがあります。
どのライブラリも概念はおおまかな考え方は一緒です。概念だけでも１度覚えておくと他の言語でもすぐに扱えるようになるためこの機会にぜひ覚えてみましょう！

## RxSwiftの特徴

WIP

## RxSwiftで何が解決できる？

まず１番わかりやすくて簡単なのは「DelegateやIBActionだと動作するところと処理が離れている」の解決です。

UIButtonとUILabelが画面に配置されていて、ボタンをタップすると文字列が変更されるという仕様のアプリをIBActionを使って作ってみましょう。

画面

simpletap1.png
simpletap2.png

### RxSwiftを使わないコード

class SimpleTapViewController: UIViewController {
    
    @IBOutlet weak var messageLabel: UILabel!
    
    @IBAction func buttonTap(_ sender: Any) {
        messageLabel.text = "Changed!!"
    }
}

通常の書き方だと、１つのボタンに対して１つの関数を定義します。
この場合だとUIと処理が1対1で非常に強い結合度になり、
ボタンを１つ増やすたびに対応する関数が１つずつ増えていき、コード量が次第に大きくなってしまいます。

次に、RxSwiftを用いて書いてみます。

### RxSwiftを使ったコード

class SimpleTapViewController: UIViewController {
    
    @IBOutlet weak var tapButton: UIButton!
    @IBOutlet weak var messageLabel: UILabel!
    
    private let disposeBag = DisposeBag()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tapButton.rx.tap
            .subscribe(onNext: { [weak self] in
                self?.messageLabel.text = "Changed!!"
            })
            .disposed(by: disposeBag)
    }
}

RxSwiftで書くと、UIと処理を分けて書くことができます。
１つのボタンと１つの関数が強く結合していたのが、１つのボタンと１つのプロパティの結合ですむようになります
ボタンを１つ増やすたびに対応するプロパティが１行増えるだけなので、コードがとてもシンプルになります。
また、画面上のUIを変更してもソースコードへの影響は少なくなるので楽になります。

addTargetを利用するパターンも見てみましょう

UILabel, UITextFieldを画面に２つずつ配置し、入力したテキストをバリデーションしてUILabelに反映する機能をイメージして作ってみます

画面

simpletextfieldlabelexample1.png

文字を入力するたびに「あとN文字」と表示してくれるUIを作ります

### addTarget を用いたコード

class ExampleViewController: UIViewController {
    
    @IBOutlet weak var nameField: UITextField!
    @IBOutlet weak var nameLabel: UILabel!
    
    @IBOutlet weak var addressField: UITextField!
    @IBOutlet weak var addressLabel: UILabel!

    let maxNameFieldSize = 10
    let maxAddressFieldSize = 50
    
    let limitText: (Int) -> String = {
        return "あと\($0)文字"
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        nameField.addTarget(self, action: #selector(nameFieldEditingChanged(sender:)), for: .editingChanged)
        addressField.addTarget(self, action: #selector(addressFieldEditingChanged(sender:)), for: .editingChanged)
    }

    @objc func nameFieldEditingChanged(sender: UITextField) {
        guard let changedText = sender.text else { return }
        let limitCount = maxNameFieldSize - changedText.count
        nameLabel.text = limitText(limitCount)
    }
    
    @objc func addressFieldEditingChanged(sender: UITextField) {
        guard let changedText = sender.text else { return }
        let limitCount = maxAddressFieldSize - changedText.count
        addressLabel.text = limitText(limitCount)
    }
}

UIと処理のコードが離れているので、パッとじゃ処理のイメージがしにくいですね。
対象のViewがもっと増えるとどの関数がどのUIの処理なのかわかりにくくなってしまいます。

次にRxSwiftを用いて書いてみます

### RxSwift version

class RxExampleViewController: UIViewController {
    
    // フィールド宣言は全く同じなので省略
    
    private let disposeBag = DisposeBag()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        nameField.rx.text
            .map { [weak self] text -> String? in
                guard let text = text else { return nil }
                guard let maxNameFieldSize = self?.maxNameFieldSize else { return nil }
                let limitCount = maxNameFieldSize - text.count
                return self?.limitText(limitCount)
            }
            .filterNil()
            .observeOn(MainScheduler.instance)
            .bind(to: nameLabel.rx.text)
            .disposed(by: disposeBag)
        
        addressField.rx.text
            .map { [weak self] text -> String? in
                guard let text = text else { return nil }
                guard let maxAddressFieldSize = self?.maxAddressFieldSize else { return nil }
                let limitCount = maxAddressFieldSize - text.count
                return self?.limitText(limitCount)
            }
            .filterNil()
            .observeOn(MainScheduler.instance)
            .bind(to: addressLabel.rx.text)
            .disposed(by: disposeBag)
    }
}

先程のaddTargetのパターンと全く同じ動作をします。
全ての処理がviewDidLoad()上で書けるようになり、UIと処理がバラバラにならないのですごく見やすいですね。
慣れていない方はまだ少し読みにくいかもしれませんが、Rxの書き方に慣れるとすごく読みやすくなります。

## 導入事例

# RxSwiftの導入方法



