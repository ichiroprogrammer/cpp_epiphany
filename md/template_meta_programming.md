# テンプレートメタプログラミング

本章でのテンプレートメタプログラミングとは、下記の2つを指す。

* ジェネリックプログラミング
* メタプログラミング

C++においては、この2つはテンプレートを用いたプログラミングとなる。

ジェネリックプログラミングとは、具体的なデータ型に依存しない抽象的プログラミングであり、
その代表的な成果物はSTLのコンテナやそれらを扱うアルゴリズム関数テンプレートである。

この利点は、

* i種の型
* j種のコンテナ
* k種のアルゴリズム

の開発を行うことを考えれば明らかである。

ジェネリックプログラミングが無ければ、コンテナの種類はi x j個必要になり、
それらに適用するアルゴリズム関数は、i x j x k個必要になる。
また、サポートする型の増加に伴いコンテナやアルゴリズム関数は指数関数的に増えて行く。
C言語のqsort()のように強引なキャストを使い、この増加をある程度食い止めることはできるが、
それによりコンパイラによる型チェックは無効化され、
静的な型付け言語を使うメリットの多くを失うことになる。

メタプログラミングとは、

* ジェネリックのサポート
* 実行時コードの最適化
* 関数やクラスを生成するコードのプログラミング

のような目的で行われるテンプレートプログラミングの総称である。

ジェネリックプログラミングとメタプログラミングに明確な境界はない、
また明確にしたところで大きなメリットはと思われるため、
本章では、これらをまとめた概念であるテンプレートメタプログラミングとして扱い、
ログ取得ライブラリやSTLを応用したNstdライブラリの実装を通して、
これらのテクニックや、使用上の注意点について解説する。

## ログ取得ライブラリの開発
ここではログ取得ライブラリの開発を行う。

### 要件

ログ取得ライブラリの要件は、

* ソースコードの場所とそこで指示されたオブジェクトの値を文字列で保持する
* 後からそれらを取り出せる

ことのみとする。下記はその文字列を取り出した例である。

```
    app/src/main.cpp: 96:Options
                         cmd       : GenPkg
                         in        : 
                         out       : 
                         recursive : true
                         src_as_pkg: false
        ...

    app/src/main.cpp: 51:start GenPkg

    file_utils/ut/path_utils.cpp: 38:1
    file_utils/ut/path_utils.cpp: 48:ut_data/app1
                         ut_data/app1/mod1
                         ut_data/app1/mod2

        ...

    app/src/main.cpp:100:Exit:0
```

単純化のためログの番号やタイムスタンプのサポートはしない。
また、実行速度や仕様メモリ量の制限等も本章の趣旨とは離れるため考慮しない。

### ログ取得ライブラリのインターフェース

ログ取得コードにより、コードクローンが増えたり、主なロジックの可読性が下がったのでは、
本末転倒であるため、下記のようにワンライナーで記述できるべきだろう。

```cpp
    LOGGER("start GenPkg", objA, objB, objC);
```

また、要件で述べた通り、ソースコード位置を特定できなければならないため、
上記LOGGERは下記のような関数型マクロにならざるを得ない。

```cpp
    #define LOGGER(...) CppLoggerFunc(__FILE__, __LINE__, __VA_ARGS__)
```

CppLoggerFuncをクラス外の関数として実装した場合、ログ保持のための静的なオブジェクトが必要になる。
これは避けるべきなので、「[Singleton](---)」で述べた構造を導入すると、

```cpp
    #define LOGGER(...) Logger::Inst().Set(__FILE__, __LINE__, __VA_ARGS__)
```

のような関数型マクロとなる。これがログ取得ライブラリの主なインターフェースとなる。

C言語プログラミングばかりをやりすぎて、
視神経と手の運動神経が直結してしまった大脳レス・プログラマーは、

```cpp
    __VA_ARGS__
```

を見るとprintf(...)のような可変長引数を取る関数を思い浮かべる。
「人は一昨日も行ったことを昨日も行ったという理由で、今日もそれを行う」
という諺を思い出すと気持ちは分からなくもないが、 
C++ではprintf(...)のような危険な可変長引数を取る関数を作ってはならない。
パラメータパックを使って実装するべきである。

### パラメータパック
C++11で導入されたパラメータパックはやや複雑なシンタックスを持つため、
まずは単純な例から説明する。

次のような単体テストをパスする関数テンプレートsumをパラメータパックで実装することを考える。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #0:1 begin -1
```

sumの要件は、

* 可変長引数を持つ
* [用語解説|型とインスタンス|算術型](---)の引数と戻り値を持つ
* すべての引数の和を返す

のようなものになるため、関数テンプレートsumは下記のように書ける。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #0:0 begin
```

1つ目の関数テンプレートsumは引数が一つの場合に対応する。
2つ目の関数テンプレートsumは引数が2つ以上の場合に対応する。

2つ目の関数テンプレートsumのテンプレートパラメータ

```cpp
    typename... TAIL
```

がパラメータパックであり、0個以上の型が指定されることを意味する。
これを関数の引数として表すシンタックスが

```cpp
    TAIL... tails
```

であり、同様に0個以上のインスタンスが指定されることを表している。

HEADとTAILより、2つ目のsumは1個以上の引数を取れることになるため、
引数が1つの場合、どちらのsumを呼び出すかが曖昧になるように思えるが、
ベストマッチの観点から1つ目のsumが呼び出される。

sum(1, 2, 3)の呼び出し時のsumの展開を見てみることでパラメータパックの振る舞いを解説する。

この呼び出しは、2つ目のsumにマッチする。従って下記のように展開される。

```cpp
    return 1 + sum(2, 3);
```

sum(2, 3)も同様に展開されるため、上記コードは下記のようになる。

```cpp
    return 1 + 2 + sum(3);
```

sum(3)は1つ目のsumにマッチするため、最終的には下記のように展開される。

```cpp
    return 1 + 2 + 3;
```

これで基本的な要件は満たしたが、このsumでは下記のようなコードもコンパイルできてしまう。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #0:2 begin -1
```

これを認めるかどうかはsumの仕様次第だが、ここではこれらを認めないようにしたい。
また、引数に浮動小数が与えられた場合でも、sumの戻り値の型がintなる仕様には問題がある。
合わせてそれも修正する。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/template/parameter_pack_ut.cpp #1:1 begin -1
```

以上で示したようにパラメータパックにより、
C言語での可変長引数関数では不可能だった引数の型チェックができるようになったため、
C言語でのランタイムエラーがコンパイルエラーにできるようになった。

なお、上記コードで使用した[std::is_same](---)は、
与えられた2つのテンプレートパラメータが同じ型であった場合、
valueをtrueで初期化するクラステンプレートであり、 type_traitsで定義されている
(後ほど使用するstd::is_same_vはstd::is_same<>::valueと等価な定数テンプレート)。
この実装については、後ほど説明する。

[演習-パラメータパック](~~~)  

#### パラメータパックの畳みこみ式
上記したsumは、パラメータパックの展開に汎用的な再帰構造を用いたが、
C++17で導入された畳みこみ式を用い、以下の様に簡潔に記述することもできる。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/template/parameter_pack_ut.cpp #2:1 begin -1
```

畳みこみ式で使用できる演算子を以下に示す。

```
    + - * / % ^ & | = < > << >> += -= *= /= %= ^= &= |= <<= >>= == != <= >= && || , .* ->*
```

これらの演算子がオーバーロードである場合でも、畳みこみ式は利用できる。


#### 前から演算するパラメータパック
パラメータパックを使うプログラミングでは、
上記したHEADとTAILによるリカーシブコールがよく使われるパターンであるが、
これには後ろから処理されるという、微妙な問題点がある。

これまでのsumに代えて下記のようなproduct(掛け算)を考える。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #3:0 begin
```

このコードの単体テストは、

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #3:1 begin -1
```

のようになるだろうが、
std::numeric_limits<>::epsilonを使用していないため
(「[浮動小数点型](~~~)」参照)
、このテストはパスしない。一方で、以下のテストはパスする。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #3:2 begin -1
```

一般に0.01の2進数表現は無限小数になるため、これを含む演算にはepsilon以下の演算誤差が発生する。
前者単体テストでは、後ろから演算されるために処理の途中に0.01が現れるが、
後者では現れないため、この誤差の有無が結果の差になる。

このような演算順序による微妙な誤差が問題になるような関数を開発する場合、
演算は見た目の順序通りに行われた方が良いだろう。
ということで、productを前から演算するように修正する。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #4:0 begin
```

HEAD、TAILに加えHEAD2を導入することで、前からの演算を実装できる
(引数が一つのproductに変更はない)。当然ながら、これにより、

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #4:1 begin -1
```

はパスし、下記はパスしなくなる。

```cpp
    // @@@ example/template/parameter_pack_ut.cpp #4:2 begin -1
```


### Loggerの実装

パラメータパックを使用したログ取得コードは以下のようになる。

```cpp
    // @@@ example/template/logger_0.h #0:1 begin
```

予定していたものと若干違う理由は、\_\_VA\_ARGS\_\_が1個以上の識別子を表しているからである。
従って、通過ポイントのみをロギングしたい場合、LOGGER_P()を使うことになる。
gcc拡張を使えば、LOGGER_PとLOGGERを統一できるが、そのようなことをすると別のコンパイラや、
静的解析ツールが使用できなくなることがあるため、残念だが上記のように実装するべきである。

Loggerクラスの実装は、下記のようになる。

```cpp
    // @@@ example/template/logger_0.h #0:0 begin
```

すでに述べた通り、

* クラスはシングルトンにする
* パラメータパックにより可変長引数を実現する

ようにした。
また、識別子の衝突を避けるために、名前空間Loggingを導入し、Loggerはその中で宣言した。

次に、どのように動作するのかを単体テストで示す。

```cpp
    // @@@ example/template/logger_0_ut.cpp #0:0 begin -1
```

行を含む出力の期待値をソースコードに直接書くと行増減のたびにそれらを修正する必要ある。
期待値の一部を自動計算する下記コード(上記コードで使用)を単体テストに導入することで、
そういった修正を避けている。

```cpp
    // @@@ example/template/logger_ut.h #0:0 begin
```

アプリケーションの開発では、
下記のようなユーザが定義した名前空間とクラスを用いることがほとんどである。

```cpp
    // @@@ example/template/app_ints.h #1:0 begin
```

このApp::Xのインスタンスのログを取得できることも、当然Logging::Loggerの要件となる。
従って、下記の単体テストはコンパイルでき、且つパスすることが必要になる。


```cpp
    // @@@ example/template/logger_0_ut.cpp #1:2 begin -1
```

Logging::Loggerのコードからオブジェクトのログを登録するためには、
Logging::Logger::set_innerがコンパイルできなければならない。
つまり、

```cpp
    std::ostream& operator<<(std::ostream&, ログ登録オブジェクトの型)
```

の実装が必要条件となる。App::Xでは下記のようなコードになる。

```cpp
    // @@@ example/template/app_ints.h #1:1 begin
```

他の任意のユーザ定義型に対しても、このようにすることでログ登録が可能になる。

なお、ヒューマンリーダブルな文字列でその状態を表示できる関数をユーザ定義型に与えることは、
デバッガを使用したデバッグ時にも有用である。

### ユーザ定義型とそのoperator\<\<のname lookup

ここで、一旦Logging::Loggerの開発を止め、
Logging::Logger::set_innerでのApp::operator<<の[name lookup](---)について考えてみることにする。

ここまでで紹介したログ取得ライブラリやそれを使うユーザ定義型等の定義、宣言の順番は、

1. Logging::Logger
2. App::X
3. App::operator<<
4. 単体テスト(Logger::set_innerのインスタンス化される場所)

となっている。
name lookupの原則に従い、App::Xの宣言は、App::operator<<より前に行われている。
これを逆にするとコンパイルできない。
しかし、Logging::Loggerは、後から宣言されたApp::operator<<を使うことができる。
多くのプログラマは、これについて気づいていないか、その理由を間違っての認識している。

その認識とは、「テンプレート内の識別子の[name lookup](---)は、
それがインスタンス化される時に行われる」というものであり、これにより
「 Logging::Loggerのname lookupは単体テスト内で行われる。
それはApp::operator<<宣言後であるためコンパイルできる」と考えることができるが、
[two phase name lookup](---)で行われるプロセスと反するため誤りである。

まずは、この認識の誤りを下記のコードで説明する。

```cpp
    // @@@ example/template/logger_0_ut.cpp #2:0 begin
```

このコードは、もともとのコードの名前空間名をApp2とApp3にしただけのものである。
もし、前記した「認識」の内容が正しいのであれば、このコードもコンパイルできるはずであるが
(実際にApp3と書いた部分をApp2に書き換えればコンパイルできる)、
実際には下記のようなエラーが発生する。


```
    logger_0.h:37:21: error: no match for ‘operator<<’ 
    (operand types are ‘std::basic_ostream<char>’ and ‘const App2::X’)
       37 |         oss_ << ":" << head;
          |         ~~~~~~~~~~~~^~~~~~~

```

エラー内容からoperator<<が発見できないことは明らかである。
単体テスト内でのusing namespace App3はLogging::Logger::set_innerの宣言より後に書かれているため、
このエラーを防ぐ効果はない。

Logging::Logger::set_innerの中でusing namespace App3とした上で、
two phase name lookupの原則に従い、
App2::XとApp3::operator<<をLogging::Loggerの宣言より前に宣言することで、
ようやくコンパイルすることができる。

名前空間Appの例と名前空間App2、App3の例での本質的な違いは、
「型Xとそのoperator<<が同じ名前空間で宣言されているかどうか」である。

