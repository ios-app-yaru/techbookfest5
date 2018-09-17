= RxSwift入門

== 覚えておきたい用語と１行概要

* Reactive Extensions
** GoFのデザインパターンの１つ、 `オブザーバパターン` を表したインターフェース
* RxSwift
** ReactiveExtensionsをSwiftで扱えるように拡張されたライブラリ
* RxCocoa
** UIKitでRxを使えるように様々なUIクラスをextension定義しているライブラリで、RxSwiftとにこいちでよく導入されます。
* オブザーバパターン
** プログラム内のオブジェクトのイベント（事象）を他のオブジェクトへ通知する処理で使われるデザインパターンの一種

== RxSwiftって何？

RxSwift とは「ReactiveExtensions」をSwiftで扱えるように拡張されたライブラリのことを指します。
github上でオープンソースライブラリと公開されていて様々な人が日々コントリビュートしています。

Reactive Extensionsについては後述しますが非同期操作とイベント/データストリーム（時系列処理）の実装を容易にできるライブラリのことを指します

== Reactive Extensionsって何？

=== 思想

Reactive Extensionsとは、「Reactive Programming」を実現するための「デザイン」とその実装ができる「ライブラリ」のことを指します。名前の通り、Reactive Programmingをするために既存のプラットフォームの機能を拡張します。

=== 歴史

元々は `Microsoft` が研究して開発した `.NET用のライブラリ` で、２００９年に「Reactive Extensions」という名前で公開しました。現在はオープンソース化され「ReactiveX」という名前に変更されています。
この「ReactiveExtensions」の考え方がとても有用だったため JavaScriptやJava、Swiftなど、垣根を越えて様々な言語に移植されていて、その中の１つが本書で紹介する「RxSwift」です。

本書では RxSwiftと関連するライブラリ群についてのみ解説しますが世の中には `RxJava`, `RxJS`, `RxScala` など様々なライブラリがあります。
どのライブラリも概念はおおまかな考え方は一緒です。概念だけでも１度覚えておくと他の言語でもすぐに扱えるようになるためこの機会にぜひ覚えてみましょう！

== RxSwiftの特徴

RxSwiftの特徴として、「値の変化が検知しやすい」「非同期処理を簡潔に書ける」等が挙げられます。
これは主にUIの検知（タップや文字入力の検知）や通信処理等で使われ、RxSwiftを用いるとdelegateやcallbackを用いたコードよりもスッキリと見やすいコードを書けるようになります。

その他のメリットとしては次のものが挙げられます。

* 時間経過に関する処理をシンプルに書ける
* ViewControllerの呼び出し側で処理が書ける
* コード全体が一貫する
* まとまった流れが見やすい
* 差分がわかりやすい
* スレッドを変えやすい

また、デメリットとして主に「学習コストが高い」「デバッグしにくい」が挙げられます。
プロジェクトメンバーが全員RxSwiftを書けるわけではないのにも関わらず、とりあえずRxSwiftを使えば開発速度が早くなるんでしょ？という考え方で安易に導入すると逆に開発速度が落ちる可能性があります。

その他のデメリットとしては次のものが挙げられます。

* 一度errorが発生すると止まってしまう
** UIとバインドするような時は止まってしまうと困るので、errorが流れないものを使う
* 簡単な処理で使うと長くなりがち

プロジェクトによってRxSwiftの有用性が変わるので、そのプロジェクトの特性とRxSwiftのメリット・デメリットを照らし合わせた上で検討しましょう。

== RxSwiftは何が解決できる？

RxSwiftでは本当に色々なことができますが、１番わかりやすくて簡単なのは「DelegateやIBActionだと動作するところと処理が離れている」の解決だと思います。

実際にコードを書いて見てみましょう。

UIButtonとUILabelが画面に配置されていて、ボタンをタップすると文字列が変更されるという仕様のアプリを題材として作ります。

画面

//image[simpletap1][イメージ１]{
  simpletap1
//}

//image[simpletap2][イメージ２]{
  simpletap2
//}

まずは従来のIBActionを使った方法で作ってみましょう。


//listnum[ibaction][IBActionを用いたコード][swift]{
class SimpleTapViewController: UIViewController {

    @IBOutlet weak var messageLabel: UILabel!

    @IBAction func buttonTap(_ sender: Any) {
        messageLabel.text = "Changed!!"
    }
}
//}

通常の書き方だと、１つのボタンに対して１つの関数を定義します。
この場合だとUIと処理が1対1で非常に強い結合度になりますね。
仕様が非常にシンプルなため、コードもシンプルに書けてはいますが、ボタンを１つ増やすたびに対応する関数が１つずつ増えていき、コード量が次第に大きくなってしまいます。

次に、RxSwiftを用いて書いてみます。


//listnum[ibaction-to-rxswift][RxSwiftを用いたコード][swift]{
import RxSwift
import RxCocoa

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
//}

全く同じ処理をRxSwiftで書きました。
tapButtonのタップイベントを購読し、イベントが発生したらUILabelのテキストを変更しています。
コードを見比べてみると、１つのボタンと１つの関数が強く結合していたのが、１つのボタンと１つのプロパティの結合で済むようになっていて、UIと処理の制約を少し緩くできました。

シンプルな処理なのでコード量はRxSwiftを用いた場合のほうが長いですが、この先ボタンを増やすことを考えると、１つ増やすたびに対応するプロパティが１行増えるだけなので、コードがとてもシンプルになります。
また、画面上のUIを変更してもソースコードへの影響は少なくなるので変更が楽になります。

addTargetを利用する場合のコードも見てみましょう

UILabel, UITextFieldを画面に２つずつ配置し、入力したテキストをバリデーションして「あとN文字」とUILabelに反映するよくある仕組みのアプリを作ってみます

画面のイメージ

simpletextfieldlabelexample1.png


//listnum[addtarget][addTarget を用いたコード][swift]{
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
        nameField.addTarget(self, action: =selector(nameFieldEditingChanged(sender:)), for: .editingChanged)
        addressField.addTarget(self, action: =selector(addressFieldEditingChanged(sender:)), for: .editingChanged)
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
//}

UIと処理のコードが離れているので、パッとじゃ処理のイメージがしにくいですね。
対象のViewがもっと増えるとどの関数がどのUIの処理なのかわかりにくくなってしまいます。

次にRxSwiftを用いて書いてみます

//listnum[addtarget-to-rxswift][RxSwift version][swift]{
import RxSwift
import RxCocoa

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
            .filterNil() // import RxOptional が必要
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
            .filterNil() // import RxOptional が必要
            .observeOn(MainScheduler.instance)
            .bind(to: addressLabel.rx.text)
            .disposed(by: disposeBag)
    }
}
//}

先程のaddTargetのパターンとまったく同じ動作をします。
全ての処理がviewDidLoad()上で書けるようになり、UIと処理がバラバラにならないのですごく見やすいですね。
慣れていない方はまだ少し読みにくいかもしれませんが、Rxの書き方に慣れるとすごく読みやすくなります。

== 導入事例

* LINE（プロダクト不明）
** 出典元：iOSDCのノベルティ
* NIKKEI 日経電子版
** 出典元：iOSDCのノベルティ
