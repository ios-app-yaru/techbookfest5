= さまざまなRxSwift系ライブラリ

//lead{
この章ではさまざまなRxSwiftの拡張ライブラリについて紹介していきます。多くの有名な拡張ライブラリは、RxSwift CommunityというGitHub Projectで管理されていて、さまざまな人が日々コントリビュートしています。
//}

== RxDataSources ★1.4k

RxDataSourcesはざっくりいうと、UITableView、UICollectionViewをRxSwiftの仕組みを使ってイイ感じに差分更新をしてくれるライブラリです。@<br>{}
このライブラリを使うと、UITableViewやUICollectionViewを使ったアプリを作る際にdelegateの実装の負担が少なく済むようになったり、セクションを楽に組み立てられるようになったりします。

=== 作ってみよう！

RxDataSourcesを使って簡単なUITableViewアプリを作ってみましょう。

=== イメージ

//image[rxdatasource-tableview-sample][RxDataSources+UITableViewのサンプル][scale=0.3]{
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

５章の開発を加速させる設定を済ませた前提で進めます。

まずは@<code>{SectionModel}というのを作成します@<br>{}
@<code>{SectionModel}は@<code>{SectionModelType}プロトコルに準拠する構造体で定義されており、これをうまく使うことでセクションとその中のセクションセルを表現できます。

//list[section-of-person][SettingsSectionModel][Swift]{
typealias SettingsSectionModel = SectionModel<SettingsSection, SettingsItem>

enum SettingsSection {
    case account
    case common

    var headerHeight: CGFloat {
        return 40.0
    }

    var footerHeight: CGFloat {
        return 1.0
    }
}

enum SettingsItem {
    // account section
    case account
    case security
    case notification
    case contents
    // common section
    case sounds
    case dataUsing
    case accessibility

    // other
    case description(text: String)

    var title: String? {
        switch self {
        case .account:
            return "アカウント"
        case .security:
            return "セキュリティ"
        case .notification:
            return "通知"
        case .contents:
            return "コンテンツ設定"
        case .sounds:
            return "サウンド設定"
        case .dataUsing:
            return "データ利用時の設定"
        case .accessibility:
            return "アクセシビリティ"
        case .description:
            return nil
        }
    }

    var rowHeight: CGFloat {
        switch self {
        case .description:
            return 72.0
        default:
            return 48.0
        }
    }

    var accessoryType: UITableViewCellAccessoryType {
        switch self {
        case .account, .security, .notification, .contents,
             .sounds, .dataUsing, .accessibility:
            return .disclosureIndicator
        case .description:
            return .none
        }
    }
}
//}

@<code>{enum}で定義した@<code>{SettingsSection}の各@<code>{case}が１つのセクションで、@<code>{SettingsItem}がセクション内のセルデータ群です。@<br>{}

次に、ViewModelを作っていきましょう。

//list[SettingsViewModel][SettingsViewModel.swift][swift]{
import RxSwift
import RxDataSources

class SettingsViewModel {

    let items = BehaviorSubject<[SettingsSectionModel]>(value: [])

    func updateItem() {
        let sections: [SettingsSectionModel] = [
            accountSection(),
            commonSection()
        ]
        items.onNext(sections)
    }

    private func accountSection() -> SettingsSectionModel {
        let items: [SettingsItem] = [
            .account,
            .security,
            .notification,
            .contents
        ]
        return SettingsSectionModel(model: .account, items: items)
    }

    private func commonSection() -> SettingsSectionModel {
        let items: [SettingsItem] = [
            .sounds,
            .dataUsing,
            .accessibility,
            .description(text: "基本設定はこの端末でログインしている全てのアカウントに適用されます。")
        ]
        return SettingsSectionModel(model: .common, items: items)
    }
}
//}

最後にViewControllerを作ります。

//list[SettingsViewController][SettingsViewController.swift][swift]{
import UIKit
import RxSwift
import RxDataSources

class SettingsViewController: UIViewController {

  @IBOutlet weak var tableView: UITableView!

  private var disposeBag = DisposeBag()

  private lazy var dataSource =
    RxTableViewSectionedReloadDataSource<SettingsSectionModel>(configureCell: configureCell)

  private lazy var configureCell:
    RxTableViewSectionedReloadDataSource<SettingsSectionModel>.ConfigureCell =
      { [weak self] (dataSource, tableView, indexPath, _) in
        let item = dataSource[indexPath]
        switch item {
        case .account, .security, .notification, .contents,
             .sounds, .dataUsing, .accessibility:
            let cell = tableView.dequeueReusableCell
              (withIdentifier: "cell", for: indexPath)
            cell.textLabel?.text = item.title
            cell.accessoryType = item.accessoryType
            return cell
        case .description(let text):
            let cell = tableView.dequeueReusableCell
              (withIdentifier: "cell", for: indexPath)
            cell.textLabel?.text = text
            cell.isUserInteractionEnabled = false
            return cell
      }
  }

  private var viewModel: SettingsViewModel!

  override func viewDidLoad() {
      super.viewDidLoad()
      setupViewController()
      setupTableView()
      setupViewModel()
  }

  private func setupViewController() {
      navigationItem.title = "設定"
  }

