
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

ストリームの実行スレッドを決めるオペレータで、このオペレータよりあとに書かれているストリームに対して適用されます
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
