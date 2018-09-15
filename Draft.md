# はじめに

- WIP

## 対象読者

本書は次の読者を対象として作成しています。 

- プログラミング歴1年以上（種類問わず）
- Swift による iOS アプリの開発経験が少しだけある（3ヶ月〜1年未満）
- RxSwiftライブラリを使った開発をしたことが全くない・ほんの少しだけある

## 必須知識
- Swiftの基本的な言語仕様
  - if, for, switch, enum, class, struct
  - mapやfilterなどの高階関数の扱い
- Xcode の基本的な操作
- よく使われる UIKitの仕様
  - UILabel UITextView UITableView UICollectionView

## 推奨知識

- 設計パターン
  - MVVMアーキテクチャ
- デザインパターン
  - delegateパターン
  - KVOパターン
  - Observerパターン

## 想定環境
- OSX High Sierra
- Xcode 9.4
- Swift 4.1
- cocoapods 1.5.3

## お問い合わせ先

- Twitter
  - @k0uhashi

## 免責事項

- WIP

# 目次

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
- RxSwiftの導入
  - 導入要件
  - 導入方法
- 基本的な書き方
  - メソッドチェーンのように直感的に書ける
  - Hello world
  - よく使われるクラス・メソッドについて
    - Observable
    - Dispose
    - Subject, Relay
    - bind
    - Operator
  - HotなObservableとColdなObservable
- 簡単なアプリを作ってみよう！
  - カウンターアプリ
    - プロジェクトの作成
    - 環境設定
    - callback
    - delegate
    - RxSwift
  - WKWebViewを使ったアプリ
    - KVO
    - RxSwift
    - RxWebkit
- Githubリポジトリ検索アプリを作ってみよう！
  - アプリの概要
    - 機能要件
    - 画面イメージ
  - リポジトリの作成
  - xibで動くようにする
  - APIモデルを作る
- 次のステップ
  - 学習方法
  - コミュニティ
- 様々なRxSwift系ライブラリ
  - RxOptional
  - RxWebkit
  - RxDataSources

# iOSアプリ開発とSwift

iOSのアプリ開発において、いまではほぼSwift一択の状況ではないでしょうか？
Swiftが登場したころからStoryboardの機能も充実し、UIと処理の分けてさらに書きやすくなりました。
Objective-Cを使っていたころよりもアプリ開発が楽になり、iOSアプリ開発初心者でもかなりとっつきやすくなったのがわかります。

ですが、かなりとっつきやすくなったと言ってもまだ問題はいくつかあります。
例えば、「非同期処理が実装しにくい、読みにくい」「通信処理の成功・失敗の制御」「DelegateやaddTarget, IBAction等、UIと処理が離れている」などあります。
これを解決する１つの方法としてあるのが、RxSwift（リアクティブプログラミング）の導入です。

では具体的にどう解決できるのか簡単なサンプルを例に出しながら解説します。

# RxSwift入門

## 覚えておきたい用語と１行概要

- Reactive Extensions
  - GoFのデザインパターンの１つ、 `オブザーバパターン` を表したインターフェース
- RxSwift
  - ReactiveExtensionsをSwiftで扱えるように拡張されたライブラリ
- RxCocoa
  - UIKitでRxを使えるように様々なUIクラスをextension定義しているライブラリで、RxSwiftとにこいちでよく導入されます。
- オブザーバパターン
  - プログラム内のオブジェクトのイベント（事象）を他のオブジェクトへ通知する処理で使われるデザインパターンの一種


## RxSwiftって何？

RxSwift とは「ReactiveExtensions」をSwiftで扱えるように拡張されたライブラリのことを指します。
github上でオープンソースライブラリと公開されていて様々な人が日々コントリビュートしています。

Reactive Extensionsについては後述しますが非同期操作とイベント/データストリーム（時系列処理）の実装を容易にできるライブラリのことを指します

## Reactive Extensionsって何？

### 思想
Reactive Extensionsとは、「Reactive Programming」を実現するための「デザイン」とその実装ができる「ライブラリ」のことを指します。名前の通り、Reactive Programmingをするために既存のプラットフォームの機能を拡張します。

### 歴史
元々は `Microsoft` が研究して開発した `.NET用のライブラリ` で、２００９年に「Reactive Extensions」という名前で公開しました。現在はオープンソース化され「ReactiveX」という名前に変更されています。
この「ReactiveExtensions」の考え方がとても有用だったため JavaScriptやJava、Swiftなど、垣根を越えて様々な言語に移植されていて、その中の１つが本書で紹介する「RxSwift」です。

本書では RxSwiftと関連するライブラリ群についてのみ解説しますが世の中には `RxJava`, `RxJS`, `RxScala` など様々なライブラリがあります。
どのライブラリも概念はおおまかな考え方は一緒です。概念だけでも１度覚えておくと他の言語でもすぐに扱えるようになるためこの機会にぜひ覚えてみましょう！

## RxSwiftの特徴

RxSwiftの特徴として、「値の変化が検知しやすい」「非同期処理を簡潔に書ける」等が挙げられます。
これは主にUIの検知（タップや文字入力の検知）や通信処理等で使われ、RxSwiftを用いるとdelegateやcallbackを用いたコードよりもスッキリと見やすいコードを書けるようになります。

その他のメリットとしては次のものが挙げられます。

- 時間経過に関する処理をシンプルに書ける
- ViewControllerの呼び出し側で処理が書ける
- コード全体が一貫する
- まとまった流れが見やすい
- 差分がわかりやすい
- スレッドを変えやすい

また、デメリットとして主に「学習コストが高い」「デバッグしにくい」が挙げられます。
プロジェクトメンバーが全員RxSwiftを書けるわけではないのにも関わらず、とりあえずRxSwiftを使えば開発速度が早くなるんでしょ？という考え方で安易に導入すると逆に開発速度が落ちる可能性があります。

その他のデメリットとしては次のものが挙げられます。

- 一度errorが発生すると止まってしまう
  - UIとバインドするような時は止まってしまうと困るので、errorが流れないものを使う
- 簡単な処理で使うと長くなりがち

プロジェクトによってRxSwiftの有用性が変わるので、そのプロジェクトの特性とRxSwiftのメリット・デメリットを照らし合わせた上で検討しましょう。

## RxSwiftは何が解決できる？

RxSwiftでは本当に色々なことができますが、１番わかりやすくて簡単なのは「DelegateやIBActionだと動作するところと処理が離れている」の解決だと思います。

実際にコードを書いて見てみましょう。

UIButtonとUILabelが画面に配置されていて、ボタンをタップすると文字列が変更されるという仕様のアプリを題材として作ります。

画面

simpletap1.png
simpletap2.png

まずは従来のIBActionを使った方法で作ってみましょう。

### IBActionを用いたコード

```
class SimpleTapViewController: UIViewController {
    
    @IBOutlet weak var messageLabel: UILabel!
    
    @IBAction func buttonTap(_ sender: Any) {
        messageLabel.text = "Changed!!"
    }
}
```

通常の書き方だと、１つのボタンに対して１つの関数を定義します。
この場合だとUIと処理が1対1で非常に強い結合度になりますね。
仕様が非常にシンプルなため、コードもシンプルに書けてはいますが、ボタンを１つ増やすたびに対応する関数が１つずつ増えていき、コード量が次第に大きくなってしまいます。

次に、RxSwiftを用いて書いてみます。

### RxSwiftを用いたコード

```
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
```

全く同じ処理をRxSwiftで書きました。
tapButtonのタップイベントを購読し、イベントが発生したらUILabelのテキストを変更しています。
コードを見比べてみると、１つのボタンと１つの関数が強く結合していたのが、１つのボタンと１つのプロパティの結合で済むようになっていて、UIと処理の制約を少し緩くできました。

