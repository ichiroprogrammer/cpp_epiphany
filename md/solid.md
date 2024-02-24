# SOLID
SOLIDとは、オブジェクト指向(OOD/OOP)プログラミングにおいて特に重要な下記の5つの原則である。

* [単一責任の原則(SRP)](---)
* [オープン・クローズドの原則(OCP)](---)
* [リスコフの置換原則(LSP)](---)
* [インターフェース分離の原則(ISP)](---)
* [依存関係逆転の原則(DIP)](---)

## 単一責任の原則(SRP)
単一責任の原則(SRP, Single Responsibility Principle)とは、

* 一つのクラスは、ただ一つの責任(機能)を持つようにしなければならない
* 一つのクラスは、ただ一つの理由で変更されるように作られなければならない

というクラスデザイン上の制約である。

下記クラスSentenceHolderNotSRPは、一見問題ないように見えるが、std::stringの保持と、
その出力という二つの責務を持つため、SRP違反である。

```cpp
    // @@@ example/solid/srp_ut.cpp #0:0 begin
```

実践的にはこの程度の単純なクラスでのSRP違反が問題になることは少ないが、
下記のコメントで示す通り、単体テストの実施が困難になる。

```cpp
    // @@@ example/solid/srp_ut.cpp #0:1 begin -1
```

クラスSentenceHolderNotSRPの二つの責務をクラスSentenceHolderSRPと、
Output()に分離したコード実装例を下記する。

```cpp
    // @@@ example/solid/srp_ut.cpp #1:0 begin
```

下記のコードで示したように、この分離の効果で単体テストの実施が容易になった。

```cpp
    // @@@ example/solid/srp_ut.cpp #1:1 begin -1
```

[演習-SRP](~~~)  

## オープン・クローズドの原則(OCP)
オープン・クローズドの原則(OCP, Open-Closed Principle)とは、

* クラスは拡張に対して開いて (open) いなければならず、
* クラスは修正に対して閉じて (closed) いなければならない

というクラスデザイン上の制約である。

まずは、アンチパターンから示す。

```cpp
    // @@@ example/solid/ocp_ut.cpp #0:0 begin
```

Transaction Method(enum TransactionMethod)が増えた場合、
少なくとも3か所に手を入れなけばならなくなる(修正に対してclosedでない)。
従って、下記のTransactorNotOCP::Charge()や、TransactorNotOCP::Pay()は
Transaction Methodの追加、変更に対して、脆弱な構造だと言える。

次に上記ソースコードのクラス図を下記する。

クラス図が示す通り、
TransactorNotOCPは、TransactorGoogle, TransactorSuica, TransactorEdy
(Transaction Methodに対応した具体的なクラス)に強く依存する。
したがって、新たなTransactor Methodが追加されれば、
Transaction Methodを使用しているTransactorNotOCPのすべてのメンバ関数は影響を受ける。
この構造は、上位概念が下位概念に依存しているとも言えるため、
後述する「[依存関係逆転の原則(DIP)](---)」にも反している。

![クラス図例](plant_uml/ocp_ng.png)

下記は、TransactorIFを導入することによって、上例をOCPに沿うように改善したクラス図と実装である。
TransactorOCPは、TransactorIFの効果によりTransaction Methodの追加に対して全く影響を受けなくなった
(実際には、TransactorIFから派生する具象クラスの生成用Factory関数(「[Factory](---)」参照)
が必要になるため全く影響がないわけではないが、
そのような箇所はソースコード全体でただ一つにすることができるため、
Transaction Methodの追加に対して強固な構造になったと言える)。

![クラス図例](plant_uml/ocp_ok.png)

下記にこのクラス図に従ったコードを示す。

```cpp
    // @@@ example/solid/ocp_ut.cpp #1:0 begin
```

ここでは、この原則に沿う実装方法としてポリモーフィズムを使うパターンを紹介したが、
[Pimpl](---)のようにラッピングを使用するパターンも有用である。

[演習-OCP](~~~)  

## リスコフの置換原則(LSP)
リスコフの置換原則(LSP, Liskov Substitution Principle)とは、

