# 用語解説

この章では、このドキュメントで使用する用語の解説をする。

## 型とインスタンス

### 算術型
算術型とは下記の型の総称である。

* [用語解説|型とインスタンス|汎整数型](---)(bool, char, int, unsigned int, long long等)
* 浮動小数点型(float、double、long double)

算術型のサイズは下記のように規定されている。

* 1 == sizeof(bool) == sizeof(char)
* sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)
* 4 <= sizeof(long)
* 8 <= sizeof(long long)
* 4 == sizeof(float)
* 8 == sizeof(double) <= sizeof(long double)

### 汎整数型
汎整数型とは下記の型の総称である。

* 論理型(bool)
* 文字型(char、wchar_t等)
* [用語解説|型とインスタンス|整数型](---)(int、unsigned int、long等)

### 整数型
整数型とは下記の型の総称である。

* char
* signed char
* unsigned char
* short
* unsigned short
* int
* unsigned int
* long
* unsigned long
* long long
* unsigned long long

### 算術変換
C++における算術変換とは、算術演算の1つのオペランドが他のオペランドと同じ型でない場合、
1つのオペランドを他のオペランドと同じ型に変換するプロセスのことを指す。

算術変換は、[汎整数拡張](---)と通常算術変換に分けられる。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #0:0 begin -1
```

一様初期を使用することで、
変数定義時の算術変換による意図しない値の変換(縮小型変換)を防ぐことができる。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #0:1 begin -2
```

以下に示すように、算術変換の結果は直感に反することがあるため、注意が必要である。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #0:2 begin -2
```

### 汎整数拡張
bool、char、signed char、unsigned char、short、unsigned short型の変数が、
算術のオペランドとして使用される場合、

* その変数の型の取り得る値全てがintで表現できるのならば、int型に変換される。
* そうでなければ、その変数はunsigned int型に変換される。

この変換を汎整数拡張と呼ぶ。

従って、sizof(short) < sizeof(int)である処理系では、
bool、char、signed char、unsigned char、short、unsigned short型の変数は、
下記のようにintに変換される。

```cpp
    // @@@ example/term_explanation/integral_promotion_ut.cpp #1:0 begin -1
```

### POD
PODとは、 Plain Old Dataの略語であり、

```cpp
    std::is_pod<T>::value
```

がtrueとなる型Tを指す。下記のコードはその使用例である。

```cpp
    // @@@ example/term_explanation/pod_ut.cpp #0:0 begin -2
```

概ね、C言語と互換性のある型を指すと思って良い。

「型が[トリビアル型](---)且つ[標準レイアウト型](---)であること」と
「型が[POD](---)であること」は等価であるため、C++20では、
[PODという用語は非推奨](https://cpprefjp.github.io/lang/cpp20/deprecate_pod.html)となった。

### 標準レイアウト型
標準レイアウト型とは、

```cpp
    std::is_standard_layout<T>::value
```

がtrueとなる型Tを指す。下記のコードはその使用例である。

```cpp
    // @@@ example/term_explanation/pod_ut.cpp #0:1 begin -2
```

型がPODである場合、その型は標準レイアウト型である。

### トリビアル型
トリビアル型とは、

```cpp
    std::is_trivial<T>::value
```

がtrueとなる型Tを指す。下記のコードはその使用例である。

```cpp
    // @@@ example/term_explanation/pod_ut.cpp #0:2 begin -2
```

型がPODである場合、その型はトリビアル型である。

### underlying type
underlying typeとは、enumやenum classの汎整数表現を指定できるようにするために、
C++11で導入されたシンタックスである。

```cpp
    // @@@ example/term_explanation/underlying_type_ut.cpp #0:0 begin
```

```cpp
    // @@@ example/term_explanation/underlying_type_ut.cpp #0:1 begin -1
```


### 不完全型
不完全型とは、型のサイズや構造が不明な型を指す。
下記のような場合、不完全型となる。

```cpp
    // @@@ example/term_explanation/incomplete_type_ut.cpp #0:0 begin -1
```

### 完全型
[不完全型](---)ではない型を指す。

### ポリモーフィックなクラス
ポリモーフィックなクラスとは、仮想関数を持つクラスを指す。
なお、純粋仮想関数を持つクラスは、仮想クラスと呼ばれれる。

### インターフェースクラス
インターフェースクラスとは、純粋仮想関数のみを持つ抽象クラスのことを指す。
インターフェースクラスは、クラスの実装を提供することなく、
クラスのインターフェースを定義するために使用される。
インターフェースクラスは、クラスの仕様を定義するために使用されるため、
多くの場合、抽象基底クラスとして使用される。


```cpp
    // @@@ example/term_explanation/interface_class.cpp #0:0 begin
```

### constインスタンス

constインスタンスとはランタイムの初期化時に値が確定し、その後、状態が不変であるインスタンスである。

### constexprインスタンスと関数
constexprインスタンスとはコンパイル時に値が確定するインスタンスである。
当然、ランタイム時でも不変である。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp #0:0 begin
```

constexprとして宣言された関数の戻り値がコンパイル時に確定する場合、
その関数の呼び出し式はconstexprと扱われる。
従って、この値はテンプレートパラメータやstatic_assertのオペランドに使用することができる。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp #0:1 begin
```
```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp #0:2 begin -1
```

下記のようなリカーシブな関数でも場合によってはconstexprにできる。
これによりこの関数の実行速度は劇的に向上する。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp #0:3 begin
```

下記の単体テストが示すように、

* 引数がconstexprである場合、上記constexpr関数の戻り値はconstexprになるため、
  static_assertのオペランド式としても利用できる。
* 引数がconstexprでない場合、上記constexpr関数は通常の関数として振舞うため、
  戻り値はconstexprとならない。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp #0:4 begin -1
```

下記のようにクラスのコンストラクタをconstexprとすることで、
コンパイル時にリテラルとして振る舞うクラスを定義することができる。

```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp #0:5 begin
```
```cpp
    // @@@ example/term_explanation/constexpr_ut.cpp #0:6 begin -1
```

### ユーザ定義リテラル演算子
ユーザ定義リテラル演算子とは以下のようなものである。

```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp #0:1 begin -1
```

### std::string型リテラル
"xxx"sとすることで、std::string型のリテラルを作ることができる。

```cpp
    // @@@ example/term_explanation/user_defined_literal_ut.cpp #1:0 begin -1
```

## オブジェクトと生成

### 特殊メンバ関数
特殊メンバ関数とは下記の関数を指す。

* デフォルトコンストラクタ
* copyコンストラクタ
* copy代入演算子
* moveコンストラクタ
* move代入演算子
* デストラクタ

ユーザがこれらを一切定義しない場合、または一部のみを定義する場合、
コンパイラは、下記のテーブル等で示すルールに従い、特殊関数メンバの宣言、定義の状態をを定める。

左1列目がユーザによる各関数の宣言を表し、2列目以降はユーザ宣言の影響による各関数の宣言の状態を表す。  
下記表において、

* 「= default」とは、「コンパイラによってその関数が= defaultと宣言された」状態であることを表す。
* 「~~= default~~」とは、= defaultと同じであるが、バグが発生しやすいので推奨されない。
* 「宣言無し」とは、「コンパイラによってその関数が= defaultと宣言された状態ではない」ことを表す。
    * 「moveコンストラクタが= defaultと宣言された状態ではない」且つ
      「copyコンストラクタが宣言されている」場合、
      rvalueを使用したオブジェクトの初期化には、
      moveコンストラクタの代わりにcopyコンストラクタが使われる。
    * 「move代入演算子が= defaultと宣言された状態ではない」且つ
      「copy代入演算子が宣言されている」場合、
      rvalueを使用したオブジェクトの代入には、
      move代入演算子の代わりにcopy代入演算子が使われる。
* 「= delete」とは「コンパイラによってその関数が= deleteと宣言された」状態であることを表す。

|ユーザによる特殊関数の宣言|デフォルト<br>コンストラクタ |デストラクタ|copy<br>コンストラクタ|copy<br>代入演算子|move<br>コンストラクタ|move<br>代入演算子|
|:------------------------:|:-------------:|:-----------:|:------------:|:------------:|:---------:|:---------:|
|宣言無し                  | = default     | = default   | = default    | = default    | = default | = default |
|非デフォルトコンストラクタ| 宣言なし      | = default   | = default    | = default    | = default | = default |
|デフォルトコンストラクタ  | -             | = default   | = default    | = default    | = default | = default |
|デストラクタ              | = default     | -           | ~~= default~~| ~~= default~~| 宣言なし  | 宣言なし  |
|copyコンストラクタ        | 宣言なし      | = default   | -            | ~~= default~~| 宣言なし  | 宣言なし  |
|copy代入演算子            | = default     | = default   | ~~= default~~| -            | 宣言なし  | 宣言なし  |
|moveコンストラクタ        | 宣言なし      | = default   | = delete     | = delete     | -         | 宣言なし  |
|move代入演算子            | = default     | = default   | = delete     | = delete     | 宣言なし  | -         |


上記表より、下記のようなことがわかる。

* ユーザが上記6メンバ関数を一切宣言しない場合、それらはコンパイラにより暗黙に宣言、定義される。
* ユーザがcopyコンストラクタを宣言した場合、デフォルトコンストラクタは暗黙に宣言、定義されない。
* ユーザがcopyコンストラクタを宣言した場合、copy代入演算子はコンパイラにより暗黙に宣言、
  定義されるが、そのことは推奨されない(~~= default~~は非推奨のdefault宣言を指す)。
* moveコンストラクタ、move代入演算子は、
  以下のいずれもが明示的に宣言されていない場合にのみ暗黙に宣言、定義される。
    * copyコンストラクタ
    * copy代入演算子(operator =)
    * moveコンストラクタ
    * move代入演算子
    * デストラクタ

* ユーザがmoveコンストラクタまたはmove代入演算子を宣言した場合、
  copyコンストラクタ、copy代入演算子は= deleteされる。


### 初期化子リストコンストラクタ
初期化子リストコンストラクタ(リスト初期化用のコンストラクタ)とは、
{}によるリスト初期化をサポートするためのコンストラクタである。
下記コードでは、 E::E(std::initializer_list\<uint32_t>)が初期化子リストコンストラクタである。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp #0:0 begin
```

デフォルトコンストラクタと初期化子リストコンストラクタが、
それぞれに定義されているクラスの初期化時に空の初期化子リストが渡された場合、
デフォルトコンストラクタが呼び出される。

初期化子リストコンストラクタと、
「その初期化子リストの要素型と同じ型の仮引数のみを受け取るコンストラクタ
(上記コードのE::E(uint32_t, uint32_t))」
の両方を持つクラスの初期化時にどちらでも呼び出せる初期化子リストが渡された場合({}を使った呼び出し)、
初期化子コンストラクタが呼び出される。