シンプルな処理なのでコード量はRxSwiftを用いた場合のほうが長いですが、この先ボタンを増やすことを考えると、１つ増やすたびに対応するプロパティが１行増えるだけなので、コードがとてもシンプルになります。
また、画面上のUIを変更してもソースコードへの影響は少なくなるので変更が楽になります。

addTargetを利用する場合のコードも見てみましょう

UILabel, UITextFieldを画面に２つずつ配置し、入力したテキストをバリデーションして「あとN文字」とUILabelに反映するよくある仕組みのアプリを作ってみます

画面のイメージ

simpletextfieldlabelexample1.png

### addTarget を用いたコード

```
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
```

UIと処理のコードが離れているので、パッとじゃ処理のイメージがしにくいですね。
対象のViewがもっと増えるとどの関数がどのUIの処理なのかわかりにくくなってしまいます。

次にRxSwiftを用いて書いてみます

### RxSwift version

```
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
```

先程のaddTargetのパターンと全く同じ動作をします。
全ての処理がviewDidLoad()上で書けるようになり、UIと処理がバラバラにならないのですごく見やすいですね。
慣れていない方はまだ少し読みにくいかもしれませんが、Rxの書き方に慣れるとすごく読みやすくなります。

## 導入事例

- LINE（プロダクト不明）
  - 出典元：iOSDCのノベルティ
- NIKKEI 日経電子版
  - 出典元：iOSDCのノベルティ

# RxSwiftの導入

## 導入要件

- RxSwiftリポジトリより引用（２０１８年８月３１日現在）
- Xcode 9.0
- Swift 4
- Swift 3.x （rxswift-3.0 ブランチを指定)
- Swift 2.3 （rxswift-2.0 ブランチを指定)

## 導入方法

RxSwiftの導入方法はCocoaPodsやCarthage、SwiftPackageManager等いくつかありますが、ここでは１番簡単でよく使われる（著者の観測範囲）CocoaPodsでの導入方法を紹介します。

CocoaPodsとは、iOS/Mac向けのアプリを開発する際のライブラリ管理をしてくれるツールのことで、これを使うと外部ライブラリが簡単に導入できます

CocoaPodsを導入するにはRubyが端末にインストールされてる必要があります。（Macではデフォルトで入っているのであまり気にしなくても良いですが）

次のコマンドでCocoaPodsを導入できます

```
gem install cocoapods
gem install -v 1.5.3 cocoapods # バージョンを本書と同じにしたい場合はコッチ
```

これでCocoaPodsを端末に導入することができました。
次に、CocoaPodsを用いて、プロジェクトに外部ライブラリを導入してみます。

大まかな流れは次の通りです。

1. Podfileというファイルを作成
2. Podfileに導入したいライブラリを定義
3. ターミナルで pod install と入力

では、実際にやってみましょう。

```
# プロジェクトのルートディレクトリで実行
vi Podfile
```

```
# Podfile
use_frameworks!

target 'YOUR_TARGET_NAME' do
    pod 'RxSwift',    '~> 4.0'
    pod 'RxCocoa',    '~> 4.0'
end
```

- `YOUR_TARGET_NAME` は各自のプロジェクト名に置き換えてください

```
# プロジェクトのルートディレクトリで実行
pod install
```

`Pod installation complete!` というメッセージが出力されたら導入成功です！
もし導入できていなさそうな出力であれば、書き方やtypoをもう一度確認してみてください。

Tips: PodfileはRubyと全く同じ構文で定義されています

# 基本的な書き方

ここからは、RxSwiftの書き方を説明していきます。
RxSwiftの書き方は様々あり、本書では全てを紹介できませんがよく使われるところを抜粋して紹介します。

## メソッドチェーンのように直感的に書ける

RxSwift/RxCocoaは、メソッドチェーンのように直感的に書くことができます。
メソッドチェーンとは、その名前の通りメソッドを実行してその結果に対してさらにメソッドを実行するような書き方を指します。jQueryを扱ってた人はなんとなくわかるかと思います。

具体的には次のように書くことができます

```
hogeButton.rx.tap
  .subscribe(onNext: { [weak self] in 
    // 処理
  })
  .disposed(by: disposeBag)
```

hogeButtonのタップイベントを購読し、タップされたときにsubscribeメソッド引数であるonNextのクロージャ内の処理を実行します。
最後にクラスが解放されたとき自動的に購読を破棄してくれるようにdisposed(by:)メソッドを使っています。

まとめると、次の流れで定義しています。

1. イベントの購読
2. イベントが流れてきたときの処理
3. クラスが破棄と同時に購読を破棄

## Hello World

RxSwiftでのHello World的な書き方を書いてみます。
公式なHelloWorldな書き方ではありませんが、なんとなく概念は掴めるかと思います

- ストリームを定義し、好きなタイミングで値を流し、購読する

```
import RxSwift
import RxCocoa

let helloWorldSubject = PublishSubject<String>()

helloWorldSubject
  .subscribe(onNext: { [weak self] value in
    print("value = \(value)")
  })
  .disposed(by: disposeBag)

helloWorldSubject.onNext("Hello World!")
helloWorldSubject.onNext("Hello World!!")
helloWorldSubject.onNext("Hello World!!!")
```

結果は次の通り

```
value = Hello World!
value = Hello World!!
value = Hello World!!!
```

処理の流れのイメージは次の通りです。

1. `helloWorldSubject` というSubjectを定義
2. Subjectを購読
3. 値が流れてきたら print文で値を出力させる
4. 定義したクラスが破棄されたら購読も自動的に破棄させる
5. 値を流す

Subjectを使った書き方はよく使われます。例えば、ViewController/ViewModel間のデータの受け渡しや親子ViewController間でのデータ受け渡しで使われます。

上記のコードは同じクラス内に書いていて、Subjectの強みがあまり出ません。
実際にViewController/ViewModelに分けて書いてみましょう。

```
class HogeViewController: UIViewController {
    
  private let disposeBag = DisposeBag()
  
  var viewModel: HogeViewModel!

  override func viewDidLoad() {
    super.viewDidLoad()
    
    viewModel = HogeViewModel()
    
    viewModel.helloWorldSubject
      .subscribe(onNext: { [weak self] value in
        print("value = \(value)")
      })
      .disposed(by: disposeBag)
    
    viewModel.updateItem()
  }

}

class HogeViewModel {
  let helloWorldSubject = PublishSubject<String>()
  
  func updateItem() {
    helloWorldSubject.onNext("Hello World!")
    helloWorldSubject.onNext("Hello World!!")
    helloWorldSubject.onNext("Hello World!!!")
  }
}
```

出力結果自体は先程と同じです。
このコードを図で表してみましょう。

image: helloworldsubject.png

ViewControllerがViewModelのSubjectを購読したことで、変更があった場合にイベント・値を受け取れます。
ViewModelはデータの変更をViewControllerに伝える必要がなくなるので、ViewControllerを知らなくても良くなり、「UIとロジックの分離」ができました。テストも書きやすくなりますね。

なんとなく処理の流れは掴めたでしょうか？ここで記載したHelloWorldのコードはRxSwiftの数ある機能の中ではほんの一部にしかすぎないので、やりながら覚えていきましょう！

## よく使われるクラス・メソッドについて

さて、ここまで本書を読み進めてきていくつか気になるワードやメソッド、クラス名が出てきたのではないでしょうか？
ここからようやくそれらのクラスとそれらを支える概念についてもう少し深く触れていきたいと思います。

### Observable

Observableは翻訳すると観測可能という意味で文字通り観測可能なものを表現するクラスで、イベントを検知するためのものです。

まずは、次の図を見てください。

image: what-observable.png