名前空間Appの例の場合、型Xとそのoperator<<が同じ名前空間で宣言されているため、
[ADL](---)(実引数依存探索)が働く。
また、Logging::Logger::set_inner(x)はテンプレートであるため、[two phase name lookup](---)
が使用される。
その結果、Logging::Logger::set_inner(x)でのname lookupの対象には、
「Logging::Logger::set_inner(x)がインスタンス化される場所
(単体テスト内でのLOGGER_PやLOGGERが使われている場所)より前方で宣言された名前空間App」も含まれる。
こういったメカニズムにより、
Logging::Logger::set_inner定義位置の後方で宣言されたApp::operator<<も発見できることになる。

一方で、名前空間App2、App3の例では、型XがApp2で宣言されているため、
Logging::Logger::set_inner(x)でのname lookupの対象にApp3は含ず、
App3::operator<<は発見されない(繰り返すが、インスタン化の場所直前でのusing nameには効果がない)。

型Xとそのoperator<<を同じ名前空間で宣言することは本質的に重要なことであるが、
名前空間を使用する場合、自然にそのような構造になるため、
その重要性の理由を知る必要はないように思われる。
しかし、次の例で示すようにこのメカニズムを知らずに解決することができないケースが存在する。

### Ints_tのログ登録
話題はログ取得ライブラリの開発に戻る。
アプリケーションの開発では、下記のように宣言された型エイリアスを使うことは珍しくない。

```cpp
    // @@@ example/template/app_ints.h #0:0 begin
```

そのoperator<<を下記のように定義したとする。

```cpp
    // @@@ example/template/logger_0_ut.cpp #3:0 begin
```

単体テストは下記のように書けるが、残念ながらコンパイルエラーになり、

```cpp
    // @@@ example/template/logger_0_ut.cpp #3:1 begin -1
```

下記のようなエラーメッセージが表示される。

```
    logger_0_ut.cpp:140:9: error: no match for ‘operator<<’
        (operand types are ‘std::ostringstream’ {aka ‘std::basic_ostringstream<char>’}
         and ‘App::Ints_t’ {aka ‘std::vector<int>’})
      140 |     oss << ints;
          |     ~~~ ^~ ~~~~
          |     |      |
          |     |      App::Ints_t {aka std::vector<int>}
          |     std::ostringstream {aka std::basic_ostringstream<char>}
```

Ints_tはAppで定義されているが、実際の型はstdで定義されているため、
instsの[関連名前空間](---)もstdであり、Appではない。
その結果App::operator<<は発見できず、このようなエラーになった。

LOGGERからApp::operator<<を使う場合の単体テストは下記のようになるが、
[ADL](---)によってLogging::Logger::set_inner(ints)内に導入される名前空間はstdのみであり、
前記単体テスト同様にコンパイルできない。

```cpp
    // @@@ example/template/logger_0_ints_ut.h #0:0 begin -1
```

この解決方法は、

* [operator\<\<をstd内で宣言する](---)
* [operator\<\<をグローバル名前空間内で宣言する](---)
* [operator\<\<をLogging内で宣言する](---)
* [Logging::Logger::set_inner(ints)内でusing namespace Appを行う](---)
* [Ints_tを構造体としてApp内に宣言する](---)
* [operator\<\<を使わない](---)

のようにいくつか考えられる。以下では、順を追ってこれらの問題点について解説を行う。


#### operator\<\<をstd内で宣言する
ここで解決したい問題は、すでに示した通り、
「[ADL](---)によってLogging::Logger::set_inner(ints)内に導入される名前空間はstdである」
ことにって発生する。であれば、App内でのoperator<<の宣言をstdで行えばコンパイルできるはずである。
下記はその変更を行ったコードである。

```cpp
    // @@@ example/template/app_ints.h #0:0 begin
```
```cpp
    // @@@ example/template/logger_0_std_ut.cpp #0:0 begin
```

上記コードは[two phase name lookup](---)等の効果により、想定通りコンパイルできるが、
stdをユーザが拡張することは一部の例外を除き未定義動作を引き起こす可能性があり、
たとえこのコードがうまく動作したとしても
(実際、このコードはこのドキュメント作成時には正常動作している)、
未来においてその保証はなく、このようなプログラミングは厳に避けるべきである。


#### operator\<\<をグローバル名前空間内で宣言する
すでに述べた通り、
「ADLによってLogging::Logger::set_inner(ints)内に導入される名前空間はstdのみである」ため、
この関数の中でのname lookupに使用される名前空間は、std、グローバル名前空間、
Loggerを宣言しているLoggingの3つである。

ここでは、下記のコードのようにグローバル名前空間内でのoperator<<の宣言を試す。

```cpp
    // @@@ example/template/app_ints.h #0:0 begin
```
```cpp
    // @@@ example/template/logger_0_global_ut.cpp #0:0 begin
```

このドキュメントで使用している[g++](---)ではこのコードはコンパイルでき、
動作も問題ないように思われるが、[clang++](---)では以下のようなエラーが発生し、コンパイルできない。

```
    ./logger_0.h:37:21: error: call to function 'operator<<' that is neither 
    visible in the tem plate definition nor found by argument-dependent lookup
            oss_ << ":" << head;
```

この理由は「[two phase name lookup](---)」の後半で詳しく解説したので、ここでは繰り返さないが、
このようなコードを使うと、コード解析ツール等が使用できなくなることがあるため、
避けるべきである
(「[scan-buildによる静的解析](~~~)」参照)
。

多くのプログラマは、コードに問題があるとしても、
それが意図通りに動くように見えるのであればその問題を無視する。
今回のような難題に対しては、なおさらそのような邪悪な欲求に負けやすい。
そのような観点でclang++が吐き出したエラーメッセージを眺めると、上記したメッセージの後に、
下記のような出力を見つけるかもしれない。

```
    logger_0_global_ut.cpp:13:15: note: 'operator<<' should be declared prior to the call site
    std::ostream& operator<<(std::ostream& os, App::Ints_t const& ints)
```

clang++は「LOGGERの前にoperator<<を宣言せよ」と言っている。
実際そうすれば、clang++でのコンパイルも通り、単体テストもパスする。
しかし、それには下記のような問題がある。

* operator<<(std::ostream& os, App::Ints_t const& ints)
  という名前空間Appローカルな宣言をグローバル名前空間で行うことによって、
  グローバル名前空間を汚染してしまう
  (このコードは名前空間を正しく使うことに対しての割れ窓
  (「[割れ窓理論](---)」参照)になってしまうかもしれない)。
* 例示したコードでのoperator<<(std::ostream& os, App::Ints_t const& ints)の定義は、
  単体テストファイル内にあったが、実際には何らかのヘッダファイル内で定義されることになる。
  その場合、ロガーのヘッダファイルよりも、
  そのヘッダファイルを先にインクルードしなければならなくなる。
  これは大した問題ではないように見えるが、
  ヘッダファイル間の暗黙の依存関係を生み出し将来の保守作業を難しくさせる。


以上述べた理由からこのアイデアを選択するべきではない。


#### operator\<\<をLogging内で宣言する
前節でのグローバル名前空間内でのoperator<<の宣言はうまく行かなかったので、
同様のことをLoggingで試す。

```cpp
    // @@@ example/template/app_ints.h #0:0 begin
```
```cpp
    // @@@ example/template/logger_0_logging_ut.cpp #0:0 begin
```

動作はするものの、当然ながら結果は「[operator\<\<をグローバル名前空間内で宣言する](---)」
で述べた状況とほぼ同様であるため、このアイデアを採用することはできない。

#### Logging::Logger::set_inner(ints)内でusing namespace Appを行う
Logging::Logger::set_inner(ints)内でusing namespace Appを行えば、
意図通りに動作させることができるが、App内のロギングは名前空間Loggingに依存するため、
AppとLoggingが循環した依存関係を持ってしまう。
また、LoggingはAppに対して上位概念であるため、[依存関係逆転の原則(DIP)](---)にも反する。
よって、このアイデアを採用することはできない。

#### Ints_tを構造体としてApp内に宣言する

App::Ints_t用のoperator<<がLogging::Logger::set_inner内でname lookup出来ない理由は、
これまで述べてきたようにApp::Inst_tの[関連名前空間](---)がAppではなく、stdになってしまうからである。

これを回避するためにはその原因を取り払えばよく、
つまり、App::Inst_tの[関連名前空間](---)がAppになるようにすればよい。
これを実現するために、次のコードを試してみる。

```cpp
    // @@@ example/template/logger_0_struct_ut.cpp #0:0 begin
```

上記のコードでは、

* App::Ints_tをstd::vectorからpublic継承
* using宣言によりstd::vectorのすべてのコンストラクタをApp::Ints_tに導入
  (「[オブジェクトと生成|継承コンストラクタ](---)」参照)

としているため、エイリアスで宣言されたInts_tと等価である。
C++03では、[オブジェクトと生成|継承コンストラクタ](---)が使えなかったため、
上記のような構造体を定義するためには、
std::vectorのすべてのコンストラクタと等価なコンストラクタをApp::Ints_t内に定義することが必要で、
実践的にはこのようなアイデアは使い物にならなかったが、
C++11での改善により、実践的なアイデアとして使用できるようになった。

実際、名前空間の問題もなく、すでに示した単体テストもパスするので有力な候補となるが、
若干の「やりすぎ感」は否めない。


