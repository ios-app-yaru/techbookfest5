= 簡単なアプリを作ってみよう！

//lead{
ここまではRxSwift/RxCocoaの概念や基本的な使い方について紹介してきました。
本章では実際にアプリを作りながら解説していきます。
//}

まずは簡単なアプリから作ってみましょう。@<br>{}
いきなりRxSwiftを使ってコードを書いても理解に時間がかかるかと思うので、１つのテーマごとにcallbackやdelegate、KVOパターンを使って実装し、これをどうRxSwiftに置き換えるか？という観点でアプリを作っていきます。@<br>{}
（本書のテーマである「比較して学ぶ」というのはこのことを指しています）

では、作っていきましょう！

== カウンターアプリを作ってみよう！

//lead{
この節ではカウンターアプリをテーマにcallback、delegate、RxSwift、それぞれのパターンで実装したコードを比較し、どう書くかを学びます。
//}

まずはアプリの機能要件を決めます。

=== 機能要件

  * カウントの値が見れる
  * カウントアップができる
  * カウントダウンができる
  * リセットができる

=== 画面のイメージ

//image[counterappscreenshot][作成するアプリのイメージ][scale=0.3]{
  作成するアプリのイメージ
//}

=== プロジェクトの作成

まずはプロジェクトを作成します。ここは特別なことをやっていないのでサクサクといきます。

//image[create-project][プロジェクトの作成][scale=0.6]{
  プロジェクトの作成
//}

Xcodeを新規で起動して、 Create a new Xcode project を選択します。

//image[select-template][テンプレートの選択][scale=0.6]{
  テンプレートの選択
//}

テンプレートを選択します。 Single View App を選択

//image[project-name-settings][プロジェクトの設定][scale=0.6]{
  プロジェクトの設定
//}

プロジェクトの設定をします。 ここは各自好きなように設定してください。@<br>{}
Nextボタンを押してプロジェクトの作成ができたら、一度Xcodeを終了します

=== 環境設定

terminal.app を起動し、作成したプロジェクトのディレクトリまで移動します。

//cmd{
ryo-takahashi@~/CounterApp>
//}

ライブラリの導入を行います。プロジェクト内でCocoapodsの初期化を行いましょう。

//cmd{
pod init
//}

成功すると、ディレクトリ内にPodfileというファイルが生成されているのでこれを編集します。

//cmd{
vi Podfile
//}

ファイルを開いたら、次のように編集してください。

//listnum[podfile-rxswift-rxcocoa][Podfileの編集][ruby]{
# platform :ios, '9.0'

target 'CounterApp' do
  use_frameworks!

  pod 'RxSwift' # ★この行を追加
  pod 'RxCocoa' # ★この行を追加

end
//}

編集して保存したら、導入のためインストール用コマンドを入力します。

//cmd{
pod install
//}

次のような結果が出たら成功です。

//cmd{
Analyzing dependencies
Downloading dependencies
Installing RxCocoa (4.2.0)
Installing RxSwift (4.2.0)
Generating Pods project
Integrating client project

[!] Please close any current Xcode sessions and use
`CounterApp.xcworkspace` for this project from now on.Sending stats
Pod installation complete! There are 2 dependencies
from the Podfile and 2 total pods installed.

[!] Automatically assigning platform `ios` with version `11.4` on
target `CounterApp` because no platform was specified.
Please specify a platform for this target in your Podfile.
See `https://guides.cocoapods.org/syntax/podfile.html=platform`.
//}

環境設定はこれで完了です。@<br>{}
次回以降プロジェクトを開く時は、必ず「CounterApp.xcworkspace」 から開くようにしましょう

（Xcode上、もしくはFinder上でCounterApp.xcworkspaceを指定しないと導入したライブラリが使えません）

=== 開発を加速させる設定

//lead{
■この節は今後何度も使うので付箋やマーカーを引いておきましょう！
//}

