

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