#### operator\<\<を使わない
色々なアイデアを試してみたが、これまでの議論ではこれといった解決方法を発見できなかった。
「[バーニーの祈り](https://ja.wikipedia.org/wiki/%E3%83%8B%E3%83%BC%E3%83%90%E3%83%BC%E3%81%AE%E7%A5%88%E3%82%8A)」
が言っている通り、時にはどうにもならないことを受け入れることも重要である。
LOGGERの中でname lookupできる、エイリアスApp::Ints_tのoperator<<の開発をあきらめ、
ここでは一旦、下記のような受け入れがたいコードを受け入れることにする。


```cpp
    // @@@ example/template/app_ints.h #0:0 begin
```
```cpp
    // @@@ example/template/logger_0_no_put_to_ut.cpp #0:0 begin
```

当然だが、恥を忍んで受け入れたコードにも単体テストは必要である。

```cpp
    // @@@ example/template/logger_0_no_put_to_ut.cpp #0:1 begin -1
```

上記コードから明らかな通り、App::Ints_tのインスタンスをログ登録する場合、
App::ToString()によりstd::stringへ変換する必要があり、残念なインターフェースとなっている。


#### Ints_tのログ登録のまとめ
製品開発では、満足できる仕様の関数やクラスが作れず、妥協せざるを得ないことはよくあることである。
このような場合、将来、良いアイデアが見つかった時に備えて、
妥協コードを簡単に修正できるようなレベルにした後、捲土重来を期してさっさと退却するのがベストである。
ただし、漫然と過ごしても良いアイデアは浮かばない。
時間を作り、関連書籍やウェブドキュメント等を読み、学習を継続する必要があることは言うまでもない。

[演習-名前空間による修飾不要なoperator\<\<](~~~)  

## Nstdライブラリの開発

「[operator\<\<を使わない](---)」で導入したコードは、短いながらも汎用性が高い。
このようなコードをローカルなファイルに閉じ込めてしまうと、
コードクローンや、[車輪の再発明](---)による開発効率の低下につながることがある。

通常、プロジェクトの全ファイルから参照可能で且つ、
プロジェクトの他のパッケージに非依存なパッケージを用意することで、このような問題を回避できる。

ここでは、そのようなパッケージをNstd(not standard library)とし、名前空間も同様に宣言する。
そうした場合、この章の例題で使用している名前空間の依存関係は下記のようになる。

![依存関係](plant_uml/template_dependency.png)

このように整理された依存関係は、
「[パッケージとその構成ファイル](~~~)」でも述べた通り、
大規模ソフトウェア開発においては特に重要であり、決して循環しないように維持しなければならない。


### Nstdライブラリを使用したリファクタリング
すでに述べた通り、「[operator\<\<を使わない](---)」で導入したコードは、Nstdで定義するべきである。
その場合、下記のようにさらに一般化するのが良いだろう。

```cpp
    // @@@ example/template/nstd_0.h #0:0 begin
```

その単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_0_ut.cpp #0:0 begin -1
```

勘のいい読者なら、このコードをLOGGERから利用することで、
App:Ints_tのログ登録問題を解消できると思うかもしれない。
実際その通りなのであるが、そうした場合、
std::list等の他のコンテナや配列には対応できないという問題が残るため、
以降もしばらくNstdの開発を続ける。


### 安全なvector
std::vector、std::basic_string、std::array等の配列型コンテナは、

* operator[]経由でのメンバアクセスについて範囲の妥当性をチェックしない
* 範囲のチェックが必要ならばat()を使用する

という仕様になっているが、
ここではoperator[]にも範囲のチェックを行う配列型コンテナが必要になった場合について考える。

手始めにoperator[]にも範囲のチェックを行うstd::vector相当のコンテナSafeVectorを作ると、
下記のコードのようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #0:0 begin -1
```

このコードで行ったことは、

* std::vectorからSafeVectorをpublic継承する
* 継承コンストラクタの機能を使い、std::vectorのコンストラクタをSafeVectorで宣言する
* std::vector::atを使い、SafeVector::operator[]を定義する

である。単体テストは下記のようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #0:1 begin -1
```

[演習-std::arrayの継承](~~~)  

### 安全な配列型コンテナ
配列型コンテナはすでに述べたようにstd::vectorの他にすくなともstd::basic_string、
std::arrayがあるため、それらにも範囲チェックを導入する。

std::basic_stringはstd::vectorとほぼ同様に下記のようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #1:0 begin
```

std::stringはstd::basic_string\<char>のエイリアスであるため、
上記では、通常使われる形式であるstd::stringを継承したSafeStringを定義した。

この単体テストはSafeVectorの場合と同様に下記のようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #1:1 begin -1
```

std::arrayでは少々事情が異なるが、
std::vectorのコードパターンをそのまま適用すると下記のようになる。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #2:0 begin
```

ただし、この実装には問題がある。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #2:1 begin -2
```

上記コードでは、その問題が露見することはないが、以下のコードはコンパイルできない。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #2:2 begin -2
```

std::arrayにはコンストラクタが明示的に定義されていないため、
std::arrayにはデフォルトで自動生成される

* デフォルトコンストラクタ
* copyコンストラクタ
* moveコンストラクタ

以外のコンストラクタがないことが原因である。
従って、SafeArray(std::initializer_list)が定義されず前述したようにコンパイルエラーとなる。

この問題に対処したのが以下のコードである。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #3:0 begin
    // @@@ example/template/safe_vector_ut.cpp #3:1 begin
    // @@@ example/template/safe_vector_ut.cpp #3:2 begin
```

上記コードで注目すべきは、パラメータパックをテンプレートパラメータとしたコンストラクタである。
これにより、前例ではコンパイルすらできなかった下記のような初期化子リストを用いた単体テストが、
このコンストラクタによりパスするようになった。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #3:3 begin -1
```

この効果を生み出した上記を抜粋した下記のコードには解説が必要だろう。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #3:1 begin -1
```

一般にコンストラクタには「メンバ変数の初期化」と「基底クラスの初期化」が求められるが、
SafeArrayにはメンバ変数が存在しないため、
このコンストラクタの役割は「基底クラスの初期化」のみとなる。
基底クラスstd::array(上記例ではbase_typeにエイリアスしている)
には名前が非規定の配列メンバのみを持つため、
これを初期化するためには初期化子リスト
(「[オブジェクトと生成|初期化子リストコンストラクタ](---)」、
「[オブジェクトと生成|一様初期化](---)」参照)を用いるのが良い。

ということは、SafeArrayの初期化子リストコンストラクタには、
「基底クラスstd::arrayに初期子リストを与えて初期化する」形式が必要になる。
値を持つパラメータパックは初期化子リストに展開できるため、
ここで必要な形式はパラメータパックとなる。
これを実現したのが上記に抜粋したわずか数行のコードである。


### 初期化子リストの副作用
上記SafeArrayの初期化子リストコンストラクタは以下のようなコードを許可しない。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #3:4 begin -1
```

このコードをコンパイルすると、

```
    safe_vector_ut.cpp:147:41: error: narrowing conversion of ‘
                                        args#0’ from ‘double’ to ‘int’ -Werror=narrowing]
      147 |     SafeArray(ARGS... args) : base_type{args...}
          |                                         ^~~~
```

のようなエラーが出力されるが、

* double(上記例では1.0)をintに変換する際に縮小変換(narrowing conversio)nが起こる
* 初期化子リストでの縮小変換は許可されない

が原因である。これは意図しない縮小変換によるバグを防ぐ良い機能だと思うが、
ここではテンプレートメタプログラミングのテクニックを解説するため、
あえてこのコンパイルエラーを起こさないSafeArray2を開発する
(言うまでもないが、通常のソフトウェア開発では、
縮小変換によるコンパイルエラーを回避するようなコードを書いてはならない)。

SafeArray2のコードは、

* STLのtype_traitsの使用
* テンプレートの特殊化
* メンバ関数テンプレートとオーバーロードによる静的ディスパッチ(コンパイル時ディスパッチ)
* [SFINAE](---)

等のメタ関数系のテクニックが必要になるため、
まずはこれらを含めたテンプレートのテクニックについて解説し、
その後SafeArray2を見ていくことにする。

## メタ関数のテクニック
本章で扱うメタ関数とは、型、定数、クラステンプレート等からなるテンプレート引数から、
型、エイリアス、定数等を宣言、定義するようなクラステンプレート、関数テンプレート、
定数テンプレート、エイリアステンプレートを指す
(本章ではこれらをまとめて単にテンプレート呼ぶことがある)。

[演習-エイリアステンプレート](~~~)  

### STLのtype_traits
メタ関数ライブラリの代表的実装例はSTLの
[type_traits](https://cpprefjp.github.io/reference/type_traits.html)である。

ここでは、よく使ういくつかのtype_traitsテンプレートの使用例や解説を示す。


#### std::true_type/std::false_type
std::true_type/std::false_typeは真/偽を返すSTLメタ関数群の戻り型となる型エイリアスであるため、
最も使われるテンプレートの一つである。

これらは、下記で確かめられる通り、後述する[std::integral_constant](---)を使い定義されている。

```cpp
    // @@@ example/template/type_traits_ut.cpp #0:0 begin -1
```

それぞれの型が持つvalue定数は、下記のように定義されている。

```cpp
    // @@@ example/template/type_traits_ut.cpp #0:1 begin -1
```

これらが何の役に立つのか直ちに理解することは難しいが、
true/falseのメタ関数版と考えれば、追々理解できるだろう。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp #0:2 begin
```

上記の単体テストは下記のようになる。

```cpp
    // @@@ example/template/type_traits_ut.cpp #0:3 begin -1
```

IsCovertibleToIntの呼び出しをdecltypeのオペランドにすることで、
std::true_typeかstd::false_typeを受け取ることができる。

#### std::integral_constant
std::integral_constantは
「テンプレートパラメータとして与えられた型とその定数から新たな型を定義する」
クラステンプレートである。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp #1:0 begin -1
```

また、すでに示したようにstd::true_type/std::false_typeを実装するためのクラステンプレートでもある。

#### std::is_same

すでに上記の例でも使用したが、std::is_sameは2つのテンプレートパラメータが

* 同じ型である場合、std::true_type
* 違う型である場合、std::false_type

から派生した型となる。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp #3:0 begin -1
```

また、 C++17で導入されたstd::is_same_vは、定数テンプレートを使用し、
下記のように定義されている。

```cpp
    // @@@ example/template/type_traits_ut.cpp #3:1 begin
```

```cpp
    // @@@ example/template/type_traits_ut.cpp #3:2 begin -1
```

このような簡潔な記述の一般形式は、

```
   T::value  -> T_v
   T::type   -> T_t
```

のように定義されている(このドキュメントのほとんど場所では、簡潔な形式を用いる)。

第1テンプレートパラメータが第2テンプレートパラメータの基底クラスかどうかを判断する
std::is_base_ofを使うことで下記のようにstd::is_sameの基底クラス確認することもできる。

```cpp
    // @@@ example/template/type_traits_ut.cpp #3:3 begin -1
```


#### std::enable_if
std::enable_ifは、bool値である第1テンプレートパラメータが

* trueである場合、型である第2テンプレートパラメータをメンバ型typeとして宣言する。
* falseである場合、メンバ型typeを持たない。

下記のコードはクラステンプレートの特殊化を用いたstd::enable_ifの実装例である。

```cpp
    // @@@ example/template/type_traits_ut.cpp #4:0 begin
```

std::enable_ifの使用例を下記に示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp #4:1 begin -1
```

実装例から明らかなように

* std::enable_if\<true>::typeは[well-formed](---)
* std::enable_if\<false>::typeは[ill-formed](---)

となるため、下記のコードはコンパイルできない。

```cpp
    // @@@ example/template/type_traits_ut.cpp #4:2 begin -1
```

std::enable_ifのこの特性と後述する[SFINAE](---)により、
様々な静的ディスパッチを行うことができる。


#### std::conditional

std::conditionalは、bool値である第1テンプレートパラメータが

* trueである場合、第2テンプレートパラメータ
* falseである場合、第3テンプレートパラメータ

をメンバ型typeとして宣言する。

下記のコードはクラステンプレートの特殊化を用いたstd::conditionalの実装例である。

```cpp
    // @@@ example/template/type_traits_ut.cpp #5:0 begin
```

std::conditionalの使用例を下記に示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp #5:1 begin -1
```

#### std::is_void
std::is_voidはテンプレートパラメータの型が

* voidである場合、std::true_type
* voidでない場合、std::false_type

から派生した型となる。

以下に簡単な使用例を示す。

```cpp
    // @@@ example/template/type_traits_ut.cpp #2:0 begin -1
```

### is_void_xxxの実装
ここではstd::is_voidに似た以下のような仕様を持ついくつかのテンプレートis_void_xxxの実装を考える。

|テンプレートパラメータ|戻り値            |
|----------------------|------------------|
|void                  |true              |
|非void                |false             |

それぞれのis_void_xxxは下記テーブルで示した言語機能を使用して実装する。

|is_void_xxx      |実装方法                                               |
|-----------------|-------------------------------------------------------|
|is_void_f        |関数テンプレートの特殊化                               |
|is_void_s        |クラステンプレートの特殊化                             |
|is_void_sfinae_f |SFINAEと関数テンプレートのオーバーロード               |
|is_void_sfinae_s |SFINAEとクラステンプレートの特殊化                     |
|is_void_ena_s    |std::enable_ifによるSFINAEとクラステンプレートの特殊化 |
|is_void_cond_s   |std::conditionalと関数テンプレートの特殊化             |

なお、実装例をシンプルに保つため、
理解の妨げとなり得る下記のような正確性(例外条件の対応)等のためのコードを最低限に留めた。

* テンプレートパラメータの型のチェック
* テンプレートパラメータの型からのポインタ/リファレンス/const/volatileの削除
* 戻り型からのconst/volatileの削除

これは、「テンプレートプログラミングでの有用なテクニックの解説」
というここでの目的を見失わないための措置である。


#### is_void_fの実装
関数テンプレートの特殊化を使用したis_void_fの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #0:0 begin
```

単純なので解説は不要だろう。これらの単体テストは下記のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #0:1 begin -1
```

関数テンプレートの特殊化には、

* 特殊化された関数テンプレートとそのプライマリテンプレートのシグネチャ、
  戻り値は一致しなければならない
* クラステンプレートのような部分特殊化は許可されない

のような制限があるため用途は限られるが、関数テンプレートはオーバーロードすることが可能である。

[演習-SFINAEを利用しない関数テンプレートの特殊化によるis_void](~~~)

#### is_void_sの実装
クラステンプレートの特殊化を使用したis_void_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #1:0 begin
```

is_void_fと同様に単純なので解説は不要だろう。これらの単体テストは下記のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #1:2 begin -1
```

[演習-SFINAEを利用しないクラステンプレートの特殊化によるis_void](~~~)

#### is_void_sfinae_fの実装
[SFINAE](---)を使用した関数テンプレートis_void_sfinae_fの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #2:0 begin
```

関数テンプレートである2つのis_void_sfinae_f_detectorのオーバーロードにSFINAEを使用している。

1つ目のis_void_sfinae_f_detectorでは、

| T       | t = v の診断(コンパイル) |
|---------|--------------------------|
| == void | well-formed              |
| != void | ill-formed               |

であるため、Tがvoidの時のみ[name lookup](---)の対象になる。

2つ目のis_void_sfinae_f_detectorでは、

| T       | sizeof(T)の診断(コンパイル) |
|---------|-----------------------------|
| == void | ill-formed                  |
| != void | well-formed                 |

であるため、Tが非voidの時のみ[name lookup](---)の対象になる。

is_void_sfinae_fはこの性質を利用し、

* T == voidの場合、1つ目のis_void_sfinae_f_detectorが選択され、戻り値はtrue
* T != voidの場合、2つ目のis_void_sfinae_f_detectorが選択され、戻り値はfalse

となる。念のため単体テストを示すと下記のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #2:1 begin -1
```

一般にファイル外部に公開するテンプレートは、
コンパイルの都合上ヘッダファイルにその全実装を記述することになる。
これは、本来外部公開すべきでない実装の詳細である
is_void_sfinae_f_detectorのようなテンプレートに関しては大変都合が悪い。
というのは、外部から使用されたくない実装の詳細が使われてしまうことがあり得るからである。
上記の例では、こういうことに備え
「これは外部非公開である」ということを示す名前空間Inner\_
(「[スコープ|名前空間](~~~)」参照)
を導入した。

関数テンプレートはクラステンプレート内にも定義することができるため、
is_void_sfinae_fは下記のように実装することも可能である。この場合、名前空間Inner\_は不要になる。

```cpp
    // @@@ example/template/is_void_ut.cpp #3:0 begin
```

```cpp
    // @@@ example/template/is_void_ut.cpp #3:1 begin -1
```

[演習-SFINAEを利用した関数テンプレートの特殊化によるis_void](~~~)

#### is_void_sfinae_sの実装
[SFINAE](---)を使用したクラステンプレートis_void_sfinae_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #4:0 begin
    // @@@ example/template/is_void_ut.cpp #4:1 begin
    // @@@ example/template/is_void_ut.cpp #4:2 begin
```

1つ目のis_void_sfinae_sはプライマリテンプレートである。
is_void_sfinae_sの特殊化が[name lookup](---)の対象の中に見つからなかった場合、
これが使われる。

2つ目のis_void_sfinae_sは、上記を抜粋した下記のコード

```cpp
    // @@@ example/template/is_void_ut.cpp #4:1 begin -1 1
```

がT == voidの時のみ、well-formedになり、このテンプレートは下記のようにインスタンス化される。

```cpp
    struct is_void_sfinae_s<void, void*&>
```

この形状はプライマリテンプレートの

* 第1パラメータにvoidを与える
* 第2パラメータには何も与えない(デフォルトのまま)

とした場合の、つまりプライマリテンプレートを

```cpp
    struct is_void_sfinae_s<void>   // プライマリテンプレート
```

としてインスタンス化した場合と一致する。
プライマリと特殊化が一致した場合、特殊化されたものがname lookupで選択される。

T != voidの場合、 2つ目のis_void_sfinae_sはill-formedになり、name lookupの対象から外れるため、
プライマリが選択される。

以上をまとめると、

| T       | is_void_sfinae_sの基底クラス |
|---------|------------------------------|
| == void | std::true_type               |
| != void | std::false_type              |

となる。以下の単体テストによって、このことを確かめることができる。

```cpp
    // @@@ example/template/is_void_ut.cpp #4:3 begin -1
```

上記コードのように「プライマリテンプレートのデフォルトパラメータ」と、

```cpp
    // @@@ example/template/is_void_ut.cpp #4:1 begin -1 1
```

が「well-formedであった場合に生成される型」が一致することを利用した静的ディスパッチは、
SFINAEとクラステンプレートの特殊化を組み合わせたメタ関数の典型的な実装パターンである。
ただし、一般にはill-formedを起こすためにst::enable_ifを使うことが多いため、
「[is_void_ena_sの実装](---)」でその例を示す。

[演習-SFINAEを利用したクラステンプレートの特殊化によるis_void](~~~)

#### is_void_ena_sの実装
[std::enable_if](---)による[SFINAE](---)とクラステンプレートの特殊化を使用した
is_void_ena_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #5:0 begin
    // @@@ example/template/is_void_ut.cpp #5:1 begin
    // @@@ example/template/is_void_ut.cpp #5:2 begin
```

この例では、「[is_void_sfinae_sの実装](---)」の

```cpp
    // @@@ example/template/is_void_ut.cpp #4:1 begin -1 1
```

で示したSFINAEの処理を上記を抜粋した下記のコード

```cpp
    // @@@ example/template/is_void_ut.cpp #5:1 begin -1 1
```

で行っている。
std::enable_ifの値パラメータis_void_f\<T>()は、「[is_void_fの実装](---)」で示したものである。

単体テストは、「[is_void_sfinae_sの実装](---)」で示したものとほぼ同様で、以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #5:3 begin -1
```


#### is_void_cond_sの実装
[std::conditional](---)と関数テンプレートの特殊化を使用したis_void_cond_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_void_ut.cpp #6:0 begin
```

std::conditionalの値パラメータis_void_f\<T>()は、「[is_void_fの実装](---)」で示したものである。
この例では、SFINAEもクラステンプレートの特殊化も使用していないが、
下記単体テストからわかる通り、「[is_void_sfinae_sの実装](---)」と同じ機能を備えている。

```cpp
    // @@@ example/template/is_void_ut.cpp #6:1 begin -1
```


### is_same_xxxの実装
ここではstd::is_same\<T, U>に似た、
以下のような仕様を持ついくつかのテンプレートis_same_xxxの実装を考える。

|テンプレートパラメータ|戻り値            |
|----------------------|------------------|
|T == U                |true              |
|T != U                |false             |

それぞれのis_same_xxxは下記テーブルで示された言語機能を使用して実装する。

|is_same_xxx      |実装方法                                               |
|-----------------|-------------------------------------------------------|
|is_same_f        |関数テンプレートのオーバーロード                       |
|is_same_v        |定数テンプレートの特殊化                               |
|is_same_s        |クラステンプレートの特殊化                             |
|is_same_sfinae_f |SFINAEと関数テンプレート/関数のオーバーロード          |
|is_same_sfinae_s |SFINAEとクラステンプレートの特殊化                     |
|is_same_templ    |テンプレートテンプレートパラメータ                     |
|IsSameSomeOf     |パラメータパックと再帰                                 |

#### is_same_fの実装
関数テンプレートのオーバーロードを用いたis_same_fの実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #0:0 begin
```

すでに述べたように関数テンプレートの部分特殊化は言語仕様として認められておらず、

```cpp
    // @@@ example/template/is_same_ut.cpp #0:1 begin
```

上記のようなのようなコードは、以下のようなコンパイルエラーになる
(g++/clang++のような優れたコンパイラを使えば、
以下のメッセージのように簡単に問題点が理解できることもある)。

```
    is_same_ut.cpp:35:32: error: non-class, non-variable partial specialization ‘
                                          is_same_f<T, T>’ is not allowed
        35 | constexpr bool is_same_f<T, T>()
```

関数テンプレートは部分特殊化が出来ない代わりに、
同じ識別子を持つ関数や関数テンプレートとのオーバーロードができる。関数とのオーバーロードの場合、
is_same_f_helper\<T>()のようなテンプレートパラメータを直接使用した静的ディスパッチが出来ないため、
常に型推測によるディスパッチが必要になる。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #0:2 begin -1
```

#### is_same_vの実装
定数テンプレートの特殊化を用いたis_same_vの実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #1:0 begin
```

単純であるため、解説は不要だろう。 単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #1:1 begin -1
```


#### is_same_sの実装
クラステンプレートの特殊化を用いたis_same_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #2:0 begin
```

「[is_same_vの実装](---)」と同様に単純であるため、解説は不要だろう。 単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #2:1 begin -1
```


#### is_same_sfinae_fの実装
SFINAEと関数テンプレート/関数のオーバーロードを用いたis_same_sfinae_f実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #3:0 begin
    // @@@ example/template/is_same_ut.cpp #3:1 begin
    // @@@ example/template/is_same_ut.cpp #3:2 begin
```

上記の抜粋である下記コードのコメントで示したように、

```cpp
    // @@@ example/template/is_same_ut.cpp #3:1 begin -1 1
```

T != Uの場合、この関数テンプレートはill-formedとなりname lookupの対象ではなくなる。
その結果、関数is_same_sfinae_f_detectorが選択される。
省略記号"..."(ellipsis)を引数とする関数は、そのオーバーロード群の中での最後の選択となるため、
T == Uの場合は、関数テンプレートis_same_sfinae_f_detectorが選択される。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #3:3 begin -1
```


#### is_same_sfinae_sの実装
SFINAEとクラステンプレートの特殊化を用いたis_same_sfinae_sの実装は以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #4:0 begin
```

「[is_void_sfinae_sの実装](---)」とほぼ同様であるため、解説は不要だろう。 
単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #4:1 begin -1
```


#### is_same_templの実装
例えば、std::stringとstd::basic_string\<T>が同じもしくは違う型であることを確認するためには、
すでに示したis_same_sを使用し、

```cpp
    // @@@ example/template/is_same_ut.cpp #5:1 begin -1
```

のようにすればよいが、
以下に示したコードのようにテンプレートテンプレートパラメータを使うことでも実装できる。

```cpp
    // @@@ example/template/is_same_ut.cpp #5:0 begin
```

上記のis_same_templは、第2引数にクラステンプレート、
第3引数以降にそのクラステンプレートの1個以上の引数を取ることができる。
使用例を兼ねた単体テストは以下のようになる。

```cpp
    // @@@ example/template/is_same_ut.cpp #5:2 begin -1
```

これを応用したエイリアステンプレート

```cpp
    // @@@ example/template/is_same_ut.cpp #5:3 begin
```

は与えられたテンプレートパラメータがstd::stringを生成するかどうかを判定することができる。

```cpp
    // @@@ example/template/is_same_ut.cpp #5:4 begin -1
```

[演習-テンプレートテンプレートパラメータ](~~~)

#### IsSameSomeOfの実装
IsSameSomeOfはこれまでの例とは少々異なり、

* 第1パラメータが第2パラメータ以降で指定された型の
    * どれかと同じであれば、std::true_typeから派生する
    * どれとも違えば、std::false_typeから派生する
* 2つの型の同一性の判定にはstd::is_sameを使用する
* 汎用性が高いため名前空間Nstdで定義し、命名はキャメルにする

のような特徴のを持つ。
このようなIsSameSomeOfをパラメータパックと再帰を使用して実装すると以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits.h #0:0 begin
```

IsSameSomeOfは、TがUsのいずれかと一致するかどうかの処理をInner\_::is_same_some_ofに移譲する。

Usが1つだった場合、特殊化されたInner\_::is_same_some_ofのvalueがstd::is_same::valueで初期化される。
Usが複数だった場合、 プライマリのInner\_::is_same_some_ofは、
IsSameSomeOfから渡されたパラメータパックUsを、UとパラメータパックUsに分割後、
TとUをstd::is_sameで比較し、

* 同じ場合、valueはtrueで初期化される
* 違う場合、valueは再帰的に読み出されたInner\_::is_same_some_of\<T, Us...>::valueで初期化される

再帰的なInner\_::is_same_some_of::valueの読み出しは、
IsSameSomeOfが受け取ったパラメータパックをひとつずつ左シフトしながら、
それが1つになるまで(特殊化されたInner\_::is_same_some_ofが使われるまで)、続けられる。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp #0:0 begin -1
```

[演習-テンプレートパラメータを可変長にしたstd::is_same](~~~)

### AreConvertibleXxxの実装
std::is_convertible\<FROM, TO>は、

* 型FROMが型TOに変換できる場合、std::true_typeから派生する
* 型FROMが型TOに変換できない場合、std::false_typeから派生する

のような仕様を持つテンプレートである。

ここでは、

* std::is_convertibleを複数のFROMが指定できるように拡張したNstd::AreConvertible
* 縮小無しでの型変換ができるかどうかを判定するAreConvertibleWithoutNarrowConv

の実装を考える。


#### AreConvertibleの実装
AreConvertibleの実装は以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits.h #1:0 begin
```


「[IsSameSomeOfの実装](---)」のコードパターンとほぼ同様であるため、解説は不要だろうが、

* パラメータパックの都合上、TOとFROMのパラメータの位置がstd::is_convertibleとは逆になる
* IsSameSomeOfでは条件の一つがtrueであればIsSameSomeOf::valueがtrueとなるが、
  AreConvertibleでは全条件がtrueとならない限り、AreConvertible::valueがtrueとならない

ので注意が必要である。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp #1:0 begin -1
```


#### AreConvertibleWithoutNarrowConvの実装
縮小無しの型変換ができるかどうかを判定するAreConvertibleWithoutNarrowConvは、
AreConvertibleと同じように実装できるが、
その場合、AreConvertibleに対してstd::is_convertibleが必要になったように、
AreConvertibleWithoutNarrowConvに対しis_convertible_without_narrow_convが必要になる。

縮小無しでFROMからTOへの型変換ができるかどうかを判定するis_convertible_without_narrow_convは、
SFINAEと関数テンプレート/関数のオーバーライドを使用し以下のように実装できる。

```cpp
    // @@@ example/template/nstd_type_traits.h #2:0 begin
    // @@@ example/template/nstd_type_traits.h #2:1 begin
    // @@@ example/template/nstd_type_traits.h #2:2 begin
```

AreConvertibleWithoutNarrowConvはNstdで定義するため、その内部のみで用いる
is_convertible_without_narrow_convはNstd::Inner\_で定義している。

上記を抜粋した下記のコードは「縮小型変換を発生さる{}による初期化は[ill-formed](---)になる」
ことをSFINAEに利用している。

```cpp
    // @@@ example/template/nstd_type_traits.h #2:1 begin -2 1
```

単体テストは以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp #2:0 begin -1
```

is_convertible_without_narrow_convを利用したAreConvertibleWithoutNarrowConv
の実装は以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits.h #2:3 begin
```

単体テストは以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp #2:1 begin -1
```

### 関数の存在の診断
Nstdライブラリの開発には関数の存在の診断が欠かせない。
例えば、

* テンプレートパラメータに特定のメンバ関数がある場合、特殊化を作る
* テンプレートパラメータに範囲for文が適用できる場合にのみoperator<<を適用する
* テンプレートパラメータに適用できるoperator<<がすでにあった場合、
  自作operator<<を不活性化する

等、応用範囲は多岐にわたる。
ここでは、上記の場合分けを可能とするようなメタ関数に必要なテクニックや、
それらを使用したNstdのメタ関数の実装を下記のように示す。

* テンプレートパラメータである型が、メンバ関数void func()を持つかどうかの診断について、
  次の表のように実装を示す。

|メタ関数名                |メタ関数の目的                                          |
|--------------------------|--------------------------------------------------------|
|exists_void_func_sfinae_f |メンバ関数void func()を持つかどうかの判断               |
|exists_void_func_sfinae_s |同上                                                    |
|exists_void_func_sfinae_s2|同上                                                    |

* テンプレートパラメータに範囲for文ができるかどうかの診断について、
  次の表のように実装を示す。

|メタ関数名                 |メタ関数の目的                                               |
|---------------------------|-------------------------------------------------------------|
|exists_begin               |std::begin(T)が存在するか否かの診断                          |
|exists_end                 |std::end(T)が存在するか否かの診断                            |
|IsRange                    |T const& tの時に、for(auto const& : t)ができるかどうかの診断 |

* テンプレートパラメータにoperator<<(put toと発音する)ができるかどうかの診断について、
  次の表のように実装を示す。

|メタ関数名                 |メタ関数の目的                                         |
|---------------------------|-------------------------------------------------------|
|exists_put_to_as_member    |std::ostream::operator<<(T)が存在するか否かの診断      |
|exists_put_to_as_non_member|operator<<(std::ostream&, T)が存在するか否かの診断     |
|ExistsPutTo                |std::ostream& << Tができるかどうかの診断               |

* テンプレートパラメータがT[N]やC\<T>の形式である時のTに、
  operator<<が適用できるかの診断については、Tの型を取り出す必要がある。
  そのようなメタ関数ValueTypeの実装を示す。


#### exists_void_func_sfinae_fの実装
「テンプレートパラメータである型が、メンバ関数void func()を持つかどうかを診断する」
exists_void_func_sfinae_f
のSFINAEと関数テンプレート/関数のオーバーロードを用いた実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #0:0 begin
    // @@@ example/template/exists_func_ut.cpp #0:1 begin
    // @@@ example/template/exists_func_ut.cpp #0:2 begin
```

decltypeの中での関数呼び出しは、実際には呼び出されず関数の戻り値の型になる。
上記の抜粋である下記のコードはこの性質を利用してSFINAEによる静的ディスパッチを行っている。

```cpp
    // @@@ example/template/exists_func_ut.cpp #0:1 begin -1 1
```

単体テストは以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #0:3 begin
```

```cpp
    // @@@ example/template/exists_func_ut.cpp #0:4 begin -1
```


#### exists_void_func_sfinae_sの実装
「テンプレートパラメータである型が、メンバ関数void func()を持つかどうかを診断」する
exists_void_func_sfinae_s
のSFINAEとクラステンプレートの特殊化を用いた実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #1:0 begin
```

exists_void_func_sfinae_fとほぼ等しいSFINAEを利用したクラステンプレートの特殊化により、
静的ディスパッチを行っている。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #1:1 begin -1
```


#### exists_void_func_sfinae_s2の実装
exists_void_func_sfinae_sとほぼ同様の仕様を持つexists_void_func_sfinae_s2の

* SFINAE
* メンバ関数テンプレート/メンバ関数のオーバーロード
* メンバ関数へのポインタ

を用いた実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #2:0 begin
    // @@@ example/template/exists_func_ut.cpp #2:1 begin
    // @@@ example/template/exists_func_ut.cpp #2:2 begin
```

前2例とは異なり、上記の抜粋である下記コードのように、
メンバ関数へのポインタを使用しSFINAEを実装している。

```cpp
    // @@@ example/template/exists_func_ut.cpp #2:1 begin -1
```

あまり応用範囲が広くない方法ではあるが、
decltypeを使っていないのでC++03コンパイラにも受け入れられるメリットがある。

exists_void_func_sfinae_fと同じテスト用クラスを用いた単体テストは以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #2:3 begin -1
```

[演習-メンバ関数の存在の診断](~~~)

#### exists_begin/exsits_endの実装
「テンプレートパラメータTに対して、std::begin(T)が存在するか否かの診断」をするexists_beginの実装は、
「[exists_void_func_sfinae_sの実装](---)」
で用いたパターンのメンバ関数を非メンバ関数に置き換えて使えば以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #3:0 begin
```

上記で使用したstd::void_tは、テンプレートパラメータが

* [ill-formed](---)ならばill-formedになる
* [well-formed](---)ならvoidを生成する

テンプレートである。

下記単体テストでは問題ないように見えるが、

```cpp
    // @@@ example/template/exists_func_ut.cpp #3:1 begin -1
```

下記の単体テストはstatic_assertがフェールするためコンパイルできない。

```cpp
    // @@@ example/template/exists_func_ut.cpp #3:2 begin -1
```

理由は、

```cpp
    std::declval<int[3]>())
```

の戻り型が配列型の[expressionと値カテゴリ|rvalue](---)である"int (&&) [3]"となり、
これに対応するstd::beginが定義されていないためである。

これに対処する方法方はいくつかあるが、
すべての配列は常にstd::beginの引数になれることに気づけば、
テンプレートパラメータが配列か否かで場合分けしたクラステンプレートの特殊化を使い、
下記のように実装できることにも気付けるだろう。

```cpp
    // @@@ example/template/exists_func_ut.cpp #4:0 begin
```

2個目のexists_beginはTが配列でない場合、
3個目のexists_beginはTが配列ある場合にそれぞれが対応しているが、複雑すぎて何とも醜い。
ということで、このコードは却下して、別のアイデアを試そう。

テンプレートパラメータが配列である場合でも、
そのオブジェクトが[lvalue](---)(この例ではint (&)[3])であれば、
std::beginはそのオブジェクトを使用できるので、
decltype内で使用できるlvalueのT型オブジェクトを生成できれば、
と考えれば下記のような実装を思いつくだろう。

```cpp
    // @@@ example/template/nstd_type_traits.h #3:0 begin
```

十分にシンプルなのでこれを採用し、exists_endも同様に実装する。

```cpp
    // @@@ example/template/nstd_type_traits.h #3:1 begin
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp #3:0 begin -1
```


#### IsRangeの実装
[範囲for文](https://cpprefjp.github.io/lang/cpp11/range_based_for.html)
文の":"の後ろにT型オブジェクトが指定できる要件は、

* std::begin(T)、std::end(T)がTのイテレータであるITOR型のオブジェクトを返す
* std::begin(T)が返すITORオブジェクトはTが保持する先頭の要素を指す
* std::end(T)が返すITORオブジェクトはTが保持する最後の要素の次を指す
* ++ITORによりTが保持する全要素にアクセスできる

ようなことである。多くの要件はセマンティクス的なものであり、
メタ関数で診断できることは前項で見たようなstd::begin(T)、std::end(T)の可否のみであると考えれば、
IsRangeの実装は以下のようになる。

```cpp
    // @@@ example/template/nstd_type_traits.h #3:2 begin
```

なお、上記のコードでは、exists_begin/exsits_endは、IsRangeの実装の詳細であるため、
名前空間Inner\_で宣言している。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp #4:0 begin -1
```

[演習-範囲for文のオペランドになれるかどうかの診断](~~~)

#### exists_put_to_as_memberの実装
std::ostreamのメンバ関数operator<<の戻り型はstd::ostream&であるため、
exists_put_to_as_memberの実装は以下のようになる("<<"は英語で"put to"と発音する)。

```cpp
    // @@@ example/template/exists_func_ut.cpp #5:0 begin
```

「[exists_void_func_sfinae_fの実装](---)」と同様のパターンを使用したので解説は不要だろう。

単体テストは以下のようになる。

```cpp
    // @@@ example/template/test_class.h #0:0 begin
```

```cpp
    // @@@ example/template/exists_func_ut.cpp #5:1 begin -1
    // @@@ example/template/exists_func_ut.cpp #5:2 begin -1
```

やや驚きなのは、上記の抜粋である下記コードがコンパイルできることである。

```cpp
    // @@@ example/template/exists_func_ut.cpp #5:2 begin -1 1
```

これは、

```cpp
    std::ostream& std::ostream::operator<<(void const*)
```

が定義されているため、配列がポインタに変換されてこのメンバ関数にバインドした結果である。


#### exists_put_to_as_non_memberの実装
exists_put_to_as_non_memberの実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #5:3 begin
```

「[exists_begin/exsits_endの実装](---)」と「[exists_put_to_as_memberの実装](---)」
で使用したパターンを混合しただけなので解説や単体テストは省略する。


#### ExistsPutToの実装
テンプレートパラメータT、T型オブジェクトtに対して、
std::ostream << tができるかどうかを判断するExistsPutToの実装は以下のようになる。

```cpp
    // @@@ example/template/exists_func_ut.cpp #5:4 begin
```

「[IsRangeの実装](---)」に影響されて、一旦このように実装したが、先に書いた通り、
そもそものExistsPutToの役割はstd::ostream << tができるかどうかの診断であることを思い出せば、
下記のように、もっとシンプルに実装できることに気づくだろう。

```cpp
    // @@@ example/template/nstd_type_traits.h #4:0 begin
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp #5:0 begin -1
```

#### ValueTypeの実装
下記で示す通り、

```cpp
    // @@@ example/template/nstd_type_traits_ut.cpp #5:1 begin
```

型Xが与えれ、その形式が、

* クラステンプレートCとその型パラメータTにより、C\<T>
* 型Tと定数整数Nにより、T[N]

のような場合、ExistsPutToV\<X>がtrueであっても、ExistsPutToV\<T>の真偽はわからない。
従って上記のようなTに対して、ExistsPutToV\<T>がtrueかどうかを診断するためには、
XからTを導出することが必要になる。ここでは、そのようなメタ関数ValueTypeの実装を考える。
このValueTypeは上記のX、Tに対して、

```cpp
    std::is_same<ValueType<X>::type, T>::value == true
```

となるような機能を持たなければならないことは明らかだろう。
その他の機能については実装しながら決定していく。

一見、難しそうなテンプレートを作るコツは、条件を絞って少しずつ作っていくことである。
いきなり大量のテンプレートを書いてしまうと、
その何十倍ものコンパイルエラーに打ちのめされること必至である。

ということで、まずは、1次元の配列に対してのみ動作するValueTypeの実装を示す
(下記で使用するstd::remove_extent_t\<T>は、テンプレートパラメータが配列だった場合に、
その次元を一つだけ除去するメタ関数である)。

```cpp
    // @@@ example/template/value_type_ut.cpp #0:0 begin
```

このコードは問題なく動作するが、下記の通り、2次元配列に対するValueType::typeは1次元配列となる。

```cpp
    // @@@ example/template/value_type_ut.cpp #0:1 begin -1
```

これを多次元配列に拡張する前に、配列の次元をValueType::Nestで返す機能を追加することにすると、
コードは下記のようになるだろう。

```cpp
    // @@@ example/template/value_type_ut.cpp #1:0 begin
```

動作は下記のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp #1:1 begin -1
```

ここで、下記のような仕様をもつValueType::type_n\<N>を考える。

```
    ValueType<int[1][2][3]>::type_n<0>が表す型は、int[1][2][3]
    ValueType<int[1][2][3]>::type_n<1>が表す型は、int[2][3]
    ValueType<int[1][2][3]>::type_n<2>が表す型は、int[3]
    ValueType<int[1][2][3]>::type_n<3>が表す型は、int
```

ValueType::type_n\<N>は玉ねぎの皮を一枚ずつむくようなメンバエイリアステンプレートになる。
プライマリの実装は以下のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp #2:0 begin
```

Nが非0の場合、Value::type_n\<N>はvoidになる仕様にした。

配列に対する特殊化は以下のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp #2:1 begin
```

下記コードのコンパイル時の展開を説明することで、上記の解説を行う。

```cpp
    // @@@ example/template/value_type_ut.cpp #2:2 begin -2
```

1. ValueTypeのテンプレートパラメータが配列であるため、配列への特殊化であるValueTypeが選択され、
   下記の疑似コードのように展開される。

```cpp
    ValueType<int[1][2][3], void> {
        using type = int[2][3];
        static constexpr size_t Nest = ...
        using type_n = ConditionalValueTypeN<int[1][2][3], 3>::type;
    };
```

2. ConditionalValueTypeNは下記のように展開される。
   なお、下記のコードの中のtype_n前で使われているキーワードtemplateは、
   「外部のクラステンプレートのメンバテンプレートにアクセスする」際に必要になる記法である。

```cpp
    struct ConditionalValueTypeN<int[1][2][3], 3> {
        using type = typename std::conditional<
            true,   // ValueType<int[1][2][3]>::Nest == 3であるためtrue
            ValueType<ValueType<int[1][2][3]>::type>::template type_n<3 - 1>,
            int[1][2][3]>::type;
    };
```

3.  ValueType\<int\[1]\[2]\[3]>::typeは一枚皮をむいたint\[2]\[3]なので、
    上記はさらに下記のように展開される。

```cpp
    struct ConditionalValueTypeN<int[1][2][3], 3> {
        using type = ValueType<int[2][3]>::template type_n<2>;
    };
```

4. ConditionalValueTypeN\<int\[1]\[2]\[3], 3>::typeを展開するため、
   ValueType\<int\[2]\[3]>::template type_n<2>の展開が上記1 - 3のように繰り返される。
   この繰り返しはN == 0になるまで続く。

5. 3回の皮むきによりN == 0となる。
   この時点で、下記の特殊化が選択されるため再帰は終了し、ConditionalValueTypeN<>::typeはintとなる。

```cpp
    struct ConditionalValueTypeN<int, 0> {
        using type = int;
    };
```

6. 1 - 5により最終的には下記のように展開される。

```cpp
    ValueType<int[1][2][3]> {
        using type = int[2][3];
        static constexpr size_t Nest = 3;
        using type_n = int;
    };
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp #2:3 begin -2
```

また、ValueType::NestとValueType::type_n<>の関係に注目すれば、
上記エイリアスTに対して下記が成立する。

```cpp
    // @@@ example/template/value_type_ut.cpp #2:4 begin -2
```


このテンプレートにコンテナが渡された時の特殊化を与えることができればValueTypeは完成するが、
その前に名前の整理をした方が良いため、下記のような変更を行う。

* この例では、typeは配列が直接保持する型を表すが、
  この名前は慣例的にメタ関数の戻り型を表すことが多いため、現在の仕様では混乱を招く。
  また名は体を表す方が良いため、typeを改めtype_directとする。
* ValueTypeの結果は、上記のようにNestとtype_nの組み合わせで得られるが、このままでは使い勝手が悪い。
  慣例に従いこれをtypeとする。
* ConditionalValueTypeNは実装の詳細であるため外部から使われたくない。
  これまで通り、名前空間Inner\_で定義し、名前を小文字と\_で生成する。

これによりValueTypeは下記のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp #3:0 begin
```

準備は整ったので上記のValueTypeに下記のようなコンテナ用特殊化を追加する。

```cpp
    // @@@ example/template/value_type_ut.cpp #4:0 begin
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/value_type_ut.cpp #4:1 begin -2
```

最初のValueTypeには、その単純さとは不釣り合いな、やや複雑なSFINAE用のコードを記述をしたが、
ここまで来ればその理由は明らかだろう。
今回のように限定的な機能を作ってから、一般化して行く開発スタイルでも、
静的ディスパッチにはSFINAE等の汎用的手法を使った方が後の修正が少なく済むことが多い。
一方で、完成時にその静的ディスパッチが不要に複雑であると気づいた場合は、
リファクタリングを行い、コードを程よいレベルに留めなければならないことは言うまでもない。

ValueTypeの開発はまだ終わらない。静的ディスパッチは最初のカンが当り修正の必要はないと思うが、
「配列用特殊化とコンテナ用特殊化のほとんどがコードクローンになっている」という問題がある。
この程度のクローンは問題のないレベルであるとも言えるが、演習のため修正する。
また、合わせてTが配列かどうかを示すための定数IsBuiltinArrayも追加すると下記のようなコードになる。

```cpp
    // @@@ example/template/nstd_type_traits.h #5:0 begin
```

[演習-配列の長さの取り出し](~~~)
[演習-配列の次元の取り出し](~~~)

## Nstdライブラリの開発2
ここでは予定していた通りSafeArray2を開発し、その後Nstdに必要なライブラリの開発を続ける。


### SafeArray2の開発
「[安全な配列型コンテナ](---)」で断念したSafeArray2の開発を再開する前に、
SafeArray2の要件をまとめると、

* std::arrayを基底クラスとする
* operator[]に範囲チェックを行う
* SafeArrayでのパラメータパックによる初期化機能はそのまま残す
* SafeArrayではできなかった縮小型変換が起こる初期化にも対応する
* 新規要件として、
  縮小型変換により初期化されたかどうかを示すメンバ関数InitializedWithNarrowConv()を持つ。

となる。この要件を満たすためには、SafeArrayが

```cpp
    // @@@ example/template/safe_vector_ut.cpp #3:1 begin -1
```

で行っていた初期化を、SafeArray2では、
「縮小型変換が起こるか否かによる場合分けを行い、
それぞれの場合に対応するコンストラクタテンプレートによって初期化」
するようにすれば良いことがわかる。

パラメータパックによるコンストラクタのシグネチャは上記した一種類しかないため、
関数のシグネチャの差異よるオーバーロードは使えない。
とすれば、テンプレートパラメータの型の差異によるオーバーロードを使うしか方法がない。
縮小型変換が起こるか否かの場合分けはSFINAEで実現させることができる。
という風な思考の変遷により以下のコードにたどり着く。


```cpp
    // @@@ example/template/safe_vector_ut.cpp #4:0 begin
    // @@@ example/template/safe_vector_ut.cpp #4:1 begin
    // @@@ example/template/safe_vector_ut.cpp #4:2 begin
    // @@@ example/template/safe_vector_ut.cpp #4:3 begin
    // @@@ example/template/safe_vector_ut.cpp #4:4 begin
    // @@@ example/template/safe_vector_ut.cpp #4:5 begin
```

下記のようなコードでのコンストラクタ呼び出しには、

```cpp
    // @@@ example/template/safe_vector_ut.cpp #4:6 begin -2
```

上記の抜粋である下記のコンストラクタが置換失敗により排除される(SFINAE)。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #4:2 begin -1
    // @@@ example/template/safe_vector_ut.cpp #4:3 begin -1
    // @@@ example/template/safe_vector_ut.cpp #4:4 begin -1
```

従って、マッチするコンストラクタは

```cpp
    // @@@ example/template/safe_vector_ut.cpp #4:1 begin -1
```

のみとなり、無事にコンパイルが成功し、下記の単体テストもパスする。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #4:6 begin -2
    // @@@ example/template/safe_vector_ut.cpp #4:7 begin -2
```

下記の単体テストの場合、SFINAEにより、先述の例とは逆のコンストラクタが選択され、
コンパイルも単体テストもパスする。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #4:8 begin -2
```

ここで紹介した2つのコンストラクタテンプレートの最後のパラメータには、かなりの違和感があるだろうが、
引数や戻り値に制限の多いコンストラクタテンプレートでSFINAEを起こすためには、
このような記述が必要になる。

なお、2つ目のコンストラクタテンプレートの中で使用した下記のコードは、
パラメータパックで与えられた全引数をそれぞれにT型オブジェクトに変換するための記法である。

```cpp
    // @@@ example/template/safe_vector_ut.cpp #4:3 begin -2 1
```

これにより、std::array\<T, N>のstd::initializer_listによる初期化が縮小変換を検出しなくなる。


### Nstd::SafeIndexの開発
「[安全なvector](---)」、「[安全な配列型コンテナ](---)」等の中で、

* Nstd::SafeVector
* Nstd::SafeString
* Nstd::SafeArray

を定義した。これらは少しだけランタイム速度を犠牲にすることで、
安全な(未定義動作を起こさない)インデックスアクセスを保障するため、
一般のソフトウェア開発にも有用であると思われるが、コードクローンして作ったため、
リファクタリングを行う必要がある。

まずは、Nstd::SafeVectorとNstd::SafeStringの統一を考える。

std::stringは、実際にはstd::basic_string\<char>のエイリアスであることに注目すれば、
Nstd::SafeStringの基底クラスはstd::basic_string\<char>であることがわかる。
この形式は、std::vector\<T>と同形であるため、
Nstd::SafeVectorとNstd::SafeStringの共通コードはテンプレートテンプレートパラメータ
(「[is_same_templの実装](---)」参照)を使用し下記のように書ける。


```cpp
    // @@@ example/template/nstd_safe_index.h #0:0 begin
```

このコードの使用例を兼ねた単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp #0:0 begin -1
```

このままでは使いづらいので下記のようにエイリアスを使い、元のテンプレートと同じ名前を与える。

```cpp
    // @@@ example/template/nstd_safe_index.h #1:0 begin
```

このコードの単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp #1:0 begin -1
```

これで、Nstd::SafeVectorとNstd::SafeStringは統一できたので、
Nstd::SafeIndexにNstd::SafeArrayの実装が取り込めれば、リファクタリングは終了となるが、
残念ながら、下記のコードはコンパイルできない。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp #0:1 begin -1
```

理由は、パラメータパックにはそのすべてに型を指定するか、そのすべてに値を指定しなければならず、
上記のコードのような型と値の混在が許されていないからである。

値を型に変換する[std::integral_constant](---)を使用し、この問題を解決できる。
std::arrayから派生した下記のStdArrayLikeは、std::integral_constant::valueから値を取り出し、
基底クラスstd::arrayの第2テンプレートパラメータとする。
この仕組みにより、StdArrayLikeは、
Nstd::SafeIndexのテンプレートテンプレートパラメータとして使用できるようになる。

```cpp
    // @@@ example/template/nstd_safe_index.h #2:0 begin
```

まずは、このコードの使用例を兼ねた単体テストを下記に示す。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp #2:0 begin -1
```

これを使えば、下記のような記述が可能となる。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp #2:1 begin -1
```

このままでは使いづらいのでNstd::SafeVector、Nstd::Stringと同様にエイリアスを使えば、
下記のようになる。

```cpp
    // @@@ example/template/nstd_safe_index.h #3:0 begin
```

このコードの単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp #1:1 begin -1
```

これにより、当初の目的であったコードクローンの除去が完了した。
この効果により、下記に示したような拡張もコードクローンせずに簡単に行えるようになった。

```cpp
    // @@@ example/template/nstd_safe_index.h #4:0 begin
```

```cpp
    // @@@ example/template/nstd_safe_index_ut.cpp #1:2 begin -1
```

### Nstd::SafeIndexのoperator\<\<の開発
ここでは、Nstd::SafeIndexのoperator\<\<の開発を行う。

他のoperator\<\<との間で定義が曖昧にならないようにするためには、
テンプレートテンプレートパラメータを使って以下のようにすることが考えられる。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp #0:0 begin
```

以下の単体テストで動作確認する。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp #1:0 begin -1
```

ここまではうまく行っているが、以下の単体テストによりバグが発覚する。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp #1:1 begin -1
```

この原因は、Nstd::SafeStringオブジェクトに対して、std::operator<<が使用されなかったからである。

「[メタ関数のテクニック](---)」で紹介したSFINAEにより、この問題は下記のように対処できる。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp #2:0 begin
```

これにより先ほど問題が発生した単体テストも下記のようにパスする。

```cpp
    // @@@ example/template/safe_index_put_to_ut.cpp #3:0 begin -2
```


### コンテナ用Nstd::operator\<\<の開発
「[Nstd::SafeIndexのoperator\<\<の開発](---)」で定義したNstd::operator\<\<の構造は、
範囲for文に適用できる配列やstdコンテナにも使えるため、ここではその拡張を考える。

すでに述べたように注意すべきは、

* 使い勝手の良いstd::operator\<\<(例えばchar[N]やstd::stringのoperator\<\<)はそのまま使う
* ほとんど使い物にならないstd::operator\<\<(例えば、int[N]のような配列に対するoperator\<\<(void\*))
  の代わりに、ここで拡張するNstd::operator\<\<を使う

であるため、型Tが新しいNstd::operator\<\<を使用できる条件は、

* Tの型が、以下の条件を満たす
    * T == U[N]であった場合、Uはcharではない
    * std::stringおよびその派生型ではない
* Nstd::ValueType\<T>::typeがoperator\<\<を持つ

となるだろう。この条件を診断するためのメタ関数は以下のようになる。

```cpp
    // @@@ example/template/nstd_put_to.h #0:0 begin
```

ただし、このようなコードはコンパイラのバグによりコンパイルできないことがある。
実際、現在使用中の[g++](---)ではこのコードはコンパイルできず、
上記コードではそのワークアラウンドを行っている。

このような場合、条件分岐に三項演算子を使うことで回避できることが多いが、
ここではg++の問題を明示するためにプリプロセッサ命令を用いた。

このような複雑なメタ関数には単体テストは必須である。

```cpp
    // @@@ example/template/test_class.h #0:0 begin
```

```cpp
    // @@@ example/template/nstd_put_to_ut.cpp #0:0 begin -1
```

以上によりstd::enable_ifの第1引数に渡す値(enable_range_put_to_vはconstexpr)が用意できたので、
Nstd::operator\<\<は下記のように定義できる。

```cpp
    // @@@ example/template/nstd_put_to.h #1:0 begin
```

値表示用のセパレータに","のみを用いるとコンテナや配列が多次元(ValueType::Nest > 2)の場合、
各次元でのデータの判別が難しくなるため、ValueType::Nestの値によってセパレータの種類を変える
range_put_to_sep<>()を用意した。
下記単体テストでわかる通り、この効果により値の構造が見やすくなっている。

まずは、配列の単体テストを示す。

```cpp
    // @@@ example/template/nstd_put_to_ut.cpp #1:0 begin -1
```

次に、コンテナの単体テストを示す。

```cpp
    // @@@ example/template/nstd_put_to_ut.cpp #2:0 begin -1
```

最後に、Nstd::SafeIndexの単体テストを示す。

```cpp
    // @@@ example/template/nstd_put_to_ut.cpp #3:0 begin -1
```

## ログ取得ライブラリの開発2
ログ取得ライブラリでの問題は「Logging名前空間が依存してよい名前空間」に

```cpp
    // @@@ example/template/app_ints.h #0:0 begin
```

のようなコンテナに共通したoperator<<を定義することで解決する。
それは 「[コンテナ用Nstd::operator\<\<の開発](---)」で示したコードそのものであるため、
これを使い、問題を解決したログ取得ライブラリを以下に示す。

```cpp
    // @@@ example/template/logger.h #0:0 begin
    // @@@ example/template/logger.h #0:1 begin
    // @@@ example/template/logger.h #0:2 begin
```

問題のあったコードとの差分は、メンバ関数テンプレートset_innerの

```cpp
    // @@@ example/template/logger.h #0:1 begin -2 1
```

のみである。実際に解決できたことを以下の単体テストで示す。

```cpp
    // @@@ example/template/logger_0_ints_ut.h #0:0 begin -1
```

また、

```cpp
    // @@@ example/template/app_ints.h #1:0 begin
```

のように定義されたクラスも、

```cpp
    // @@@ example/template/app_ints.h #1:1 begin
```

のような型専用のoperator<<があれば、そのオブジェクトのみではなく、
コンテナや配列に対しても下記のようにログ取得が可能となる。

```cpp
    // @@@ example/template/logger_ut.cpp #0:0 begin -1
```

「[Nstdライブラリの開発](---)」で示した依存関係も維持されており、
これでログ取得ライブラリは完成したと言って良いだろう。


## その他のテンプレートテクニック
ここでは、これまでの議論の対象にならなかったテンプレートのテクニックや注意点について記述する。

### ユニバーサルリファレンスとstd::forward
2個の文字列からstd::vector\<std::string>を生成する下記のような関数について考える。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #0:0 begin
```

これは下記のように動作する。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #0:1 begin -1
```

このコードは正しく動作するものの、move代入できず、パフォーマンス問題を引き起こす可能性があるため、
[ユニバーサルリファレンス](---)を使って下記のように書き直した。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #1:0 begin
```

残念ながら、このコードは意図したようには動作せず、下記に示した通り相変わらずmove代入ができない。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #1:1 begin -1
```

この原因は、
「関数が受け取った[expressionと値カテゴリ|rvalue](---)リファレンスは、
その関数から別の関数に受け渡される時に[lvalue](---)リファレンスとして扱われる」からである。

この現象について下記の関数テンプレートを用いて解説を行う。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #1:2 begin
```

下記に示した通り、universal_refとuniversal_ref2のパラメータが同じ型であるとは限らない。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #1:3 begin -1
```

この問題はstd::forwardにより対処できる。これによって改良されたコードを下記に示す。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #2:0 begin
```

下記単体テストが示す通り、rvalueリファレンスはmove代入され、lvalueリファレンスはcopy代入されている。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #2:1 begin -2
```

しかし残念ながら、このコードにも改良すべき点がある。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #2:2 begin -2
```

上記の"b"のような文字列リテラルを引数にした場合、
それをstd::vector\<std::string>::push_backに渡した時に、
"b"はテンポラリオブジェクトstd::string("b")のに変換されてしまう。
std::vector\<std::string>へのオブジェクトの挿入は、文字列リテラルから行うことが出来るため、
このテンポラリオブジェクトの生成は明らかに不要な処理である。

下記は、この対策を施すとともに任意の数の引数を受け取れるように改良したコードである。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #3:0 begin
```

上記の

```cpp
    sizeof...(tails)
```

はパラメータパックの個数を受け取るための記法である。
従ってこのコードではすべてのパラメータパック変数を消費するまでリカーシブコールを続けることになる
(が、このリカーシブコールはコンパイル時に行われるため、実行時の速度低下は起こさない)。

上記の

```cpp
    std::forward<TAIL>(tails)...
```

は、それぞれのパラメータパック変数をstd::forwardに渡した戻り値を、
再びパラメータパックにするための記法である。

このコードは下記の単体テストが示すように正しく動作する
(が、残念ならがテンポラリオブジェクトが生成されていないことを単体テストで証明することはできない)。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #3:1 begin -1
```

ユニバーサルリファレンスはconstにすることができないが
(T const&&はconstな[expressionと値カテゴリ|rvalue](---)リファレンスである)、
ユニバーサルリファレンスが[lvalue](---)リファレンスであった場合は、
constなlvalueリファレンスとして扱うべきである。

従って、下記のようなコードは書くべきではない。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #4:0 begin
```

もしそのようにしてしまえば、
下記単体テストが示すように非constな実引数はmoveされてしまうことになる。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #4:1 begin -1
```

任意の型Tのrvalueのみを引数に取る関数テンプレートを下記のように記述した場合、
すでに述べたように引数はユニバーサルリファレンスとなってしまうため、lvalueにもバインドしてしまう。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #5:0 begin
```

このような場合、下記の記述が必要になる。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #5:1 begin
```

この効果により、下記に示した通りlvalueにはバインドできなくなり、
当初の目的通り、rvalueのみを引数に取る関数テンプレートが定義できたことになる。

```cpp
    // @@@ example/template/universal_ref_ut.cpp #5:2 begin -1
```

なお、ユニバーサルリファレンスは、[リファレンスcollapsing](---)の一機能としても理解できる。

### ジェネリックラムダ
下記のようなクラスとoperator<<があった場合を考える。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp #0:0 begin
```

「[Nstd::SafeIndexの開発](---)」や「[コンテナ用Nstd::operator\<\<の開発](---)」
の成果物との組み合わせの単体テストは下記のように書けるだろう。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp #0:1 begin -2
```

std::sortによるソートができるかどうかのテストは、C++11までは、

```cpp
    // @@@ example/template/generic_lambda_ut.cpp #0:2 begin -2
```

のように書くのが一般的だろう。ラムダ式の引数の型を指定しなければならないのは、
範囲for文でautoが使用出来ること等と比べると見劣りがするが、
C++14からは下記のコードで示した通り引数にautoが使えるようになった。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp #0:3 begin -2
```

この記法はジェネリックラムダと呼ばれる。
この機能により関数の中で関数テンプレートと同等のものが定義できるようになった。

#### ジェネリックラムダの内部構造
ジェネリックラムダは下記のように使用することができる。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp #1:0 begin
```

この例で使用しているクロージャは一見、型をダイナミックに扱っているように見えるが、
下記のような「テンプレートoperator()を持つ関数型」オブジェクトとして展開されていると考えれば、
理解できる。

```cpp
    // @@@ example/template/generic_lambda_ut.cpp #1:1 begin
```

#### std::variantとジェネリックラムダ
unionは、オブジェクトを全く無関係な複数の型に切り替えることができるため、
これが必要な場面では有用な機能であるが、未定義動作を誘発してしまう問題がある。
この対策としてC++17で導入されたものが、std::variantである。

まずは、std::variantの使用例を下記する。

```cpp
    // @@@ example/template/variant_ut.cpp #0:0 begin -1
```

上記からわかる通り、std::variantオブジェクトは、直前に代入されたオブジェクトの型以外で、
値を読み出した場合、問題なく読み出せるが、それ以外ではエクセプションを発生させる。

このstd::variantオブジェクトの保持する型とその値を文字列として取り出すラムダ式は、
下記のように書ける。

```cpp
    // @@@ example/template/variant_ut.cpp #0:1 begin -1
```

このラムダは、
3つの型をテンプレートパラメータとするstd::variantオブジェクト以外には適用できないため、
型の個数に制限のない方法を考える。

この実装は、

* 保持する型が何番目かを見つけるための関数テンプレート
* 関数テンプレートの引数となるジェネリックラムダ

の2つによって下記のように行うことができる。

```cpp
    // @@@ example/template/variant_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/template/variant_ut.cpp #1:1 begin -1
```

単体テストは、以下のようになる。

```cpp
    // @@@ example/template/variant_ut.cpp #1:2 begin -1
```

下記のように継承関係のない複数のクラスが同じシグネチャのメンバ関数を持つ場合、

```cpp
    // @@@ example/template/variant_ut.cpp #2:0 begin
```

std::variant、上に示した関数テンプレート、ジェネリックラムダを使い、
下記に示したような疑似的なポリモーフィズムを実現できる。

```cpp
    // @@@ example/template/variant_ut.cpp #2:1 begin -1
```

ここで示した関数テンプレートは、デザインパターン[Visitor](---)の例であり、
ほぼこれと同様のものがstd::visitとして定義されている。

```cpp
    // @@@ example/template/variant_ut.cpp #2:2 begin -1
```

### クラステンプレートと継承の再帰構造
クラステンプレートと継承の再帰構造はCRTPと呼ばれる。
このコードパターンについては、
「[CRTP(curiously recurring template pattern)](---)」で説明している。

### constexpr if文
C++17で導入された[constexpr if文](https://cpprefjp.github.io/lang/cpp17/if_constexpr.html)とは、
文を条件付きコンパイルすることができるようにするための制御構文である。

まずは、この構文を使用しない例を示す。

```cpp
    // @@@ example/template/constexpr_if_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/template/constexpr_if_ut.cpp #0:1 begin -1
```

このような場合、[SFINAE](---)によるオーバーロードが必須であったが、
この文を使用することで、下記のようにオーバーロードを使用せずに記述できるため、
条件分岐の可読性の向上が見込める。

```cpp
    // @@@ example/template/constexpr_if_ut.cpp #0:2 begin
```

この構文は[パラメータパック](---)の展開においても有用な場合がある。

```cpp
    // @@@ example/template/constexpr_if_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/template/constexpr_if_ut.cpp #1:1 begin -1
```

C++14までの構文を使用する場合、
上記のようなオーバーロードとリカーシブコールの組み合わせが必要であったが、
constexpr ifを使用することで、やや単純に記述できる。

```cpp
    // @@@ example/template/constexpr_if_ut.cpp #1:2 begin
```


### 意図しないname lookupの防止
下記のようにクラスや関数テンプレートが定義されている場合を考える。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #0:0 begin
    // @@@ example/template/suppress_adl_ut.cpp #0:1 begin
```

これに対しての単体テストは下記のようになる。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #0:2 begin -1
    // @@@ example/template/suppress_adl_ut.cpp #0:3 begin -1
    // @@@ example/template/suppress_adl_ut.cpp #0:4 begin -1
```

上記の抜粋である

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #0:3 begin -1
```

が名前空間Appの指定なしでコンパイルできる理由は、
[ADL](---)(実引数依存探索)により、Appもis_equalの[name lookup](---)の対象になるからである。
これは便利な機能であるが、その副作用として意図しないname
lookupによるバグの混入を起こしてしまうことがある。

上記の名前空間での定義が可視である状態で、下記のようなコードを書いた場合を考える。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #1:0 begin
```

この単体テストは、やはりADLを使い下記のように書ける。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #1:1 begin -2
```

これに問題はないが、下記のテストもコンパイルでき、且つテストもパスしてしまうことには問題がある。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #1:2 begin -2
```

このセマンティクス的に無意味な(もしくは混乱を引き起こしてしまうであろう)コードは、

* is_equalの引数の型XY、XYZはそれぞれ名前空間App、App2で定義されている
* 従って、ADLによりis_equalのname lookupには名前空間App、App2も使われる
* 引数の型XY、XYZを取り得るis_equalはAppで定義されたもののみである

というメカニズムによりコンパイルできてしまう。

こういったname lookup、特にADLの問題に対処する方法は、

* [ジェネリックすぎるテンプレートを書かない](---)
* [ADLが本当に必要でない限り名前を修飾する](---)
* [ADL Firewallを使う](---)

のようにいくつか考えられる。これらについて以下で説明を行う。

#### ジェネリックすぎるテンプレートを書かない
ここでの「ジェネリックすぎるテンプレート」とは、
シンタックス的には適用範囲が広いにもかかわらず、セマンティクス的な適用範囲は限られているものを指す。
従って下記のような関数テンプレートを指す概念ではない。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #2:0 begin
```

前記で問題を起こした関数テンプレート

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #0:1 begin
```

が典型的な「ジェネリックすぎるテンプレート」である。
これに対する最も安全な対処は下記コードで示す通りテンプレートを使わないことである。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #2:1 begin
```

ジェネリックなis_equalが必要であれば下記単体テストのように
[その他のテンプレートテクニック|ジェネリックラムダ](---)を使えばよい。
こうすることでその適用範囲はそれを定義した関数内に留まる。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #2:2 begin -1
```

上記で示した

* テンプレートを使わない
* 適用範囲の広いテンプレート(ジェネリック)に対してはアクセスできる箇所を局所化する

といった方法の他にも、「[コンテナ用Nstd::operator\<\<の開発](---)」で示した

* std::enable_if等を使用してテンプレートに適用できる型を制限する

ことも考えられる。
ベストな方法は状況に大きく依存するため一概には決められない。
その状況でのもっとも単純は方法を選ぶべきだろう(が、何が単純かも一概に決めることは難しい)。


#### ADLが本当に必要でない限り名前を修飾する
下記のコードについて考える。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #3:0 begin
```

基底クラスのメンバ関数を呼び出す場合は、T::f()、もしくは、this->f()と書く必要があるため、
下記コードで呼び出した関数fは外部関数fの呼び出しになる
([two phase name lookup](---)の一回目のname lookupでfがバインドされるため)。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #3:1 begin -1
```

これだけでも十分わかりづらいが、
ExecFのテンプレートパラメータにはクラスAしか使われないことがわかったので、
下記のようにリファクタリングしたとしよう。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #4:0 begin
```

すると、fのname lookupの対象が変わってしまい、元の単体テストはパスしなくなる。

```cpp
    // @@@ example/template/suppress_adl_ut.cpp #4:1 begin -1
```

こういった場合に備え単体テストを実行すべきなのだが、この程度の問題はコンパイルで検出したい。
[ADL](---)や[two phase name lookup](---)が絡む場合ならなおさらである。

こういう意図しない[name lookup](---)に備えるためには、
修飾されていない識別子を使わないこと、つまり、
識別子には、名前空間、クラス名、this->等による修飾を施すことが重要である。

ただし、「[コンテナ用Nstd::operator\<\<の開発](---)」で示したコード等にはADLが欠かせないため、
修飾することをルール化することはできない。場合に合わせた運用が唯一の解となる。


#### ADL Firewallを使う
下記のコードについて考える。

```cpp
    // @@@ example/template/adl_firewall_0_ut.cpp #0:0 begin
```

上記のApp::ToStringは「[コンテナ用Nstd::operator\<\<の開発](---)」のコードを使用し、
std::vectorオブジェクトをstd::stringに変換する。

これに対しての単体テストは下記のようになる。

```cpp
    // @@@ example/template/adl_firewall_0_ut.cpp #0:1 begin -1
```

これは想定通りの動作だが、上記のAppの後に下記のコードを追加するとApp::ToStringは影響を受ける。

```cpp
    // @@@ example/template/adl_firewall_1_ut.cpp #0:0 begin
```

これにより元の単体テストはエラーとなり、新しい単体テストは下記のようになる。

```cpp
    // @@@ example/template/adl_firewall_1_ut.cpp #0:1 begin -1
```

これが意図通りなら問題ないが、
ここでは「新たに追加した関数テンプレートApp::operator<<はstd::vector\<App::XY>用ではなかった」
としよう。その場合、これは意図しないADLによるバグの混入となる。
「[ジェネリックすぎるテンプレートを書かない](---)」
で述べたように追加した関数テンプレートの適用範囲が広すぎることが原因であるが、
XY型から生成されたオブジェクト(std::vector\<App::XY>も含む)によるADLのため、
Appの宣言がname lookupの対象になったことにも原因がある。

下記のコードは後者の原因を解消する。

```cpp
    // @@@ example/template/adl_firewall_2_ut.cpp #0:0 begin
```

XY型オブジェクトを引数にした関数呼び出しによる[関連名前空間](---)は、
極小なApp::XY_Firewall\_であるため、意図しないADLは起こりづらく、起こっても発見しやすい。
また、XY型用operator<<もApp::XY_Firewall\_で定義し、
App内でusing XYを宣言したことで、これまで通りApp::XYが使える。

このようなテクニックをADL firewallと呼ぶ。


### Nstd::Type2Strの開発
「[Nstdライブラリの開発](---)」等で行ったメタ関数の実装は、

* 入り組んだ<>や()の対応漏れ
* &や&&のつけ忘れ
* typenameやtemplateキーワードの漏れ
* メタ関数メンバー::valueや::typeの漏れ

等によるコンパイルエラーとの戦いである。
また、これをクリアしても[two phase name lookup](---)や[ADL](---)が次の関門になる。
これには、デバッガのステップ実行が強力な武器となるが、
型を文字列に変換する関数があればこれもまた強力な武器となる。

以下に示すNstd::Type2Strは、「[Nstdライブラリの開発](---)」等で実際に使用したそのような関数である。

```cpp
    // @@@ h/nstd_type2str.h #0:0 begin
    // @@@ h/nstd_type2str.h #0:1 begin
    // @@@ h/nstd_type2str.h #0:2 begin
```

typeid::name()が返す文字列リテラルは引数の型の文字列表現を持つが、
マングリングされているためヒューマンリーダブルではない。
それをデマングルするのがabi::\_\_cxa\_demangleであるが、
残念なことにこの関数は非標準であるため、
それを使っているNstd::Inner\_::demangleは[g++](---)/[clang++](---)
でなければコンパイルできないだろう。

それを除けば、
複雑なシンタックスを持つ型を文字列で表現できるNstd::Type2Strは、
テンプレートプログラミングおける有効なデバッグツールであると言える。

下記単体テストは、そのことを示している。

```cpp
    // @@@ example/template/nstd_type2str_ut.cpp #0:0 begin -1
```

### 静的な文字列オブジェクト
std::stringは文字列を扱うことにおいて、非常に有益なクラスではあるが、
コンパイル時に文字列が決定できる場合でも、動的にメモリを確保する。

この振る舞いは、

* ランタイム時にnew/deleteを行うため、処理の遅さにつながる。
* 下記のようにエクセプションオブジェクトにファイル位置を埋め込むことは、
  デバッグに便利であるが、メモリ確保失敗を通知するような場面ではこの方法は使えない。

```cpp
    // @@@ example/template/nstd_exception_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/template/nstd_exception_ut.cpp #0:1 begin -1
```

このような問題を回避するために、ここでは静的に文字列を扱うためのクラスStaticStringを開発する。

#### StaticStringのヘルパークラスの開発
StaticStringオブジェクトは、char配列をメンバとして持つが、
コンパイル時に解決できる配列の初期化にはパラメータパックが利用できる。
そのパラメータパック生成クラスを下記のように定義する。

```cpp
    // @@@ example/template/nstd_seq.h #0:0 begin
```

このクラスにより、下記のような配列メンバの初期ができるようになる。

```cpp
    // @@@ example/template/nstd_seq_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/template/nstd_seq_ut.cpp #0:1 begin -1
```

これを下記のように使うことで、メンバである文字列配列のコンパイル時初期化ができるようになる。

```cpp
    // @@@ example/template/nstd_seq_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/template/nstd_seq_ut.cpp #1:1 begin -1
```

上記とほぼ同様のクラステンプレートstd::index_sequence、std::make_index_sequenceが、
utilityで定義されているため、以下ではこれらを使用する。

#### StaticStringの開発
StaticStringはすでに示したテクニックを使い、下記のように定義できる。

```cpp
    // @@@ example/h/nstd_static_string.h #0:0 begin
```

文字列リテラルからStaticStringを生成する単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_ut.cpp #0:0 begin -2
```

また、std::initializer_list\<char>による初期化の単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_ut.cpp #0:1 begin -2
```

次にこのクラスにoperator == を追加する。

```cpp
    // @@@ example/h/nstd_static_string.h #0:1 begin
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_ut.cpp #1:0 begin -1
```

[非explitなコンストラクタによる暗黙の型変換](---)
を利用した文字列リテラルからStaticStringオブジェクトへの変換は、
StaticStringがテンプレートであるため機能せず、上記のように書く必要がある。

同様にoperator + を追加する。

```cpp
    // @@@ example/h/nstd_static_string.h #0:2 begin
```

```cpp
    // @@@ example/template/nstd_static_string_ut.cpp #2:0 begin -1
```

#### 整数をStaticStringに変換する関数の開発
コンパイル時に__LINE__をStaticStringに変換できれば、
ファイル位置をStaticStringで表現できるため、
ここではその変換関数Int2StaticString\<>()の実装を行う。

行番号を10進数での文字列で表現するため、いくつかのヘルパ関数を下記のように定義する。

```cpp
    // @@@ example/h/nstd_static_string_num.h #0:0 begin
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_num_ut.cpp #0:0 begin -1
```

このままでは使いづらいため、これをラッピングした関数を下記のように定義することで、
Int2StaticString\<>()が得られる。

```cpp
    // @@@ example/h/nstd_static_string_num.h #0:1 begin
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_static_string_num_ut.cpp #1:0 begin -2
```

#### ファイル位置を静的に保持したエクセプションクラスの開発
「[静的な文字列オブジェクト](---)」で見たように、
ファイル位置を動的に保持するエクセプションクラスは使い勝手が悪い。
ここでは、その問題を解決するためのExceptionクラスの実装を示す。

```cpp
    // @@@ example/h/nstd_exception.h #0:0 begin
```

StaticStringと同様に、このままでは不便であるため、下記の関数を定義する。

```cpp
    // @@@ example/h/nstd_exception.h #0:1 begin
```

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_exception_ut.cpp #1:0 begin -2
```

Exceptionクラスの利便性をさらに高めるため、下記の定義を行う。

```cpp
    // @@@ example/h/nstd_exception.h #0:2 begin
```

上記は、関数型マクロの数少ない使いどころである。

単体テストは下記のようになる。

```cpp
    // @@@ example/template/nstd_exception_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/template/nstd_exception_ut.cpp #2:1 begin -1
```

### 関数型をテンプレートパラメータで使う
ここで使う「関数型」とは、

* 関数へのポインタの型
* クロージャの型、もしくはラムダ式の型
* 関数オブジェクトの型

の総称を指す。

std::unique_ptrは、

* 第1パラメータにポインタの型
* 第2パラメータにそのポインタの解放用の関数ポインタの型

を取ることができるが、通常は第2パラメータは省略される。
省略時にはstd::default_deleteが割り当てられ、そのオブジェクトによって、
第1パラメータに対応するポインタがdeleteされる。

下記コードではこの第2パラメータにstd::freeのポインタの型を与え、
それから生成されるstd::unique_ptrオブジェクトを、

* abi::\__cxa_demangleがstd::mallocで取得したchar型ポインタ
* std::freeのポインタ

で初期化することでメモリの解放を行っている。

```cpp
    // @@@ h/nstd_type2str.h #0:1 begin -1
```

std::unique_ptrの第2パラメータには、上記のような関数へのポインタのみではなく、
関数型を取ることができる。

そのことを順を追って示す。
まずは、std::unique_ptrの動作を確かめるためのクラスを下記のように定義する。

```cpp
    // @@@ example/template/func_type_ut.cpp #0:0 begin
```

次に示すのは、第2パラメータに何も指定しないパターンである。
テスト用クラスAの動作確認ができるはずである。

```cpp
    // @@@ example/template/func_type_ut.cpp #1:0 begin -1
```

次に示すのは、

```cpp
    // @@@ example/template/func_type_ut.cpp #0:1 begin
```

のポインタをstd::unique_ptrの第2パラメータに与えた例である。

```cpp
    // @@@ example/template/func_type_ut.cpp #1:1 begin -1
```

次に示すのは、std::unique_ptrの第2パラメータにラムダを与えた例である。

```cpp
    // @@@ example/template/func_type_ut.cpp #1:2 begin -1
```

次に示すのは、
std::unique_ptrの第2パラメータに関数型オブジェクトの型(std::function)を与えた例である。

```cpp
    // @@@ example/template/func_type_ut.cpp #1:3 begin -1
```

以上で見てきたようにstd::unique_ptrの第2パラメータには、
第1パラメータのポインタを引数に取る関数型であれば指定できる。

このようなテンプレートパラメータを持つクラステンプレートの実装例を示すため、
「[RAII(scoped guard)](---)でも示したScopedGuardの実装を下記する。

やや意外だが、このようなテンプレートパラメータに特別な記法はなく、以下のようにすれば良い。

```cpp
    // @@@ h/scoped_guard.h #0:0 begin
    // @@@ h/scoped_guard.h #0:1 begin
    // @@@ h/scoped_guard.h #0:2 begin
```

上記コードの抜粋である下記は、テンプレートパラメータである関数型を規定するものである。

```cpp
    // @@@ h/scoped_guard.h #0:1 begin -2 1
```

これがなければ、誤った型の関数型をテンプレートパラメータに指定できてしまう。

以下にこのクラステンプレートの単体テストを示す。

まずは、以下の関数と静的変数の組み合わせ

```cpp
    // @@@ example/template/func_type_ut.cpp #2:0 begin
```

を使った例である。

```cpp
    // @@@ example/template/func_type_ut.cpp #2:1 begin -1
```

次に示すのは、それぞれにラムダ式とstd::functionを使った2例である。

```cpp
    // @@@ example/template/func_type_ut.cpp #2:2 begin -1
```

次に示すのは関数型オブジェクト

```cpp
    // @@@ example/template/func_type_ut.cpp #3:0 begin
```

を使った例である。

```cpp
    // @@@ example/template/func_type_ut.cpp #3:1 begin -1
```

C++17からサポートされた「クラステンプレートのテンプレートパラメータ推論」が使えないC++14以前では、
下記に示すようにScopedGuardのテンプレートラメータ型を指定しなければならない煩雑さがある。

```cpp
    // @@@ example/template/func_type_ut.cpp #3:2 begin -1
```

これを回避するためには下記のような関数テンプレートを用意すればよい。

```cpp
    // @@@ example/template/func_type_ut.cpp #4:0 begin
```

下記に示した単体テストから明らかな通り、関数テンプレートの型推測の機能により、
テンプレートパラメータを指定する必要がなくなる。

```cpp
    // @@@ example/template/func_type_ut.cpp #4:1 begin -1
```

このような便利なテンプレートは、Nstdのようなライブラリで定義、宣言し、
ソースコード全域からアクセスできるようにするとプロジェクトの開発効率が少しだけ高まる。

[演習-関数型のテンプレートパラメータを持つクラステンプレート](~~~)  

## 注意点まとめ
本章では、テンプレートメタプログラミングのテクニックや注意点について解説したが、
本章の情報量は多く、また他の章で行ったものもあるため以下にそれらをまとめる。

* [name lookup](---)には複雑なルールが適用されるため、非直感的なバインドが行われる場合がある。
  従って、テンプレートライブラリの開発には単体テストは必須である。

* 使用しているコンパイラが[two phase name lookup](---)をサポートしているか否かに気を付ける。
  それがオプションである場合は、two phase name lookupを活性化させる。

* 関数型マクロはそれ以外に実装方法がない時のみに使用する
  (「[関数型マクロ](~~~)」参照)
  。

* 可変長引数を持つ関数の実装には[パラメータパック](---)を使う。

* 処理速度や関数のリターンの型に影響する場合があるため、
  パラメータパックの処理の順番に気を付ける(「[前から演算するパラメータパック](---)」参照)。

* [ADL](---)を利用しない場合、テンプレートで使う識別子は名前空間名やthis->等で修飾する
  (「[意図しないname lookupの防止](---)」参照)。

* テンプレートのインターフェースではないが、実装の都合上ヘッダファイルに記述する定義は、
  "namespace Inner\_"を使用し、非公開であることを明示する。
  また、"namespace Inner\_"で宣言、定義されている宣言、定義は単体テストを除き、
  外部から参照しない(「[is_void_sfinae_fの実装](---)」参照)。

* ユニバーサルリファレンスの実際の型がlvalueリファレンスであるならば、
  constなlvalueリファレンスとして扱う
  (「[非メンバ関数/メンバ関数共通|実引数/仮引数](~~~)」参照)
  。

* ユニバーサルリファレンス引数を他の関数に渡すのであれば、std::forwardを使う
  (「[ユニバーサルリファレンス](---)」、「[ユニバーサルリファレンスとstd::forward](---)」参照)。

* 関数テンプレートとその特殊化はソースコード上なるべく近い位置で定義する
  (「[two phase name lookup](---)」参照)。

* [two phase name lookup](---)により意図しない副作用が発生する可能性があるため、
  STLが特殊化を想定しているstd::hash等を除き、STLの拡張は行わない。

* ユーザが定義するテンプレートは適切に定義された名前空間内で定義する
  (「[スコープの定義と原則](~~~)」参照)
  。

* 型とその2項演算子オーバーロードは同じ名前空間で定義する
  (「[two phase name lookup](---)」参照)。

* 関数テンプレートのオーバーロードと特殊化の[name lookup](---)の優先度に気を付ける。
  オーバーロードのベストマッチ選択後に特殊化された関数テンプレートがname lookupの対象になるため、
  下記コードが示すように直感に反する関数が選択される場合がある。

```cpp
    // @@@ example/template/etc_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/template/etc_ut.cpp #0:1 begin -1
```

* ユニバーサルリファレンスを持つ関数テンプレートをオーバーロードしない。
  「[ユニバーサルリファレンスとstd::forward](---)」で述べたように、
  ユニバーサルリファレンスはオーバーロードするためのものではなく、
  lvalue、rvalue両方を受け取ることができる関数テンプレートを、
  オーバーロードを使わずに実現するための記法である。

* テンプレートに関数型オブジェクトを渡す場合、リファレンスの付け忘れに気を付ける
  (「[関数型をテンプレートパラメータで使う](---)」
  、「[現象6 - STLのバグ?](~~~)」
  参照)。

* 意図しないテンプレートパラメータによるインスタンス化の防止や、
  コンパイルエラーを解読しやすくするために、
  適切にstatic_assert(「[exists_begin/exsits_endの実装](---)」
  、「[assertion](~~~)」
  参照)を使う。

* ランタイム時の処理を削減する、static_assertを適切に用いる等の目的のために、
  関数テンプレートには適切にconstexprを付けて宣言する
  (「[コンテナ用Nstd::operator\<\<の開発](---)」
  、「[constexpr関数](~~~)」
  参照)。


