= RxSwiftの基本的な書き方

//lead{
本章では、RxSwiftの基本的な書き方や仕組みについて解説していきます。
RxSwiftを支える全ての仕組みを解説することは本書のテーマから逸れてしまうので、良く使われるところを抜粋して解説します。
//}

== メソッドチェーンのように直感的に書ける

RxSwift/RxCocoaは、メソッドチェーンのように直感的にコードを書くことができます。@<br>{}
メソッドチェーンとは、その名前のとおりメソッドを実行し、その結果に対してさらにメソッドを実行するような書き方を指します。
jQueryを扱ってた人はなんとなく分かるのではないでしょうか？

たとえば、次のように書けます。

//listnum[method-chain-example][hogeButtonのイベント購読][swift]{
hogeButton.rx.tap
  .subscribe(onNext: { [weak self] in
    // 処理
  })
  .disposed(by: disposeBag)
//}

hogeButtonのタップイベントを購読し、タップされたときにsubscribeメソッドの引数であるonNextのクロージャ内の処理を実行します。@<br>{}
最後にクラスが解放されたとき、自動的に購読を破棄してくれるように@<code>{disposed(by:)}メソッドを使っています。

まとめると、次のように処理を定義しています。

  1. イベントの購読
  2. イベントが流れてきたときの処理
  3. クラスが破棄されると同時に購読を破棄

== Hello World

RxSwiftでのHelloWorld的なものを書いてみます。@<br>{}

//listnum[ストリームを定義し、好きなタイミングで値を流し、購読する][subject-example][swift]{
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
//}

結果

//emlist[]{
value = Hello World!
value = Hello World!!
value = Hello World!!!
//}

処理の流れのイメージは次のとおりです。

  1. @<code>{helloWorldSubject}というSubjectを定義
  2. Subjectを購読
  3. 値が流れてきたら print文で値を出力させる
  4. 定義したクラスが破棄されたら購読も自動的に破棄させる
  5. 値を流す

Subjectを使った書き方はViewController/ViewModel間のデータの受け渡しや、@<br>{}
遷移元/遷移先のViewController間でのデータの受け渡しでよく使われます。

また、前述したコードは同じクラス内に書いていて、Subjectの強みが生かせていません。@<br>{}
実際にViewController/ViewModelに分けて書いてみましょう。

//listnum[subject-example-viewmodel][ViewController/ViewModelに分けて書く][swift]{
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
//}

出力結果自体はさきほどと同じです。@<br>{}
このコードを図で表してみましょう。

//image[helloworldsubject][Subject イメージ図]{
Subject イメージ図
//}

ViewControllerがViewModelのSubjectを購読し、イベントを受け取っています。@<br>{}
なんとなく処理の流れは掴めたでしょうか？@<br>{}
ここで記載したHelloWorldのコードで使われている仕組みはRxSwiftの機能のほんの一部にしかすぎないので、やりながら覚えていきましょう！

== よく使われるクラス・メソッドについて

さて、ここまで本書を読み進めてきた中で、いくつか気になるワードやクラス、メソッドが出てきたのではないでしょうか？@<br>{}
ここからようやくそれらのクラスと支える概念についてもう少し深く触れていきます。

=== Observable

Observableは翻訳すると観測可能という意味で文字どおり観測可能なものを表現するクラスで、イベントを検知するためのものです。@<br>{}
Observableが通知するイベントには次の種類あります。

  * onNext
  ** デフォルトのイベントを流す
  ** イベント内に値を格納でき、何度でも呼びせる
  * onError
  ** エラーイベント
  ** １度だけ呼ばれ、その時点で終了、購読を破棄
  * onCompleted
  ** 完了イベント
  ** １度だけ呼ばれ、その時点で終了、購読を破棄

コードでは、次のように扱います。

//listnum[subject-subscribe-example][hogeObservableの購読の仕方][swift]{
hogeObservable
  .subscribe(onNext: {
    print("next")
  }, onError: {
    print("error")
  }, onCompleted: {
    print("completed")
  })
//}

onNextイベントが流れてきたときはonNextのクロージャが実行され、onErrorイベントが流れてきたときはonErrorのクロージャが実行されます。@<br>{}
また、onErrorやonCompletedは省略することができ、それらのイベントが流れてこないことが保証されている場合は省略しましょう。

省略する場合は次のように書きます。

//listnum[abridgement-other-than-onnext][onNext以外を省略する][swift]{
hogeObservable
  .subscribe(onNext: {
    print("next")
  })
//}

また、Observableは次の図を使って説明されることがよくあります。

//image[what-observable][Observable]{
Observable
//}