RxSwift（ReactiveExtension）について少し調べた方は大体みたことあるような図ではないでしょうか？
これがまさにObservableを表しています。

Observableに流れるイベントには次の種類あります。

- onNext
  - デフォルトのイベント
  - 値を格納でき、何度でも呼ばれる
- onError
  - エラーイベント
  - １度だけ呼ばれ、その時点で終了、購読を破棄
- onCompleted
  - 完了イベント
  - １度だけ呼ばれ、その時点で終了、購読を破棄

これをコードでどう扱うかと言うと、次のように扱います

```
hogeSubject
  .subscribe(onNext: { 
    print("next")
  }, onError: {
    print("error")
  }, onCompleted: {
    print("completed")
  })
```

コードを見てなんとなく予想できるかと思いますが、onNextイベントが流れてきたときはonNextのクロージャが実行され、onErrorイベントが流れてきたときはonErrorのクロージャーが実行されます。

また、UIKitをsubscribeする場合はonErrorやonCompletedイベントが流れてこないので、onNextのクロージャ以外は省略できます。

Tips: ObservableとObserver

様々な資料に目を通していると、ObservableとObserverという表現が出てきますがどちらも違う意味です。イベント発生元がObservableでイベント処理がObserverです。ややこしいですね。
コードで見てみましょう。

```
hogeObservable // Observable (イベント発生元)
  .map { $0 * 2 } // Observable (イベント発生元)
  .subscribe(onNext: { 
    // Observer(イベント処理)
  })
  .disposed(by: disposeBag)
```

コードで見てみるとわかりやすいですね。名前は似てますが違う意味だというのを頭の隅に入れておくと理解がより進むかと思います。

### Dispose

ここまでコードを見てくると、なにやらsubscribeしたあとに必ずdisposed(by:)メソッドが呼ばれているのがわかるかと思います。さてこれは何でしょう？
一言で説明すると、メモリリークを回避するための仕組みです。

Disposeは購読を解除（破棄）するためのもので、dispose()メソッドを呼ぶことで購読を破棄できます。
ただし、Observableセクションで記述した通りonErrorやonCompletedイベントが流れてくると購読が自動で破棄してくれるのでdispose()メソッドを呼ぶ必要はありません。

しかし、onErrorやonCompletedイベントが全ての場合で必ず流れてくるわけではありません。
とはいえ、購読を破棄するタイミング、難しいですよね？その画面がしばらく呼ばれなくなった時？インスタンスが破棄されたとき？いちいちそんなこと考えてられないしdeinitにつらつらと書くのも面倒ですね

そこで活躍するのがDisposeBagという仕組みです。
DisposeBagはDisposableインスタンスを貯めておいて、自身が解放(deinit)されたときに管理している購読を全て自動で解放(unsubscribe)してくれる便利なものです。楽ですね。

コードで見てみましょう

```
class HogeViewController {
  @IBOutlet weak var hogeButton: UIButton!
  @IBOutlet weak var fooButton: UIButton!
  private let disposeBag = DisposeBag()

  override viewDidLoad() {
    super.viewDidLoad()
    hogeButton.rx.tap
      .subscribe(onNext: { // .. })
      .disposed(by: disposeBag)
    
    fooButton.rx.tap
      .subscribe(onNext: { // .. })
      .disposed(by: disposeBag)
  }
}
```

上記の場合だと、HogeViewControllerが解放(deinit)されるときに保持しているhogeButtonのsubscribeとfooButtonのsubscribeのDisposableをdisposeしてくれます。

とりあえず購読したら必ずdisposed(by: disposeBag)しておけば大体間違いないです。

Tips: シングルトンインスタンス内でDisposeBagを扱うときは注意！

DisposeBagはとても便利な仕組みですが、シングルトンインスタンス内でDisposeBagを扱う時は注意が必要です。DisposeBagの仕組みはそのクラスが解放されたとき、管理してるDisposableをdisposeすると先程記述しましたね。
つまりDisposeBagのライフサイクルは保持しているクラスのライフサイクルと同一のものになります。
しかし、シングルトンインスタンスのライフサイクルはアプリのライフサイクルと同一のため、いつまでたってもdisposableされず、メモリリークになる可能性があります。
回避策が全くないわけではありませんが、ここでは詳細を省きます。シングルトンインスタンスで扱う場合には注意が必要！ということだけ覚えておいてください。

### Subject, Relay

Subject、Relayは簡単に言うと、イベントの検知に加えてイベントの発生もできるすごいやつです。

ここで少しObservableについて振り返ってみましょう。Observableとは、イベントを検知するためのクラスでしたね。Subject、Relayはそれに加えてイベントを発生させることもできるクラスです。

Subject、Relayは色々ありますが、代表としてよく使われる次の４種類を紹介します。

- PublishSubject
- BehaviorSubject
- PublishRelay
- BehaviorRelay

それぞれの違いをざっくりとですが、下記のテーブル図にまとめました。

| |流れるイベント|初期値|
|:-:|:-:|:-:|
|PublishSubject|onNext, onError, onComplete|持たない|
|BehaviorSubject|onNext, onError, onComplete|持つ|
|PublishRelay|onNext|持たない|
|BehaviorRelay|onNext|持つ|

- 初期値について

初期値を持つ・持たないの違いは、Subject、RelayをObserverした瞬間にイベントが流れるか流れないかの違いです。やりたいことに合わせて使い分けましょう。

- それぞれの使い分け

とはいえ、最初はどう使い分けるかが難しいと思います。特にSubjectとRelayはどう使えばよいか困る人も多いと思います。簡単な使い分けを紹介すると、「通信処理やDB処理等」エラーが発生したときに分岐させたい場合はSubject、UIに値をBindするようなものはRelayを使いましょう。（RxSwiftのデメリットでも触れましたが、UIにBindしているObservableでonErrorやonCompleteが発生しまうと、購読が止まってしまう為、onNextのみが流れるRelayを使うのが適切です。）

- Tips: internal(public)なSubject,Relay

Subject, Relayはすごく便利でいろいろなことができます。便利なのは良いことですが、いろいろ出来すぎてしまうと逆にコードが複雑になってしまうことがあります。

どういうことかというと、クラスの外からもイベントを発生させることができるためどのクラスがどこでイベント発生させているか段々わからなくなり、デバッグをするのがかなりしんどくなります。

その解決策として、Subject、Relayはprivateとして定義して、外部用のObservableを用意するのが一般的に用いられています。

次のコードのように定義します。

```
private let items: BehaviorRelay<[Item]>(value: [])

var itemsObservable: Observable<[Item]> {
  return items
}
```

- Tips: SubjectとRelay

SubjectとRelay、それぞれ特徴が違うと書きましたが、コードを見てみると、実はRelayはSubjectの薄いラッパーとして定義されています。

それぞれonNextイベントは流せますが、Relayの場合は呼び出すメソッドがSubjectと異なるので注意しましょう

```
let hogeSubject = PublishSubject<String>()
let hogeRelay = PublishRelay<String>()

hogeSubject.onNext("ほげ")
hogeRelay.accept("ほげ")
```

呼び出すメソッドが違うからなにか特別なことしてるのかな？と思うかもしれませんが、特別なことはしていません。PublishRelayのコードを見てみましょう。

```
public final class PublishRelay<Element>: ObservableType {
  public typealias E = Element

  private let _subject: PublishSubject<Element>
  
  // Accepts `event` and emits it to subscribers
  public func accept(_ event: Element) {
    _subject.onNext(event)
  }
  
  // ...
```

コードを見てみると、内部的にはonNextを呼んでいるので、特別なことはしていないというのがわかります。

### bind