* 基底クラスを使っているX(関数もしくはクラス)に、
  基底クラスの代わりにその派生クラスを渡した場合でも、
  Xはその実際の型を知ること無しに正常動作できなければならない

というクラスデザイン上の制約であり、
この制約を守るために下記のような契約プログラミングを行うことが求められる。

* 事前条件を派生クラスで強めることはできない。
  つまり、基底クラスよりも強い事前条件を持つ派生クラスを作ってはならない。
* 事後条件を派生クラスで弱めることはできない。
  つまり、基底クラスよりも弱い事後条件を持つ派生クラスを作ってはならない。

この原則に従わない実装例を示すために、以下のようなクラスRectangleとその派生クラスSquareを定義する。

![クラス図例](plant_uml/rectangle_square.png)

```cpp
    // @@@ example/solid/lsp.h #0:0 begin
```

Rectangleのリファレンスを受け取るSetX()とその単体テストを以下のようにすると、
Rectangleのテストでは問題は起こらないが、同じことをSquareに行うとアボートしてしまう
(下記例ではASSERT_DEATHを使用しアボートすることを確認している)。

```cpp
    // @@@ example/solid/lsp_ut.cpp #0:0 begin
```

上記コードがアボート(assertion fail)してしまったのは  

* Rectangle::SetX()は、この実行によるy\_の値が不変であることを表明している
* この表明は、Rectangle::set_x()の事後条件となる
* Square::set_x()は、この事後条件を守らず、y\_の値を変えてしまった

が原因である。このデザイン上の問題には目をつぶり(Rectangle、Squareを修正せずに)、
しかもアボートしないSetX()の実装を考えてみよう。

SetX()は仮引数で渡されたオブジェクトの実際の型がわからなければアボートを避けることはできない。
従って、 新しいSetX()のコード実装例は以下のようになる。

```cpp
    // @@@ example/solid/lsp_ut.cpp #1:0 begin
```

上記の新たなSetX()は、アボートはしないがきわめて醜悪且つ、
Rectangleの全派生クラスに依存した、変更に弱い関数となる。  

なお、リスコフの置換原則とは関係しないが、上記のdynamic_castを含むSetX()は、
下記のように修正することができる。

```cpp
    // @@@ example/solid/lsp_ut.cpp #2:0 begin
```

このSetX()は、Rectangleからの派生クラスに依存していないため、良い解法に見える。
ところが実際にはオブジェクトの[オブジェクトのコピー|スライシング](---)という別の問題を引き起こす。  

例示した問題は結局のところデザインの誤りが原因であり、それを修正しない限り、
問題の回避は容易ではない。

一般に、継承関係は、IS-Aの関係と呼ばれる。数学の世界では「正方形 is a 長方形」であるため、
この関係を継承で表したのだが、
「Rectangle::SetX()の性質より導き出されたRectangle::set_x()の事後条件」
により、「クラスSquare is **NOT** a クラスRectangle」となり、
SquareとRectangleは継承関係ではないため問題が発生した。

継承を用いなければこのような問題は発生しないため、public継承を使用する際には、
「本当にその関係は継承で表すべきか(それが最もシンプルな方法か)？」
について熟慮する必要がある。