RxSwift（Reactive Extensions）について少し調べた方は大体みたことのあるような図ではないでしょうか？@<br>{}
この図はマーブルダイアグラムといい、横線が時間軸で左から右に時間が流れるようなイメージです。@<br>{}
マーブルダイアグラムを使った解説は初級者向けではないのでここでは割愛しますが、頭の隅に置いといてください。

==== Tips: ObservableとObserver

さまざまな資料に目を通していると、ObservableとObserverという表現が出てきますがどちらも違う意味です。@<br>{}
イベント発生元がObservableでイベント処理がObserverです。ややこしいですね。

コードで見てみましょう。

//listnum[subject-observable-observer][ObservableとObserver][swift]{
hogeObservable // Observable (イベント発生元)
  .map { $0 * 2 } // Observable (イベント発生元)
  .subscribe(onNext: {
    // Observer(イベント処理)
  })
  .disposed(by: disposeBag)
//}

コードで見てみるとわかりやすいです。名前は似てますが違う意味だというのを頭の隅に入れておくと理解がより進みます。

=== Dispose

ここまでコードを見てくると、なにやらsubscribeしたあとに必ず@<code>{disposed(by:)}メソッドが呼ばれているのが分かるかと思います。さてこれは何でしょう？

一言で説明すると、これはイイ感じに購読を破棄して、メモリリークを回避するための仕組みです。

Observableをsubscribe（bind等）すると、Disposeクラスのインスタンスが帰ってきます。@<br>{}
Disposeは購読を解除（破棄）するためのクラスで、@<code>{dispose()}メソッドを呼ぶことで明示的に購読を破棄できます。

今回はそのDisposeクラスの@<code>{disposed(by:)}メソッドを使っていています。@<br>{}
クラス内にDisposeBagクラスのインスタンスを保持しておいて、引数のそのインスタンスを渡します。@<br>{}
引数として渡すとDisposeBagはDisposeインスタンスを貯め、自身が解放（deinit）されたときに管理している購読を全て自動で解放（unsubscribe）してくれるようになります。@<br>{}
特に購読の破棄を意識することなく、Observableを扱えるようになっているのはこの仕組みのおかげです。

コードで見てみましょう。

//listnum[dispose-example][Disposeのサンプルコード][swift]{
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
//}

@<list>{dispose-example}では、HogeViewControllerが解放（deinit）されるときに保持しているhogeButtonとfooButtonのDisposableをdisposeしてくれます。

とりあえず購読したら@<code>{disposed(by: disposeBag)}しておけば大体間違いないです。

==== Tips: シングルトンインスタンス内でDisposeBagを扱うときは注意！

DisposeBagはとても便利な仕組みですが、シングルトンインスタンス内で扱う時は注意が必要です。@<br>{}
DisposeBagの仕組みはそのクラスが解放されたとき、管理してるDisposableをdisposeするとさきほど記述しました。@<br>{}
つまりDisposeBagのライフサイクルは保持しているクラスのライフサイクルと同一のものになります。

しかし、シングルトンインスタンスのライフサイクルはアプリのライフサイクルと同一のため、いつまでたっても@<code>{dispose}されず、最悪の場合メモリリークになる可能性があります。@<br>{}
回避策がまったくないわけではありませんが、ここでは詳細を省きます。@<br>{}
シングルトンインスタンスで扱う場合には注意が必要！ということだけ覚えておいてください。

=== Subject、Relay

Subject、Relayは簡単にいうと、イベントの検知に加えてイベントの発生もできる便利なクラスです。@<br>{}
ここで少しObservableについて振り返ってみましょう。Observableとは、イベントを検知するためのクラスでした。@<br>{}
Subject、Relayはそれに加えて自身でイベントを発生させることもできるクラスです。

代表としてよく使われる次の４種類を紹介します。

  * PublishSubject
  * BehaviorSubject
  * PublishRelay
  * BehaviorRelay

それぞれの違いをざっくりとですが、次のテーブル図にまとめました。

//table[kind-of-subject-relay][SubjectとRelayの主な種類]{
.	流れるイベント	初期値
-------------------------------
PublishSubject	onNext, onError, onComplete	持たない
BehaviorSubject	onNext, onError, onComplete	持つ
PublishRelay	onNext	持たない
BehaviorRelay	onNext	持つ
//}

=== 初期値について

初期値をもつ・もたないの違いは、Subject、Relayをsubscribe（bind等）した瞬間にイベントが流れるか流れないかの違いで、やりたいことに合わせて使い分けます。

=== それぞれの使い分け