この節では、開発を加速させる設定を行います。@<br>{}
具体的には、Storyboardを廃止して ViewController.swift + xib を使って開発する手法に切り替えるための設定を行います。

==== Storyboardの廃止

Storyboardは画面遷移の設定が簡単にできたり、一見で画面がどう遷移していくかわかりやすくてよいのですが、@<br>{}
アプリが大きくなってくると画面遷移が複雑になって見辛くなったり、小さなViewController（アラートやダイアログを出すものなど）の生成が面倒だったり、@<br>{}
チーム人数が複数になると*.storyboardがconflictしまくるなど色々な問題があるので、Storyboardを使うのをやめます。

Storyboardを廃止するために、次のことを行います

  * Main.storyboardの削除
  * Info.plistの設定
  * AppDelegateの整理
  * ViewController.xibの作成

===== Main.storyboardの削除

  * CounterApp.xcworkspaceを開く
  * /CouterApp/Main.storyboardをDelete
  ** Move to Trashを選択

===== Info.plist@<br>{}

Info.plistにはデフォルトでMain.storyboardを使ってアプリを起動するような設定が書かれているので、その項目を削除します。

  * Info.plistを開く
  * Main storyboard file base name の項目を削除する

===== AppDelegateの整理

Main.storyboardを削除したことによって、一番最初に起動するViewControllerの設定が失われるため、アプリを正しく起動できません。
これを解消するために、AppDelegate.swiftに一番最初に起動するViewControllerを設定します。

//listnum[appdelegate][AppDelegate.swiftを開く][swift]{
//AppDelegate.swift
import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

  var window: UIWindow?

  func application(_ application: UIApplication,
      didFinishLaunchingWithOptions launchOptions:
        [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
    self.window = UIWindow(frame: UIScreen.main.bounds)
    let navigationController =
      UINavigationController(rootViewController: ViewController())
    self.window?.rootViewController = navigationController
    self.window?.makeKeyAndVisible()
    return true
  }

}
//}

===== ViewController.xibの作成

Main.storyboardを削除してことによって一番最初に起動するViewControllerの画面のデータがなくなってしまったので新しく作成します。

  * New File > View > Save As: ViewController.xib > Create
  * ViewController.xibを開く
  * Placeholders > File's Owner を選択
  * Class に ViewControllerを指定

//image[viewcontroller-filesowner][ViewController.xibの設定１][scale=0.8]{
  ViewController.xibの設定
//}

OutletsのviewとViewControllerのViewを接続します。

//image[viewcontroller-view-outlet][ViewController.xibの設定２][scale=0.8]{
  ViewController.xibの設定２
//}

これでアプリの起動ができるようになりました。Build & Run で確認してみましょう。@<br>{}
次のような画面が出たら成功です。

//image[init-clearn-viewcontroller][起動したアプリの画面][scale=0.3]{
  起動したアプリの画面
//}

起動に失敗する場合、ViewController.xibが正しく設定されているかもういちど確認してみましょう。

これで環境設定は終了です。@<br>{}
今後画面を追加していくときは同様の手順で作成していきます。

===== 新しい画面を追加するときの手順まとめ

  1. ViewController.swiftの作成
  2. ViewController.xibの作成
  3. ViewController.xibの設定
  4. Classの指定
  5. ViewのOutletの設定

===== Tips: 画面遷移

ViewController.swift + Xib構成にしたことによって、ViewControllerの生成が楽になり、画面遷移の実装がが少ない行で済むようになりました。@<br>{}
画面遷移は次のコードで実装できます。

//listnum[pushviewcontroller][画面遷移の実装][swift]{
let viewController = ViewController()
navigationController?.pushViewController(viewController, animated: true)
//}

=== CallBackパターンで作るカウンターアプリ

さて、ようやくここから本題に入ります、まずはViewController.swiftを整理しましょう。

  * ViewController.swiftを開く
  * 次のように編集
  ** didReceiveMemoryWarningメソッドは特に使わないので削除します。