Observable/Observerに対してbindメソッドを使うと指定したObserverにイベントストリームを接続することができます。
「bind」と聞くと双方向データバインディングを想像しますが、RxSwiftでは単方向データバインディングです。双方向データバインディングが不可能というわけではありませんが、筆者の観測範囲では使っている人は少ないです。
bindメソッドが独自でなにか難しいことをやっているわけではなく、振る舞いはsubscribeと同じです。

実際にコードを比較してみましょう。

```
import RxSwift
import RxCocoa

// ...

@IBOutlet weak var nameTextField: UITextView!
@IBOutlet weak var nameLabel: UILabel!
private let disposeBag = DisposeBag()

// ①bindを利用
nameTextField.rx.text
  .bind(to: nameLabel.rx.text)
  .disposed(by: disposeBag)

// ②subscribeを利用
nameTextField.rx.text
  .subscribe(onNext: { [weak self] text in 
    nameLabel.text = text
  })
  .disposed(by: disposeBag)
```

上記のコードでは①bindを利用した場合と②subscribeを利用した場合それぞれ定義しましたが、全く同じ動作をします。振る舞いが同じという意味が伝わったでしょうか？

### Operator

ここまでコードでは、入力された値をそのままbindするコードが多く登場しました。ですが実際のプロダクションコードではそのままbindする場合はほぼ無く、何か加工してbindする場合が多いかと思います。

例えば、入力されたテキストの文字数を数えて「あとN文字」とラベルのテキストに反映する仕組みは代表的なパターンの１つだと思います。

そこで活躍するのがOperatorという概念です。
OperatorというはObservableのイベント値を途中で変換したり、絞り込んだりすることや、２つのObservableのイベントを合成したり結合したりすることを指します。

Operatorは本当に沢山、色々なことができてそれだけで１冊の本が書けるレベルです。なので、ここではよく使われるOperatorを簡単に紹介します。

- 変換
  - map
    - 通常の高階関数と同じ動き
  - flatMap
    - 通常の高階関数と同じ動き
  - reduce
    - 通常の高階関数と同じ動き
  - scan
    - reduceに似ていて途中結果もイベント発行ができる
  - debounce
    - 指定時間イベントが発生しなかったら最後に流されたイベントを流す
- 絞り込み
  - filter
    - 通常の高階関数と同じ動き
  - take
    - 指定時間の間だけイベントを通知してonCompletedする
  - skip
    - 名前の通り、指定時間の間はイベントを無視する
  - distinct
    - 重複イベントを除外する
- 組み合わせ
  - zip
    - 複数のObservableを組み合わせる（異なる型でも可能）
  - merge
    - 複数のObservableを組み合わせる（異なる型では不可能）
  - combineLatest
    - 複数のObservableの最新値を組み合わせる（異なる型でも可能）
  - sample
    - 引数に渡したObservableのイベントが発生されたら元のObservableの最新イベントを通知
  - concat
    - 複数のObservableのイベントを順番に組み合わせる（異なる方では不可能）

ここで一覧で紹介されてもおぼえきれねーよ！と思うかもしれませんがその通りです。すぐ覚えなくても良いので、こんなことできるのか〜とフワっと覚えていただければ良いです。

また、RxSwiftを書き始めたばっかりの人はどれがどんな動きをするか全然わからないとは思いますが、やっていきながら段々と覚えていきましょう！

更にスコープを狭めて、割と簡単に使いやすくてよく使うものをサンプルコードを加えてさらにピックアップしてみました。

- map

```
// hogeTextFieldのテキスト文字数を数えてfooTextLabelのテキストへ反映
hogeTextField.rx.text
  .map { return "あと\($0.count)文字 }
  .bind(to: fooTextLabel.rx.text)
  .disposed(by: disposeBag)
```

- filter

```
// 整数が流れるObservableから偶数のイベントのみに絞り込んでevenObservableに流す
numberObservable
  .filter { $0 % 2 == 0 }
  .bind(to: evenObservable)
  .disposed(by: disposeBag)
```

また、かならずObservableのイベントを使う必要はありません、次のようにクラス変数やメソッド内変数を取り入れてbindすることもできます

```
// ボタンをタップしたときにnameLabelにユーザの名前を表示する
let user = User(name: "k0uhashi")

showUserNameButton.rx.tap
  .map { [weak self] in
    return self?.user.name
  }
  .filterNil() // import RxOptionalが必要
  .bind(nameLabel.rx.text)
  .disposed(by: disposeBag)
```

- zip

複数のAPIにリクエストして同時に反映したい場合に使用します

```
Observable.zip(api1Observable, api2Observable)
  .subscribe(onNext { (api1, api2) in 
    // ↑タプルとして受け取ることができます
    //...
  })
  .disposed(by: disposeBag)
```

## HotなObservableとColdなObservable

これまでObservableとそれらを支える仕組みについて記載してきましたが、ObservableといってもHotなObservableとColdなObservableと２つの種類があります。

本書ではHotなObservableを主に扱いますが、ColdなObservableもあるということを頭の中に入れておきましょう。

HotなObservableの特徴は次の通りです。

- subscribeされなくても動作する
- 複数の箇所でsubscribeしても全てのObservableで同じイベントが同時に流れる

ColdなObservableの特徴は次の通りです。

- subscribeしたときに動作する
- 複数の箇所でsubscribeしたとき、それぞれのObservableでそれぞれのイベントが流れる

ColdなObservableは主に非同期通信処理で使われます。
試しにsubscribe時に１つの要素を返すObservableを作成する関数（ややこしい）を定義してみましょう

```
func myJust<E>(_ element: E) -> Observable<E> {
  return Observable.create { observer in
    observer.on(.next(element))
    observer.on(.completed)
    return Disposables.create()
  }
}

_ = myJust(100)
  .subscribe(onNext: { value in
    print(value)
  })
```

余談ですが、このような１回通知してonCompletedするObservableのことは「just」と呼ばれてます
Observableのcreate関数はコードを見てわかる通り、クロージャを使用してsubscribeメソッドを簡単に実装できる便利な関数です。
subscribeメソッドと同様にobserverを引数にとり、disposableを返却します。

振り返りTips: myJustはdisposed(by:)しなくても良い

あれ、そういえばdisposed関数呼んでないな？と思う方のために少し振り返ってみましょう。
Observableの特徴としてonError、onCompletedイベントは１度しか流れず、その時点で購読を破棄するというのがありましたね。myJust関数内ではonNextイベントが送られたあと、onCompletedイベントを送っていますね。なのでdisposed(by:)しなくても良いということになります。

# 簡単なアプリを作ってみよう！

ここまではRxSwift/RxCocoaの概念や基本的な使い方について紹介してきましたが、ここからは実際にアプリを作りながら説明していきます。

まずは簡単なアプリから作ってみましょう。
とはいえ、いきなりRxSwiftでコードを書いても理解に時間がかかるかと思います。（自分はそうでした）

なので、１つのテーマごとにcallbackやKVO、delegateで書かれたコードを最初に書いて、これをどうRxSwiftに置き換えるか？という観点でアプリを作っていきます。（本書のテーマである「比較して学ぶ」というのはこのことを指しています）

では、作っていきましょう！テーマはこちら！

## カウンターアプリ

この節ではカウンターアプリをテーマにcallback、delegate、RxSwiftでどうかくかを書きます。
まずはアプリの機能要件を決めます！

### 機能要件

- カウントの値が見れる
- カウントアップができる
- カウントダウンができる
- リセットができる

### アーキテクチャ

- MVVM

### 画面のイメージ

image: counterappscreenshot.png

### プロジェクトの作成

まずはプロジェクトを作成します。ここは特別なことをやっていないのでサクサクといきます。

image: create-project.png

Xcodeを新規で起動して、 Create a new Xcode project を選択します。

image: select-template.png

