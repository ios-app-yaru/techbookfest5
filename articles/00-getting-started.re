= はじめに

この本を手にとって頂き、ありがとうございます。本書では、「比較して学ぶ」をテーマに、CallBack、Delegate、KVOパターンなどさまざまな書き方と比較しながら、RxSwiftについて解説していきます。@<br>{}
解説はできるだけRxSwiftをまったく触ったことのない人向けにRxSwiftの思想・歴史から基礎知識、よく使われる文法から、実際にアプリのパーツとしてどう書くかまでできるだけわかりやすく書きました。

RxSwiftは2016年頃に一気に普及し、2018年現在ではいわゆる「イケてる」アプリのほとんどはRxSwift（もしくはReactiveSwift）を採用していると思われます。@<br>{}
ほとんどがRxSwiftを採用しているということは、もちろんRxSwiftについて知っていなければ機能を開発していくことができません。@<br>{}
しかし、その概念を習得するのはなかなかコストが高く、iOSアプリエンジニアになってから日の浅い人にとっては、とても高い壁になっているのではないかと感じます。

筆者もRxSwiftを触り始めてからまだ日が浅く、習得する時はとても苦労しました。@<br>{}
RxSwiftについて学ぶ時、ドキュメントのチュートリアルやGoogle検索で調べて出てきた技術ブログの記事やQiitaの記事など、いろいろな情報元を見ながら実装していました。@<br>{}
学んでいく上で、技術ブログの記事やQiitaの記事にはかなりお世話になっていて、今でも時折参照します。

しかし、当時から「RxSwiftを日本語で解説してて体系的に学べる本、無いかなーあったら楽だなー」とずっと思っていて、かなり試行錯誤しながらコードを書いていました。@<br>{}
本書はそんな過去の自分と、これからRxSwiftについて学びたい方に向けて体系的に学べるコンテンツを提供したいという思いから生まれました。

この本を読んで、RxSwiftの概念がようやくわかった！RxSwiftの理解がもっと深まった！となってくれたら嬉しいです。

== 対象読者

本書は次の読者を対象として作成しています。

  * Swift による iOS アプリの開発経験が少しだけある（3ヶ月〜1年未満）
  * RxSwiftライブラリを使った開発をしたことがまったくない・ほんの少しだけある

== 必須知識

  * Swiftの基本的な言語仕様
  ** if, for, switch, enum, class, struct
  ** mapやfilterなどの高階関数の扱い
  * Xcode の基本的な操作
  * よく使われる UIKitの大まかな仕様
  ** UILabel, UITextView, UITableView, UICollectionView

== 推奨知識

  * 設計パターン
  ** MVPアーキテクチャ
  ** MVVMアーキテクチャ
  * デザインパターン
  ** delegateパターン
  ** KVOパターン
  ** Observerパターン

== 想定環境

  * OSX High Sierra
  * Xcode 9.4
  * Swift 4.1
  * cocoapods 1.5.3

== お問い合わせ先

  * Twitter
  ** @k0uhashi

== 免責事項

本書は有志によって作成されているもので、米Apple社とは一切関係がありません。@<br>{}
また、掲載されている内容は情報の提供のみを目的にしていて、本書を用いた開発・運用は必ず読者の責任と判断によって行ってください。@<br>{}
著者は本書の内容による開発、運用の結果によっての結果について、いかなる責任も負いません。