//listnum[viewcontroller-arrangement][ViewControllerの整理][swift]{
import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
    }
}
//}

スッキリしました。使わないメソッドやコメントは積極的に削除していきましょう。@<br>{}
次に、画面を作成します。

UIButton３つとUILabelを１つ配置します。

//image[counter-app-interfacebuilder][部品の設置][scale=0.8]{
部品の設置
//}

UI部品の配置が終わったら、ViewController.swiftとUIを繋げます。@<br>{}
UILabelはIBOutlet、UIButtonはIBActionとして繋げていきます。

//listnum[viewcontroller-add-ibaction][IBActionの作成][swift]{
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
//}

次に、ViewModelを作ります。ViewModelには次の役割をもたせています。

  * カウントデータの保持
  * カウントアップ、カウントダウン、カウントリセットの処理

//listnum[create-callback-pattern-viewmodel][ViewModelの作成][swift]{
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
//}

ViewModelを作ったので、ViewControllerでViewModelを使うように修正します。

//listnum[fix-callback-viewcontroller-viewmodel][ViewControllerの修正][swift]{
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
//}

これで、機能要件を満たすことができました。@<br>{}
実際に Build & Run して確認してみましょう。

callbackで書く場合のメリット・デメリットをまとめてみます。

  * メリット
  ** 記述が簡単
  * デメリット
  ** ボタンを増やすたびに対応するボタンの処理メソッドが増えていく
  *** ラベルの場合も同様
  *** 画面が大きくなっていくにつれてメソッドが多くなり、コードが読みづらくなってくる
  ** ViewControllerとViewModelに分けたものの、完全にUIと処理の切り分けができているわけではない

=== Delegateで作るカウンターアプリ

次に、delegateを使って実装してみましょう。@<br>{}
delegateを使う場合、設計はMVPパターンのほうが向いてるので、MVPパターンに沿って実装していきます。

まずはDelegateを作ります

//listnum[create-counter-delegate-protocol][Delegateの作成][swift]{
protocol CounterDelegate {
  func updateCount(count: Int)
}
//}

次に、Presenterを作ります

//listnum[create-counter-presenter][Presenterの作成][swift]{
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
//}

最後に、ViewControllerをさきほど作成したPresenterを使うように修正しましょう。

//listnum[fix-viewcontroller-presenter][ViewControllerの修正][swift]{
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
//}

Build ＆ Run してみましょう。callbackの場合とまったく同じ動きをしていたら成功です。@<br>{}
Delegateを使った書き方のメリット・デメリットをまとめます。

  * メリット
  ** 処理を委譲できる
  *** @<code>{incrementCount()}、@<code>{decrementCount()}、@<code>{resetCount()}がデータの処理に集中できる
  *** @<code>{callback(count)}しなくてもよい
  * デメリット
  ** ボタンを増やすたびに対応する処理メソッドが増えていく

データを処理する関数が完全に処理に集中できるようになったのはよいことですが、ボタンとメソッドの個数が１：１になっている問題がまだ残っています。@<br>{}
このままアプリが大きくなっていくにつれてメソッドが多くなり、どのボタンの処理がどのメソッドの処理なのかパッと見た感じではわからなくなり、コード全体の見通しが悪くなってしまいます。

この問題はRxSwift/RxCocoaを使うことで解決できます。@<br>{}
実際にRxSwiftを使って作ってみましょう。

=== RxSwiftで作るカウンターアプリ

さきほどのPresenterとCounterProtocolはもう使わないので削除しておきましょう。@<br>{}
まずはRxSwiftを用いたViewModelを作るためのProtocolとInput用の構造体を作ります

//list[create-protocol-struct-rxswift-pattern][ProtocolとStructの作成][swift]{
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
//}

次にViewModelを作ります。CallBackパターンでも作りましたが、紛らわしくならないように新しい名前で作り直します。

