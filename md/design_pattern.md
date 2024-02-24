# デザインパターン
ソースコードを劣化させるアンチパターンには、

* 大きすぎる関数、クラス、ファイル等のソフトウェア構成物
* 複雑怪奇な依存関係
* コードクローン

等があるだろう。
こういった問題は、ひどいソースコードを書かないという強い意志を持ったプログラマの不断の努力と、
そのプログラマを支えるソフトウェア工学に基づいた知識によって回避可能である。
本章ではその知識の一翼をになうデザインパターン、イデオム等を解説、例示する。

なお、ここに挙げるデザインパターン、イデオム等は「適切な場所に適用される場合、
ソースコードをよりシンプルに記述できる」というメリットがある一方で、
「不適切な場所に適用される場合、ソースコードの複雑度を不要に上げてしまう」
という負の一面を持つ。

また、デザインパターン、イデオム等を覚えたてのプログラマは、
自分のスキルが上がったという一種の高揚感や顕示欲を持つため、
それをむやみやたらに多用してしまう状態に陥ることある。このようなプログラマの状態を 

* パターン病に罹患した
* パターン猿になった、もしくは単に、猿になった  

と呼ぶ。
猿になり不要に複雑なソースコードを書かないために、デザインパターン、イデオム等を使用する場合、
本当にそれが必要か吟味し、不要な場所への適用を避けなければならない。

## ガード節
ガード節とは、
「可能な場合、処理を早期に打ち切るために関数やループの先頭に配置される短い条件文(通常はif文)」
であり、以下のような利点がある。

* 処理の打ち切り条件が明確になる。
* 関数やループのネストが少なくなる。

まずは、ガード節を使っていない例を上げる。

```cpp
    // @@@ example/design_pattern/guard_ut.cpp #0:0 begin
```

上記の例を読んで一目で何が行われているか、理解できる人は稀である。
一方で、上記と同じロジックである下記関数を一目で理解できない人も稀である。

```cpp
    // @@@ example/design_pattern/guard_ut.cpp #1:0 begin
```

ここまで効果的な例はあまりない。

もう一例、(ガード節導入の効果が前例ほど明確でない)ガード節を使っていないコードを示す。

```cpp
    // @@@ example/design_pattern/guard_ut.cpp #0:1 begin
```

上記にガード節を適用した例を下記する。

```cpp
    // @@@ example/design_pattern/guard_ut.cpp #1:1 begin
```

ガード節を使っていない例に比べて、

* ネストが減って読みやすくなった
* max_numが1, 2, 65535, 65536である場合がロジックの境界値であることが一目でわかるようになった

といった改善はされたものの、最初の例ほどのレベル差はない。
しかし、ソースコードの改善やリファクタリングのほとんどは、このようなものであり、
この少しのレベルアップが数か月後、数年後に大きな差を生み出すことを忘れてはならない。

[演習-ガード節](~~~)  

## BitmaskType
下記のようなビットマスク表現は誤用しやすいインターフェースである。
修正や拡張等に関しても脆弱であるため、避けるべきである。

```cpp
    // @@@ example/design_pattern/enum_operator.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/enum_operator_ut.cpp #0:0 begin -1
```

上記のような誤用を防ぐために、
enumによるビットマスク表現を使用して型チェックを強化した例を以下に示す。
このテクニックは、STLのインターフェースとしても使用されている強力なイデオムである。

```cpp
    // @@@ example/design_pattern/enum_operator.h #1:0 begin
```

```cpp
    // @@@ example/design_pattern/enum_operator_ut.cpp #1:0 begin -1
```

この改善により、Animalのコンストラクタに域値外の値を渡すことは困難になった
(少なくとも不注意で間違うことはないだろう)。
この修正の延長で、Animal::GetPhisicalAbility()の戻り値もenumになり、これも誤用が難しくなった。

[演習-BitmaskType](~~~)  

## Pimpl
このパターンは、「クラスA(a.cpp、a.hで宣言、定義)を使用するクラスにAの実装の詳細を伝搬させたくない」
ような場合に使用する。
そのため[オープン・クローズドの原則(OCP)](---)の実装方法としても有用である。

一般的に、STLライブラリのパースは多くのCPUタイムを消費する。
クラスAがSTLクラスをメンバに使用し、a.hにそのSTLヘッダファイルがインクルードされた場合、
a.hをインクルードするファイルをコンパイルする度にそのSTLヘッダファイルはパースされる。
これはさらに多くのCPUタイムの消費につながり、ソースコード全体のビルドは遅くなる。
こういった問題をあらかじめ避けるためにも有効な手段ではあるが、
そのトレードオフとして実行速度は若干遅くなる。

下記は、Pimplイデオム未使用の、std::stringに依存したクラスStringHolderOldの例である。

```cpp
    // @@@ example/design_pattern/string_holder_old.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/string_holder_old.cpp #0:0 begin
```


下記は、上記クラスStringHolderOldにPimplイデオムを適用したクラスStringHolderNewの例である。

```cpp
    // @@@ example/design_pattern/string_holder_new.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/string_holder_new.cpp #0:0 begin
```

下記図は、上記ファイルやそれらを使用するファイルの依存関係である。
string_holder_old.hは、std::stringに依存しているが、string_holder_new.hは、
std::stringに依存していないこと、
それによってStringHolderNewを使用するファイルから、std::stringへの依存を排除できていることがわかる。

![ファイルの依存関係](plant_uml/pimpl_pattern.png)

このパターンを使用して問題のある依存関係をリファクタリングする例を示す。

まずは、リファクタリング前のコードを下記する。

```cpp
    // in lib/h/widget.h

    // @@@ example/design_pattern/widget_ng.h #0:0 begin 0 -1
    // @@@ example/design_pattern/widget_ng.h #0:1 begin 0 -1
    // @@@ example/design_pattern/widget_ng.h #0:2 begin 0 -1
```
```cpp
    // in lib/src/widget.cpp

    #include "widget.h"
    // @@@ example/design_pattern/widget_ng.cpp #0:0 begin 0 -1
```
```cpp
    // in lib/ut/widget_ut.cpp

    #include "widget.h"
    // @@@ example/design_pattern/widget_ng_ut.cpp #0:0 begin 0 -1
```

何らかの事情により、単体テストでprivateなメンバにアクセスする必要があったため、
単体テストクラスをテスト対象クラスのfriendすることで、それを実現している。

単体テストクラスをテスト対象クラスのfriendにするためには、
上記コードの抜粋である下記を記述する必要がある。

```cpp
    // @@@ example/design_pattern/widget_ng.h #0:1 begin 0 -1
```