テンプレートを選択します。 Single View App を選択

image: project-name-settings.png

プロジェクトの設定をします。 ここは各自好きなように設定してください。

Nextボタンを押してプロジェクトの作成ができたら、一度Xcodeを終了します

### 環境設定

terminal.app を起動し、作成したプロジェクトのルートまで移動します

```
ryo-takahashi@~/CounterApp>
```

まず最初にRxSwift/RxCocoaライブラリの導入を行います。プロジェクト内で `pod init` を実行します。
成功すると、ディレクトリ内に Podfile というファイルが生成されているのでこれを編集します。

```
vi Podfile
```

ファイルを開いたら、次のように編集してください。

```
# Uncomment the next line to define a global platform for your project          
# platform :ios, '9.0'
 
target 'CounterApp' do
  # Comment the next line if you're not using Swift and don't want to use dynamic frameworks
  use_frameworks!
 
  # Pods for CounterApp
  pod 'RxSwift' # ★この行を追加
  pod 'RxCocoa' # ★この行を追加
 
end
```

編集して保存したら、導入のためインストール用コマンドを入力します。

```
pod install
```

次のような結果が出たら成功です。

```
Analyzing dependencies
Downloading dependencies
Installing RxCocoa (4.2.0)
Installing RxSwift (4.2.0)
Generating Pods project
Integrating client project

[!] Please close any current Xcode sessions and use `CounterApp.xcworkspace` for this project from now on.
Sending stats
Pod installation complete! There are 2 dependencies from the Podfile and 2 total pods installed.

[!] Automatically assigning platform `ios` with version `11.4` on target `CounterApp` because no platform was specified. Please specify a platform for this target in your Podfile. See `https://guides.cocoapods.org/syntax/podfile.html#platform`.
```

環境設定はこれで完了です。

次回以降プロジェクトを開く時は、必ず "YOUR_PROJECT_NAME.xcworkspace" から開くようにしましょう
（*.xcworkspaceから開かないと導入したライブラリが使えません）

### 開発を加速させる設定

★ このセクションは今後何度も使うので付箋やマーカーを引いておきましょう！

この節では、節タイトルの通り開発を加速させる簡単な設定を行います。本書のテーマとは少しずれるので早足で進めます。
具体的には、Storyboardを廃止して ViewController + Xib を使って開発する手法に切り替えます。

#### Storyboardの廃止

Storyboardは画面遷移の設定が簡単にできたり、パッと見るだけで画面がどう遷移していくかわかりやすくて良いのですが、
反面としてアプリが大きくなってくると画面遷移が複雑で逆に見辛くなったり、小さなViewController（アラートやダイアログを出すものなど）の生成が面倒だったり、チーム人数が複数になると*.storyboardがconflictしまくるなど色々問題があるので、Storyboardを使うのをやめます。

Storyboardを廃止するために、次のことを行います

- Main.storyboardの削除
- Info.plistの設定
- AppDelegateの整理
- ViewController.xibの作成

##### Main.storyboardの削除

- CounterApp.xcworkspaceを開く
- /CouterApp/Main.storyboardをDelete
  - Move to Trashを選択

##### Info.plist

Info.plistにはデフォルトでMain.storyboardを使ってアプリを起動するような設定が書かれているので、それを削除します

- Info.plistを開く
- Main storyboard file base name の項目を削除する

##### AppDelegateの整理

Main.storyboardを削除したことによって、一番最初に起動するViewControllerの設定が失われ、アプリの起動が失敗するようになってしまったので、AppDelegateに一番最初に起動するViewControllerを設定します。

- AppDelegate.swiftを開く

```
# AppDelegate.swift
import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {
    
    var window: UIWindow?
    
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        self.window = UIWindow(frame: UIScreen.main.bounds)
        let navigationController = UINavigationController(rootViewController: ViewController())
        self.window?.rootViewController = navigationController
        self.window?.makeKeyAndVisible()
        return true
    }
    
}
```

##### ViewController.xibの作成

Main.storyboardを削除してことによって一番最初に起動するViewControllerの画面のデータがなくなってしまったので作成します。

- New File > View > Save As: ViewController.xib > Create
- ViewController.xibを開く
- Placeholders > File's Owner を選択
- Class に ViewControllerを指定

image: viewcontroller-filesowner.png

- OutletsのviewとViewControllerのViewをつなげる

image: viewcontroller-view-outlet.png

これでアプリの起動ができるようになりました。Build & Run で確認してみましょう。

次のような画面が出たら成功です。

image: init-clearn-viewcontroller.png

これで環境設定は終了です。
今後画面を追加していくときは同様の手順で作成していきます。

1. ViewController.swiftの作成
2. ViewController.xibの作成
3. ViewController.xibの設定
  3-1. Classの指定
  3-2. ViewのOutletの設定

Tips: 画面遷移

ViewController.swift + ViewController.xib構成にしたことによって、ViewControllerの生成が楽になりました。
また、そのおかげで画面遷移が少ない行で実装できるようになりました。次のコードで画面遷移を実装できます。

```
let viewController = ViewController()
navigationController?.pushViewController(viewController, animated: true)
```

### CallBackで作るカウンターアプリ

ようやくここから本題に入ります、まずはViewController.swiftを整理しましょう

- ViewController.swiftを開く
- 次のように編集
  - didReceiveMemoryWarningメソッドは特に使わないので削除します。

```
import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
    }
}
```

次に、画面を作成します。
UIButton３つとUILabelを１つ配置しましょう

image: counter-app-interfacebuilder.png

UI部品の配置が終わったら、早速ViewControllerと繋げましょう。

UILabelはIBOutlet、UIButtonはIBActionとして繋げます

```
import UIKit

class ViewController: UIViewController {
    
    @IBOutlet weak var countLabel: UILabel!

    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    @IBAction func countUp(_ sender: Any) {
    }
    
    @IBAction func countDown(_ sender: Any) {
    }
    
    @IBAction func countReset(_ sender: Any) {
    }
}
```

次に、ViewModelを作ります。ViewModelには次の役割をもたせます

- カウントデータの保持
- カウントアップ、カウントダウン、カウントリセットの処理

```
class ViewModel {
    private(set) var count = 0
    
    func incrementCount(callback: (Int) -> ()) {
        count += 1
        callback(count)
    }
    
    func decrementCount(callback: (Int) -> ()) {
        count -= 1
        callback(count)
    }
    
    func resetCount(callback: (Int) -> ()) {
        count = 0
        callback(count)
    }
}

```

ViewModelを作ったので、ViewControllerでViewModelを使うように修正と、IBActionの修正を行っていきます。

```
class ViewController: UIViewController {
    
    @IBOutlet weak var countLabel: UILabel!
    
    private var viewModel: ViewModel!

    override func viewDidLoad() {
        super.viewDidLoad()
        viewModel = ViewModel()
    }
    
    @IBAction func countUp(_ sender: Any) {
        viewModel.incrementCount(callback: { [weak self] count in
            self?.updateCountLabel(count)
        })
    }
    
    @IBAction func countDown(_ sender: Any) {
        viewModel.decrementCount(callback: { [weak self] count in
            self?.updateCountLabel(count)
        })
    }
    
    @IBAction func countReset(_ sender: Any) {
        viewModel.resetCount(callback: { [weak self] count in
            self?.updateCountLabel(count)
        })
    }
    