//listnum[create-viewmodel-rxswift-pattern][swift]{
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
//}

ViewControllerも修正しましょう。@<br>{}
全てのIBActionと接続を削除してIBOutletを定義し、接続しましょう。

//listnum[fix-viewcontroller-rxswift-pattern][ViewControllerの修正][swift]{
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
    let input = RxViewModelInput(
        countUpButton: countUpButton.rx.tap.asObservable(),
        countDownButton: countDownButton.rx.tap.asObservable(),
        countResetButton: countResetButton.rx.tap.asObservable()
      )
    viewModel = RxViewModel(input: input)

    viewModel.outputs?.counterText
      .drive(countLabel.rx.text)
      .disposed(by: disposeBag)
  }
}
//}

Build & Run で実行してみましょう。まったく同じ動作をしていたら成功です。

==== Tips: あれ？コード間違っていないのにクラッシュする？そんな時は

//lead{
新しい画面を作成・既存の画面をいじっていて、ふとBuild＆Runを実行したとき、あれ？コードに手を加えてないのにクラッシュするようになった？？と不思議になる場面は初心者の頃はあるあるな問題かと思います。

そんなときは、１度いじっていたxib/storyboardのIBActionの接続・接続解除、IBOutletの接続・接続解除が正しくできているか確認してみましょう。
//}

ViewController内では、setupViewModel関数として切り出して定義してviewDidLoad()内で呼び出しています。

この書き方についてまとめてみます。

  * メリット
  ** ViewController
  *** スッキリした
  *** Input/Outputだけ気にすれば良くなった
  ** ViewModel
  *** 処理を集中できた
  *** increment, decrement, resetがデータの処理に集中できた
  *** ViewControllerのことを意識しなくてもよい
  **** 例: @<code>{delegate?.updateCount(count: count)} のようなデータの更新の通知を行わなくてもよい
  ** テストがかきやすくなった
  * デメリット
  ** コード量が他パターンより多い
  ** 書き方に慣れるまで時間がかかる

一番大きなメリットはやはり「ViewModelはViewControllerのことを考えなくてもよくなる」ところです。@<br>{}
ViewControllerがViewModelの値を監視して変更があったらUIを自動で変更させているため、ViewModel側から値が変わったよ！と通知する必要がなくなるのです。

次に、テストが書きやすくなりました。@<br>{}
今まではViewControllerとViewModel（Presenter)が密になっていてテストが書きづらい状況でしたが、今回は分離ができているので書きやすいです。@<br>{}
やり方としてはViewModelをインスタンス化するときにInputを注入し、Outputを期待したとおりになっているかのテストを書くようなイメージです。

また、RxSwift+MVVMの書き方は慣れるまで時間がかかるかと思うので、まずはUIButton.rx.tapだけ使う、PublishSubject系だけを使う…など小さく始めるのも１つの方法です。

=== まとめ

この章では、callback、delegate、RxSwift、３つのパターンでカウンターアプリを作りました。@<br>{}
callback、delegateパターンで課題であったUIとデータの分離できていない問題に関しては、RxSwiftを用いたことで解決できました。

全ての開発においてRxSwiftを導入した書き方が正しいとは限りませんが、１つの解決策として覚えておくだけでもよいと思います。


==== おまけ：カウンターアプリを昇華させよう１

この章はおまけです。さきほど作ったコードに加えて、次の機能を追加してみましょう！

  * 追加の機能要件
  ** +10カウントアップできる
  ** -10カウントダウンできる
  ** カウンターの値をDBに保存しておいて、復帰時にDBから参照させるように変更

== WebViewアプリを作ってみよう！

//lead{
この章ではWKWebViewを使ったアプリをテーマに、KVOの実装パターンをRxSwiftに置き換える方法について学びます。
//}

=== この章のストーリー

  * WKWebView+KVOを使ったWebViewアプリを作成
  * WKWebView+RxSwiftに書き換える