このマクロは、gtest.h内で定義されているため、widget.hからgtest.hをインクルードしている。

このため、ファイルの依存関係は下記のようになる。

![ファイルの依存関係](plant_uml/widget_ng.png)

この依存関係は、Widgetのクライアントに不要な依存関係を強要してしまう問題のある構造を作り出す。

この問題をPimplによるリファクタリングで解決したコードを以下に示す
(コンパイラのインクルードパスにはlib/hのみが入っていることを前提とする)。

```cpp
    // in lib/h/widget.h

    // @@@ example/design_pattern/widget_ok.h #0:0 begin 0 -1
```
```cpp
    // in lib/src/widget.cpp

    #include "widget_internal.h"
    // @@@ example/design_pattern/widget_ok.cpp #0:0 begin 0 -1
```
```cpp
    // in lib/src/widget_internal.h

    #include "widget.h"
    // @@@ example/design_pattern/widget_ok_internal.h #0:0 begin 0 -1
```
```cpp
    // in lib/ut/widget_ut.cpp

    #include "../src/widget_internal.h"  // 単体テストのみに、このようなインクルードを認める
    // @@@ example/design_pattern/widget_ok_ut.cpp #0:0 begin 0 -1
```

このリファクタリングにより、ファイルの依存は下記のようになり、
問題のある構造は解消された。

![ファイルの依存関係](plant_uml/widget_ok.png)

[演習-Pimpl](~~~)  

## Accessor
publicメンバ変数とそれにアクセスするソースコードは典型的なアンチパターンであるため、
このようなコードを禁じるのが一般的なプラクティスである。

```cpp
    // @@@ example/design_pattern/accessor_ut.cpp #0:0 begin
```

とはいえ、ソフトウェアのプラクティスには必ずといってほど例外があり、
製品開発の現場において、オブジェクトのメンバ変数にアクセスせざるを得ないような場面は、
稀にではあるが発生する。
このような場合に適用するがのこのイデオムである。

```cpp
    // @@@ example/design_pattern/accessor_ut.cpp #1:0 begin
```

メンバ変数への直接のアクセスに比べ、以下のようなメリットがある。

* アクセスのログを入れることができる。
* メンバ変数へのアクセスをデバッガで捕捉しやすくなる。
* setterに都合の悪い値が渡された場合、何らかの手段を取ることができる(assertや、エラー処理)。
* リファクタリングや機能修正により対象のメンバ変数がなくなった場合においても、
  クラスのインターフェースの変更を回避できる(修正箇所を局所化できる)。

一方で、クラスに対するこのような細かい制御は、カプセル化に対して問題を起こしやすい。
下記はその典型的なアンチパターンである。

```cpp
    // @@@ example/design_pattern/accessor_ut.cpp #2:0 begin
```

上記ソースコードは、オブジェクトaのA::a\_が変更された場合、
その後、それをもとに何らかの動作を行うこと(a.DoSomething)を表しているが、
本来オブジェクトaの状態が変わったかどうかはオブジェクトa自体が判断すべきであり、
a.DoSomething()の実行においても、それが必要かどうかはオブジェクトaが判断すべきである。
この考えに基づいた修正ソースコードを下記に示す。

```cpp
    // @@@ example/design_pattern/accessor_ut.cpp #3:0 begin
```

setterを使用する場合、上記のように処理の隠蔽化には特に気を付ける必要がある。

[演習-Accessorの副作用](~~~)  
[演習-Accessor](~~~)  

## Copy-And-Swap
メンバ変数にポインタやスマートポインタを持つクラスに

* copyコンストラクタ
* copy代入演算子
* moveコンストラクタ
* move代入演算子

が必要になった場合、コンパイラが生成するデフォルトの
[オブジェクトと生成|特殊メンバ関数](---)では機能が不十分であることが多い。

下記に示すコードは、そのような場合の上記4関数の実装例である。

```cpp
    // @@@ example/design_pattern/no_copy_and_swap_ut.cpp #0:0 begin
```

コード内のコメントで示したように、このコードには以下のような問題がある。

* copy代入演算子には、[エクセプション安全性の保証](---)がない。
* 上記4関数は似ているにも関わらず、微妙な違いがあるためコードクローンとなっている。

ここで紹介するCopy-And-Swapはこのような問題を解決するためのイデオムである。

実装例を以下に示す。

```cpp
    // @@@ example/design_pattern/copy_and_swap_ut.cpp #0:0 begin
```

上記CopyAndSwapのcopyコンストラクタ、moveコンストラクタに変更はない。
また、CopyAndSwap::Swapに関してもstd::vector等が持つswapと同様のものである。
このイデオムの特徴は、copy代入演算子、
move代入演算子が各コンストラクタとSwap関数により実装されている所にある。
これにより[エクセプション安全性の保証](---)を持つ4関数をコードクローンすることなく実装できる。

[演習-Copy-And-Swap](~~~)  

## Immutable
クラスに対するimmutable、immutabilityの定義を以下のように定める。

* immutable(不変な)なクラスとは、初期化後、状態の変更ができないクラスを指す。
* immutability(不変性)が高いクラスとは、
  状態を変更するメンバ関数(非constなメンバ関数)が少ないクラスを指す。

immutabilityが高いほど、そのクラスの使用方法は制限される。
これにより、そのクラスやそのクラスを使用しているソースコードの可読性やデバッグ容易性が向上する。
また、クラスがimmutableでなくても、そのクラスのオブジェクトをconstハンドル経由でアクセスすることで、
immutableとして扱うことができる。

一方で、「[Accessor](---)」で紹介したsetterは、クラスのimmutabilityを下げる。
いつでも状態が変更できるため、ソースコードの可読性やデバッグ容易性が低下する。
また、マルチスレッド環境においてはこのことが競合問題や、
それを回避するためのロックがパフォーマンス問題やデッドロックを引き起こしてしまう。

従って、クラスを宣言、定義する場合、immutabilityを出来るだけ高くするべきであり、
そのクラスのオブジェクトを使う側は、
可能な限りimmutableオブジェクト(constオブジェクト)として扱うべきである。

[演習-Immutable](~~~)  

## Clone(仮想コンストラクタ)
オブジェクトコピーによる[オブジェクトのコピー|スライシング](---)を回避するためのイデオムである。

下記は、オブジェクトコピーによるスライシングを起こしてしまう例である。

```cpp
    // @@@ example/design_pattern/clone_ut.cpp #0:0 begin
```

下記は、上記にcloneイデオムを適用した例である。

```cpp
    // @@@ example/design_pattern/clone_ut.cpp #0:1 begin
```