    private func updateCountLabel(_ count: Int) {
        countLabel.text = String(count)
    }
}
```

これで、機能要件を満たすことができました。
実際に Build & Run して確認してみましょう。

callbackで書く場合の良いところと悪いところをまとめてみます。

- 良い
  - 記述が簡単
- 悪い
  - ボタンを増やすたびにボタンを押下時の処理メソッドが増えていく
    - ラベルの場合も同様
    - 画面が大きくなっていくにつれてメソッドが多くなり、コードが読みづらくなってくる
  - ViewControllerとViewModelに分けたものの、完全にUIと処理の切り分けができているわけではない

### Delegateで作るカウンターアプリ

次に、delegateを使って実装してみましょう。

delegateを使う場合、設計はMVPパターンのほうが向いてるので、
MVPパターンに沿って実装していきます。

まずはDelegateを作ります

```
protocol CounterDelegate {
    func updateCount(count: Int)
}
```

次に、Presenterを作ります

```
class CounterPresenter {
    private var count = 0 {
        didSet {
            delegate?.updateCount(count: count)
        }
    }
    
    private var delegate: CounterDelegate?
    
    func attachView(_ delegate: CounterDelegate) {
        self.delegate = delegate
    }
    
    func detachView() {
        self.delegate = nil
    }
    
    func incrementCount() {
        count += 1
    }
    
    func decrementCount() {
        count -= 1
    }
    
    func resetCount() {
        count = 0
    }
}
```

最後に、ViewControllerを先程作成したPresenterを使うように修正するのと、Delegateをextensionするように修正しましょう

```
class ViewController: UIViewController {
    
    @IBOutlet weak var countLabel: UILabel!
    
    private let presenter = CounterPresenter()

    override func viewDidLoad() {
        super.viewDidLoad()
        presenter.attachView(self)
    }
    
    @IBAction func countUp(_ sender: Any) {
        presenter.incrementCount()
    }
    
    @IBAction func countDown(_ sender: Any) {
        presenter.decrementCount()
    }
    
    @IBAction func countReset(_ sender: Any) {
        presenter.resetCount()
    }
}

extension ViewController: CounterDelegate {
    func updateCount(count: Int) {
        countLabel.text = String(count)
    }
}
```

Build ＆ Run してみましょう。callbackの場合と同じ動きをします。
Delegateを使った書き方の良し悪しをまとめます。

- 良い
  - 処理を委譲できる
  - incrementCount(), decrementCount(), resetCount()がデータの処理に集中できる
    - callback(count)しなくてもよい
- 悪い
  - ボタンを増やすたびにメソッドが増えていく

データを処理する関数が完全に処理に集中できるようになったのは良いことですが、まだボタンとメソッドの個数が１：１になっている問題が残っていて、このままアプリが大きくなっていくにつれてメソッドが多くなり、どのボタンの処理がどのメソッドの処理なのかパッと見た感じではわからなくなってしまいます。

この問題はRxSwift/RxCocoaを使うことで解決できます。
実際にRxSwiftを使って作ってみましょう。

### RxSwiftで作るカウンターアプリ

先程のPresenterとCounterProtocolはもう使わないので削除しても大丈夫です。

まずはViewModelを作るためのProtocolとInput用の構造体を作ります

```
// ViewModelと同じクラスファイルに定義したほうが良いかも（好みやチームの規約による）

// ボタンの入力シーケンス
struct RxViewModelInput {
    let countUpButton: Observable<Void>
    let countDownButton: Observable<Void>
    let countResetButton: Observable<Void>
}

// ViewControllerに監視させる対象を定義
protocol RxViewModelOutput {
    var counterText: Driver<String> { get }
}

// ViewModelに継承させるprotocolを定義
protocol RxViewModelType {
    var outputs: RxViewModelOutput? { get }
    init(input: RxViewModelInput)
}
```

次にViewModelを作ります。CallBackパターンでも作りましたが、紛らわしくならないように新しい名前で作り直します

```
import RxSwift
import RxCocoa

class RxViewModel: RxViewModelType {
    var outputs: RxViewModelOutput?
    
    private let countRelay = BehaviorRelay<Int>(value: 0)
    private let initialCount = 0
    private let disposeBag = DisposeBag()
    
    required init(input: RxViewModelInput) {
        self.outputs = self
        resetCount()

        input.countUpButton
            .subscribe(onNext: { [weak self] in
                self?.incrementCount()
            })
            .disposed(by: disposeBag)

        input.countDownButton
            .subscribe(onNext: { [weak self] in
                self?.decrementCount()
            })
            .disposed(by: disposeBag)

        input.countResetButton
            .subscribe(onNext: { [weak self] in
                self?.resetCount()
            })
            .disposed(by: disposeBag)
        
    }
    
    
    private func incrementCount() {
        let count = countRelay.value + 1
        countRelay.accept(count)
    }
    
    private func decrementCount() {
        let count = countRelay.value - 1
        countRelay.accept(count)
    }
    
    private func resetCount() {
        countRelay.accept(initialCount)
    }
    
}

extension RxViewModel: RxViewModelOutput {
    var counterText: SharedSequence<DriverSharingStrategy, String> {
        let counterText = countRelay
            .map {
                "Rxパターン:\($0)"
            }
            .asDriver(onErrorJustReturn: "")
        return counterText
    }
}
```

ViewControllerも修正しましょう。全てのIBActionと接続を消してIBOutletを定義して接続しましょう。

注意！！：ここでIBActionの接続解除・IBOutletの接続が正しくできていない場合、起動時にクラッシュするので、要注意！
もしクラッシュしてしまう場合、ここを見直しましょう！

```
import RxSwift
import RxCocoa

class RxViewController: UIViewController {
    
    @IBOutlet weak var countLabel: UILabel!
    @IBOutlet weak var countUpButton: UIButton!
    @IBOutlet weak var countDownButton: UIButton!
    @IBOutlet weak var countResetButton: UIButton!
    
    private let disposeBag = DisposeBag()
    
    var viewModel: RxViewModel!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupViewModel()
    }
    
    private func setupViewModel() {
        let input = RxViewModelInput(countUpButton: countUpButton.rx.tap.asObservable(), countDownButton: countDownButton.rx.tap.asObservable(), countResetButton: countResetButton.rx.tap.asObservable())
        viewModel = RxViewModel(input: input)
        
        viewModel.outputs?.counterText
            .drive(countLabel.rx.text)
            .disposed(by: disposeBag)
    }
}
```

setupViewModel関数として切り出して定義してviewDidLoad()内で呼び出しています。

この書き方についてまとめてみます。

- 良い
  - ViewController
    - スッキリした
    - Input/Outputだけ気にすれば良くなった
  - ViewModel
    - 処理を集中できた
    - increment, decrement, resetがデータの処理に集中できた
    - ViewControllerのことを意識しなくても良い
      - 👉例: delegate?.updateCount(count: count) のようなデータの更新をViewControllerに伝えなくても良い
  - テストがかきやすくなった
- 悪い
  - コード量が他パターンより多い
  - 書き方に慣れるまで時間がかかる

RxSwiftを使った場合の一番大きな良い点はやはり「ViewModelはViewControllerのことを考えなくてもよくなる」ところです。ViewControllerがViewModelの値を監視して変更があったらUIを自動で変更するため、ViewModel側から値が変わったよ！と通知する必要がなくなるのです。

次に、テストが書きやすくなりました。今まではViewControllerとViewModel（Presenter)が密になっていてテストが書きづらい状況でしたが、今回は分離ができたのでとても書きやすくなりました。
やり方としてはViewModelをインスタンス化するときにInputを注入し、Outputを期待した通りになっているかのテストを書く感じになります。

ですが、この方法は慣れるまで時間がかかるかと思います。まずはUIButton.rx.tapだけ使う、PublishSubject系だけを使う・・・など小さく始めるのもありかと思います。
個人開発のアプリであれば全リプレースにチャレンジしてみても面白いかもしれませんが、業務で使うアプリでチームメンバーのほとんどがRxSwiftに慣れていない場合、キャッチアップで手一杯になって逆に開発効率が落ちることもありえるのでちゃんとチームメンバーと相談しましょう！

## WKWebViewを使ったアプリ

この章ではKVOの実装パターンをRxSwiftに置き換える方法について学びます。

### この章のストーリー

- WKWebView+KVOを使ったWebViewアプリを作成
- WKWebView+RxSwiftに書き換える

### イメージ

image: wkwebview1.png
image: wkwebview2.png
image: wkwebview3.png

WebViewとProgressViewを配置して、Webページの読み込みに合わせてゲージ、インジケータ、Navigationタイトルを変更する機能を作ります。

サクっといきましょう。KVOで実装した場合、次のコードになります。

```
import UIKit
import WebKit