大まかな使い分けは次の通りです。

  * Subject
  ** 「通信処理やDB処理等」エラーが発生したときにその内容によって処理を分岐させたい
  * Relay
  ** UIに値をBindする

UIにBindしているObservableでonErrorやonCompleteが発生しまうと購読が止まってしまい、そのさきのタップイベントや入力イベントを拾えなくなってしまうので、onNextのみが流れることが保証されているRelayを使うのが適切です。

==== Tips: internal（public）なSubject,Relay

Subject, Relayはすごく便利でいろいろなことができます。@<br>{}
便利なのは開発の幅が広がるのでよいことですが、逆にコードが複雑になってしまうことがあります。

どういうことかというと、internal（public）なSubject、Relayを定義してしまうと、@<br>{}
クラスの外からもイベントを発生させることができるため、アプリが肥大化していくうちにどこでイベント発生させているかわかりずらくなり、デバッグをするのが大変になってきます。

その解決策として、Subject、Relayはprivateとして定義して、外部へ公開するためのObservableを用意するのが一般的に用いられています。

次のコードのように定義します。

//listnum[behaviorrelay-example][BehaviorRelayのサンプル][swift]{
private let items: BehaviorRelay<[Item]>(value: [])

var itemsObservable: Observable<[Item]> {
  return items
}
//}

==== Tips: Relayは、Subjectの薄いラッパー

SubjectとRelay、それぞれ特徴が違うと書きましたが、Relayの実装コードを見てみると実はRelayはSubjectの薄いラッパーとして定義されていることがわかります。@<br>{}
それぞれonNextイベントは流せますが、Relayの場合はonNextイベントを流すメソッドがSubjectと異なるので注意しましょう

//listnum[subject-relay-on-next-diff][SubjectとRelayのonNextイベントの流し方][swift]{
let hogeSubject = PublishSubject<String>()
let hogeRelay = PublishRelay<String>()

hogeSubject.onNext("ほげ")
hogeRelay.accept("ほげ")
//}

呼び出すメソッドが違うからなにか特別なことしてるのかな？と思うかもしれませんが、特別なことはしていません。@<br>{}
PublishRelayのコードを見てみましょう。

//listnum[relay-rxswift-code][PublishRelayの実装コード（一部省略）][swift]{
public final class PublishRelay<Element>: ObservableType {
  public typealias E = Element

  private let _subject: PublishSubject<Element>

  // Accepts `event` and emits it to subscribers
  public func accept(_ event: Element) {
    _subject.onNext(event)
  }

  // ...
//}

コードを見てみると、内部的にはonNextを呼んでいるので、特別なことはしていないというのがわかります。

=== bind

Observable/Observerに対してbindメソッドを使うと指定したものにイベントストリームを接続することができます。@<br>{}
「bind」と聞くと双方向データバインディングを想像しますが、RxSwiftのbindは単方向データバインディングです。

bindメソッドは、独自でなにか難しいことをやっているわけではなく、振る舞いはsubscribeとだいたい同じです。

実際にコードを比較してみましょう。

//listnum[bind-example][Bindを用いたコードのサンプル][swift]{
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
//}

上記のコードでは①bindを利用した場合と②subscribeを利用した場合それぞれ定義しました。
２つのコードはまったく同じ動作をします、振る舞いが同じという意味が伝わったでしょうか？

=== Operator

ここまでのコードでは、Observableから流れてきた値をそのままsubscribe（もしくはbind）するコードがほとんどでした。@<br>{}
ですが実際のプロダクションコードではそのままsubscribe（bind）することはほぼ無く、何か途中で値を加工してsubscribe（bind）する場合が多いです。@<br>{}
たとえば、入力されたテキストの文字数を数えて「あとN文字」とラベルのテキストに反映する仕組みはよくあるパターンの１つです。

そこで活躍するのがOperatorという概念です。@<br>{}
OperatorはObservableに対してイベントの値の変換・絞り込み等、加工を施して新たにObservableを生成する仕組みです。@<br>{}
他にも、２つのObservableのイベントを合成・結合もできます。

Operatorは色々なことができて全てのOperatorの概要、使い方の説明だけで１冊の本が書けるレベルです。@<br>{}
本書とは少し趣旨がずれてしまうため、ここではよく使われるOperatorにフォーカスを絞って、紹介します。

よく使われるOperatorは次のとおりです。