=== イメージ

//image[wkwebview][アプリのイメージ][scale=0.8]{
アプリのイメージ
//}

WebViewとProgressViewを配置して、Webページの読み込みに合わせてゲージ・インジケータ・Navigationタイトルを変更するようなアプリを作ります。

サクっといきましょう！まずは新規プロジェクトを作成します。@<br>{}
プロジェクトの設定やViewControllerの設定は第５章の「開発を加速させる設定」を参照してください。@<br>{}
ここではWKWebViewController.xibという名前で画面を作成し、中にWKWebViewとUIProgressViewを配置します。@<br>{}
画面ができたら、ViewControllerクラスを作っていきます。

//list[kvo-webview][KVOで実装する][swift]{
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
    webView.addObserver(self, forKeyPath: "loading",
      options: .new, context: nil)
    // webView.estimatedProgressの値の変化を監視
    webView.addObserver(self, forKeyPath: "estimatedProgress",
      options: .new, context: nil)

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

  override func observeValue(forKeyPath keyPath: String?,
      of object: Any?, change: [NSKeyValueChangeKey : Any]?,
      context: UnsafeMutableRawPointer?) {
    if keyPath == "loading" {
      UIApplication.shared
        .isNetworkActivityIndicatorVisible = webView.isLoading
      if !webView.isLoading {
        // ロード完了時にProgressViewの進捗を0.0(非表示)にする
        progressView.setProgress(0.0, animated: false)
        // ロード完了時にNavigationTitleに読み込んだページのタイトルをセット
        navigationItem.title = webView.title
      }
    }
    if keyPath == "estimatedProgress" {
      // ProgressViewの進捗状態を更新
      progressView
        .setProgress(Float(webView.estimatedProgress), animated: true)
    }
  }
}
//}

KVO（Key-Value Observing:キー値監視）とは、特定のオブジェクトのプロパティ値の変化を監視する仕組みです。KVOはObjectiveーCのメカニズムを使っていて、NSValueクラスに大きく依存しています。@<br>{}
そのため、NSObjectを継承できない構造体（struct）はKVOの仕組みが使えません。

KVOをSwiftで使うためにはオブジェクトをclassで定義し、プロパティに@<code>{objc}属性と@<code>{dynamic}をつけます。
WKWebViewのプロパティのうち、@<code>{title}、@<code>{url}、@<code>{estimatedProgress}は標準でKVOに対応しているので、今回はそれを使います。

では実際コード内で何をしているかというと、@<code>{viewDidLoad()}時にWebViewのプロパティの値を監視させて、値が変更されたときにUIを更新させています。@<br>{}
addObserverの引数にプロパティ名を渡すとその値が変化された時に @<code>{observeValue(forKeyPath keyPath: String?, of object: Any?, change: [NSKeyValueChangeKey : Any]?, context: UnsafeMutableRawPointer?)} が呼ばれます。@<br>{}
@<code>{observeValue}の@<code>{keyPath}には@<code>{addObserver}で設定した@<code>{forKeyPath}の値が流れてくるので、その値で条件分岐してUIを更新します。

この方法では全ての値変化の通知を@<code>{observeValue}で受け取って条件分岐するため、段々と@<code>{observeValue}メソッドが肥大化していく問題があります。@<br>{}
また、KVOはObjective-Cのメカニズムであるため、型の安全性が考慮されていません。@<br>{}
さらに、KVOを使った場合の注意点として@<code>{addObserver}した場合、deinit時に@<code>{removeObserver}を呼ばないと、最悪の場合メモリリークを引き起こし、アプリが強制終了する可能性があります。@<br>{}
忘れずにremoveObserverを呼びましょう。

とはいえ、removeObserverを呼ぼうと注意していても人間である以上、絶対にいつか忘れます。@<br>{}
クラスが肥大化してくるにつれ、その確率は上がってきます。