class WKWebViewController: UIViewController {
  @IBOutlet weak var webView: WKWebView!
  @IBOutlet weak var progressView: UIProgressView!

  override func viewDidLoad() {
    super.viewDidLoad()
    setupWebView()
  }

  private func setupWebView() {
    // webView.isLoadingの値の変化を監視
    webView.addObserver(self, forKeyPath: "loading", options: .new, context: nil)
    // webView.estimatedProgressの値の変化を監視
    webView.addObserver(self, forKeyPath: "estimatedProgress", options: .new, context: nil)

    let url = URL(string: "https://www.google.com/")
    let urlRequest = URLRequest(url: url!)
    webView.load(urlRequest)
    progressView.setProgress(0.1, animated: true)
  }

  deinit {
    // 監視を解除
    webView?.removeObserver(self, forKeyPath: "loading")
    webView?.removeObserver(self, forKeyPath: "estimatedProgress")
  }

  override func observeValue(forKeyPath keyPath: String?, of object: Any?, change: [NSKeyValueChangeKey : Any]?, context: UnsafeMutableRawPointer?) {
    if keyPath == "loading" {
      UIApplication.shared.isNetworkActivityIndicatorVisible = webView.isLoading
      if !webView.isLoading {
        // ロード完了時にProgressViewの進捗を0.0(非表示)にする
        progressView.setProgress(0.0, animated: false)
        // ロード完了時にNavigationTitleに読み込んだページのタイトルをセット
        navigationItem.title = webView.title
      }
    }
    if keyPath == "estimatedProgress" {
      // ProgressViewの進捗状態を更新
      progressView.setProgress(Float(webView.estimatedProgress), animated: true)
    }
  }
}
```

KVO（Key-Value Observing:キー値監視）とは、特定のオブジェクトのプロパティ値の変化を監視する仕組みです。Objective−Cのメカニズムを使っていて、NSValueクラスに大きく依存しています。
また、構造体(struct)はNSObjectを継承できないためKVOの仕組みは使えません。

KVOをSwiftで使うためにはオブジェクトをクラスで定義し、プロパティに @objc属性とdynamicをつけます。

WKWebViewにはtitle, url, estimatedProgressなどKVOに対応したプロパティがあるので今回はそれを使っています。

では実際コード内で何をしているかというと、viewDidLoad()時にwebViewの値を監視させて、値が変更されたときにUIを更新させています。

addObserverの引数にプロパティ名を渡すとその値が変化された時に `observeValue(forKeyPath keyPath: String?, of object: Any?, change: [NSKeyValueChangeKey : Any]?, context: UnsafeMutableRawPointer?)` が呼ばれるようになります。

observeValueのkeyPathにはaddObserverで設定したforKeyPathの値が流れてくるので、その値で条件分岐してUIを更新します。

ただ、全ての通知をobserveValueで受け取って条件分岐するため、段々とobserveValueメソッドが肥大化していく問題があります。

また、KVOはObjective-Cのメカニズムであるため、型の安全性が考慮されているわけではありません。

KVOを使った場合の注意点として、addObserverした場合、deinit時にremoveObserverを呼ばないとアプリが強制終了する可能性があります。忘れずにremoveObserverを呼びましょう。

とはいえ、removeObserverを呼ぼうと注意していても人間は忘れます、それにクラスが大きくなってくるとなおさらremoveObserverを呼ぶのを忘れます。

こういった問題はRxSwiftがまるまるっと解決してくれます！！RxSwiftに書き換えてみましょう。

と、その前にRxOptionalというRxSwiftの拡張ライブラリを導入します。理由は後述しますが、簡単に言うとOptionalな値を流すストリームに対して様々なことができるようにするライブラリです。

Podfileにライブラリを追加しましょう

```
pod 'RxSwift'
pod 'RxCocoa'
pod 'RxOptional' # New!
```

では、導入したライブラリも使いつつ、KVOで書かれた実装をRxSwiftを使うようにリプレースしていきます。

```
import UIKit
import WebKit
import RxSwift
import RxCocoa
import RxOptional

class WKWebViewController: UIViewController {
    @IBOutlet weak var webView: WKWebView!
    @IBOutlet weak var progressView: UIProgressView!

    private let disposeBag = DisposeBag()

    override func viewDidLoad() {
        super.viewDidLoad()
        setupWebView()
    }

    private func setupWebView() {

        // プログレスバーの表示制御、ゲージ制御、アクティビティインジケータ表示制御で使うため、一旦オブザーバを定義
        let loadingObservable = webView.rx.observe(Bool.self, "loading")
            .filterNil()
            .share()

        // プログレスバーの表示・非表示
        loadingObservable
            .map { return !$0 }
            .observeOn(MainScheduler.instance)
            .bind(to: progressView.rx.isHidden)
            .disposed(by: disposeBag)

        // iPhoneの上部の時計のところのバーの（名称不明）アクティビティインジケータ表示制御
        loadingObservable
            .bind(to: UIApplication.shared.rx.isNetworkActivityIndicatorVisible)
            .disposed(by: disposeBag)

        // NavigationControllerのタイトル表示
        loadingObservable
            .map { [weak self] _ in return self?.webView.title }
            .observeOn(MainScheduler.instance)
            .bind(to: navigationItem.rx.title)
            .disposed(by: disposeBag)

        // プログレスバーのゲージ制御
        webView.rx.observe(Double.self, "estimatedProgress")
            .filterNil()
            .map { return Float($0) }
            .observeOn(MainScheduler.instance)
            .bind(to: progressView.rx.progress)
            .disposed(by: disposeBag)

        let url = URL(string: "https://www.google.com/")
        let urlRequest = URLRequest(url: url!)
        webView.load(urlRequest)
    }
}
```

どうでしょうか？ネストも浅くなり、かなり読みやすくなってのではないでしょうか。
色々説明するところはありますが、この章で初めてでてきたメソッドについて説明していきます。

- import RxOptional

導入したRxOptionalライブラリをswiftファイル内で使用するために宣言

- rx.observe

rx.observeはKVOを取り巻く単純なラッパーです。単純であるため、パフォーマンスが優れていますが、用途は限られています。

self.から始まるパスと、子オブジェクトだけ監視できます。
例えば、self.view.frameを監視したい場合、第二引数に"view.frame"を指定します。

ただし、プロパティに対して強参照するため、循環参照を引き起こし最悪アプリがクラッシュする可能性があります。弱参照したい場合は、rx.observeWeaklyを使いましょう。

KVOはObjective-Cの仕組みで動いていると書きましたが、RxCocoaでは実は構造体であるCGRect、CGSize、CGPointに対してKVOを行う仕組みが実装されています。
他の構造体を購読したいときはNSValueから手動で抽出する仕組みを実装することでできます。
RxCocoaのKVORepresentable+CoreGraphics.swiftにKVORepresentableプロトコルを使って抽出する実装コードが書かれているので、独自で作りたい場合はここを参照しましょう。

- filterNil()

RxOptionalで定義されているOperator
名前でなんとなくイメージできるかもしれませんが、nilの場合は値を流さず、nilじゃない場合はunwrapして値を流すOperatorです。

わかりやすく、コードで比較してみましょう。次のコードはどちらも全く同じ動作をします。

```
// RxSwift
Observable<String?>
  .of("One",nil,"Three")
  .filter { $0 != nil }
  .map { $0! }
  .subscribe { print($0) }