  private func setupTableView() {
      tableView.register
        (UITableViewCell.self, forCellReuseIdentifier: "cell")
      tableView.contentInset.bottom = 12.0
      tableView.rx.setDelegate(self).disposed(by: disposeBag)
      tableView.rx.itemSelected
          .subscribe(onNext: { [weak self] indexPath in
              guard let item = self?.dataSource[indexPath] else { return }
              self?.tableView.deselectRow(at: indexPath, animated: true)
              switch item {
              case .account:
                  // 遷移させる処理
                  // コンパイルエラー回避のためにbreakをかいていますが処理を書いていればbreakは必要ありません。
                  break
              case .security:
                  // 遷移させる処理
                  break
              case .notification:
                  // 遷移させる処理
                  break
              case .contents:
                  // 遷移させる処理
                  break
              case .sounds:
                  // 遷移させる処理
                  break
              case .dataUsing:
                  // 遷移させる処理
                  break
              case .accessibility:
                  // 遷移させる処理
                  break
              case .description:
                  break
              }
          })
          .disposed(by: disposeBag)
  }

  private func setupViewModel() {
      viewModel = SettingsViewModel()

      viewModel.items
          .bind(to: tableView.rx.items(dataSource: dataSource))
          .disposed(by: disposeBag)

      viewModel.updateItem()
  }
}

extension SettingsViewController: UITableViewDelegate {
  func tableView(_ tableView: UITableView,
      heightForRowAt indexPath: IndexPath) -> CGFloat {
      let item = dataSource[indexPath]
      return item.rowHeight
  }
  func tableView(_ tableView: UITableView,
      heightForHeaderInSection section: Int) -> CGFloat {
      let section = dataSource[section]
      return section.model.headerHeight
  }

  func tableView(_ tableView: UITableView,
      heightForFooterInSection section: Int) -> CGFloat {
      let section = dataSource[section]
      return section.model.footerHeight
  }

  func tableView(_ tableView: UITableView,
      viewForHeaderInSection section: Int) -> UIView? {
      let headerView = UIView()
      headerView.backgroundColor = .clear
      return headerView
  }

  func tableView(_ tableView: UITableView,
      viewForFooterInSection section: Int) -> UIView? {
      let footerView = UIView()
      footerView.backgroundColor = .clear
      return footerView
  }
}
//}

Build & Run で実行確認できます。

=== その他セクションを追加してみよう！

//lead{
さきほど作ったRxDataSources+UITableViewのサンプルアプリを題材に、新しくセクションとセクションアイテムを追加する方法について学びます。
//}

まずはセクションを追加するために、@<code>{SettingsSection}に@<code>{case}を追加します。

//emlist[]{
enum SettingsSection {
  case account
  case common
  case other // 追加
  // ...
}
//}

次に、セクションアイテムを追加するため、@<code>{SettingsItem}に@<code>{case}を追加します。

//list[add-section-item][SettingsItem][swift]{
enum SettingsItem {
  // ...
  // common section
  case sounds
  case dataUsing
  case accessibility
  // other section
  case credits
  case version
  case privacyPolicy
  // ...

  var title: String? {
        switch self {
        // ..
        case .credits:
            return "クレジット"
        case .version:
            return "バージョン情報"
        case privacyPolicy:
            reutrn "プライバシーポリシー"
        }
    }


    var accessoryType: UITableViewCellAccessoryType {
        switch self {
        case .account, .security, .notification, .contents, .sounds,
             .dataUsing, .accessibility, .credits, .version, .privacyPolicy:
            return .disclosureIndicator
        case .description:
            return .none
        }
    }
}
//}

セクションとそのアイテムの定義ができたら、実際に表示させるためにViewModelのitemsへデータを追加します。

//list[add-viewmodel][ViewModelを編集][Swift]{
    func updateItem() {
        let sections: [SettingsSectionModel] = [
            accountSection(),
            commonSection(),
            otherSection() // 追加
        ]
        items.onNext(sections)
    }

    // ...

    private func otherSection() -> SettingsSectionModel {
        let items: [SettingsItem] = [
            .credits,
            .version,
            .privacyPolicy
        ]
        return SettingsSectionModel(model: .other, items: items)
    }
//}

データの追加ができたので、今度はそのセクションセルのUIを定義します。
今回は他のメニューと同じUIでよいので、switch文を軽く対応させるだけです。

//list[add-viewcontroller][ViewControllerを編集][Swift]{
// ...
private lazy var configureCell:
  RxTableViewSectionedReloadDataSource<SettingsSectionModel>.ConfigureCell =
    { [weak self] (dataSource, tableView, indexPath, _) in
      let item = dataSource[indexPath]
      switch item {
      case .account, .security, .notification, .contents,
           .sounds, .dataUsing, .accessibility, .credits,
           .version, .privacyPolicy:
            let cell = tableView.dequeueReusableCell
              (withIdentifier: "cell", for: indexPath)
            cell.textLabel?.text = item.title
            cell.accessoryType = item.accessoryType
            return cell
    // ...
}

// ...

private func setupTableView() {
    // ...
    tableView.rx.itemSelected
        .subscribe(onNext: { [weak self] indexPath in
            guard let item = self?.dataSource[indexPath] else { return }
            self?.tableView.deselectRow(at: indexPath, animated: true)
            switch item {
            case .account:
                // 遷移させる処理
                // コンパイルエラー回避のためにbreakをかいていますが処理を書いていればbreakは必要ありません。
                break
            // ...
            case .credits:
                // 遷移させる処理
                break
            case .version:
                // 遷移させる処理
                break
            case .privacyPolicy:
                // 遷移させる処理
                break
            case .description:
                break
            }
        })
        .disposed(by: disposeBag)
  // ...
//}

Build & Runで次のような画面になっていたら成功です。

//image[rxdatasource-add-other-section][その他セクション追加後の画面][scale=0.3]{
  その他セクション追加後の画面
//}

== RxAlamofire ★1k

== RxKeyboard ★800

== RxOptional ★441

== RxFirebase ★42