こういった問題はRxSwiftを使うことで簡単に解決できます！RxSwiftに書き換えてみましょう。@<br>{}
と、その前にRxOptionalというRxSwiftの拡張ライブラリを導入します。理由は後述しますが、簡単にいうとOptionalな値を流すストリームに対してさまざまなことができるようになるライブラリです。

Podfileにライブラリを追加しましょう

//list[add-lib-rxoptional][Podfileの修正][ruby]{
pod 'RxSwift'
pod 'RxCocoa'
pod 'RxOptional' # ★この行を追加
//}

では、導入したライブラリも使いつつ、KVOで書かれた実装をRxSwiftを使うようにリプレースしていきます。

//list[fix-kvo-webview-to-rxswift-pattern][RxSwiftでリプレース][swift]{
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
//}

どうでしょうか？ネストも浅くなり、かなり読みやすくなりました。@<br>{}
色々説明するポイントはありますが、この章ではじめ出てきたメソッドについて説明していきます。

==== import RxOptional

導入したRxOptionalライブラリをswiftファイル内で使用するために宣言

==== rx.observe

@<code>{rx.observe}はKVOを取り巻く単純なラッパーです。単純であるため、パフォーマンスが優れていますが、用途は限られています。@<br>{}
@<code>{self.}から始まるパスと、子オブジェクトのみ監視できます。

たとえば、@<code>{self.view.frame}を監視したい場合、第二引数に@<code>{view.frame}を指定します。@<br>{}
ただし、プロパティに対して強参照するため、@<code>{self}内のパラメータに対して@<code>{rx.observe}してしまうと、循環参照を引き起こし最悪の場合アプリがクラッシュします。@<br>{}
弱参照したい場合は、@<code>{rx.observeWeakly}を使いましょう。

KVOはObjective-Cの仕組みで動いていると書きましたが、RxCocoaでは実は構造体であるCGRect、CGSize、CGPointに対してKVOを行う仕組みが実装されています。@<br>{}
これはNSValueから値を手動で抽出する仕組みを使っていて、RxCocoaライブラリ内のKVORepresentable+CoreGraphics.swiftにKVORepresentableプロトコルを使って抽出する実装コードが書かれているので、独自で作りたい場合はここを参照しましょう。

==== filterNil()

RxOptionalで定義されているOperator

名前でなんとなくイメージできるかもしれませんが、nilの場合は値を流さず、nilじゃない場合は@<code>{unwrap}して値を流すOperatorです。@<br>{}
コードで比較するとわかりやすいです、次のコードを見てみましょう。どちらもまったく同じ動作をします。

//list[operator-filternil-example][filterNil()の比較][swift]{
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
//}

==== share()

一言で説明すると、ColdなObservableをHotなObservableへ変換するOperatorです。@<code>{}
まずは次のコードを見てください。

//list[operator-share-example][share()がない場合][swift]{
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
//}

上記のコードはUITextFieldである@<code>{textField}へのテキスト入力を監視し、ストリームの途中で値を加工して複数のLabelへbindしています。

ここでtextFieldへ「123」と入力した場合、@<code>{print("call")}は何回呼ばれるか予想してみましょう。@<br>{}
パッと見た感じだと、3回入力するので3回出力するのでは？と思いがちですが実際は違います。実行して試してみましょう！

//emlist[]{
call
call
call
call
call
call
call
call
call
//}

callは９回呼ばれます。なるほど？@<br>{}
値を入力するたびにmap関数が３回呼ばれてますね。これはいけない。

今回のように値を変換したりprint出力するだけならそれほどパフォーマンスに影響はありませんが、データベースアクセスするものや、通信処理が発生するものではこの動作は好ましくありません。

なぜこの現象が起こるのか？@<br>{}
その前に、textField.rx.textが何なのかを紐解いて見ましょう。