なお、エクセプション記述子は、関数のエクセプション仕様を強制的にLSPに従わせる仕組みであるが、
C++11から非推奨になり、C++17では規格から削除された。
その理由は、
「[非推奨だった古い例外仕様を削除](https://cpprefjp.github.io/lang/cpp17/remove_deprecated_exception_specifications.html)」
の説明の通り、これを使用し場合、OCPに違反する可能性が高いからである。
従って、原則に従うのみでなく、その他の原則とのバランスも考慮する必要がある。

[演習-LSP](~~~)  

## インターフェース分離の原則(ISP)
インターフェース分離の原則 (ISP, Interface Segregation Principle)とは、

* クラスは、そのクライアントが使用しないメソッドへの依存を、そのクライアントに強制するべきではない。
    * クラスのインターフェースを巨大にしない。
    * 一つのヘッダファイルに互いが密接な関係を持たない複数のクラスを定義、宣言すべきでない。
    * 一つのヘッダファイルにそのファイルのコンパイルに不要なヘッダファイルをインクルードすべきでない。

というクラスデザイン上の制約である。

まずは、ISPに従っていない例を示す。
下記のStreamReadWriterは、ClientRからはStreamReadWriter::Read()のみが、
ClientWからはStreamReadWriter::Write()のみが使用されている。  

![UML例](plant_uml/isp_ng.png)

ほとんどのStreamReadWriter使用ファイルでこのような依存関係がある場合、
このクラスは下記のようにStreamReaderとStreamWriterに分割した方が良い(依存関係が小さくなる)。

![UML例](plant_uml/isp_ok.png)

クラスの設計時に統合か分割かで悩むことは多いが、一度統合してしまえば分割は困難であり、
逆に分割されたものを統合することは容易である。このことを考慮すれば、
このような逡巡に解を与えることは簡単である。言うまでもないが、「まずは分割」が原則である。

[演習-ISP](~~~)  

## 依存関係逆転の原則(DIP)
依存関係逆転の原則 (DIP, Dependency Inversion Principle)とは、

* 上位レベルのモジュールは下位レベルのモジュールに依存すべきではない。
* 抽象は具象に依存すべきではない。

というクラス デザイン上の制約である。

下記ServerNG::Serverは、ClientNG::Clientに非同期サービスを提供する
(従って、ServerNG::ServerはClientNG::Clientに対して上位概念である)。

![UML例](plant_uml/dip_ng_seq.png)

非同期サービスであるServerNG::Server::RequireStringAsync()の完了は
ServerNG::ServerがClientNG::Client::Done()を呼び出すことにより通知される。  
その実装、使用例を下記に示す。

```cpp
    // @@@ example/solid/dip_server_ng.h #0:0 begin
```

```cpp
    // @@@ example/solid/dip_server_ng.cpp #0:0 begin
```

```cpp
    // @@@ example/solid/dip_client_ng.h #0:0 begin
```

```cpp
    // @@@ example/solid/dip_client_ng.cpp #0:0 begin
```

```cpp
    // @@@ example/solid/dip_ut.cpp #0:0 begin
```

上記ソースコードから明らかなようにServerNG::ServerとClientNG::Clientは相互に依存している。
このうちの一つはサーバがクライアントに依存(上位概念が下位概念に依存)する問題のある構造となっている。

![UML例](plant_uml/dip_ng.png)

このため、クライアントのバリエーションが増えた場合、容易にServerNG::Serverのコードは肥大化する。
また、ServerNG::Serverを介して各クライアント間にも(暗黙、明示両方の)依存関係が生まれやすいため、
ServerNG::Serverのコード修正は非常に困難になることが予想される。

次にDIPに従い上記コードを改善した例を示す。

```cpp
    // @@@ example/solid/dip_server_ok.h #0:0 begin
```

```cpp
    // @@@ example/solid/dip_server_ok.cpp #0:0 begin
```

```cpp
    // @@@ example/solid/dip_client_ok.h #0:0 begin
```

```cpp
    // @@@ example/solid/dip_client_ok.cpp #0:0 begin
```

```cpp
    // @@@ example/solid/dip_ut.cpp #0:1 begin
```

修正後のコードは、

* ServerOK::ServerはServerOK::ClientIFに依存する。
* ClientOK::ClientはServerOK::ClientIFから派生する。

このクラス図を以下に示す。

![UML例](plant_uml/dip_ok.png)

ServerNGとClientNGの双方向依存関係は、ClientOKからServerOKへの単方向依存関係へと改善され、
サーバに影響を与えることなく、クライアントの機能変更やバリエーション追加を行うことが可能となった。

[演習-DIP](~~~)  

## まとめ
以上で述べたように、SOLIDはオブジェクト指向(OOD/OOP)プログラミングにおいて極めて重要な原則である。
この逸脱はソースコードを劣化させ、ソフトウェアの品質低下や開発費増大に直結するため、
厳守することが求められる。

[演習-SOLIDの定義](~~~)  


