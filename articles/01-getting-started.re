= はじめに

この本を手にとって頂き、ありがとうございます。本書では、「比較して学ぶ」をテーマにしてCallBack、Delegate、KVOパターン等と比較しながら、RxSwiftの基礎知識、アプリのパーツとしてどう書くかについて解説していきます。

RxSwiftは2016年頃に一気に普及し、2018年現在ではいわゆる「イケてる」アプリのほとんどはRxSwift（もしくはReactiveSwift）を採用しています。
ほとんどがRxSwiftを採用しているということは、もちろんRxSwiftについて知っていなければ機能を開発していくことができません。しかし、その概念を習得するのはなかなかコストが高く、iOSアプリエンジニアになってから日の浅い人にとっては、とっっっても高い壁になっているのではないかと感じます。
正直にいうと、筆者もRxSwiftを触り始めてからまだ日が浅く、習得する時はとっっっても苦労しました。
RxSwiftについて学ぶ時、Google検索で調べて技術ブログやQiitaの記事を漁ったり、公式ドキュメントを見たりしていました。学んでいく上で、技術ブログの記事やQiitaの記事はとても勉強になりましたし、今でも時折参照します。
当時から「RxSwiftを日本語で解説してて体系的に学べる本、無いかなーあったら楽だなー」とずっと思っていて、かなり試行錯誤しながらコードを書いていました。
本書は、そんな過去の自分が最強に欲しかった本になるように書きました。
この本を読んで、RxSwiftの概念がようやくわかった！RxSwiftの理解がもっと深まった！となってくれたら嬉しいです。

== 対象読者

本書は次の読者を対象として作成しています。

  * プログラミング歴1年以上（種類問わず）
  * Swift による iOS アプリの開発経験が少しだけある（3ヶ月〜1年未満）
  * RxSwiftライブラリを使った開発をしたことが全くない・ほんの少しだけある

== 必須知識

  * Swiftの基本的な言語仕様
  ** if, for, switch, enum, class, struct
  ** mapやfilterなどの高階関数の扱い
  * Xcode の基本的な操作
  * よく使われる UIKitの仕様
  ** UILabel UITextView UITableView UICollectionView

== 推奨知識

  * 設計パターン
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

本書は有志によって作成されているもので、米Apple社とは一切関係がありません。

//emlist[]{
RxSwift License

The MIT License Copyright © 2015 Krunoslav Zaher All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//}