@<code>{textField.rx.text}はRxCocoaでextension定義されているプロパティで、@<code>{Observable<String?>}ではなく、@<code>{ControlProperty<String?>}として定義されています。（実態はObservableですが。）@<br>{}
ControlPropertyは主にUI要素のプロパティで使われていて、メインスレッドで値が購読されることが保証されています。

また、実はこれはColdなObservableです。@<br>{}
ColdなObservableの仕様として、subscribeした時点で計算リソースが割当られ、複数回subscribeするとその都度ストリームが生成されるという仕組みがあると説明しました。

そのため、今回の場合３回@<code>{subscribe(bind)}したので、３個のストリームが生成されます。@<br>{}
するとどうなるかというと、値が変更されたときにOperatorが３回実行されてしまうようになります。

このままではまずいので、どうにかして何回購読してもOperatorを１回実行で済むように実装したいですね。@<br>{}
では、どうすればよいのかというと、HotなObservableに変換してあげるとよいです。

やりかたはいくつかあるのですが、今回は @<code>{share()} というOperatorを使います。@<br>{}

//list[use-share-example][share()を使う][swift]{
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
//}

Build & Run を実行してもう一度「１２３」とテキストに入力してみましょう。
出力結果が次のようになっていたら成功です。

//emlist[]{
call
call
call
//}

==== observeOn

ストリームの実行スレッドを決めるOperatorで、これよりあとに書かれているストリームに対して適用されます@<br>{}
引数には「ImmediateSchedulerType」プロトコルに準拠したクラスを指定します。

==== MainScheduler.instance

MainSchedulerのシングルトンインスタンスを指定しています。@<br>{}
observeOnの引数にMainSchedulerクラスのシングルトンインスタンスを渡してあげると、その先のOperatorはメインスレッドで処理してくれるようになります。

==== 本題へ

KVOで書いた処理をRxSwiftに置き換えてみた結果、かなり読みやすくなりました。@<br>{}
特に、@<code>{removeObserver}を気にしなくてもよくなるので多少は安全です。

@<code>{removeObserver}を気にしなくてもよくなったというよりは、RxSwiftの場合はremoveObserverの役割が@<code>{.disposed(by:)}に変わったイメージのほうがわかりやすいかもしれません。@<br>{}
@<code>{disposed(by:)} を結局呼ばないといけないのなら、そんなに変わらなくない？と思うかもしれませんが、RxSwiftでは呼び忘れるとWarningが出るのでremoveObserverだったころより忘れる確率はグッと低くなります。

しかし、書きやすくなったといっても、まだこの書き方では次の問題が残っています。

  * Key値がベタ書きになっている
  * 購読する値の型を指定してあげないといけない

自分でextensionを定義するのも１つの方法としてありますが、@<br>{}
実はもっと便利にWKWebViewを扱える「RxWebKit」というRxSwift拡張ライブラリがあるので、それを使ってみましょう。

Podfileを編集します

//list[add-rxwebkit-on-podfile][Podfileの編集][ruby]{
  pod 'RxSwift'
  pod 'RxCocoa'
  pod 'RxOptional'
  pod 'RxWebKit'
//}

ライブラリをインストールします。

//cmd{
pod install
//}

さきほど書いたRxSwiftパターンのコードを次のコードに書き換えてみましょう！

//listnum[use-rxwebkit][RxWebKitを用いる][swift]{
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
//}

Build & Run で実行してみましょう。まったく同じ動作であれば成功です。

RxWebKitを使ったことでさらに可動性があがりました。@<br>{}
RxWebKitは、WebKitをRxSwiftで使いやすくしてくれるように拡張定義しているラッパーライブラリです。@<br>{}
これを使うことで、「Keyのべた書き」と「値の型指定」問題がなくなりました。感謝です。

RxWebKitには他にも@<code>{canGoBack()}、@<code>{canGoForward()}に対してsubscribeやbindすることもできるので、いろいろな用途に使えそうですね。