B1::Clone()やそのオーバーライドであるD1::Clone()を使うことで、
スライシングを起こすことなくオブジェクトのコピーを行うことができるようになった。

[演習-Clone](~~~)  

## NVI(non virtual interface)
NVIとは、「virtualなメンバ関数をpublicにしない」という実装上の制約である。

下記のようにクラスBaseが定義されているとする。

```cpp
    // @@@ example/design_pattern/nvi_ut.cpp #0:0 begin
```

これを使うクラスはBase::DoSomething()に依存する。
また、このクラスから派生した下記のクラスDerivedもBase::DoSomething()に依存する。

```cpp
    // @@@ example/design_pattern/nvi_ut.cpp #0:1 begin
```

この条件下ではBase::DoSomething()へ依存が集中し、この関数の修正や機能追加の作業コストが高くなる。
このイデオムは、この問題を軽減する。

これを用いた上記2クラスのリファクタリング例を以下に示す。

```cpp
    // @@@ example/design_pattern/nvi_ut.cpp #1:0 begin
```

オーバーライド元の関数とそのオーバーライドのデフォルト引数の値は一致させる必要がある。

それに従わない下記のようなクラスとその派生クラス

```cpp
    // @@@ example/design_pattern/nvi_ut.cpp #2:0 begin
```

には下記の単体テストで示したような、
メンバ関数の振る舞いがその表層型に依存してしまう問題を持つことになる。

```cpp
    // @@@ example/design_pattern/nvi_ut.cpp #2:1 begin -1
```

この例のように継承階層が浅く、デフォルト引数の数も少ない場合、
この値を一致させることは難しくないが、
これよりも遥かに複雑な実際のコードではこの一致の維持は困難になる。

下記のようにNVIに従わせることでこのような問題に対処できる。

```cpp
    // @@@ example/design_pattern/nvi_ut.cpp #3:0 begin
```

下記の単体テストにより、この問題の解消が確認できる。

```cpp
    // @@@ example/design_pattern/nvi_ut.cpp #3:1 begin -1
```

なお、メンバ関数のデフォルト引数は、
そのクラス外部からのメンバ関数呼び出しを簡潔に記述するための記法であるため、
privateなメンバ関数はデフォルト引数を持つべきではない。

[演習-NVI](~~~)  

## RAII(scoped guard)
RAIIとは、「Resource Acquisition Is Initialization」の略語であり、
リソースの確保と解放をオブジェクトの初期化と破棄処理に結びつけるパターンもしくはイデオムである。
特にダイナミックにオブジェクトを生成する場合、
RAIIに従わないとメモリリークを防ぐことは困難である。

下記は、関数終了付近でdeleteする素朴なコードである。

```cpp
    // @@@ example/design_pattern/raii_ut.cpp #0:0 begin
```

このコードは下記の単体テストが示す通り、第1パラメータが5以上の場合、
エクセプションが発生しメモリリークしてしまう。

```cpp
    // @@@ example/design_pattern/raii_ut.cpp #0:1 begin -1
```

以下は、std::unique_ptrによってRAIIを導入し、この問題に対処した例である。

```cpp
    // @@@ example/design_pattern/raii_ut.cpp #1:0 begin
```

下記単体テストで確認できるように、
エクセプション発生時にもstd::unique_ptrによる自動解放によりメモリリークは発生しない。

```cpp
    // @@@ example/design_pattern/raii_ut.cpp #1:1 begin -1
```

RAIIのテクニックはメモリ管理のみでなく、ファイルディスクリプタ(open-close、socket-close)
等のリソース管理においても有効であるという例を示す。

下記は、生成したソケットを関数終了付近でcloseする素朴なコードである。

```cpp
    // @@@ example/design_pattern/raii_ut.cpp #2:0 begin
```

エクセプションを扱うために関数の2か所でソケットをcloseしている。
この程度であれば大きな問題にはならないだろうが、実際には様々な条件が重なるため、
リソースの解放コードは醜悪にならざるを得ない。

このような場合には、下記するようなリソース解放用クラス

```cpp
    // @@@ h/scoped_guard.h #0:0 begin
    // @@@ h/scoped_guard.h #0:1 begin
    // @@@ h/scoped_guard.h #0:2 begin
```

を使用し、下記のようにすることで安全なコードをすっきりと書くことができる。

```cpp
    // @@@ example/design_pattern/raii_ut.cpp #2:1 begin
```

クリティカルセクションの保護をlock/unlockで行うstd::mutex等を使う場合にも、
std::lock_guard<>によってunlockを行うことで、同様の効果が得られる。

[演習-RAIIの効果](~~~)  
[演習-RAII](~~~)  