### 継承コンストラクタ
継承コンストラクタとは、基底クラスで定義したコンストラクタ群を、
派生クラスのインターフェースとしても使用できるようにするための機能である。
下記コードのように、継承コンストラクタは派生クラス内でusingを用いて宣言される。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp #1:0 begin
```

### 委譲コンストラクタ
委譲コンストラクタとは、コンストラクタから同じクラスの他のコンストラクタに処理を委譲する機能である。
以下のコード中では、委譲コンストラクタを使い、
A::A(uint32_t)の処理をA::A(std::string const&)へ委譲している。

```cpp
    // @@@ example/term_explanation/constructor_ut.cpp #2:0 begin
```

### 非explitなコンストラクタによる暗黙の型変換
非explitなコンストラクタによる暗黙の型変換とは、

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:0 begin
```

上記のクラスPersonを使用して、下記のようなコードをコンパイルできるようにする機能である。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:1 begin
```

この記法は下記コードの短縮形であり、コードの見た目をシンプルに保つ効果がある。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:2 begin
```

この記法は下記のようにstd::string等のSTLでも多用され、その効果は十分に発揮されているものの、

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:3 begin -2
```

以下のようなコードがコンパイルできてしまうため、わかりづらいバグの元にもなる。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:4 begin -2
```

下記のようにコンストラクタにexplicitを付けて宣言することにより、この問題を防ぐことができる。

```cpp
    // @@@ example/term_explanation/implicit_conversion_ut.cpp #0:5 begin
```

std::stringは暗黙の型変換を許して良く、(多くの場合)Personには暗黙の型変換をしない方が良い理由は、

* std::stringの役割は文字列の管理と演算のみであるため、
  std::stringを文字列リテラルと等価なもののように扱っても違和感がない
* Personは、明らかに文字列リテラルと等価なものではない

といったセマンティクス的観点(「[シンタックス、セマンティクス](---)」参照)によるものである。

クラスPersonと同様に、
ほとんどのユーザ定義クラスには非explitなコンストラクタによる暗黙の型変換は必要ない。


### NSDMI
NSDMIとは、non-static data member initializerの略語であり、
下記のような非静的なメンバ変数の初期化子を指す。

```cpp
    // @@@ example/term_explanation/nsdmi.cpp #0:0 begin
```

### 一様初期化
一様初期化(uniform initialization)とは、
C++11で導入された、コンストラクタの呼び出しをリスト初期化と合わせて{}で記述する構文である。

```cpp
    // @@@ example/term_explanation/uniform_initialization_ut.cpp #0:0 begin -1
```

変数による一様初期化が縮小型変換を起こす場合や、
リテラルによる一様初期化がその値を変更する場合、コンパイルエラーとなるため、
この機能を積極的に使用することで、縮小型変換による初期化のバグを未然に防ぐことができる。

```cpp
    // @@@ example/term_explanation/uniform_initialization_ut.cpp #0:1 begin -1
```


### AAAスタイル
このドキュメントでのAAAとは、単体テストのパターンarrange-act-assertではなく、
almost always autoを指し、
AAAスタイルとは、「可能な場合、型を左辺に明示して変数を宣言する代わりに、autoを使用する」
というコーディングスタイルである。
この用語は、Andrei Alexandrescuによって造られ、Herb Sutterによって広く推奨されている。

特定の型を明示して使用する必要がない場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp #0:0 begin -1
```

特定の型を明示して使用する必要がある場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp #1:0 begin -1
```

関数の戻り値を受け取る変数を宣言する場合、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp #2:0 begin -1
```

ただし、関数の戻り値型が容易に推測しがたい下記のような場合、
型を明示しないAAAスタイルは使うべきではない。

```cpp
    // @@@ example/term_explanation/aaa.cpp #2:1 begin -1
```

インライン関数や関数テンプレートの宣言は、下記のように書く。

```cpp
    // @@@ example/term_explanation/aaa.cpp #3:0 begin
```

ただし、インライン関数や関数テンプレートが複雑な下記のような場合、
AAAスタイルは出来る限り避けるべきである。

```cpp
    // @@@ example/term_explanation/aaa.cpp #3:1 begin
```

このスタイルには下記のような狙いがある。

* コードの安全性の向上  
  autoで宣言された変数は未初期化にすることができないため、未初期化変数によるバグを防げる。
  また、下記のように縮小型変換(下記では、unsignedからsignedの変換)を防ぐこともできる。

```cpp
    // @@@ example/term_explanation/aaa.cpp #4:0 begin -1
```

* コードの可読性の向上  
  冗長なコードを排除することで、可読性の向上が見込める。

* コードの保守性の向上  
  「変数宣言時での左辺と右辺を同一の型にする」非AAAスタイルは
  [DRYの原則](https://ja.wikipedia.org/wiki/Don%27t_repeat_yourself#:~:text=Don't%20repeat%20yourself%EF%BC%88DRY,%E3%81%A7%E3%81%AA%E3%81%84%E3%81%93%E3%81%A8%E3%82%92%E5%BC%B7%E8%AA%BF%E3%81%99%E3%82%8B%E3%80%82)
  に反するが、この観点において、AAAスタイルはDRYの原則に沿うため、
  コード修正時に型の変更があった場合でも、それに付随したコード修正を最小限に留められる。


AAAスタイルでは、以下のような場合に注意が必要である。

* 関数の戻り値をautoで宣言された変数で受ける場合  
  上記で述べた通り、AAAの過剰な仕様は、可読性を下げてしまう。

* autoで推論された型が直感に反する場合  
  下記のような型推論は、直感に反する場合があるため、autoの使い方に対する習熟が必要である。

```cpp
    // @@@ example/term_explanation/aaa.cpp #5:0 begin -1
```

### オブジェクトの所有権
オブジェクトxがオブジェクトaの解放責務を持つ場合、
xはaの所有権を持つ(もしくは、所有する) という。

定義から明らかな通り、ダイナミックに生成されたaをxが所有する場合、
xはa(へのポインタ)をdeleteする責務を持つ。

xがaを所有し、且つxがaを他のオブジェクトと共有しない場合、「xはaを排他所有する」という。

オブジェクト群x0、x1、...、xNがaを所有する場合、
「x0、x1、...、xNはaを共有所有する」という。

x0、x1、...、xNがaを共有所有する場合、x0、x1、...、xN全体で、a(へのポインタ)をdeleteする責務を持つ。

下記で示したような状況では、
ダイナミックに生成されたオブジェクトの所有権の所在をコードから直ちに読み取ることは困難であり、
その解放責務も曖昧となる。

```cpp
    // @@@ example/term_explanation/ambiguous_ownership_ut.cpp #0:0 begin -1
```

こういった問題に対処するためのプログラミングパターンを以下の
「[オブジェクトの排他所有](---)」と「[オブジェクトの共有所有](---)」で解説する。

#### オブジェクトの排他所有
オブジェクトの排他所有や、それを容易に実現するための
[std::unique_ptr](https://cpprefjp.github.io/reference/memory/unique_ptr.html)
の仕様をを説明するために、下記のようにクラスA、Xを定義する。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:0 begin
```

下記に示した上記クラスの単体テストにより、
オブジェクトの所有権やその移動、
std::unique_ptr、std::move()、[expressionと値カテゴリ|rvalue](---)の関係を解説する。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:1 begin -1
```

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:2 begin -1
```