// RxOptional
Observable<String?>
    .of("One", nil, "Three")
    .filterNil()
    .subscribe { print($0) }
```

- share()

文章で説明するより、まずは次のコードを見てください。

```
    let text = textField.rx.text
        .map { text -> String in
            print("call")
            return "☆☆\(text)☆☆"
        }

    text
        .bind(to: label1.rx.text)
        .disposed(by: disposeBag)

    text
        .bind(to: label2.rx.text)
        .disposed(by: disposeBag)

    text
        .bind(to: label3.rx.text)
        .disposed(by: disposeBag)
```

上記のコードはUITextFieldであるtextFieldへのテキスト入力を監視し、値を複数のLabelへbind、リアルタイムで入力したテキストをラベルへ反映する仕組みを実装するコードです。

ここでtextFieldへ「123」と入力した場合、print("call")は何回呼ばれるか予想してみましょう。
パッと見た感じだと、3回入力するので3回出力するのでは？と思いがちですが実際は違います。実行して試してみましょう！

```
call
call
call
call
call
call
call
call
call
```

callは９回呼ばれます。なるほど？
値を入力するたびにmap関数が３回呼ばれてますね。これはいけない。
今回のように値を変換したりprint出力するだけならそれほどパフォーマンスに影響はありませんが、データベースアクセスするものや、通信処理が発生するものではこの動作は好ましくありません。

では、なぜこの現象が起こるのか？
その前に、textField.rx.textが何なのかを紐解いて見ましょう。

textField.rx.textはRxCocoaでextension定義されているプロパティで、Observable<String?>ではなく、ControlProperty<String?>として定義されています。（が、実態はObservableです）
ControlPropertyは主にUI要素のプロパティで使われていて、メインスレッドで値が購読されることを保証しています。
また、これはColdなObservableです。
ColdなObservableの仕様として、subscribeした時点で計算リソースが割当られ、複数回subscribeするとその都度ストリームが生成されるという仕組みがあります。

今回の場合、３回subscribe(bind)したので、３個のストリームが生成されます。
するとどうなるかというと、値が変更されたときにOperatorが３回実行されてしまうようになります。

このままではまずいので、どうにかして何回購読してもOperatorを１回実行で済むように実装したいです。
では、どうすればよいのかというと、HotなObservableに変換してあげると良いです。

やりかたはいくつかあるのですが、今回は share() というOperatorを使います。実際のコードは次の通りです。

```
// これを
let text = textField.rx.text
    .map { text -> String in
        print("call")
        return "☆☆\(text)☆☆"
    }
// こうしましょう
let text = textField.rx.text
    .map { text -> String in
        print("call")
        return "☆☆\(text)☆☆"
    }
    .share() // ☆追加
```

Build & Run を実行してもう一度「１２３」とテキストに入力してみましょう。
出力結果が次のようになっていたら成功です。

```
call
call
call
```

- observeOn

subscribe内で処理を行うスレッドを指定するOperatorです。
引数には「ImmediateSchedulerType」プロトコルに準拠したクラスを指定します。

- MainScheduler.instance

MainSchedulerのシングルトンインスタンスを指定しています。
observeOnの引数にMainSchedulerのシングルトンインスタンスを渡してあげると、その先のOperatorはメインスレッドで処理してくれるようになります。

説明が長くなりましたが、本題に戻りましょう。
KVOで書いた処理をRxSwiftに置き換えてみた結果、かなり読みやすくなりましたね。

特に、removeObserverを気にしなくてもよくなるのはだいぶ安全になりますね。
というよりは、RxSwiftの場合はremoveObserverの役割が「.disposed(by:)」に変わったイメージのほうがわかりやすいかもしれません。
disposed(by:) を結局呼ばないといけないのなら、そんなに変わらなくね？と思うかもしれませんが、RxSwiftではWarningが出るのでremoveObserverだったころより忘れる確率は低くなります。

しかし、この方法ではKey値がベタ書きになっていることと、値の型を指定してあげないといけないという問題も残っています。
もっと使いやすくするように自分でextensionを定義するのもアリですが、
実はもっと便利にWKWebViewを扱える「RxWebKit」というRxSwift拡張ライブラリがあるので、それを使ってみましょう。

Podfileを編集します

```
  pod 'RxSwift'
  pod 'RxCocoa'
  pod 'RxOptional'
  pod 'RxWebKit'
```

ライブラリをインストールします。

```
pod install
```

先程書いたRxSwiftパターンのコードを次のコードに書き換えてみましょう！

```
import UIKit
import WebKit
import RxSwift
import RxCocoa
import RxOptional
import RxWebKit

class RxWebkitViewController: UIViewController {
    @IBOutlet weak var webView: WKWebView!
    @IBOutlet weak var progressView: UIProgressView!

    private let disposeBag = DisposeBag()

    override func viewDidLoad() {
        super.viewDidLoad()
        setupWebView()
    }

    private func setupWebView() {

        // プログレスバーの表示制御、ゲージ制御、アクティビティインジケータ表示制御で使うため、一旦オブザーバを定義
        let loadingObservable = webView.rx.loading
            .share()

        // プログレスバーの表示・非表示
        loadingObservable
            .map { return !$0 }
            .observeOn(MainScheduler.instance)
            .bind(to: progressView.rx.isHidden)
            .disposed(by: disposeBag)

        // iPhoneの上部の時計のところのバーの（名称不明）アクティビティインジケータ表示制御
        loadingObservable
            .bind(to: UIApplication.shared.rx.isNetworkActivityIndicatorVisible)
            .disposed(by: disposeBag)

        // NavigationControllerのタイトル表示
        webView.rx.title
            .filterNil()
            .observeOn(MainScheduler.instance)
            .bind(to: navigationItem.rx.title)
            .disposed(by: disposeBag)

        // プログレスバーのゲージ制御
        webView.rx.estimatedProgress
            .map { return Float($0) }
            .observeOn(MainScheduler.instance)
            .bind(to: progressView.rx.progress)
            .disposed(by: disposeBag)


        let url = URL(string: "https://www.google.com/")
        let urlRequest = URLRequest(url: url!)
        webView.load(urlRequest)
    }
}
```

Build & Run で実行してみましょう。全く同じ動作であれば成功です。
RxWebKitを使ったことで更に可動性がよくなりました。

RxWebKitはその名前の通り、WebKitをRxSwiftで使いやすくしてくれるように拡張定義しているラッパーライブラリです。
これを使うことで、「Keyのべた書き」と「値の型指定」問題がなくなりました。感謝です。

RxWebKitには他にもcanGoBack()、canGoForward()に対してsubscribeすることもできるので、色々な用途に使えそうですね。

## Github Search サンプルアプリ

# 次のステップ

- RxSwiftを使って何かアプリを作ってみる
- 技術書を書く
- ブログやQiita等にアウトプットする
- 仕事で使っているアプリに導入してみる

## 学習方法

- 自分の勉強方法を書く
  - アプリを作った
  - 仕事でRxSwift+MVVMのコードを書いた
- アウトプットが大事
  - この本を書きはじめた時はフワっとした理解だったけど本として体系的にまとめているうちに知識が定着してきた
  - 深いところも少し知ることができた

## コミュニティ

- RxSwift Slack
- iOSアプリ開発がんばるぞ！！の会
- iOSDC
- try! Swift

# 様々なRxSwift系ライブラリ
## RxOptional
## RxWebkit
## RxDataSources