## Future
[Future](https://ja.wikipedia.org/wiki/Future_%E3%83%91%E3%82%BF%E3%83%BC%E3%83%B3)とは、
並行処理のためのデザインパターンであり、別スレッドに何らかの処理をさせる際、
その結果の取得を、必要になるまで後回しにする手法である。

C++11では、std::future, std::promise, std::asyncによって実現できる。

まずは、C++03以前のスタイルから示す。

```cpp
    // @@@ example/design_pattern/future_ut.cpp #0:0 begin
```

上記は、

1. 時間がかかる処理を並行して行うために、スレッドを二つ作る。
2. それぞれの完了をthread::join()で待ち合わせる。
3. その結果を参照キャプチャによって受け取る。
4. その2つの結果を別の関数に渡す。

という処理を行っている。

この程度の単純なコードでは特に問題にはならないが、目的外の処理が多いことがわかるだろう。

次にFutureパターンによって上記をリファクタリングした例を示す。

```cpp
    // @@@ example/design_pattern/future_ut.cpp #0:1 begin
```

リファクタリングした例では、時間のかかる処理をstd::future型のオブジェクトにし、
その結果を必要とする関数に渡すことができるため、目的をよりダイレクトに表すことができる。

なお、

```cpp
    std::async(関数オブジェクト)
```

という形式を使った場合、関数オブジェクトは、

```cpp
    std::launch::async | std::launch::deferred
```

が指定されたとして実行される。この場合、

```cpp
    std::launch::deferred
```

の効果により、関数オブジェクトは、並行に実行されるとは限らない
(この仕様はランタイム系に依存しており、std::future::get()のコンテキストで実行されることもあり得る)。
従って、並行実行が必要な場合、上記例のように

```cpp
    std::launch::async
```

のみを明示的に指定するべきである。

[演習-Future](~~~)  

## DI(dependency injection)
メンバ関数内でクラスDependedのオブジェクトを直接、生成する
(もしくは[Singleton](---)オブジェクトや静的オブジェクト(std::coutやstd::cin等)に直接アクセスする)
クラスNotDIがあるとする。
この場合、クラスNotDIはクラスDependedのインスタンスに依存してしまう。
このような依存関係はクラスNotDIの可用性とテスト容易性を下げる。
これは、「仮にクラスDependedがデータベースをラップするクラスだった場合、
クラスNotDIの単体テストにデータベースが必要になる」ことからも容易に理解できる。

```cpp
    // @@@ example/design_pattern/di_ut.cpp #0:0 begin
```

下記は上記NotDIにDIパターンを適用した例である。
この場合、クラスDIは、クラスDependedの型にのみ依存する。

```cpp
    // @@@ example/design_pattern/di_ut.cpp #0:1 begin
```

下記は、クラスNotDIとクラスDIがそれぞれのDoSomething()を呼び出すまでのシーケンス図である。

![シーケンス図](plant_uml/di.png)

このパターンの効果により、
DIオブジェクトにはDependedかその派生クラスのオブジェクトを渡すことができるようになった。
これによりクラスDIは拡張性に対して柔軟になっただけでなく、テスト容易性も向上した。

次に示すのは、このパターンを使用して問題のある単体テストを修正した例である。

まずは、問題があるクラスとその単体テストを下記する。

```cpp
    // in device_io.h
    // @@@ example/design_pattern/di_ut.cpp #1:0 begin 0 -1
```
```cpp
    // in widget.h
    
    #include "device_io.h"
    // @@@ example/design_pattern/di_ut.cpp #1:1 begin 0 -1
```
```cpp
    // in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #1:2 begin -1 -1
```

当然であるが、この単体テストは、UNIT_TESTマクロを定義している場合のWidgetの評価であり、
UNIT_TESTを定義しない実際のコードの評価にはならない。

以下では、DIを用い、この問題を回避する。

```cpp
    // in device_io.h
    // @@@ example/design_pattern/di_ut.cpp #2:0 begin 0 -1
```
```cpp
    // in widget.h
    // @@@ example/design_pattern/di_ut.cpp #2:1 begin 0 -1
```
```cpp
    // in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #2:2 begin 0 -1
```
```cpp
    // 上記DeviceIO_Mockと同様に、in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #2:3 begin -1 -1
```

この例では、単体テストのためだけに仮想関数を導入しているため、多少やりすぎの感がある。
そのような場合、下記のようにテンプレートを用いればよい。

```cpp
    // in device_io.h
    // @@@ example/design_pattern/di_ut.cpp #3:0 begin 0 -1
```
```cpp
    // in widget.h
    // @@@ example/design_pattern/di_ut.cpp #3:1 begin 0 -1
```
```cpp
    // in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #3:2 begin 0 -1
```
```cpp
    // 上記DeviceIO_Mockと同様に、in widget_ut.cpp
    // @@@ example/design_pattern/di_ut.cpp #3:3 begin -1 -1
```

以上からわかるように、
ここで紹介したDIは単体テストを容易にするクラス設計のためにも非常に有用なパターンである。

[演習-DI](~~~)  

## Singleton
このパターンにより、特定のクラスのインスタンスをシステム全体で唯一にすることができる。
これにより、グローバルオブジェクトを規律正しく使用しやすくなる。

以下は、Singletonの実装例である。

```cpp
    // @@@ example/design_pattern/singleton_ut.cpp #0:0 begin
```

このパターンを使用する場合、以下に注意する。  

* Singletonはデザインパターンの中でも、特にパターン猿病を発生しやすい。
  Singletonは「ほとんどグローバル変数である」ことを理解した上で、控えめに使用する。
* Singletonを定義する場合、以下の二つを定義する。
    * インスタンスを返すstaticメンバ関数Inst()
    * constインスタンスを返すstaticメンバ関数InstConst()
* InstConst()は、Inst()が返すオブジェクトと同じオブジェクトを返すようにする。
* Singletonには、可能な限りInstConst()経由でアクセスする。

Singletonオブジェクトの初期化(最初のコンストラクタ呼び出し)は、
C++03以前はスレッドセーフでなかったため、「 Double Checked Lockingを使って競合を避ける」か、
「他のスレッドを起動する前にメインスレッドから各SingletonのInstConst()を呼び出す」
ことが必要であった。
C++11から上記例のようなSingletonオブジェクトのコンストラクタ呼び出しはスレッドセーフとなったため、
このような黒魔術が不要になった。

なお、Inst()のような関数を複数定義する場合、そのパターンはNamed Constructor
(「[Named Constructor](---)」参照)と呼ばれる。

[演習-Singleton](~~~)  

## State
Stateは、オブジェクトの状態と、それに伴う振る舞いを分離して記述するためのパターンである。
これにより状態の追加、削減、変更に伴う修正範囲が限定される
(「[オープン・クローズドの原則(OCP)](---)」参照)。
またオブジェクトのインターフェース変更(パブリックメンバ関数の変更)に関しても、修正箇所が明確になる。

![ステートマシン図](plant_uml/state_machine.png)

上記ステートマシン図の「オールドスタイルによる実装」と、「stateパターンによる実装」、
それぞれを例示する。

まずは、下記にオールドスタイルな実装例を示す。
この実装では、状態を静的なenum変数thread_old_style_stateで管理するため、
ThreadOldStyleStateStr()、ThreadOldStyleRun()、ThreadOldStyleAbort()、ThreadOldStyleSuspend()
には、thread_old_style_stateに対する同型のswitch文が入ることになる(下記例では一部省略)。
これは醜悪で、バグを起こしやすい構造である。
ただし、要求される状態遷移がこの例程度であり、状態ごとに決められた振る舞いの数が少なければ、
この構造でも問題ないともいえる。

```cpp
    // @@@ example/design_pattern/state_machine_old.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_old.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_ut.cpp #0:0 begin
```

下記は、上記例へstateパターンを適用した例である。
まずは、stateパターンを形成するクラスの関係をクラス図で示す。

![クラス図](plant_uml/state_machine_class.png)

次に上記クラス図の実装例を示す。

```cpp
    // @@@ example/design_pattern/state_machine_new.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_new.h #0:1 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_new.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_ut.cpp #1:0 begin
```

オールドスタイルな構造に比べると一見複雑に見えるが同型のswitch構造がないため、
状態の増減や振る舞いの変更等への対応が容易である。
一方で、前述したとおり、この例程度の要求であれば、
シンプルさという意味においてオールドスタイルのソースコードの方が優れているともいえる。
従って、オールドスタイルとstateパターンの選択は、
その要求の複雑さと安定度によって決定されるべきものである。

なお、C++でのstateパターンの実装には、下記に示すようなメンバ関数を使う方法もある。
多くのクラスを作る必要はないが、
各状態での状態管理変数を別の状態のものと分けて管理することができないため、
複雑な状態管理が必要な場合には使えないが、単純な状態管理で十分な場合には便利なパターンである。

```cpp
    // @@@ example/design_pattern/state_machine_new.h #1:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_new.cpp #1:0 begin
```

```cpp
    // @@@ example/design_pattern/state_machine_ut.cpp #2:0 begin
```

[演習-State](~~~)  

## Null Object
オブジェクトへのポインタを受け取った関数が
「そのポインタがnullptrでない場合、そのポインタが指すオブジェクトに何かをさせる」
というような典型的な条件文を削減するためのパターンである。

```cpp
    // @@@ example/design_pattern/null_object_ut.cpp #0:0 begin
```

上記例にNull Objectパターンを適用した例を下記する。

```cpp
    // @@@ example/design_pattern/null_object_ut.cpp #0:1 begin
```

この単純な例では、逆にソースコードが複雑化したように見えるが、

```cpp
    if(a != nullptr)
```

を頻繁に使うような関数、
クラスではソースコードの単純化やnullptrチェック漏れの防止に非常に有効である。

[演習-Null Object](~~~)  

## Templateメソッド
Templateメソッドは、雛形の形式(書式等)を定めるメンバ関数(templateメソッド)と、
それを埋めるための振る舞いやデータを定めるメンバ関数を分離するときに用いるパターンである。

以下に実装例を示す。

```cpp
    // @@@ example/design_pattern/template_method.h #0:0 begin
    // @@@ example/design_pattern/template_method.h #0:1 begin
    // @@@ example/design_pattern/template_method.h #0:3 begin
    // @@@ example/design_pattern/template_method.h #0:4 begin
```

上記XxxDataFormatterIFでは、以下のようなメンバ関数を宣言、定義している。

|メンバ関数  |                      | 振る舞い                                                 |
|:-----------|:---------------------|:---------------------------------------------------------|
| header()   | private pure-virtual | ヘッダをstd::stringオブジェクトとして生成                |
| footer()   | private pure-virtual | フッタをstd::stringオブジェクトとして生成                |
| body()     | private pure-virtual | XxxDataからボディをstd::stringオブジェクトとして生成     |
| ToString() | public  normal       | header(),body(),footer()の出力を組み合わせた全体像を生成 |

この構造により、XxxDataFormatterIFは、

* 全体の書式を定義している。
* 各行の生成をXxxDataFormatterIFから派生した具象クラスに委譲している。

下記XxxDataFormatterXml、XxxDataFormatterCsv、XxxDataFormatterTableでは、
header()、body()、footer()をオーバーライドすることで、それぞれの機能を実現している。

```cpp
    // @@@ example/design_pattern/template_method.cpp #0:0 begin
```

以下の単体テストで、これらのクラスの振る舞いを示す。

```cpp
    // @@@ example/design_pattern/template_method_ut.cpp #0:0 begin
```

上記で示した実装例は、public継承による動的ポリモーフィズムを使用したため、
XxxDataFormatterXml、XxxDataFormatterCsv、XxxDataFormatterTableのインスタンスやそのポインタは、
XxxDataFormatterIFのリファレンスやポインタとして表現できる。
この性質は、[Factory](---)や[Named Constructor](---)の実装には不可欠であるが、
逆にこのようなポリモーフィズムが不要な場合、このよう柔軟性も不要である。

そういった場合、private継承を用いるか、
テンプレートを用いた静的ポリモーフィズムを用いることでこの柔軟性を排除できる。

下記のコードはそのような実装例である。

```cpp
    // @@@ example/design_pattern/template_method_ut.cpp #1:0 begin
```

上記の単体テストは下記のようになる。

```cpp
    // @@@ example/design_pattern/template_method_ut.cpp #1:1 begin
```

[演習-Templateメソッド](~~~)  

## Factory
Factoryは、専用関数(Factory関数)にオブジェクト生成をさせるためのパターンである。
オブジェクトを生成するクラスや関数をそのオブジェクトの生成方法に依存させたくない場合や、
オブジェクトの生成に統一されたルールを適用したい場合等に用いられる。
DI(「[DI(dependency injection)](---)」参照)と組み合わせて使われることが多い。

「[Templateメソッド](---)」の例にFactoryを適用したソースコードを下記する。

下記のXxxDataFormatterFactory関数により、

* XxxDataFormatterIFオブジェクトはstd::unique_ptrで保持されることを強制できる
* XxxDataFormatterIFから派生したクラスはtemplate_method.cppの無名名前空間で宣言できるため、
  これらのクラスは他のクラスから直接依存されることがない

といった効果がある。

```cpp
    // @@@ example/design_pattern/template_method.h #1:0 begin
```

```cpp
    // @@@ example/design_pattern/template_method.cpp #1:0 begin
```

以下に上記クラスの単体テストを示す。

```cpp
    // @@@ example/design_pattern/template_method_factory_ut.cpp #0:0 begin
```

一般にFactory関数はヒープを使用してオブジェクトを生成する場合が多いため、
それを例示する目的でXxxDataFormatterFactoryもヒープを使用している。

この例ではその必要はないため、ヒープを使用しないFactory関数の例を下記する。

```cpp
    // @@@ example/design_pattern/template_method.cpp #1:1 begin
```

次に示すのは、このパターンを使用して、プリプロセッサ命令を排除するリファクタリングの例である。

まずは、出荷仕分け向けのプリプロセッサ命令をロジックの内部に記述している問題のあるコードを示す。
このようなオールドスタイルなコードは様々な開発阻害要因になるため、避けるべきである。

```cpp
    // in shipping.h
    // @@@ example/design_pattern/factory_shipping.h #0:0 begin 0 -1
```
```cpp
    // in shipping_japan.h
    // @@@ example/design_pattern/factory_shipping_lib.cpp #0:0 begin 0 -1
```
```cpp
    // in xxx.cpp 仕分けに依存した処理
    // @@@ example/design_pattern/factory_shipping_lib.cpp #1:0 begin 0 -1
```

このコードは、
関数テンプレートの特殊化を利用したFactoryを以下のように定義することで改善することができる。

```cpp
    // in shipping.h
    // @@@ example/design_pattern/factory_shipping.h #1:0 begin 0 -1
    // @@@ example/design_pattern/factory_shipping.h #1:1 begin 0 -1
```
```cpp
    // in shipping_japan.cpp
    // ファクトリーの効果で、ShippingOp_Japanは外部への公開が不要
    // @@@ example/design_pattern/factory_shipping_lib.cpp #0:0 begin 0 -1
    // @@@ example/design_pattern/factory_shipping_lib.cpp #2:0 begin 0 -1
```
```cpp
    // in xxx.cpp 仕分けに依存した処理
    // @@@ example/design_pattern/factory_shipping_ut.cpp #1:1 begin -1 -1
```

もしくは、
関数オーバーロードを利用したFactoryを以下のように定義することで改善することもできる。

```cpp
    // in shipping.h
    // @@@ example/design_pattern/factory_shipping.h #1:0 begin 0 -1
    // @@@ example/design_pattern/factory_shipping.h #2:0 begin 0 -1
```
```cpp
    // in shipping_japan.cpp
    // ファクトリーの効果で、ShippingOp_Japanは外部への公開が不要
    // @@@ example/design_pattern/factory_shipping_lib.cpp #0:0 begin 0 -1
    // @@@ example/design_pattern/factory_shipping_lib.cpp #3:0 begin 0 -1
```
```cpp
    // in xxx.cpp 仕分けに依存した処理
    // @@@ example/design_pattern/factory_shipping_ut.cpp #1:2 begin -1 -1
```

[演習-Factory](~~~)  

## Named Constructor
Named Connstructorは、[Singleton](---)のようなオブジェクトを複数、生成するためのパターンである。

```cpp
    // @@@ example/design_pattern/enum_operator.h #2:0 begin
```

上記例のHuman()、Bat()、Whale()は、人、コウモリ、クジラに対応するクラスMammalsオブジェクトを返す。

次に示したのは「[Factory](---)」の例にこのパターンを適応したコードである。

```cpp
    // @@@ example/design_pattern/template_method.h #0:1 begin
    // @@@ example/design_pattern/template_method.h #0:2 begin
    // @@@ example/design_pattern/template_method.h #0:4 begin
```

```cpp
    // @@@ example/design_pattern/template_method.cpp #2:0 begin
```

これまでにXxxDataFormatterIFオブジェクトを取得するパターンを以下のように3つ示した。

1. Factory関数によってstd::unique_ptr\<XxxDataFormatterIF>オブジェクトを返す。
2. Factory関数によってstaticなXxxDataFormatterIFオブジェクトを返す。
3. Named ConstructorによってstaticなXxxDataFormatterIFオブジェクトを返す。

最も汎用的な方法はパターン1であるが、
上記例のようにオブジェクトが状態を持たない場合、これは過剰な方法であり、
パターン3が最適であるように思える。このような考察からわかるように、
(単にnewする場合も含めて)オブジェクトの取得にどのような方法を用いるかは、
クラスの性質に依存する。

[演習-Named Constructor](~~~)  

## Proxy
Proxyとは代理人という意味で、
本物のクラスに代わり代理クラス(Proxy)が処理を受け取る
(実際は、処理自体は本物クラスに委譲されることもある)パターンである。

以下の順番で例を示すことで、Proxyパターンの説明を行う。

1. 内部構造を外部公開しているサーバ クラス
2. そのサーバをラッピングして、使いやすくしたサーバ クラス(Facadeパターン)
3. サーバをラップしたクラスのProxyクラス

まずは、内部構造を外部公開しているの醜悪なサーバの実装例である。

```cpp
    // @@@ example/design_pattern/bare_server.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/bare_server.cpp #0:0 begin
```

下記は、上記BareServerを使用するクライアントの実装例である。通信がpipe()によって行われ、
その中身がPacket{}であること等、不要な依存関係をbare_client()に強いていることがわかる。
このような構造は、機能追加、保守作業を非効率、困難にするアンチパターンである。

```cpp
    // @@@ example/design_pattern/proxy_ut.cpp #0:0 begin
```

次に、このむき出しの構造をラッピングする例を示す(このようなラッピングをFacadeパターンと呼ぶ)。

```cpp
    // @@@ example/design_pattern/bare_server_wrapper.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/bare_server_wrapper.cpp #0:0 begin
```

下記は、上記BareServerWrapperのクライアントの実装例である。
BareServerWrapperがむき出しの通信をラップしたことで、bare_wrapper_client()は、
bare_client()に比べてシンプルになったことがわかる。

```cpp
    // @@@ example/design_pattern/proxy_ut.cpp #0:1 begin
```

次の例は、BareServerとBareServerWrapperを統合し、
さらに全体をシンプルにリファクタリングしたWrappedServerである。
Packet{}やpipe等の通信の詳細がwrapped_server.cppの無名名前空間に閉じ込められ、
クラスの隠蔽性が強化されたことで、より機能追加、保守が容易になった。

```cpp
    // @@@ example/design_pattern/wrapped_server.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/wrapped_server.cpp #0:0 begin
```

WrappedServerの使用例を下記する。当然ながらbare_wrapper_client()とほぼ同様になる。

```cpp
    // @@@ example/design_pattern/proxy_ut.cpp #0:2 begin
```

WrappedServerが提供する機能はスレッド間通信を含むため処理コストが高い。
その対策として、サーバから送られてきた文字列をキャッシュするクラス(Proxyパターン)の導入により、
そのコストを削減する例を下記する。

```cpp
    // @@@ example/design_pattern/wrapped_server_proxy.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/wrapped_server_proxy.cpp #0:0 begin
```

下記図のようにWrappedServerProxyはWrappedServerからのパブリック継承であるため、
WrappedServerのクライアントは、そのままWrappedServerProxyのクライアントとして利用できる。

![クラス図](plant_uml/proxy.png)

なお、正確には下記のようなクラス構造をProxyパターンと呼ぶことが多いが、
ここでは単純さを優先した。

![クラス図](plant_uml/proxy_general.png)

[演習-Proxy](~~~)  

## Strategy
関数f(args)の振る舞いが、

* 全体の制御
* 部分的な振る舞い(何らかの条件を探す等)

に分けられるような場合、関数fを

* 「全体の制御」を行う関数g
* 「部分的な振る舞い」を規定するStrategyオブジェクト(関数へのポインタ、関数オブジェクト、ラムダ式)

に分割し、下記のように、Strategyオブジェクトをgの引数として外部から渡せるようにしたパターンである
(std::sort()のようなパターン)。

```cpp
    g(args, Strategyオブジェクト)
```

Strategyオブジェクトにいろいろなバリエーションがある場合、このパターンを使うと良い。
なお、このパターンの対象はクラスになる場合もある。

「ディレクトリをリカーシブに追跡し、引数で指定された属性にマッチしたファイルの一覧を返す関数」
を開発することを要求されたとする。

まずは、拡張性のない実装例を示す。

```cpp
    // @@@ example/design_pattern/find_files_old_style.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/find_files_old_style.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/find_files_ut.cpp #0:0 begin
```

この関数は、見つかったファイルが「引数で指定された属性」にマッチするかどうかを検査する。
検査は、「引数で指定された属性」に対するswitch文によって行われる。
これにより、この関数は「引数で指定された属性」の変更に強く影響を受ける。

下記は、この関数にStrategyパターンを適用したものである。

```cpp
    // @@@ example/design_pattern/find_files_strategy.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/find_files_strategy.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/find_files_ut.cpp #0:1 begin
```

捜査対象のファイル属性の指定をfind_files_recursively()の外に出しため、
その属性の追加に対して「[オープン・クローズドの原則(OCP)](---)」に対応した構造となった。

なお、上記find_files_recursivelyの第2パラメータをテンプレートパラメータとすることで、

```cpp
    // @@@ example/design_pattern/find_files_strategy.h #1:0 begin
```

のように書くこともできる。

次に示すのは、このパターンを使用して、プリプロセッサ命令を排除するリファクタリングの例である。

まずは、出荷仕分け向けのプリプロセッサ命令をロジックの内部に記述している問題のあるコードを示す。
このようなオールドスタイルなコードは様々な開発阻害要因になるため、避けるべきである。

```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #0:1 begin -1
```

このコードは、Strategyを使用し以下のようにすることで、改善することができる。

```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #1:1 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #1:2 begin -1
```

あるいは、[DI(dependency injection)](---)と組み合わせて、下記のような改善も有用である。

```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #2:1 begin
```
```cpp
    // @@@ example/design_pattern/strategy_shipping_ut.cpp #2:2 begin -1
```

[演習-Strategy](~~~)  

## Visitor
このパターンは、クラス構造とそれに関連するアルゴリズムを分離するためのものである。

最初に
「クラス構造とそれに関連するアルゴリズムは分離できているが、
それ以前にオブジェクト指向の原則に反している」
例を示す。

```cpp
    // @@@ example/design_pattern/visitor.cpp #0:0 begin
```

下記クラス図からもわかる通り、ポリモーフィズムに反したこのような構造は複雑な依存関係を作り出す。
このアンチパターンにより同型の条件文が2度出てきてしまうため、
Printerのアルゴリズム関数が増えれば、この繰り返しはそれに比例して増える。
またFileEntityの派生が増えれば、それら条件文はすべて影響を受ける。
このようなソースコードは、このようにして等比級数的に複雑化する。

![クラス図](plant_uml/visitor_ng1.png)

これをポリモーフィズムの導入で解決した例を示す。

```cpp
    // @@@ example/design_pattern/visitor.cpp #1:0 begin
```

上記例では、PrinterのアルゴリズムをFileEntityの各派生クラスのメンバ関数で実装することで、
Printerの各関数は単純化された。

![クラス図](plant_uml/visitor_ng2.png)

これはポリモーフィズムによるリファクタリングの良い例と言えるが、
SRP(「[単一責任の原則(SRP)](---)」)に反するため、
Printerの関数が増えるたびにPrintPathname1、
PrintPathname2のようなFileEntityのインターフェースが増えてしまう。

このようなインターフェースの肥大化に対処するパターンがVisitorである。

上記例にVisitorを適用してリファクタリングした例を示す。

```cpp
    // @@@ example/design_pattern/visitor.h #0:0 begin
    // @@@ example/design_pattern/visitor.h #0:1 begin
    // @@@ example/design_pattern/visitor.h #0:2 begin
```

```cpp
    // @@@ example/design_pattern/visitor.cpp #2:0 begin
```

上記クラスの関係は下記のようになる。

![クラス図](plant_uml/visitor_ok.png)

このリファクタリングには、

* FileEntityのインターフェースを小さくできる
* FileEntityVisitorから派生できるアルゴリズムについては、
  FileEntityのインターフェースに影響を与えずに追加できる
  (「[オープン・クローズドの原則(OCP)](---)」参照)

という利点がある。
一方で、この程度の複雑さの(単純な)例では、Visitorの適用によって以前よりも構造が複雑になり、
改悪してしまった可能性があるため、デザインパターンを使用する場合には注意が必要である。

なお、上記の抜粋である下記コード

```cpp
    // @@@ example/design_pattern/visitor.h #0:1 begin -1 1
```

はコードクローンだが、thisの型が違うため、
各Acceptが呼び出すFileEntityVisitor::Visit()も異り、単純に統一することはできない。
これを改めるためには、「[CRTP(curiously recurring template pattern)](---)」が必要になる。

次に示すソースコードはVisitorとは関係がないが、
FileEntityVisitorから派生するクラスを下記クラス図が示すように改善することで、
単体テストが容易になる例である(「[DI(dependency injection)](---)」参照)。

![クラス図](plant_uml/visitor_ut.png)

```cpp
    // @@@ example/design_pattern/visitor.h #1:0 begin
```

```cpp
    // @@@ example/design_pattern/visitor.cpp #3:0 begin
```

```cpp
    // @@@ example/design_pattern/visitor_ut.cpp #0:0 begin
```

[演習-Visitor](~~~)  

## CRTP(curiously recurring template pattern)
CRTPとは、

```cpp
    // @@@ example/design_pattern/crtp_ut.cpp #0:0 begin
```

のようなテンプレートによる再帰構造を用いて、静的ポリモーフィズムを実現するためのパターンである。

このパターンを用いて、「[Visitor](---)」のFileEntityの3つの派生クラスが持つコードクローン

```cpp
    // @@@ example/design_pattern/visitor.h #0:1 begin -1 1
```

を解消した例を以下に示す。

```cpp
    // @@@ example/design_pattern/crtp.h #0:0 begin
```

## Observer
Observerは、クラスSubjectと複数のクラスObserverN(N = 0, 1, 2 ...)があり、
この関係が下記の条件を満たさなければならない場合に使用されるパターンである。

* ObserverNオブジェクトはSubjectオブジェクトが変更された際、その変更通知を受け取る。
* SubjectはObserverNへ依存してはならない。

GUIアプリケーションを[MVC](---)で実装する場合のModelがSubjectであり、
ViewがObserverNである。

まずは、このパターンを使用しない実装例を示す。

```cpp
    // @@@ example/design_pattern/observer_ng.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/observer_ng.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/subject_ng.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/subject_ng.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/observer_ut.cpp #0:0 begin -1
```

上記実装例のクラス図を下記する。
これを見ればわかるように、クラスSubjectNGとクラスObserverNG_Nは相互依存しており、機能追加、
修正が難しいだけではなく、この図の通りにパッケージを分割した場合
(パッケージがライブラリとなると前提）、リンクすら難しくなる。

![observer クラス図](plant_uml/observer_class_ng.png)

このようなクラス間の依存関係は下記のようにファイル間の依存関係に反映される。
このような相互依存は、差分ビルドの長時間化等の問題も引き起こす。

![ファイルの依存関係](plant_uml/observer_file_ng.png)

次に、上記にObserverパターンを適用した実装例
(Subjectを抽象クラスにすることもあるが、下記例ではSubjectを具象クラスにしている)を示す。

```cpp
    // @@@ example/design_pattern/observer_ok.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/observer_ok.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/subject_ok.h #0:0 begin
```

```cpp
    // @@@ example/design_pattern/subject_ok.cpp #0:0 begin
```

```cpp
    // @@@ example/design_pattern/observer_ut.cpp #0:1 begin -1
```

上記実装例のクラス図を下記する。
Observerパターンを使用しない例と比べると、
クラスSubjectOKとクラスObserverOK_Nとの相互依存が消えたことがわかる。

![observer クラス図](plant_uml/observer_class_ok.png)

最後に、上記のファイルの依存関係を示す。
ファイル(パッケージ)の依存関係においてもSubjectOKはObserverOKに依存していないことがわかる
(MVCに置き換えると、ModelはViewに依存していない状態であるといえる)。

![ファイルの依存関係](plant_uml/observer_file_ok.png)

[演習-Observer](~~~)  

## MVC

MVCはデザインパターンと言うよりもアーキテクチャパターンである。
一般にGUIアプリケーションのアーキテクチャに使用されるが、
外部からの非同期要求を処理するアプリケーションのアーキテクチャにも相性が良い。

MVCのそれぞれのアルファベットの意味は、下記テーブルの通りである。

|   | MVC            | 主な役割                        |
|:-:|:---------------|:--------------------------------|
| M | Model          | ビジネスロジックの処理          |
| V | View           | UIへの出力                      |
| C | Controller     | 入力をModelへ送信               |

下記はMVCの概念モデルである(矢印は制御の流れであって、依存関係ではない)。

![MVC](plant_uml/mvc.png)

制御の流れは、

1. ユーザの入力に応じてControllerのメソッドが呼び出される。
2. Controllerのメソッドは、ユーザの入力に応じた引数とともにModelのメソッドを呼び出す。
3. Modelは、それに対応するビジネスロジック等の処理を(通常、非同期に)行い、
自分自身の状態を変える(変わらないこともある)。
4. Modelの状態変化は、そのModelのオブザーバーとして登録されているViewに通知される。
5. Viewは関連するデータをModelから取得し、それを出力(UIに表示)する。

ViewはModelの[Observer](---)であるため、ModelはViewへ依存しない。
多々あるMVC派生パターンすべてで、そのような依存関係は存在しない
(具体的なパターンの選択はプロジェクトで使用するGUIフレームワークに強く依存する)。

そのようにする理由は下記の通りで、極めて重要な規則である。

* GUIのテストは目で見る必要がある(ことが多い)ため、Viewに自動単体テストを実施することは困難である。
  一方、ViewがModelに依存しないのであれば、Modelは自動単体テストをすることが可能である。
* 通常、Viewの仕様は不安定で、Modelの仕様は安定しているため、Modelのソースコード変更は
  Viewのそれよりもかなり少ない。
  しかし、ModelがViewに依存してしまうと、Viewに影響されModelのソースコード変更も多くなる。

[演習-デザインパターン選択1](~~~)  
[演習-デザインパターン選択2](~~~)  
[演習-デザインパターン選択3](~~~)  

## Cでのクラス表現
このドキュメントは、C++でのソフトウェア開発を前提としているため、
ここで示したコードもC++で書いているが、

* 何らかの事情でCを使わざるを得ないプログラマがデザインパターンを使用できるようにする
* クラスの理解が曖昧なC++プログラマの理解を助ける(「[クラスのレイアウト](---)」参照)

ような目的のためにCでのクラスの実現方法を例示する。

下記のような基底クラスPointとその派生クラスPoint3Dがあった場合、

![ステートマシン図](plant_uml/class_c.png)

C++では、Pointのコードは下記のように表すことが一般的である。

```cpp
    // @@@ example/design_pattern/class_ut.cpp #0:0 begin
```

この単体テストは、下記のようになる。

```cpp
    // @@@ example/design_pattern/class_ut.cpp #0:1 begin -1
```

これをCで表した場合、下記のようになる。

```cpp
    // @@@ example/design_pattern/class_ut.cpp #2:0 begin
```

C++のメンバ関数はプログラマから見えない引数thisを持つ。
これを表したものが各関数の第1引数selfである。
また、ポリモーフィックな関数は関数ポインタで、
非ポリモーフィックな関数は通常の関数で表される。

この単体テストは、下記のようになる。

```cpp
    // @@@ example/design_pattern/class_ut.cpp #2:1 begin -1
```

Pointから派生したクラスPoint3DのC++での実装を以下に示す。

```cpp
    // @@@ example/design_pattern/class_ut.cpp #1:0 begin
```

この単体テストは、下記のようになる。

```cpp
    // @@@ example/design_pattern/class_ut.cpp #1:1 begin -1
```

これをCで実装したものが下記である。

```cpp
    // @@@ example/design_pattern/class_ut.cpp #3:0 begin
```

この単体テストは、下記のようになる。

```cpp
    // @@@ example/design_pattern/class_ut.cpp #3:1 begin -1
```

以上からわかる通り、Cでのクラス実装はC++のものに比べ、

* 記述が多い
* キャストを使わざるを得ない
* リファレンスが使えないため、NULLにならないハンドル変数をポインタにせざるを得ない

等といった問題があるため、「何らかの事情でC++が使えない」チームは、
なるべく早い時期にその障害を乗り越えることをお勧めする。

どうしてもその障害を超えられない場合は、
[モダンC言語プログラミング](https://www.amazon.co.jp/%E3%83%A2%E3%83%80%E3%83%B3C%E8%A8%80%E8%AA%9E%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0-%E3%82%A2%E3%82%B9%E3%82%AD%E3%83%BC%E6%9B%B8%E7%B1%8D-%E8%8A%B1%E4%BA%95-%E5%BF%97%E7%94%9F-ebook/dp/B00HWLJEKW)
が役に立つだろう。