  * 変換
  ** map
  *** 通常の高階関数と同じ動き
  ** flatMap
  *** 通常の高階関数と同じ動き
  ** reduce
  *** 通常の高階関数と同じ動き
  ** scan
  *** reduceに似ていて途中結果もイベント発行ができる
  ** debounce
  *** 指定時間イベントが発生しなかったら最後に流されたイベントを流す
  * 絞り込み
  ** filter
  *** 通常の高階関数と同じ動き
  ** take
  *** 指定時間の間だけイベントを通知してonCompletedする
  ** skip
  *** 名前のとおり、指定時間の間はイベントを無視する
  ** distinct
  *** 重複イベントを除外する
  * 組み合わせ
  ** zip
  *** 複数のObservableを組み合わせる（異なる型でも可能）
  ** merge
  *** 複数のObservableを組み合わせる（異なる型では不可能）
  ** combineLatest
  *** 複数のObservableの最新値を組み合わせる（異なる型でも可能）
  ** sample
  *** 引数に渡したObservableのイベントが発生されたら元のObservableの最新イベントを通知
  ** concat
  *** 複数のObservableのイベントを順番に組み合わせる（異なる方では不可能）

RxSwiftを書き始めたばっかりの人はどれがどんな動きをするか全然わからないと思うので、@<br>{}
さらにスコープを狭めて、簡単でよく使うものをサンプルコードを加えてピックアップしました。

==== map

//listnum[operator-map-example][Operator - map のサンプル][swift]{
// hogeTextFieldのテキスト文字数を数えてfooTextLabelのテキストへ反映
hogeTextField.rx.text
  .map { return "あと\($0.count)文字 }
  .bind(to: fooTextLabel.rx.text)
  .disposed(by: disposeBag)
}
//}

==== filter

//listnum[operator-filter-example][Operator - filter サンプル][swift]{
// 整数が流れるObservableから偶数のイベントのみに絞り込んでevenObservableに流す
numberObservable
  .filter { $0 % 2 == 0 }
  .bind(to: evenObservable)
  .disposed(by: disposeBag)
//}

また、かならずObservableに流れるイベントの値を使う必要はありません。@<br>{}
次のようにクラス変数やメソッド内変数を取り入れてbindすることもできます。

//listnum[operator-filter-2-example][Operator - filter サンプル2][swift]{
// ボタンをタップしたときにnameLabelにユーザの名前を表示する
let user = User(name: "k0uhashi")

showUserNameButton.rx.tap
  .map { [weak self] in
    return self?.user.name
  }
  .filterNil() // import RxOptionalが必要
  .bind(nameLabel.rx.text)
  .disposed(by: disposeBag)
//}

==== zip

複数のAPIにリクエストして同時に反映したい場合に使用します。

//listnum[operator-zip-example][Operator - zip サンプル][swift]{
Observable.zip(api1Observable, api2Observable)
  .subscribe(onNext { (api1, api2) in
    // ↑タプルとして受け取ることができます
    // ...
  })
  .disposed(by: disposeBag)
//}

== HotなObservableとColdなObservable

これまでObservableとそれらを支える仕組みについて記載してきました。@<br>{}
Observableには実は２種類の性質があり、HotなObservableとColdなObservableというのがあります。

本書ではHotなObservableを主に扱いますが、ColdなObservableもあるということを頭の中に入れておきましょう。@<br>{}
HotなObservableの特徴は次のとおりです。

  * subscribeされなくても動作する
  * 複数の箇所でsubscribeしたとき、全てのObservableで同じイベントが同時に流れる

ColdなObservableの特徴は次のとおりです。

  * subscribeしたときに動作する
  * 単体では意味がない
  * 複数の箇所でsubscribeしたとき、それぞれのObservableでそれぞれのイベントが流れる

ColdなObservableは主に非同期通信処理で使われます。@<br>{}
試しにsubscribe時に１つの要素を返すObservableを作成する関数を定義してみましょう

//listnum[cold-observable-example][CondなObservableのサンプル][swift]{
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
//}

余談ですが、このような１回通知してonCompletedするObservableのことは「just」と呼ばれてます@<br>{}

Observableのcreate関数はコードを見て分かるとおり、クロージャを使用してsubscribeメソッドを実装できる簡単で便利な関数です。@<br>{}
subscribeメソッドと同様にobserverを引数にとり、disposableを返却します。

==== 振り返りTips: myJustはdisposed（by:）しなくてもよい

さきほど作ったmyJust関数は、@<code>{disposed(by:)}もしくは@<code>{dispose()}を呼ばなくても大丈夫です。少し振り返ってみましょう。

Observableの特徴としてonError、onCompletedイベントは１度しか流れず、その時点で購読を破棄するというのがありましたね。@<br>{}
myJust関数内ではonNextイベントが送られたあと、onCompletedイベントを送っています。なので明示的に購読を破棄する必要はありません。
