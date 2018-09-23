= RxSwift入門

== 覚えておきたい用語と１行概要

  * Reactive Extensions
  ** 「オブザーバパターン」「イテレータパターン」「関数型プログラミング」の概念を実装したインターフェース
  * オブザーバパターン
  ** プログラム内のオブジェクトのイベント（事象）を他のオブジェクトへ通知する処理で使われるデザインパターンの一種
  * RxSwift
  ** ReactiveExtensionsの概念をSwiftで扱えるようにした拡張ライブラリ
  * RxCocoa
  ** ReactiveExtensionsの概念をUIKitで扱えるようにした拡張ライブラリ　主にRxSwiftと一緒に導入される

== RxSwiftって何？

RxSwiftとはMicrosoftが公開した.NET Framework向けのライブラリである「Reactive Extensions」の概念をSwiftでも扱えるようにした拡張ライブラリで、GitHub上でオープンソースライブラリとして公開されています。

同じく、Reactive Extensionsの概念を取り入れた「ReactiveSwift」というライブラリも存在します。@<br>{}
本書では、ReactiveSwiftについては触れず、RxSwiftにのみ焦点を当てて解説していきます。

Reactive Extensionsについては後述しますが、RxSwiftを導入することによって非同期操作とイベント/データストリーム（時系列処理）の実装が用意にできるようになります。

== Reactive Extensionsって何？

=== 思想

Reactive Extensionsとは、「オブザーバパターン」「イテレータパターン」「関数型プログラミング」の概念を実装している.NET Framework向けの拡張ライブラリです。@<br>{}
Reactive Extensionsを導入することによって、リアクティブプログラミングが実現できます。

=== 歴史

元々は Microsoftが研究して開発した.NET用のライブラリで、２００９年に「Reactive Extensions」という名前で公開しました。現在は製品化され「ReactiveX」という名前に変更されています。@<br>{}
この「Reactive Extensions」の考え方がとても有用だったため、色々な言語へと移植されています。@<br>{}
たとえば、JavaであればRxJava、JavaScriptであればRxJSと、静的型付け・動的型付けなど関係なしに、さまざまな言語に垣根を超えて移植されています。@<br>{}
その中の１つが本書で紹介する「RxSwift」です。

本書では RxSwiftと関連するライブラリ群についてのみ解説しますが世の中にはさきほど挙げたものの以外に「RxRuby」、「RxScala」などさまざまな言語向けのライブラリがあります。@<br>{}
どのライブラリも概念はおおまかな考え方は一緒です。概念だけでも１度覚えておくと他の言語でもすぐに扱えるようになるためこの機会にぜひ覚えてみましょう！

== リアクティブプログラミングとは？

リアクティブプログラミングとは「時間とともに変化する値」と「振る舞い」の関係を宣言的に記述するプログラミングの手法です。
ボタンをタップするとアラートを表示、のようなインタラクティブなシステムや通信処理、アニメーションのようにダイナミックに状態が変化するようなシステムに対して宣言的に動作を記述することができるため、フロントエンド側のシステムでよく使われます。

リアクティブプログラミングの説明の前に、少し命令型のプログラミングの書き方について振り返ってみます。
次のコードを見てみましょう。

//listnum[tetuduki-example][擬似コード][swift]{
a = 2
b = 3
c = a * b
a = 3
print(c)
//}

何の前提も無く、プログラマーにこの疑似コードで出力される値を聞くと、大体は「6」と答えます。
命令型プログラミングとしてのこの結果は正しいですが、リアクティブプログラミングの観点からみた結果としては正しくありません。

冒頭の部分で少し触れましたが、リアクティブプログラミングは「値」と「振る舞い」の「関係」を宣言的に記述するプログラミングの手法です。
リアクティブプログラミングの観点では、「@<code>{c}にはその時点での@<code>{a * b}の演算の結果を代入する」のではなく、「@<code>{c}は@<code>{a * b}の関係をもつ」という意味で解釈されます。
つまり、@<code>{c}に@<code>{a * b}の関係を定義した後は、aの値が変更されるたびにbの値がバックグラウンドで再計算されるようになります。
よって、例の疑似コードをリアクティブプログラミングの観点からみた結果としては、「９」が出力されることになります。

また、リアクティブプログラミングは、Excelを題材として説明されることがよくあります。