![所有権1](plant_uml/unique_ownership_1.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:3 begin -1
```

![所有権2](plant_uml/unique_ownership_2.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:4 begin -1
```
![所有権3](plant_uml/unique_ownership_3.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:5 begin -1
```

![所有権4](plant_uml/unique_ownership_4.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:6 begin -1
```

![所有権5](plant_uml/unique_ownership_5.png)

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #0:7 begin -1
```

![所有権6](plant_uml/unique_ownership_6.png)


また、以下に見るようにstd::unique_ptrはcopy生成やcopy代入を許可しない。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #1:0 begin -1
```

以上で示したstd::unique_ptrの仕様の要点をまとめると、以下のようになる。

* std::unique_ptrはダイナミックに生成されたオブジェクトを保持する。
* ダイナミックに生成されたオブジェクトを保持するstd::unique_ptrがスコープアウトすると、
  保持中のオブジェクトは自動的にdeleteされる。
* 保持中のオブジェクトを他のstd::unique_ptrにmoveすることはできるが、
  copyすることはできない。このため、下記に示すような不正な方法以外で、
  複数のstd::unique_ptrが1つのオブジェクトを共有することはできない。

```cpp
    // @@@ example/term_explanation/unique_ptr_ownership_ut.cpp #2:0 begin -1
```

こういった機能によりstd::unique_ptrはオブジェクトの排他所有を実現している。

#### オブジェクトの共有所有
オブジェクトの共有所有や、それを容易に実現するための
[std::shared_ptr](https://cpprefjp.github.io/reference/memory/shared_ptr.html)
の仕様をを説明するために、下記のようにクラスA、Xを定義する。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:0 begin
```

下記に示した上記クラスの単体テストにより、
オブジェクトの所有権やその移動、共有、
std::shared_ptr、std::move()、[expressionと値カテゴリ|rvalue](---)の関係を解説する。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:1 begin -1
```

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:2 begin -1
```

![所有権1](plant_uml/shared_ownership_1.png)


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:3 begin -1
```

![所有権2](plant_uml/shared_ownership_2.png)

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:4 begin -1
```

![所有権3](plant_uml/shared_ownership_3.png)

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:5 begin -1
```

![所有権4](plant_uml/shared_ownership_4.png)

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:6 begin -1
```

![所有権5](plant_uml/shared_ownership_5.png)


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:7 begin -1
```

![所有権6](plant_uml/shared_ownership_6.png)


```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #0:8 begin -1
```

![所有権7](plant_uml/shared_ownership_7.png)

以上で示したstd::shared_ptrの仕様の要点をまとめると、以下のようになる。

* std::shared_ptrはダイナミックに生成されたオブジェクトを保持する。
* ダイナミックに生成されたオブジェクトを保持するstd::shared_ptrがスコープアウトすると、
  共有所有カウントはデクリメントされ、その値が0ならば保持しているオブジェクトはdeleteされる。
* std::shared_ptrを他のstd::shared_ptrに、
    * moveすることことで、保持中のオブジェクトの所有権を移動できる。
    * copyすることことで、保持中のオブジェクトの所有権を共有できる。
* 下記のようなコードはstd::shared_ptrの仕様が想定するセマンティクスに沿っておらず、
  未定義動作に繋がる。

```cpp
    // @@@ example/term_explanation/shared_ptr_ownership_ut.cpp #1:0 begin -1
```

こういった機能によりstd::shared_ptrはオブジェクトの共有所有を実現している。


### オブジェクトのライフタイム
オブジェクトは、以下のような種類のライフタイムを持つ。

* 静的に生成されたオブジェクトのライフタイム
* thread_localに生成されたオブジェクトのライフタイム
* newで生成されたオブジェクトのライフタイム
* スタック上に生成されたオブジェクトのライフタイム
* prvalue(「[expressionと値カテゴリ|rvalue](---)」参照)のライフタイム

なお、リファレンスの初期化をrvalueで行った場合、
そのrvalueはリファレンスがスコープを抜けるまで存続し続ける。


### クラスのレイアウト
クラス(やそのクラスが継承した基底クラス)が仮想関数を持たない場合、
そのクラスは、非静的なメンバ変数が定義された順にメモリ上に配置されたレイアウトを持つ
(CPUアーキテクチャに依存したパディング領域が変数間に挿入されることもある)。
このようなクラスは[POD](---)
(C++20では、[PODという用語は非推奨](https://cpprefjp.github.io/lang/cpp20/deprecate_pod.html)
となり、トリビアル型とスタンダードレイアウト型に用語が分割された)とも呼ばれ、
C言語の構造体のレイアウトと互換性を持つことが一般的である。

クラス(やそのクラスが継承したクラス)が仮想関数を持つ場合、
仮想関数呼び出しを行う(「[オーバーライドとオーバーロードの違い](---)」参照)
ためのメモリレイアウトが必要になる。
それを示すために、まずは下記のようにクラスX、Y、Zを定義する。


```cpp
    // @@@ example/term_explanation/class_layout_ut.cpp #0:0 begin
```

通常のC++コンパイラが作り出すX、Y、Zの概念的なメモリレイアウトは下記のようになる。

![layout](plant_uml/class_layout.png)

各クラスがvtblへのポインタを保持するため、このドキュメントで使用している[g++](---)では、
sizeof(X)は8ではなく16、sizeof(Y)は16ではなく24、sizeof(Z)は24ではなく32となる。

g++の場合、以下のオプションを使用し、クラスのメモリレイアウトをファイルに出力することができる。

```cpp
    // @@@ example/term_explanation/Makefile #0:0 begin
```

X、Y、Zのメモリレイアウトは以下の様に出力される。

```
    Vtable for X
    X::_ZTV1X: 5 entries
    0     (int (*)(...))0
    8     (int (*)(...))(& _ZTI1X)
    16    (int (*)(...))X::GetX
    24    (int (*)(...))X::~X
    32    (int (*)(...))X::~X

    Class X
       size=16 align=8
       base size=16 base align=8
    X (0x0x7f54bbc23a80) 0
        vptr=((& X::_ZTV1X) + 16)

    Vtable for Y
    Y::_ZTV1Y: 6 entries
    0     (int (*)(...))0
    8     (int (*)(...))(& _ZTI1Y)
    16    (int (*)(...))Y::GetX
    24    (int (*)(...))Y::~Y
    32    (int (*)(...))Y::~Y
    40    (int (*)(...))Y::GetY

    Class Y
       size=24 align=8
       base size=24 base align=8
    Y (0x0x7f54bbc3f000) 0
        vptr=((& Y::_ZTV1Y) + 16)
      X (0x0x7f54bbc23d20) 0
          primary-for Y (0x0x7f54bbc3f000)

    Vtable for Z
    Z::_ZTV1Z: 7 entries
    0     (int (*)(...))0
    8     (int (*)(...))(& _ZTI1Z)
    16    (int (*)(...))Z::GetX
    24    (int (*)(...))Z::~Z
    32    (int (*)(...))Z::~Z
    40    (int (*)(...))Z::GetY
    48    (int (*)(...))Z::GetZ

    Class Z
       size=32 align=8
       base size=32 base align=8
    Z (0x0x7f54bbc3f068) 0
        vptr=((& Z::_ZTV1Z) + 16)
      Y (0x0x7f54bbc3f0d0) 0
          primary-for Z (0x0x7f54bbc3f068)
        X (0x0x7f54bbc43060) 0
            primary-for Y (0x0x7f54bbc3f0d0)
```

このようなメモリレイアウトは、

```cpp
    // @@@ example/term_explanation/class_layout_ut.cpp #0:1 begin -1
```

のようなオブジェクト生成に密接に関係する。その手順を下記の疑似コードにより示す。

```cpp
    // ステップ1  メモリアロケーション
    void* ptr = malloc(sizeof(Z));

    // ステップ2  ZオブジェクトのX部分の初期化
    X* x_ptr = (X*)ptr;
    x_ptr->vtbl = &vtbl_for_X       // Xのコンストラクタ呼び出し処理
    x_ptr->x_ = 1;                  // Xのコンストラクタ呼び出し処理

    // ステップ3  ZオブジェクトのY部分の初期化
    Y* y_ptr = (Y*)ptr;
    y_ptr->vtbl = &vtbl_for_Y       // Yのコンストラクタ呼び出し処理
    y_ptr->y_ = 2;                  // Yのコンストラクタ呼び出し処理

    // ステップ4  ZオブジェクトのZ部分の初期化
    Z* z_ptr = (Z*)ptr;
    z_ptr->vtbl = &vtbl_for_Z       // Zのコンストラクタ呼び出し処理
    z_ptr->z_ = 3;                  // Zのコンストラクタ呼び出し処理
```

オブジェクトの生成がこのように行われるため、Xのコンストラクタ内で仮想関数GetX()を呼び出した場合、
その時のvtblへのポインタはXのvtblを指しており(上記ステップ2)、X::GetX()の呼び出しとなる
(Z::GetX()の呼び出しとはならない)。

なお、オブジェクトの解放は生成とは逆の順番で行われる。


## オブジェクトのコピー

### シャローコピー
シャローコピー(浅いコピー)とは、暗黙的、
もしくは=defaultによってコンパイラが生成するようなcopyコンストラクタ、
copy代入演算子が行うコピーであり、[ディープコピー](---)と対比的に使われる概念である。

以下のクラスShallowOKには、コンパイラが生成するcopyコンストラクタ、
copy代入演算子と同等なものを定義したが、これは問題のないシャローコピーである
(が、正しく自動生成される関数を実装すると、メンバ変数が増えた際にバグを生み出すことがあるため、
実践的にはこのようなことはすべきではない)。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp #0:0 begin
```

コンストラクタでポインタのようなリソースを確保し、
デストラクタでそれらを解放するようなクラスの場合、シャローコピーは良く知られた問題を起こす。

下記のShallowNGはその例である。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp #1:0 begin
```

シャローコピーにより、メンバで保持していたポインタ(ポインタが指しているオブジェクトではない)
がコピーされてしまうため、下記のコード内のコメントで示した通り、
メモリリークや2重解放を起こしてしまう。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp #1:1 begin -1
```

### ディープコピー
ディープコピーとは、[シャローコピー](---)が発生させる問題を回避したコピーである。

以下に例を示す。

```cpp
    // @@@ example/term_explanation/deep_shallow_copy_ut.cpp #2:0 begin
```

上記クラスのDeepは、copyコンストラクタ、copy代入演算子でポインタをコピーするのではなく、
ポインタが指しているオブジェクトを複製することにより、シャローコピーの問題を防ぐ。


### スライシング
オブジェクトのスライシングとは、

* クラスBaseとその派生クラスDerived
* クラスDerivedのインスタンスd1、d2(解説のために下記例ではd0も定義)
* d2により初期化されたBase&型のd2_ref(クラスBase型のリファレンス)

が宣言されたとした場合、 

```cpp
    d2_ref = d1;    // オブジェクトの代入
```

を実行した時に発生するようなオブジェクトの部分コピーのことである
(この問題はリファレンスをポインタに代えた場合にも起こる)。

以下のクラスと単体テストはこの現象を表している。

```cpp
    // @@@ example/term_explanation/slice_ut.cpp #0:0 begin
```

copy代入演算子(=)によりコピーが行われた場合、=の両辺のオブジェクトは等価になるべきだが
(copy代入演算子をオーバーロードした場合も、そうなるように定義すべきである)、
スライシングが起こった場合、そうならないことが問題である(「[等価性のセマンティクス](---)」参照)。

下記にこの現象の発生メカニズムについて解説する。

1. 上記クラスBase、Derivedのメモリ上のレイアウトは下記のようになる。

![スライシングクラス](plant_uml/slicing_class.png)

2. 上記インスタンスd0、d1、d2、d2_refのメモリ上のレイアウトは下記のようになる。

![スライシング初期化](plant_uml/slicing_init.png)

3. d2 = d0をした場合の状態は下記のようになる。

![スライシング問題なし](plant_uml/slicing_normal.png)

4. 上記の状態でd2_ref = d1をした場合の状態は下記のようになる。

![スライシング問題](plant_uml/slicing_slicing.png)

d2.name1\_の値が元のままであるが(これがスライシングである)、その理由は下記の疑似コードが示す通り、
「d2_refの表層型がクラスBaseであるためd1もクラスBase(正確にはBase型へのリファレンス)へ変換された後、
d2_refが指しているオブジェクト(d2)へコピーされた」からである。

```cpp
    d2_ref.Base::operator=(d1);   // Base::operator=(Base const&)が呼び出される
```

次に示すのは、
「オブジェクトの配列をその基底クラスへのポインタに代入し、
そのポインタを配列のように使用した場合に発生する」スライシングと類似の現象である。

```cpp
    // @@@ example/term_explanation/slice_ut.cpp #1:0 begin
```

![スライシング配列](plant_uml/slicing_array.png)

## name lookupと名前空間
ここではname lookupとそれに影響を与える名前空間について解説する。

### ルックアップ
このドキュメントでのルックアップとは[name lookup](---)を指す。

### name lookup
[name lookup](https://en.cppreference.com/w/cpp/language/lookup)
とはソースコードで名前が検出された時に、その名前をその宣言と関連付けることである。
以下、name lookupの例を上げる。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #0:0 begin
```

以下のコードでの関数呼び出しf()のname lookupは、


```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #1:0 begin -1
```

1. NS_LUをその前方で宣言された名前空間と関連付けする
2. f()呼び出しをその前方の名前空間NS_LUで宣言された関数fと関連付ける

という手順で行われる。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #0:1 begin
```

以下のコードでの関数呼び出しg()のname lookupは、


```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #1:1 begin -1
```

1. NS_LUをその前方で宣言された名前空間と関連付けする
2. 名前空間NS_LU内で宣言された複数のgを見つける
3. g()呼び出しを、
   すでに見つけたgの中からベストマッチしたg(T const (&)[N])と関連付ける

という手順で行われる。

下記記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #2:0 begin
```

以下のコードでの関数呼び出しToString()のname lookupは、

```cpp
    // @@@ example/term_explanation/name_lookup_ut.cpp #2:1 begin -1
```

1. ToString()呼び出しの引数xの型Xが名前空間NS_LUで定義されているため、
   ToStringを探索する名前空間にNS_LUを組み入れる(「[関連名前空間](---)」参照)
2. ToString()呼び出しより前方で宣言されたグローバル名前空間とNS_LUの中から、
   複数のToStringの定義を見つける
3. ToString()呼び出しを、
   すでに見つけたToStringの中からベストマッチしたNS_LU::ToStringと関連付ける

という手順で行われる。


### two phase name lookup
[two phase name lookup](https://en.cppreference.com/w/cpp/language/two-phase_lookup)
とはテンプレートをインスタンス化するときに使用される、下記のような2段階でのname lookupである。

1. テンプレート定義内でname lookupを行う(通常のname lookupと同じ)。
   この時、テンプレートパラメータに依存した名前
   ([dependent_name](https://en.cppreference.com/w/cpp/language/dependent_name))は
   name lookupの対象外となる(name lookupの対象が確定しないため)。
2. 1の後、テンプレートパラメータを展開した関数内で、
   [関連名前空間](---)の宣言も含めたname lookupを行う。

以下の議論では、

* 上記1のname lookupを1st name lookup
* 上記2のname lookupを2nd name lookup

と呼ぶことにする。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:0 begin
```

以下のコードでのTypeNameのインスタンス化に伴うname lookupは、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:1 begin -1
```

1. TypeName()呼び出しの引数xの型Xが名前空間NS_TPLUで宣言されているため、
   NS_TPLUをTypeNameを探索する[関連名前空間](---)にする。
2. TypeName()呼び出しより前方で宣言されたグローバル名前空間とNS_TPLUの中からTypeNameを見つける。
3. TypeNameは関数テンプレートであるためtwo phase lookupが以下のように行われる。
    1. TypeName内でのHeader(int{})の呼び出しは、1st name lookupにより、
       Header(long)の宣言と関連付けられる。
       Header(int)はHeader(long)よりもマッチ率が高い、
       TypeNameの定義より後方で宣言されているため、name lookupの対象外となる。
    2. TypeName内でのToType(t)の呼び出しに対しては、2nd name lookupが行われる。
       このためTypeName定義より前方で宣言されたグローバル名前空間と、
       tの型がNS_TPLU::Xであるため[関連名前空間](---)となったNS_TPLUがname lookupの対象となるが、
       グローバル名前空間内のToTypeは、
       NS_TPLU内でTypeNameより前に宣言されたtemplate<> ToTypeによって[name-hiding](---)が起こり、
       TypeNameからは非可視となるためname lookupの対象から外れる。
       このため、ToType(t)の呼び出しは、NS_TPLU::ToType(X const&)の宣言と関連付けられる。

という手順で行われる。

上と同じ定義、宣言がある場合の以下のコードでのTypeNameのインスタンス化に伴うname lookupは、

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:2 begin -1
```

1. NS_TPLUを名前空間と関連付けする
   (引数の型がintなのでNS_TPLUは[関連名前空間](---)とならず、NS_TPLUを明示する必要がある)。
2. TypeName()呼び出しより前方で宣言されたNS_TPLUの中からTypeNameを見つける。
3. TypeNameは関数テンプレートであるためtwo phase lookupが以下のように行われる。
    1. TypeName内でのHeader(int{})の呼び出しは、1st name lookupにより、
       前例と同じ理由で、Header(long)の宣言と関連付けられる。
    2. TypeName内でのToType(t)の呼び出しに対しては、2nd name lookupが行われる。
       tの型がintであるためNS_TPLUは[関連名前空間](---)とならず、通常のname lookupと同様に
       ToType(t)の呼び出し前方のグローバル名前空間とNS_TPLUがname lookupの対象になるが、
       グローバル名前空間内のToTypeは、
       NS_TPLU内でTypeNameより前に宣言されたtemplate<> ToTypeによって[name-hiding](---)が起こり、
       TypeNameからは非可視となるためname lookupの対象から外れる。
       また、ToType(int const&)は、TypeNameの定義より後方で宣言されているため、
       name lookupの対象外となり、
       その結果、ToType(t)の呼び出しは、NS_TPLU内のtemplate<> ToTypeの宣言と関連付けられる。

という手順で行われる。

以上の理由から、先に示した例でのToTypeの戻り値は"X"となり、
後に示した例でのToTypeの戻り値は"unknown"となる。
これはtwo phase lookupの結果であり、
two phase lookupが実装されていないコンパイラ(こういったコンパイラは存在する)では、
結果が異なるため注意が必要である
(本ドキュメントではこのような問題をできる限り避けるために、
サンプルコードを[g++](---)と[clang++](---)でコンパイルしている)。

以下に、two phase lookupにまつわるさらに驚くべきコード例を紹介する。
上と同じ定義、宣言がある場合の以下のコードの動作を考える。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:3 begin -1
```

NS_TPLU::TypeName(int{})のintをlongにしただけなので、この単体テストはパスしないが、
この単体テストコードの後(実際にはこのファイルのコンパイル単位の中のNS_TPLU内で、
且つtemplate<> ToTypeの宣言の後方であればどこでもよい)
に以下のコードを追加するとパスしてしまう。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #0:4 begin
```

この理由は、関数テンプレート内での2nd name lookupで選択された名前が関数テンプレートであった場合、
その特殊化の検索範囲はコンパイル単位内になることがあるからである
([template_specialization](https://en.cppreference.com/w/cpp/language/template_specialization)
によるとこの動作は未定義のようだが、
[g++](---)/[clang++](---)両方ともこのコードを警告なしでコンパイルする)。

TypeName(long{})内でのtwo phase name lookupは、TypeName(int{})とほぼ同様に進み、
template<> ToTypeの宣言を探し出すが、
さらに前述したようにこのコンパイル単位のNS_TPLU内からその特殊化も探し出す。
その結果、ToType(t)の呼び出しは、NS_TPLU内のtemplate<> ToType\<long>の定義と関連付けられる。

以上の議論からわかる通り、関数テンプレートとその特殊化の組み合わせは、
そのインスタンス化箇所(この場合単体テストコード内)の後方から、
name lookupでバインドされる関数を変更することができるため、
極めて分かりづらいコードを生み出す。ここから、

* 関数テンプレートとその特殊化はソースコード上なるべく近い位置で宣言するべきである
* STL関数テンプレートの特殊化は行うべきではない

という教訓が得られる。

なお、関数とその関数オーバーロードのname lookupの対象は、呼び出し箇所前方の宣言のみであるため、
関数テンプレートToType(T const& t)の代わりに、関数ToType(...)を使うことで、
上記問題は回避可能である。

次に示す例は、一見2nd name lookupで関連付けされるように見える関数ToType(NS_TPLU2::Y const&)が、
実際には関連付けされないコードである。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #1:1 begin
```

これは先に示したNS_TPLU::Xの例と極めて似ている。本質的な違いは、
TypeNameやToTypeがグローバル名前空間で宣言されていることのみである。
だが、下記の単体テストで示す通り、
TypeName内でのname lookupで関数オーバーライドToType(NS_TPLU2::Y const&)が選択されないのである。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #1:2 begin -1
```

ここまでの現象を正確に理解するには、
「two phase lookupの対象となる宣言」を下記のように、より厳密に認識する必要がある。

* TypeNameの中で行われる1st name lookupの対象となる宣言は下記の積集合である。
    * TypeNameと同じ名前空間内かグローバル名前空間内の宣言
    * TypeName定義位置より前方の宣言

* TypeNameの中で行われる2nd name lookupの対象となる宣言は下記の和集合である。
    * 1st name lookupで使われた宣言
    * TypeName呼び出しより前方にある[関連名前空間](---)内の宣言

この認識に基づくNS_TPLU2::Yに対するグローバルなTypeName内でのtwo phase name lookupは、

1. TypeName内に1st name lookupの対象がないため何もしない。
2. TypeName内の2nd name lookupに使用される[関連名前空間](---)NS_TPLU2は、
   ToType(NS_TPLU2::Y const&)の宣言を含まないため、この宣言は2nd name lookupの対象とならない。
   その結果、ToType(t)の呼び出しは関数テンプレートToType(T const&)と関連付けられる。

という手順で行われる。

以上が、TypeNameからToType(NS_TPLU2::Y const&)が使われない理由である。

ここまでで示したようにtwo phase name lookupは理解しがたく、
理解したとしてもその使いこなしはさらに難しい。

次のコードは、この難解さに翻弄されるのが現場のプログラマのみではないことを示す。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #2:0 begin
```

上記の宣言、定義があった場合、operator+の単体テストは以下のようになる。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #2:1 begin -2
```

このテストは当然パスするが、次はどうだろう？

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #2:2 begin -2
```

これまでのtwo phase name lookupの説明では、
operator+(NS_TPLU2::Y const& y, int i)はTypeNum内でのname lookupの対象にはならないため、
このテストはエラーとならなければならないが、[g++](---)ではパスしてしまう。
2nd name lookupのロジックにバグがあるようである。

有難いことに、[clang++](---)では仕様通りこのテストはエラーとなり、
当然ながら以下のテストはパスする(つまり、g++ではエラーする)。

```cpp
    // @@@ example/term_explanation/two_phase_name_lookup_ut.cpp #2:3 begin -2
```

なお、TypeNum内のコードである

```cpp
    return t + 0;
```

を下記のように変更することで

```cpp
    return operator+(t, 0);
```

g++のname lookupはclang++と同じように動作するため、
記法に違和感があるものの、この方法はg++のバグのワークアランドとして使用できる。

また、operator+(NS_TPLU2::Y const& y, int i)をNS_TPLU2で宣言することで、
g++ではパスしたテストをclang++でもパスさせられるようになる(これは正しい動作)。
これにより、型とその2項演算子オーバーロードは同じ名前空間で宣言するべきである、
という教訓が得られる。

以上で見てきたようにtwo phase name lookupは、現場プログラマのみではなく、
コンパイラを開発するプログラマをも混乱させるほど難解ではあるが、
STLを含むテンプレートメタプログラミングを支える重要な機能であるため、
C++プログラマには、最低でもこれを理解し、出来れば使いこなせるようになってほしい。


### 実引数依存探索
実引数依存探索とは、argument-dependent lookupの和訳語であり、
通常はその略語である[ADL](---)と呼ばれる。

### ADL
ADLとは、関数の実引数の型が宣言されている名前空間(これを[関連名前空間](---)と呼ぶ)内の宣言が、
その関数の[name lookup](---)の対象になることである。

下記のようなコードがあった場合、

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp #0:0 begin
```

以下のコードでのToStringの呼び出しに対するのname lookupは、

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp #0:1 begin -1
```

* ToStringの呼び出しより前方で行われているグローバル名前空間内の宣言
* ToStringの呼び出しより前方で行われているNS_ADL内の宣言

の両方を対象として行われる。
NS_ADL内の宣言がToStringの呼び出しに対するのname lookupの対象になる理由は、
ToStringの呼び出しに使われている実引数aの型AがNS_ADLで宣言されているからである。
すでに述べたようにこれをADLと呼び、この場合のNS_ADLを[関連名前空間](---)と呼ぶ。

ADLは思わぬname lookupによるバグを誘発することもあるが、
下記コードを見れば明らかなように、また、
多くのプログラマはそれと気づかずに使っていることからもわかる通り、
コードをより自然に、より簡潔に記述するための重要な機能となっている。

```cpp
    // @@@ example/term_explanation/name_lookup_adl_ut.cpp #0:2 begin -1
```

### 関連名前空間
関連名前空間(associated namespace)とは、
[ADL](---)(実引数依存探索)によってname lookupの対象になった宣言を含む名前空間のことである。


### SFINAE
[SFINAE](https://cpprefjp.github.io/lang/cpp11/sfinae_expressions.html)
(Substitution Failure Is Not An Errorの略称、スフィネェと読む)とは、
「テンプレートのパラメータ置き換えに失敗した([ill-formed](---)になった)際に、
即時にコンパイルエラーとはせず、置き換えに失敗したテンプレートを
[name lookup](---)の候補から除外する」
という言語機能である。

### name-hiding
name-hidingとは
「前方の識別子が、その後方に同一の名前をもつ識別子があるために、
[name lookup](---)の対象外になる」現象一般をを指す通称である
([namespace](https://en.cppreference.com/w/cpp/language/namespace)参照)。

まずは、クラスとその派生クラスでのname-hidingの例を示す。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #0:0 begin
```

上記の関数fは一見オーバーロードに見えるが、そうではない。下記のコードで示したように、
Base::f()には、修飾しない形式でのDerivedクラス経由のアクセスはできない。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #0:1 begin
```

これは前述したように、
Base::fがその後方にあるDerived::f(int)によりname-hidingされたために起こる現象である
(name lookupによる探索には識別子が使われるため、シグネチャの違いはname-hidingに影響しない)。

下記のように[using宣言](---)を使用することで、
修飾しない形式でのDerivedクラス経由のBase::f()へのアクセスが可能となる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/name_hiding.cpp #1:1 begin -1
```

下記コードは、名前空間でも似たような現象が起こることを示している。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #2:0 begin
```

この問題に対しては、下記のようにf(int)の定義位置を後方に移動することで回避できる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #2:1 begin
```

また、先述のクラスでの方法と同様にusing宣言を使い、下記のようにすることもできる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #2:2 begin
```

当然ながら、下記のようにf()の呼び出しを::で修飾することもできる。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #2:3 begin
```

修飾の副作用として「[two phase name lookup](---)」の例で示したような
[ADL](---)を利用した高度な静的ディスパッチが使用できなくなるが、
通常のソフトウェア開発では、ADLが必要な場面は限られているため、
デフォルトでは名前空間を使用して修飾を行うことにするのが、
無用の混乱をさけるための安全な記法であると言えるだろう。

次に、そういった混乱を引き起こすであろうコードを示す。

```cpp
    // @@@ example/term_explanation/name_hiding.cpp #3:0 begin
```

NS_B_Inner::g()内のf(int)の呼び出しはコンパイルできるが、
name-hidingが原因で、NS_B_Inner::h()内のf(int)の呼び出しはコンパイルできず、
名前空間で修飾することが必要になる。
一方で、ADLの効果で名前空間での修飾をしていないf(S_in_B)の呼び出しはコンパイルできる。

全チームメンバがこういったname lookupを正しく扱えると確信できないのであれば、
前述の通り、デフォルトでは名前空間を使用して修飾を行うのが良いだろう。

### ドミナンス
[ドミナンス(Dominance、支配性)](https://en.wikipedia.org/wiki/Dominance_(C%2B%2B))とは、
「探索対称の名前が継承の中にも存在するような場合の[name lookup](---)の仕様の一部」
を指す慣用句である。

以下に

* [ダイヤモンド継承を含まない場合](---)
* [ダイヤモンド継承かつそれが仮想継承でない場合](---)
* [ダイヤモンド継承かつそれが仮想継承である場合](---)

のドミナンスについてのコードを例示する。

この例で示したように、[ダイヤモンド継承](---)を通常の継承で行うか、
[仮想継承](---)で行うかでは結果が全く異なるため、注意が必要である。

#### ダイヤモンド継承を含まない場合

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #0:1 begin -1
```

この[name lookup](---)については、[name-hiding](---)で説明した通りである。

#### ダイヤモンド継承かつそれが仮想継承でない場合

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #1:0 begin
```

上記コードはコードブロック内のコメントのようなメッセージが原因でコンパイルできない。

Derived_0のドミナンスにより、DerivedDerived::gはDerived_0::fを呼び出すように見えるが、
もう一つの継承元であるDerived_1が導入したDerived_1::f(実際には、Derived_1::Base::f)があるため、
Derived_1によるドミナンスも働き、その結果として、呼び出しが曖昧(ambiguous)になることで、
このような結果となる。

#### ダイヤモンド継承かつそれが仮想継承である場合

```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/term_explanation/dominance_ut.cpp #2:1 begin -1
```

これまでと同様にDerived_0のドミナンスによりBase::fは[name-hiding](---)されることになる。
この時、Derived_0、Derived_1がBaseから[仮想継承](---)した効果により、
この継承ヒエラルキーの中でBaseは１つのみ存在することになるため、
Derived_1により導入されたBase::fも併せて[name-hiding](---)される。
結果として、曖昧性は排除され、コンパイルエラーにはならず、このような結果となる。

### ダイヤモンド継承
ダイヤモンド継承(Diamond Inheritance)とは、以下のような構造のクラス継承を指す。

* 基底クラス(Base)が一つ存在し、その基底クラスから二つのクラス(Derived_0、Derived_1)が派生する。
* Derived_0とDerived_1からさらに一つのクラス(DerivedDerived)が派生する。
  したがって、DerivedDerivedはBaseの孫クラスとなる。

この継承は、多重継承の一形態であり、クラス図で表すと下記のようになるため、
ダイヤモンド継承と呼ばれる。

![diamond inheritance](plant_uml/diamond_inheritance.png)

ダイヤモンド継承は、
[仮想継承](---)(virtual inheritance)を使ったものと、使わないものに分類できる。

[仮想継承](---)を使わないダイヤモンド継承のコードを以下に示す。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #0:1 begin -1
```

これからわかるように、DerivedDerivedインスタンスの中に2つのBaseインスタンスが存在する。

下記コードは、それが原因で名前解決が曖昧になりコンパイルできない。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #0:2 begin -1
```

この問題に対処するには、クラス名による修飾が必要になるが、
Baseインスタンスが2つ存在するため、下記に示すようなわかりづらいバグの温床となる。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #0:3 begin -1
```

次に示すのは、[仮想継承](---)を使用したダイヤモンド継承の例である。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #1:1 begin -1
```

仮想継承の効果で、DerivedDerivedインスタンスの中に存在するBaseインスタンスは1つになるため、
上で示した仮想継承を使わないダイヤモンド継承での問題は解消される
(が、[仮想継承](---)による別の問題が発生する)。

```cpp
    // @@@ example/term_explanation/diamond_inheritance_ut.cpp #1:2 begin -1
```

### 仮想継承
下記に示した継承方法を仮想継承、仮想継承の基底クラスを仮想基底クラスと呼ぶ。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:0 begin
```

仮想継承は、[ダイヤモンド継承](---)の基底クラスのインスタンスを、
その継承ヒエラルキーの中で1つのみにするための言語機能である。

仮想継承の独特の動作を示すため、
上記コードに加え、仮想継承クラス、通常の継承クラス、
それぞれを通常の継承したクラスを下記のように定義する。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:1 begin
```

この場合、継承ヒエラルキーに仮想継承を含むクラスと、含まないクラスでは、
以下に示したような違いが発生する。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:2 begin -1
```

これは、「仮想継承クラスを継承したクラスが、
仮想継承クラスの基底クラスのコンストラクタを明示的に呼び出さない場合、
引数なしで呼び出せる基底クラスのコンストラクタが呼ばれる」仕様に起因している
(引数なしで呼び出せる基底クラスのコンストラクタがない場合はコンパイルエラー)。
以下では、これを「仮想継承のコンストラクタ呼び出し」仕様と呼ぶことにする。

仮想継承クラスが、基底クラスのコンストラクタを呼び出したとしても、この仕様が優先されるため、
上記コードのような動作となる。

これを通常の継承クラスと同様な動作にするには、下記のようにしなければならない。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:3 begin
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #0:4 begin -1
```
「仮想継承のコンストラクタ呼び出し」仕様は、
[ダイヤモンド継承](---)での基底クラスのコンストラクタ呼び出しを一度にするために存在する。

もし、この機能がなければ、下記のコードでの基底クラスのコンストラクタ呼び出しは2度になるため、
デバッグ困難なバグが発生してしまうことは容易に想像できるだろう。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #1:1 begin -1
```

基底クラスのコンストラクタ呼び出しは、下記のコードのようにした場合でも、
単体テストが示すように、一番最初に行われる。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #2:1 begin -1
```

このため、基底クラスのコンストラクタ呼び出しは下記のような順番で行うべきである。

```cpp
    // @@@ example/term_explanation/virtual_inheritance_ut.cpp #3:0 begin
```

### 仮想基底
仮想基底(クラス)とは、[仮想継承](---)の基底クラス指す。

### using宣言
using宣言とは、"using XXX::func"のような記述である。
この記述が行われたスコープでは、この記述後の行から名前空間XXXでの修飾をすることなく、
funcが使用できる。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:1 begin
```

### usingディレクティブ
usingディレクティブとは、"using namespace XXX"のような記述である。
この記述が行われたスコープでは、下記例のように、この記述後から名前空間XXXでの修飾をすることなく、
XXXの識別子が使用できる。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:2 begin
```

より多くの識別子が名前空間の修飾無しで使えるようになる点において、
[using宣言](---)よりも危険であり、また、
下記のように[name-hiding](---)された識別子の導入には効果がない。

```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/namespace_ut.cpp #0:3 begin
```

従って、usingディレクティブの使用は避けるべきである。


## expressionと値カテゴリ
ここでは、expression(式)の値カテゴリや、それに付随した機能についての解説を行う。

### expression

C++においてexpression、lvalue、rvalue、xvalue、glvalue、prvalueは以下のように定められている。

* [expression](https://ja.cppreference.com/w/cpp/language/expressions)(式)
  とは「演算子とそのオペランドの並び」である(オペランドのみの記述も式である)。
  演算子とは以下のようなものである。
    * 四則演算、代入(a = b、a += b ...)、インクリメント、比較、論理式
    * 明示的キャストや型変換
    * メンバアクセス(a.b、a->b、a[x]、 \*a、&a ...)
    * 関数呼び出し演算子(f(...))、sizeof、decltype等

* expressionは、以下のいずれかに分類される。lvalueでないexpressionがrvalueである。
    * lvalue
    * rvalue

* lvalueとは、関数もしくはオブジェクトを指す。

* rvalueは、以下のいずれかに分類される。
    * xvalue
    * prvalue

* xvalueとは以下のようなものである。
    * 戻り値の型がT&&(Tは任意の型)である関数の呼び出し式(std::move(x))
    * オブジェクトへのT&&へのキャスト式(static_cast<char&&>(x))
    * aを配列のrvalueとした場合のa[N]や、
      cをクラス型のrvalueとした場合のc.m(mはaの非staticメンバ)等

* prvalueとは、オブジェクトやビットフィールドを初期化する、
  もしくはオペランドの値を計算する式であり、以下のようなものである。
    * 文字列リテラルを除くリテラル
    * 戻り値の型が非リファレンスである関数呼び出し式、
      または前置++と前置--を除くオーバーロードされた演算子式(path.string()、str1 + str2、it++ ...)
    * 組み込み型インスタンスaのa++、a\-\-(++a、\-\-aはlvalue)
    * 組み込み型インスタンスa、bに対する
      a + b、 a % b、 a & b、 a && b、 a || b、 !a、 a < b、 a == b等

* glvalueは、以下のいずれかに分類される。
    * lvalue
    * xvalue

![expression分類](plant_uml/rvalue.png)

ざっくりと言えば、lvalueとは代入式の左辺になり得る式、rvalueとは代入式の左辺にはなり得ない式である。
T const&は左辺になり得ないが、lvalueである。rvalueリファレンス(T&&)もlvalueであるため、
rvalueであることとrvalueリファレンスであることとは全く異なる。

xvalueとは、多くの場合、「std::move()の呼び出し式のことである」と考えても差し支えない。

prvalueとは、いわゆるテンポラリオブジェクトのことであるため
(下記のstd::string()で作られるようなオブジェクト)、名前はない。
また、アドレス演算子(&)のオペランドになれない。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:0 begin
```

C++11でrvalueの概念の整理やstd::move()の導入が行われた目的はプログラム実行速度の向上である。

* lvalueからの代入
* rvalueからの代入
* std::move(lvalue)からの代入

の処理がどのように違うのかを見ることでrvalueの効果について説明する。

1. 下記コードにより「lvalueからの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:1 begin -1
    ```

    * ステップ1。
      str0、str1がそれぞれ初期化される
      ("hehe"を保持するバッファが生成され、それをstr1オブジェクトが所有する)。

    * ステップ2。
      st1が所有している文字列バッファと等価のバッファが作られ
      (文字列バッファ用のメモリをnewし、文字列を代入)、
      str0がそれを所有する。従って、"hehe"を保持するバッファが2つできる。
      この代入をcopy代入と呼ぶ。

![lvalueからの代入](plant_uml/rvalue_from_lvalue.png)


2. 下記コードにより「rvalueからの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:2 begin -1
    ```

    * ステップ1。str0、「std::string()により作られたテンポラリオブジェクト」がそれぞれ初期化される
      ("hehe"を保持するバッファが生成され、それをテンポラリオブジェクトが所有する)。

    * ステップ2。
      "hehe"を保持する文字列バッファをもう1つ作る代わりに、
      テンポラリオブジェクトが所有している文字列バッファをstr0の所有にする。
      この代入をmove代入と呼ぶ。

    * ステップ3。
      テンポラリオブジェクトが解体されるが、
      文字列バッファはstr0の所有であるためdeleteする必要がなく、実際には何もしない。
      move代入によって、文字列バッファの生成と破棄の回数がそれぞれ1回少なくなったため、
      実行速度は向上する(通常、new/deleteの処理コストは高い)。

![rvalueサポート](plant_uml/rvalue_from_rvalue.png)

3. 下記コードにより「std::move(lvalue)からの代入」を説明する。

    ```.cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:3 begin -1
    ```

    * ステップ1。「lvalueからの代入」のステップ1と同じである。

    * ステップ2。
      std::move()の効果により(実際にはrvalueリファレンスへのキャストが行われるだけなので、
      実行時速度に影響はない)、"hehe"を保持する文字列バッファをもう1つ作る代わりに、
      str1が所有している文字列バッファをstr0の所有にする。
      この代入もmove代入と呼ぶ。
      この動作は「rvalueからの代入」と同じであり、同様に速度が向上するが、その副作用として、
      str1.size() == 0となる。

![std::move(lvalue)からの代入](plant_uml/rvalue_from_move.png)


エッセンシャルタイプがTであるlvalue、xvalue、prvalueに対して
(例えば、std::string const&のエッセンシャルタイプはstd::stringである)、
decltypeの算出結果は下表のようになる。

|decltype           |算出された型|
|:------------------|:-----------|
|decltype(lvalue)   |T           |
|decltype((lvalue)) |T&          |
|decltype(xvalue)   |T&&         |
|decltype((xvalue)) |T&&         |
|decltype(prvalue)  |T           |
|decltype((prvalue))|T           |

この表の結果を使用した下記の関数型マクロ群により式を分類できる。
定義から明らかな通り、これらは
[テンプレートメタプログラミング](https://ja.wikipedia.org/wiki/%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%A1%E3%82%BF%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0)
に有効に活用できる。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #0:4 begin
```

### lvalue
「[expression](---)」を参照せよ。

### rvalue
「[expression](---)」を参照せよ。

#### xvalue
「[expression](---)」を参照せよ。

#### prvalue
「[expression](---)」を参照せよ。

### rvalue修飾
下記GetString0()のような関数が返すオブジェクトの内部メンバに対するハンドルは、
オブジェクトのライフタイム終了後にもアクセスすることができるため、
そのハンドルを通じて、
ライフタイム終了後のオブジェクトのメンバオブジェクトにもアクセスできてしまう。

ライフタイム終了後のオブジェクトにアクセスすることは未定義動作であり、
特にそのオブジェクトがrvalueであった場合、さらにその危険性は高まる。

こういったコードに対処するためのシンタックスが、lvalue修飾、rvalue修飾である。

下記GetString1()、GetString3()、GetString4()のようにメンバ関数をlvalue修飾やrvalue修飾することで、
rvalueの内部ハンドルを返さないようにすることが可能となり、上記の危険性を緩和することができる。

```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/term_explanation/rvalue_lvalue_ut.cpp #1:1 begin -1
```

### lvalue修飾
[rvalue修飾](---)を参照せよ。


### リファレンス修飾
[rvalue修飾](---)と[lvalue修飾](---)とを併せて、リファレンス修飾と呼ぶ。

### decltype
decltypeはオペランドに[expression](---)を取り、その型を算出する機能である。
decltype(auto)はそのオペランドの省略形である。
autoとdecltypeでは、以下に示す通りリファレンスの扱いが異なることに注意する必要がある。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #0:0 begin -1
```

decltypeは、テンプレートプログラミングに多用されるが、
クロージャ型(「[C++その他|ラムダ式](---)」参照)
のような記述不可能な型をオブジェクトから算出できるため、
下記例のような場合にも有用である。

```cpp
    // @@@ example/term_explanation/decltype_ut.cpp #0:1 begin
```

## リファレンス
ここでは、C++11から導入された

* [ユニバーサルリファレンス](---)
* [リファレンスcollapsing](---)

について解説する。

### ユニバーサルリファレンス
関数テンプレートの型パラメータや型推論autoに&&をつけて宣言された変数を、
ユニバーサルリファレンスと呼ぶ(C++17から「forwardingリファレンス」という正式名称が与えられた)。
ユニバーサルリファレンスは一見rvalueリファレンスのように見えるが、
下記に示す通り、lvalueにもrvalueにもバインドできる。

```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:0 begin
```
```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:1 begin -1
```

下記のコードはジェネリックラムダ(「[C++その他|ラムダ式](---)」参照)
の引数をユニバーサルリファレンスにした例である。

```cpp
    // @@@ example/term_explanation/universal_ref_ut.cpp #0:2 begin -1
```

通常、ユニバーサルリファレンスはstd::forwardと組み合わせて使用される。


### forwardingリファレンス
「[ユニバーサルリファレンス](---)」を参照せよ。

### perfect forwarding
perfect forwarding とは、引数の[expressionと値カテゴリ|rvalue](---)性や
[expressionと値カテゴリ|lvalue](---)性を損失することなく、
その引数を別の関数に転送する技術のことを指す。
通常は、[ユニバーサルリファレンス](---)である関数の仮引数をstd::forwardを用いて、
他の関数に渡すことで実現される。

### リファレンスcollapsing
Tを任意の型とし、TRを下記のように宣言した場合、

```cpp
    using TR = T&;
```

下記のようなコードは、C++03ではコンパイルエラーとなったが、
C++11からはエラーとならず、TRRはT&となる。

```cpp
    using TRR = TR&;
```

2つの&を1つに折り畳む、このような機能をリファレンスcollapsingと呼ぶ。

下記はTをintとした場合のリファレンスcollapsingの動きを示している。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:0 begin -1
```

リファレンスcollapsingは、型エイリアス、型であるテンプレートパラメータ、decltypeに対して行われる。
詳細な変換則は、下記のようになる。

```
    T& &   -> T&
    T& &&  -> T&
    T&& &  -> T&
    T&& && -> T&&
```

下記のようなクラステンプレートを定義した場合、

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:1 begin
```

下記のコードにより、テンプレートパラメータに対するこの変換則を確かめることができる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:2 begin -1
```

この機能がないC++03では、

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:3 begin
```

ようなクラステンプレートに下記コードのようにリファレンス型を渡すとコンパイルエラーとなる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:5 begin -1
```

この問題を回避するためには下記のようなテンプレートの特殊化が必要になる。

```cpp
    // @@@ example/term_explanation/ref_collapsing_ut.cpp #0:4 begin
```

上記したようなクラステンプレートでのメンバエイリアスの宣言は、
[テンプレートメタプログラミング](https://ja.wikipedia.org/wiki/%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%A1%E3%82%BF%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0)
で頻繁に使用されるため、
このようなテンプレートの特殊化を不要にするリファレンスcollapsingは、
有用な機能拡張であると言える。

### danglingリファレンス
Dangling リファレンスとは、破棄後のオブジェクトを指しているリファレンスを指す。
このようなリファレンスにアクセスすると、未定義動作動作に繋がるに繋がる。

```cpp
    // @@@ example/term_explanation/dangling_ut.cpp #0:0 begin

    // @@@ example/term_explanation/dangling_ut.cpp #0:1 begin -1
```

### danglingポインタ
danglingポインタとは、[danglingリファレンス](---)と同じような状態になったポインタを指す。

## エクセプション安全性の保証
関数のエクセプション発生時の安全性の保証には以下の3つのレベルが規定されている。

### no-fail保証
「no-fail保証」を満たす関数はエクセプションをthrowしない。

### 強い保証
「強い保証」を満たす関数は、この関数がエクセプションによりスコープから外れた場合でも、
この関数が呼ばれなかった状態と同じ(プログラムカウンタ以外の状態は同じ)であることを保証する。
従って、この関数呼び出しは成功したか、完全な無効だったかのどちらかになる。

### 基本保証
「基本保障」を満たす関数は、この関数がエクセプションによりスコープから外れた場合でも、
メモリ等のリソースリークは起こさず、
オブジェクトは(変更されたかもしれないが)引き続き使えることを保証する。


## シンタックス、セマンティクス
直訳すれば、シンタックスとは構文論のことであり、セマンティクスとは意味論のことである。
この二つの概念の違いをはっきりと際立たせる有名な文を例示する。

```
    Colorless green ideas sleep furiously(直訳:無色の緑の考えが猛烈に眠る)
```

この文は構文的には正しい(シンタックスは問題ない)が、
意味不明である(セマンティクスは誤り)。

C++プログラミングにおいては、コンパイルできることがシンタックス的な正しさであり、例えば

* クラスや関数がその名前から想起できる責務を持っている
    * 「[単一責任の原則(SRP)](---)」を満たしている
    * [Accessor](---)を実装する関数の名前は、GetXxxやSetXxxになっている
    * コンテナクラスのメンバ関数beginやendは、
      そのクラスが保持する値集合の先頭や最後尾の次を指すイテレータを返す
    * 等

* 「[等価性のセマンティクス](---)」を守ってる
* 「[copyセマンティクス](---)」を守ってる
* 「[moveセマンティクス](---)」を守っている

等がセマンティクス的な正しさである。

セマンティクス的に正しいソースコードは読みやすく、保守性、拡張性に優れている。

### 等価性のセマンティクス
純粋数学での実数の等号(=)は、任意の実数x、y、zに対して、

| 律   |意味                     |
|------|-------------------------|
|反射律|x = x                    |
|対称律|x = yならばy = x         |
|推移律|x = y且つy = zならばx = z|

を満たしている。x = yが成立する場合、「xはyと等しい」もしくは「xはyと同一」であると言う。

C++における組み込みの==も純粋数学の等号と同じ性質を満たしている。
下記のコードは、その性質を表している。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #0:0 begin -2
```

しかし、下記のコード内のa、bは同じ値を持つが、
アドレスが異なるため同一のオブジェクトではないにもかかわらず、組み込みの==の値はtrueとなる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #0:1 begin -2
```

このような場合、aとbは等価であるという。同一ならば等価であるが、等価であっても同一とは限らない。

ポインタや配列をオペランドとする場合を除き、C++における組み込みの==は、
数学の等号とは違い、等価を表していると考えられるが、
上記した3つの律を守っている。従ってオーバーロードoperator==も同じ性質を守る必要がある。

組み込みの==やオーバーロードoperator==のこのような性質をここでは「等価性のセマンティクス」と呼ぶ。

クラスAを下記のように定義し、

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:0 begin
```

そのoperator==を下記のように定義した場合、

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:1 begin
```

単体テストは下記のように書けるだろう。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:2 begin -2
```

これは、一応パスするが(処理系定義の動作を前提とするため、必ず動作する保証はない)、
下記のようにすると、パスしなくなる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:3 begin -2
```

一般にポインタの等価性は、その値の同一性ではなく、
そのポインタが指すオブジェクトの等価性で判断されるべきであるが、
先に示したoperator==はその考慮をしていないため、このような結果になった。

次に、これを修正した例を示す。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #1:4 begin
```

ポインタをメンバに持つクラスのoperator==については、上記したような処理が必要となる。

次に示す例は、基底クラスBaseとそのoperator==である。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #2:0 begin
```

次の単体テストが示す通り、これ自体には問題がないように見える。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #2:1 begin -1
```

しかし、Baseから派生したクラスDerivedを

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #3:0 begin
```

のように定義すると、下記の単体テストで示す通り、等価性のセマンティクスが破壊される。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #3:1 begin -1
```

Derived用のoperator==を

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #4:0 begin
```

と定義しても、下記に示す通り部分的な効果しかない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #4:1 begin -1
```

この問題は、[C++その他|RTTI](---)を使った下記のようなコードで対処できる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #5:0 begin
```

下記に示す通り、このコードは、
[オープン・クローズドの原則(OCP)](---)にも対応した柔軟な構造を実現している。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #5:1 begin
```

前例では「両辺の型が等しいこと」が「等価であること」の必要条件となるが、
この要件が、すべてのoperator==に求められるわけではない。

次に示すのは、一見すると両辺の型が違うにもかかわらず、
等価性のセマンティクスを満たしている例である。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #6:0 begin -1
```

これは、文字列リテラルを第1引数に取るstd::stringのコンストラクタが非explicitであることによって、
文字列リテラルからstd::stringへの暗黙の型変換が起こるために成立する
(「[非explitなコンストラクタによる暗黙の型変換](---)」参照)。

以上で見てきたように、等価性のセマンティクスを守ったoperator==の実装には多くの観点が必要になる。

### copyセマンティクス
copyセマンティクスとは以下を満たすようなセマンティクスである。

* a = bが行われた後に、aとbが等価である。
* a = bが行われた前後でbの値が変わっていない。

従って、これらのオブジェクトに対して[等価性のセマンティクス](---)
を満たすoperator==が定義されている場合、
以下を満たすようなセマンティクスであると言い換えることができる。

* a = bが行われた後に、a == bがtrueになる。
* b == b_preがtrueの時に、a = bが行われた後でもb == b_preがtrueとなる。

下記に示す通り、std::stringはcopyセマンティクスを満たしている。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #7:0 begin -2
```

一方で、std::auto_ptrはcopyセマンティクスを満たしていない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #7:1 begin -2
```

この仕様は極めて不自然であり、std::auto_ptrはC++11で非推奨となり、C++17で規格から排除された。

下記の単体テストから明らかな通り、
「[等価性のセマンティクス](---)」で示した最後の例も、copyセマンティクスを満たしていない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #7:2 begin -2
```

原因は、copy代入で[オブジェクトのコピー|スライシング](---)が起こるためである。


### moveセマンティクス
moveセマンティクスとは以下を満たすようなセマンティクスである(operator==が定義されていると前提)。

* copy代入の実行コスト >= move代入の実行コスト
* a == bがtrueの時に、c = std::move(a)が行われた場合、
    * b == cがtrueになる。
    * a == cはtrueにならなくても良い(aはmove代入により破壊されるかもしれない)。

  必須ではないが、「aがポインタ等のリソースを保有している場合、move代入後には、
  そのリソースはcに移動している」ことが一般的である(「[expressionと値カテゴリ|rvalue](---)」参照)。

* [no-fail保証](---)をする(noexceptと宣言し、エクセプションをthrowしない)。

moveセマンティクスはcopy代入後に使用されなくなるオブジェクト(主にrvalue)
からのcopy代入の実行コストを下げるために導入されたため、
下記のようなコードは推奨されない。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #8:0 begin
```

下記のコードのようにメンバの代入もできる限りmove代入を使うことで、
パフォーマンスの良い代入ができる。

```cpp
    // @@@ example/term_explanation/semantics_ut.cpp #8:1 begin
```

## C++コンパイラ
本ドキュメントで使用するg++/clang++のバージョンは以下のとおりである。

### g++
```
    g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
    Copyright (C) 2021 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

### clang++
```
    Ubuntu clang version 14.0.0-1ubuntu1
    Target: x86_64-pc-linux-gnu
    Thread model: posix
    InstalledDir: /usr/bin
```

## C++その他
### オーバーライドとオーバーロードの違い
下記例では、Base::g()がオーバーロードで、Derived::f()がオーバーライドである
(Derived::g()はオーバーロードでもオーバーライドでもない(「[name-hiding](---)」参照))。


```cpp
    // @@@ example/term_explanation/override_overload_ut.cpp #0:0 begin
```

下記図の通り、

* BaseのインスタンスはBase用のvtblへのポインタを内部に持ち、
  そのvtblでBase::f()のアドレスを保持する。
* DerivedのインスタンスはDerived用のvtblへのポインタを内部に持ち、
  そのvtblでDerived::f()のアドレスを保持する。
* Base::g()、Base::g(int)、
  Derived::g()のアドレスはBaseやDerivedのインスタンスから辿ることはできない。

![vtbl](plant_uml/vtbl.png)

vtblとは仮想関数テーブルとも呼ばれる、仮想関数ポインタを保持するための上記のようなテーブルである
(「[クラスのレイアウト](---)」参照)。

Base::f()、Derived::f()の呼び出し選択は、オブジェクトの表層の型ではなく、実際の型により決定される。
Base::g()、Derived::g()の呼び出し選択は、オブジェクトの表層の型により決定される。

```cpp
    // @@@ example/term_explanation/override_overload_ut.cpp #0:1 begin -1
```

上記のメンバ関数呼び出し

```cpp
    d_ref.f() 
```

がどのように解釈され、Derived::f()が選択されるかを以下に疑似コードで例示する。

```cpp
    vtbl = d_ref.vtbl             // d_refの実態はDerivedなのでvtblはDerivedのvtbl

    member_func = vtbl->f         // vtbl->fはDerived::f()のアドレス

    (d_ref.*member_func)(&d_ref)  // member_func()の呼び出し
```

このようなメカニズムにより仮想関数呼び出しが行われる。


### 実引数/仮引数
引数(もしくは実引数、argument)、仮引数(parameter)とは下記のように定義される。

```cpp
    // @@@ example/term_explanation/argument.cpp #0:0 begin
```

### 範囲for文
範囲for文は下記例のコメントで示されたようなfor文であり、
begin()、end()によって表される範囲内のすべての要素に対して付属するブロックの処理を行う。

```cpp
    // @@@ example/term_explanation/range_for_ut.cpp #8:0 begin -2
```
```cpp
    // @@@ example/term_explanation/range_for_ut.cpp #8:1 begin -2
```

### ラムダ式
ラムダ式に関する言葉の定義と例を示す。

* ラムダ式とは、その場で関数オブジェクトを定義する式。
* クロージャ(オブジェクト)とは、ラムダ式から生成された関数オブジェクト。
* クロージャ型とは、クロージャオブジェクトの型。
* キャプチャとは、ラムダ式外部の変数をラムダ式内にコピーかリファレンスとして定義する機能。
* ラムダ式からキャプチャできるのは、ラムダ式から可視である自動変数と仮引数(thisを含む)。
* [C++その他|ジェネリックラムダ](---)とは、C++11のラムダ式を拡張して、
  パラメータにautoを使用(型推測)できるようにした機能。

```cpp
    // @@@ example/term_explanation/lambda.cpp #0:0 begin -1
```

### ジェネリックラムダ
ジェネリックラムダとは、C++11のラムダ式のパラメータの型にautoを指定できるようにした機能で、
C++14で導入された。

この機能により関数の中で関数テンプレートと同等のものが定義できるようになった。

ジェネリックラムダで定義されたクロージャは、通常のラムダと同様にオブジェクトであるため、
下記のように使用することもできる便利な記法である。

```cpp
    // @@@ example/term_explanation/generic_lambda_ut.cpp #0:0 begin
```

なお、上記のジェネリックラムダは下記クラスのインスタンスの動きと同じである。

```cpp
    // @@@ example/term_explanation/generic_lambda_ut.cpp #0:1 begin
```

### 関数tryブロック
関数tryブロックとはtry-catchを本体とした下記のような関数のブロックを指す。

```cpp
    // @@@ example/term_explanation/func_try_block.cpp #0:0 begin
```

### 単純代入
代入は下記のように分類される。

* 単純代入(=)
* 複合代入(+=，++ 等)

### ill-formed
[標準規格と処理系](https://cpprefjp.github.io/implementation-compliance.html)に詳しい解説があるが、

* well-formed(適格)とはプログラムが全ての構文規則・診断対象の意味規則・
  単一定義規則を満たすことである。
* ill-formed(不適格)とはプログラムが適格でないことである。

プログラムがwell-formedになった場合、そのプログラムはコンパイルできる。
プログラムがill-formedになった場合、通常はコンパイルエラーになるが、
対象がテンプレートの場合、事情は少々異なり、[SFINAE](---)によりコンパイルできることもある。


### well-formed
「[ill-formed](---)」を参照せよ。

### one-definition rule
「[ODR](---)」を参照せよ。

### ODR
ODRとは、One Definition Ruleの略語であり、下記のようなことを定めている。

* どの翻訳単位でも、テンプレート、型、関数、またはオブジェクトは、複数の定義を持つことができない。
* プログラム全体で、オブジェクトまたは非インライン関数は複数の定義を持つことはできない。
* 型、テンプレート、外部インライン関数等、いくつかのものは複数の翻訳単位で定義することができる。

より詳しい内容がが知りたい場合は、
[https://en.cppreference.com/w/cpp/language/definition](https://en.cppreference.com/w/cpp/language/definition)
が参考になる。

### RVO(Return Value Optimization)
関数の戻り値がオブジェクトである場合、
戻り値オブジェクトは、その関数の呼び出し元のオブジェクトにコピーされた後、すぐに破棄される。
この「オブジェクトをコピーして、その後すぐにそのオブジェクトを破棄する」動作は、
「関数の戻り値オブジェクトをそのままその関数の呼び出し元で使用する」ことで効率的になる。
RVOとはこのような最適化を指す。

なお、このような最適化は、
[C++17から規格化](https://cpprefjp.github.io/lang/cpp17/guaranteed_copy_elision.html)された。


### SSO(Small String Optimization)
一般にstd::stringで文字列を保持する場合、newしたメモリが使用される。
64ビット環境であれば、newしたメモリのアドレスを保持する領域は8バイトになる。
std::stringで保持する文字列が終端の'\0'も含め8バイト以下である場合、
アドレスを保持する領域をその文字列の格納に使用すれば、newする必要がない(当然deleteも不要)。
こうすることで、短い文字列を保持するstd::stringオブジェクトは効率的に動作できる。

SOOとはこのような最適化を指す。

### Most Vexing Parse
Most Vexing Parse(最も困惑させる構文解析)とは、C++の文法に関連する問題で、
Scott Meyersが彼の著書"Effective STL"の中でこの現象に名前をつけたことに由来する。

この問題はC++の文法が関数の宣言と変数の定義とを曖昧に扱うことによって生じる。
特にオブジェクトの初期化の文脈で発生し、意図に反して、その行は関数宣言になってしまう。

```cpp
    // @@@ example/term_explanation/most_vexing_parse_ut.cpp #0:0 begin

    // @@@ example/term_explanation/most_vexing_parse_ut.cpp #0:1 begin -1
```

[オブジェクトと生成|初期化子リストコンストラクタ](---)の呼び出しでオブジェクトの初期化を行うことで、
このような問題を回避できる。


### RTTI
RTTI(Run-time Type Information)とは、プログラム実行中のオブジェクトの型を導出するための機能であり、
具体的には下記の3つの要素を指す。

* dynamic_cast
* typeid
* std::type_info

下記のようなポリモーフィックな(virtual関数を持った)クラスに対しては、

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #0:0 begin
```

dynamic_cast、typeidやその戻り値であるstd::type_infoは、下記のように振舞う。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #0:1 begin -1
```

下記のような非ポリモーフィックな(virtual関数を持たない)クラスに対しては、

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #1:0 begin
```

dynamic_cast、typeidやその戻り値であるstd::type_infoは、下記のように振舞う。

```cpp
    // @@@ example/term_explanation/rtti_ut.cpp #1:1 begin -1
```

### Run-time Type Information
「[C++その他|RTTI](---)」を参照せよ。

### simple-declaration
このための記述が
[simple-declaration](https://cpprefjp.github.io/lang/cpp17/selection_statements_with_initializer.html)
とは、C++17から導入された
「従来for文しか使用できなかった初期化をif文とswitch文でも使えるようにする」ための記述方法である。

```cpp
    // @@@ example/term_explanation/simple_declaration_ut.cpp #0:0 begin
```

### typeid
「[C++その他|RTTI](---)」を参照せよ。

### トライグラフ
トライグラフとは、2つの疑問符とその後に続く1文字によって表される、下記の文字列である。

```
    ??=  ??/  ??'  ??(  ??)  ??!  ??<  ??>  ??-
```

### フリースタンディング環境
[フリースタンディング環境](https://ja.wikipedia.org/wiki/%E3%83%95%E3%83%AA%E3%83%BC%E3%82%B9%E3%82%BF%E3%83%B3%E3%83%87%E3%82%A3%E3%83%B3%E3%82%B0%E7%92%B0%E5%A2%83)
とは、組み込みソフトウェアやOSのように、その実行にOSの補助を受けられないソフトウエアを指す。

## ソフトウェア一般
### 凝集度
[凝集度](https://ja.wikipedia.org/wiki/%E5%87%9D%E9%9B%86%E5%BA%A6)
とはクラス設計の妥当性を表す尺度の一種であり、
Lack of Cohesion in Methodsというメトリクスで計測される。

* Lack of Cohesion in Methodsの値が1に近ければ凝集度は低く、この値が0に近ければ凝集度は高い。
* 凝集度とは結合度とも呼ばれ、メンバ(メンバ変数、メンバ関数等)間の結びつきを表す。
  メンバ間の結びつきが強いほど、良い設計とされる。
* メンバ変数やメンバ関数が多くなれば、凝集度は低くなりやすい。
* 「[単一責任の原則(SRP)](---)」を守ると凝集度は高くなりやすい。
* 「[Accessor](---)」を多用すれば凝集度は低くなる。従って、下記のようなクラスは凝集度が低い。
   言い換えれば、凝集度を下げることなく、より小さいクラスに分割できる。

```cpp
    // @@@ example/term_explanation/lack_of_cohesion_ut.cpp #0:0 begin
```

良く設計されたクラスは、下記のようにメンバが結合しあっているため凝集度が高い
(ただし、「[Immutable](---)」の観点からは、QuadraticEquation::Set()がない方が良い)。
言い換えれば、凝集度を落とさずにクラスを分割することは難しい。

```cpp
    // @@@ example/term_explanation/lack_of_cohesion_ut.cpp #0:1 begin
```


### サイクロマティック複雑度
[サイクロマティック複雑度](https://ja.wikipedia.org/wiki/%E5%BE%AA%E7%92%B0%E7%9A%84%E8%A4%87%E9%9B%91%E5%BA%A6)
とは関数の複雑さを表すメトリクスである。
このメトリクスの解釈は諸説あるものの、概ね以下のテーブルのようなものである。

|サイクロマティック複雑度(CC)|複雑さの状態                              |
|:--------------------------:|:-----------------------------------------|
|           CC <= 10         |非常に良い構造                            |
|      11 < CC <  30         |やや複雑                                  |
|      31 < CC <  50         |構造的なリスクあり                        |
|      51 < CC               |テスト不可能、デグレードリスクが非常に高い|


### Spurious Wakeup
[Spurious Wakeup](https://en.wikipedia.org/wiki/Spurious_wakeup)とは、
条件変数に対する通知待ちの状態であるスレッドが、その通知がされていないにもかかわらず、
起き上がってしまう現象のことを指す。

下記のようなstd::condition_variableの使用で起こり得る。

```cpp
    // @@@ example/term_explanation/spurious_wakeup_ut.cpp #0:0 begin
```

std::condition_variable::wait()の第2引数を下記のようにすることでこの現象を回避できる。

```cpp
    // @@@ example/term_explanation/spurious_wakeup_ut.cpp #1:0 begin
```

### 副作用
プログラミングにおいて、式の評価による作用には、
主たる作用とそれ以外の
[副作用](https://ja.wikipedia.org/wiki/%E5%89%AF%E4%BD%9C%E7%94%A8_(%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0))
(side effect)とがある。
式は、評価値を得ること(関数では「引数を受け取り値を返す」と表現する)が主たる作用とされ、
それ以外のコンピュータの論理的状態(ローカル環境以外の状態変数の値)を変化させる作用を副作用という。
副作用の例としては、グローバル変数や静的ローカル変数の変更、
ファイルの読み書き等のI/O実行、等がある。



## 非ソフトウェア用語
### 割れ窓理論
[割れ窓理論](https://ja.wikipedia.org/wiki/%E5%89%B2%E3%82%8C%E7%AA%93%E7%90%86%E8%AB%96)とは、
軽微な犯罪も徹底的に取り締まることで、凶悪犯罪を含めた犯罪を抑止できるとする環境犯罪学上の理論。
アメリカの犯罪学者ジョージ・ケリングが考案した。
「建物の窓が壊れているのを放置すると、誰も注意を払っていないという象徴になり、
やがて他の窓もまもなく全て壊される」との考え方からこの名がある。

ソフトウェア開発での割れ窓とは、「朝会に数分遅刻する」、「プログラミング規約を守らない」
等の軽微なルール違反を指し、この理論の実践には、このような問題を放置しないことによって、

* チームのモラルハザードを防ぐ
* コードの品質を高く保つ

等の重要な狙いがある。


### 車輪の再発明
[車輪の再発明](https://ja.wikipedia.org/wiki/%E8%BB%8A%E8%BC%AA%E3%81%AE%E5%86%8D%E7%99%BA%E6%98%8E)
とは、広く受け入れられ確立されている技術や解決法を（知らずに、または意図的に無視して）
再び一から作ること」を指すための慣用句である。
ソフトウェア開発では、STLのような優れたライブラリを使わずに、
それと同様なライブラリを自分たちで実装するような非効率な様を指すことが多い。


