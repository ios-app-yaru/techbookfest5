= さまざまなRxSwift系ライブラリ

//lead{
  この章ではさまざまなRxSwiftの拡張ライブラリについて紹介していきます。多くの有名な拡張ライブラリは、RxSwift CommunityというGitHub Projectで管理されていて、さまざまな背景をもった人が日々コントリビュートしています。
//}

== RxDataSources ★1.4k

RxDataSourcesはざっくりいうと、UITableView、UICollectionViewをRxSwiftの仕組みを使ってイイ感じに差分更新をしてくれるライブラリです。
このライブラリを使うと、UITableViewやUICollectionViewを使ったアプリを作る際にdelegateの実装の負担が少なく済むようになったり、セクションを楽に組み立てられるようになったりします。

簡単なサンプルを作ってみましょう。

=== イメージ

//image[rxdatasource-tableview-sample][RxDataSources+UITableViewのサンプル]{
  RxDataSources+UITableViewのサンプル
//}

  * 新規プロジェクトをSingleViewAppで作成
  * ライブラリの導入

//cmd{
pod init
vi Podfile
//}

//emlist[]{
  platform :ios, '11.4'
  use_frameworks!

  target 'RxDataSourceExample' do
    pod 'RxDataSources', '~> 3.0'
  end
//}

//cmd{
pod install
//}

５章の開発を加速させる設定を済ませた前提で進めます。@<br>{}
まずはモデルを作ります。

//list[person-swift][Person.swift][swift]{
  struct Person {
    let name: String
  }
//}

次に、SectionModelというのを作成します
SectionModelはSectionModelTypeプロトコルに準拠する構造体で定義し、これをうまく使うことでセクションを

//list[section-of-person][SectionOfPerson][swift]{
  struct SectionOfPerson {
      var header: String
      var items: [Item]
  }

  extension SectionOfPerson: SectionModelType {
      typealias Item = Person

      init(original: SectionOfPerson, items: [SectionOfPerson.Item]) {
          self = original
          self.items = items
      }
  }
//}

@<code>{SectionOfPerson}の@<code>{header}がSectionのTitleで、@<code>{items}がSection内のセルデータ群です。@<br>{}
このサンプルアプリでは、@<code>{header}に@<code>{section 1}、@<code>{items}に@<code>{Persion(name: "Nozaki")}などが入るようなイメージです。

次に、ViewModelを作っていきましょう。

//list[customviewmodel][CustomViewModel.swift][swift]{
  class CustomViewModel {

    let items = BehaviorRelay<[SectionOfPerson]>(value: [])

    func updateItem() {
        var sections: [SectionOfPerson] = []
        sections.append(SectionOfPerson(header: "section 1", items: [SectionOfPerson.Item(name: "Nozaki"), SectionOfPerson.Item(name: "Sakura")]))
        sections.append(SectionOfPerson(header: "section 2", items: [SectionOfPerson.Item(name: "Kashima"), SectionOfPerson.Item(name: "Hori")]))
        sections.append(SectionOfPerson(header: "section 3", items: [SectionOfPerson.Item(name: "Seo"), SectionOfPerson.Item(name: "Wakamatsu")]))
        items.accept(sections)
    }
  }
//}

//list[customviewcontroller][CustomViewController.swift][swift]{
import UIKit
import RxSwift
import RxDataSources

class CustomViewController: UIViewController, UITableViewDelegate {

  @IBOutlet weak var tableView: UITableView!

  private var disposeBag = DisposeBag()

  # こいつとデータのBehaviorRelayをbindするとtableReloadやnumberOfSectionsをよしなにやってくれる
  private lazy var dataSource = RxTableViewSectionedReloadDataSource<SectionOfPerson>(configureCell: configureCell, titleForHeaderInSection: titleForHeaderInSection)

  # delegateでいう `cellForRowAt` の部分
  private lazy var configureCell: RxTableViewSectionedReloadDataSource<SectionOfPerson>.ConfigureCell = { [weak self] (dataSource, tableView, indexPath, person) in
    let cell = tableView.dequeueReusableCell(withIdentifier: "cell", for: indexPath)
    cell.textLabel?.text = person.name
    return cell
  }

  # delegateでいう `titleForHeaderInSection` の部分
  private lazy var titleForHeaderInSection: RxTableViewSectionedReloadDataSource<SectionOfPerson>.TitleForHeaderInSection = { [weak self] (dataSource, indexPath) in
    return dataSource.sectionModels[indexPath].header
  }

  private var viewModel: CustomViewModel!

  override func viewDidLoad() {
    super.viewDidLoad()
    setupViewController()
    setupTableView()
    setupViewModel()
  }

  private func setupViewController() {
    self.title = "タイトル"
  }
  private func setupTableView() {
    tableView.register(UITableViewCell.self, forCellReuseIdentifier: "cell")
    tableView.rx.setDelegate(self).disposed(by: disposeBag)
  }
  private func setupViewModel() {
    viewModel = CustomViewModel()

    # SectionModelとdataSourceをbindさせる
    viewModel.items
      .bind(to: tableView.rx.items(dataSource: dataSource))
      .disposed(by: disposeBag)

    viewModel.updateItem()
  }
}
//}

== RxAlamofire ★1k

== RxKeyboard ★800

== RxOptional ★441

== RxFirebase ★42