//image[excel-2x3][Excel][scale=0.8]{
  excel-2x3
//}

C1セルにはA1セル値とB1セル値を掛け算した結果を出力させています。
試しにA1セルを変更してみます。

//image[excel-3x3][Excel][scale=0.8]{
  excel-3x3
//}

A1の値の変更に合わせて、C1が自動で再計算されました。
値と振る舞いの関係を定義する、これがリアクティブプログラミングです。

== RxSwiftの特徴

RxSwiftの特徴は色々ありますが、主な特徴として「値の変化が検知しやすい」「非同期処理を簡潔に書ける」が挙げられます。@<br>{}
これは主にUIの変更の検知（タップや文字入力）や通信処理等で使われ、RxSwiftを用いるとdelegateやcallbackを用いたコードよりもスッキリと見やすいコードを書けるようになります。

その他のメリットとしては次のものが挙げられます。

  * 時間経過に関する処理をシンプルに書ける
  * ViewControllerの呼び出し側で処理が書ける
  * コード全体が一貫する
  * まとまった流れが見やすい
  * 差分がわかりやすい
  * 処理スレッドを変えやすい

デメリットとして主に「学習コストが高い」「デバッグしにくい」が挙げられます。@<br>{}
プロジェクトメンバーのほとんどがRxSwiftの扱いにあまり長けていない状況の中で、とりあえずRxSwiftを使えば開発速度が早くなるんでしょ？という考え方で安易に導入すると逆に開発速度が落ちる可能性があります。

その他のデメリットとしては次のものが挙げられます。

  * 簡単な処理で使うと長くなりがち

プロジェクトによってRxSwiftの有用性が変わるので、そのプロジェクトの特性とRxSwiftのメリット・デメリットを照らし合わせた上で検討しましょう。

== RxSwiftは何が解決できる？

RxSwiftでは本当に色々なことができますが、１番わかりやすくて簡単なのは「DelegateやIBActionだと動作するところと処理が離れている」の解決だと思います。

実際にコードを書いて見てみましょう。

UIButtonとUILabelが画面に配置されていて、ボタンをタップすると文字列が変更されるという仕様のアプリを題材として作ります。

//image[simpletap][画面のイメージ]{
  simpletap
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

通常の書き方だと、１つのボタンに対して１つの関数を定義します。@<br>{}
この場合だとUIと処理が1対1で非常に強い結合度になりますね。

仕様が非常にシンプルなため、コードもシンプルに書けてはいますが、ボタンを１つ増やすたびに対応する関数が１つずつ増えていき、コード量が次第に大きくなってしまいます。@<br>{}
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

まったく同じ処理をRxSwiftで書きました。@<br>{}
tapButtonのタップイベントを購読し、イベントが発生したらUILabelのテキストを変更しています。

コードを見比べてみると、１つのボタンと１つの関数が強く結合していたのが、１つのボタンと１つのプロパティの結合で済むようになっていて、UIと処理の制約を少し緩くできました。@<br>{}
シンプルな処理なのでコード量はRxSwiftを用いた場合のほうが長いですが、この先ボタンを増やすことを考えると、１つ増やすたびに対応するプロパティが１行増えるだけなので、コードがとてもシンプルになります。

また、画面上のUIを変更してもソースコードへの影響は少なくなるので変更が楽になります。

addTargetを利用する場合のコードも見てみましょう@<br>{}
UILabel, UITextFieldを画面に２つずつ配置し、入力したテキストをバリデーションして「あとN文字」とUILabelに反映するよくある仕組みのアプリを作ってみます

//image[simpletextfieldlabelexample1][画面のイメージ][scale=0.3]{
  画面のイメージ
//}


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

//}

UIと処理のコードが離れているので、パッとじゃ処理のイメージがしにくいですね。@<br>{}
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

さきほどのaddTargetのパターンとまったく同じ動作をします。@<br>{}
全ての処理がviewDidLoad()上で書けるようになり、UIと処理がバラバラにならないのですごく見やすいですね。

慣れていない方はまだ少し読みにくいかもしれませんが、Rxの書き方に慣れるとすごく読みやすくなります。

== 導入事例

  * LINE（プロダクト不明）
  ** 出典元：iOSDCのノベルティ
  * NIKKEI 日経電子版
  ** 出典元：iOSDCのノベルティ
