# ダイナミックメモリアロケーション
本章で扱うダイナミックメモリアロケーションとは、new/delete、malloc/free
によるメモリ確保/解放のことである。

malloc/freeは、

* 最長処理時間を規定できない(リアルタイム性の欠如)
* メモリのフラグメントを起こす

等の問題(「[malloc/freeの問題点](---)」参照)を持っている。
new/deleteは通常malloc/freeを使って実装されているため同じ問題を持っているが、
これらが汎用OS上でのアプリケーションで実際の不具合につながることはほとんどない。一方で、 

* リアルタイムな応答が要求される
* メモリの使用制限が厳しい(ページングと2次記憶がない)

ような組み込みソフトでは、上記の2点は致命的な不具合につながる。

本章では、この問題を回避するための技法を紹介する。

## malloc/freeの問題点
UNIX系のOSでの典型的なmalloc/freeの実装例の一部を以下に示す
(この実装は長いため、全体は巻末の"[example/dynamic_memory_allocation/malloc_ut.cpp](---)"に掲載する)。

```cpp
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #1:0 begin
```
```cpp
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #2:0 begin
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #2:1 begin
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #2:2 begin
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #3:0 begin
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #3:1 begin
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #3:2 begin
```

上記で示したようにmalloc/freeで使用されるメモリはHeader_t型のheaderで管理され、
このアクセスの競合はspin_lockによって回避される。
headerが管理するメモリ用域からのメモリの切り出しはmalloc_innerによって行われるが、
下のフラグメントの説明でも示す通り、
headerで管理されたメモリは長さの上限が単純には決まらないリスト構造になるため、
このリストをなぞるmalloc/freeにリアルタイム性の保証をすることは困難である。

アプリケーションが実行する最初のmallocから呼び出されるmalloc_innerは、
headerがnullptrであるため必ずnullptrを返すことになる。

上記の抜粋である下記のコードによりmalloc_innerの戻りがnullptrであった場合、sbrkが呼び出される。

```cpp
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #3:1 begin -1
```

sbrkとはOSからメモリを新たに取得するための下記のようなシステムコールである。

```cpp
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #0:0 begin
```

OSがアプリケーションに割り当てるための十分なメモリを持っていない場合、
sbrkは
[ページング](https://ja.wikipedia.org/wiki/%E3%83%9A%E3%83%BC%E3%82%B8%E3%83%B3%E3%82%B0%E6%96%B9%E5%BC%8F)
によるメモリ確保のトリガーとなる。
これはOSのファイルシステムの動作を含む処理であるため、やはりリアルタイム性の保証は困難である。

[フリースタンディング環境](---)では、sbrkのようなシステムコールは存在しないため、
アプリケーションの未使用領域や静的に確保した領域を上記コードで示したようなリスト構造で管理し、
mallocで使用することになる。
このような環境では、sbrkによるリアルタイム性の阻害は発生しないものの、
メモリ管理ためのリスト構造があるため、やはりリアルタイム性の保証は難しい。

次にもう一つの問題である「メモリのフラグメントを起こす」ことについて見て行く。

```cpp
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #4:0 begin -1
```

上記のような処理の後、解放されたメモリは、32バイト
(メモリヘッダがあるため、実際はもう少し大きい)の断片が512個ある状態になるため、
このサイズを超える新たなmallocの呼び出しには使えない。
このため、ページングが行えないようなOS上のアプリケーションでは、
メモリは十分にあるにもかかわらずmallocが失敗してしまうことが起こり得る。

また、上記freeの実装例の抜粋である下記のコードからわかるように、
このように断片化されたメモリは、そのアドレス順にソートされた単方向リストによって管理される。

```cpp
    // @@@ example/dynamic_memory_allocation/malloc_ut.cpp #2:1 begin -1
```

この状態でさらにメモリ解放が行われた場合、
freeはこのリストを辿りメモリを最適な場所に戻す必要がある。
戻したメモリがリスト前後のメモリと隣接していれば、それらは結合される。
この処理は断片化への対策であるが、
ページングの無いOS上のアプリケーションにとっては不十分であるばかりでなく、

* freeの排他ロックする期間が長い
* freeの処理が遅い

といったリアルタイム処理を阻害する別の問題も発生させる
(繰り返しになるが、windows/linuxのような通常のOS上のアプリケーションでは、
このような仕様が問題になることはほとんどない)。


## グローバルnew/deleteのオーバーロード
すでに述べたように、組み込みソフトにはmalloc/freeを使用したnew/deleteは使えない可能性が高い。
そのような場合に備えC++11ではグローバルなnew/deleteのオーバーロードをサポートする。
ここでは、そのようなnew/deleteの実装例を示すが、
その前にnew/deleteの内部実装用メモリ管理用ライブラリを実装する。


### 固定長メモリプール
malloc/freeにリアルタイム性がない原因は、

* リアルタイム性がないOSのシステムコールを使用している
* メモリを可変長で管理しているため処理が重いにもかかわらず、この処理中にグローバルロックを行う。

ためである。従って、この問題に対処するためのメモリ管理システムは、

* 初期に静的なメモリを確保
* メモリを固定長で管理

する必要がある。これを含めこの章で開発するメモリ管理システムをメモリプールと呼ぶことにする。

「[グローバルnew/deleteのオーバーロードの実装](---)」で示すように、
このメモリプールは管理する固定長のメモリブロックのサイズごとに複数必要になる一方で、
これらを統合的に扱う必要も出てくる。

そのため、固定長のメモリプールは、

* 複数個のメモリプールを統合的に扱う[インターフェースクラス](---)MPool
* MPoolを基底クラスとし、固定長メモリブロックを管理するクラステンプレートMPoolFixed

によって実装することにする。

まずは、MPoolを下記に示す
(「[ファイル位置を静的に保持したエクセプションクラスの開発](---)」参照)。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool.h #1:0 begin
```

次に、MPoolFixedを下記に示す。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_fixed.h #1:0 begin
```

上記コードからわかる通り、MPoolFixedは初期化直後、
サイズMEM_SIZのメモリブロックをMEM_COUNT個、保持する。
個々のメモリブロックは、下記のコードのalignas/alignofでアライメントされた領域となる。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_fixed.h #0:0 begin
```

MPoolFixedに限らずメモリアロケータが返すメモリは、
どのようなアライメントにも対応できなければならないため、このようにする必要がある。

MPoolFixed::alloc/MPoolFixed::freeを見ればわかる通り、malloc/freeの実装に比べ格段にシンプルであり、
これによりリアルタイム性の保障は容易である。

なお、この実装ではmalloc/freeと同様に下記のSpinLockを使用したが、
このロックは、ラウンドロビンでスケジューリングされるスレッドの競合を防ぐためのものであり、
固定プライオリティでのスケジューリングが前提となるような組み込みソフトで使用した場合、
デッドロックを引き起こす可能性がある。
組み込みソフトでは、割り込みディセーブル/イネーブルを使ってロックすることを推奨する。

```cpp
    // @@@ example/dynamic_memory_allocation/spin_lock.h #0:0 begin
```

MPoolFixedの単体テストは、下記のようになる。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_fixed_ut.cpp #0:0 begin -1
```
```cpp
    // @@@ example/dynamic_memory_allocation/mpool_fixed_ut.cpp #1:0 begin -2
```

上記テストで使用したMPoolBadAllocは下記のように定義されたクラスであり
(「[ファイル位置を静的に保持したエクセプションクラスの開発](---)」参照)、

```cpp
    // @@@ example/h/nstd_exception.h #0:0 begin
    // @@@ example/h/nstd_exception.h #0:2 begin 0 -1
```
```cpp
    // @@@ example/dynamic_memory_allocation/mpool.h #0:0 begin
```

MPoolから派生したクラスが、

* メモリブロックを保持していない状態でのMPool::alloc(size, true)の呼び出し
* MEM_SIZEを超えたsizeでのMPool::alloc(size, true)の呼び出し

のような処理の継続ができない場合に用いるエクセプション用クラスである。


### グローバルnew/deleteのオーバーロードの実装
[固定長メモリプール](---)を使用したoperator newのオーバーロードの実装例を以下に示す。

```cpp
    // @@@ example/dynamic_memory_allocation/global_new_delete.cpp #1:0 begin
    // @@@ example/dynamic_memory_allocation/global_new_delete.cpp #2:0 begin
```

上記で定義されたoperator newは、

* 32の整数倍のサイズを持つ32個のメモリプールを持つ
* 各メモリープールは128個のメモリブロックを持つ
* メモリブロックの最大長は1024バイト

のような仕様を持つため、実際に使う場合は、メモリのサイズや個数の調整が必要だろうが、
後で詳しく見るようにリアルタイム性の阻害となるようなコードはないため、
リアルタイム性が必要なソフトウェアでも使用可能である。

静的オブジェクトを含まないアプリケーションでは、
上記のコードのsetupで行っているmpool_tableの初期化は
[オブジェクトと生成|一様初期化](---)で行った方が良いが、
例で用いたアプリケーションにはnewを行う静的オブジェクトが存在するため
(google testは静的オブジェクトを利用する)、
setupで行っているような方法以外では、
最初のoperator newの呼び出しより前にmpool_tableの初期化をすることはできない。

mpool_tableはMPoolポインタを保持するが、そのポインタが指すオブジェクトの実態は、
gen_mpool<>が生成したMPoolFixed<>オブジェクトである。
gen_mpool<>については、「[プレースメントnew](---)」で説明する。

size2indexは、要求されたサイズから、
それに対応するMPoolポインタを保持するmpool_tableのインデックスを導出する関数である。

この実装では対応するMPoolが空であった場合、
それよりも大きいメモリブロックを持つMPoolからメモリを返す仕様としたが、
その時点でアサーションフェールさせ
(つまり、対応するMPoolが空である状態でのAllocの呼び出しをバグとして扱う)、
MEM_COUNTの値を見直した方が、
より少ないメモリで動作する組み込みソフトを作りやすいだろう。

operator deleteについては、下記の2種類が必要となる。

```cpp
    // @@@ example/dynamic_memory_allocation/global_new_delete.cpp #3:0 begin
    // @@@ example/dynamic_memory_allocation/global_new_delete.cpp #4:0 begin
```

operator delete(void\* mem, std::size_t size)は、完全型のオブジェクトのメモリ解放に使用され、
operator delete(void\* mem)は、それ以外のメモリ解放に使用される。

コードから明らかな通り、size付きのoperator deleteの方がループの回転数が少なくなるため、
高速に動作するが、malloc/freeの実装(「[malloc/freeの問題点](---)」参照)で使用したHeader_t
を導入することでこの実行コストはほとんど排除できる。
そのトレードオフとしてメモリコストが増えるため、ここでは例示した仕様にした。


### プレースメントnew
「[グローバルnew/deleteのオーバーロードの実装](---)」で使用したgen_mpool<>は、
下記のように定義されている。

```cpp
    // @@@ example/dynamic_memory_allocation/global_new_delete.cpp #0:0 begin
```

この関数テンプレートは、MPoolFixed<>オブジェクトを生成し、それをMPool型のポインタとして返す。
MPoolFixedの生成は、上記で示したようにプレースメントnewを使用して行っている。

gen_mpool<>内でMPoolFixedのstaticなインスタンスを定義した方がシンプルに実装できるが、
その場合、main()終了後、そのインスタンスは解放され(デストラクタが呼び出され)、その後、
他の静的オブジェクトの解放が行われると、その延長でoperator deleteが呼び出され、
ライフタイム終了後のMPoolFixedのstaticなインスタンスが使われてしまう。

現在のMPoolFixedの実装ではこの操作で不具合は発生しないが、
解放済のオブジェクトを操作することは避けるべきであるため、
MPoolFixedの生成にプレースメントnewを用いている。

プレースメントnewで生成したオブジェクトをdeleteすることはできず、
デストラクタはユーザが明示的に呼び出さない限り、呼び出されない。
ここでは、プレースメントnewのこの特性を利用したが、
逆に、この特性があるため、
ここでの実装のような特殊な事情がある場合を除き、プレースメントnewを使うべきではない
(デストラクタの明示的な呼び出しを忘れるとリソースリークしてしまう)。


### デバッグ用イテレータ
この章で例示したグローバルnew/deleteは、すでに述べたように適切なメモリの量を調整する必要がある。
そのためには、これを使用するアプリケーションをある程度動作させた後、
グローバルnew/deleteのメモリの消費量を計測しなければならない。

下記のコードは、そのためのインターフェースを提供する。

```cpp
    // @@@ example/dynamic_memory_allocation/global_new_delete.h #0:0 begin
```
```cpp
    // @@@ example/dynamic_memory_allocation/global_new_delete.cpp #5:0 begin
```

このインターフェースを下記のように使用することで、

```cpp
    // @@@ example/dynamic_memory_allocation/global_new_delete_ut.cpp #0:0 begin -1
```

下記のようにメモリの現在の状態や使用履歴を見ることができる。

```
          size current   min
          ------------------
            32      90     0
            64      78    74
            96     127   125

           ...

           992     128   128
          1024     128     0
```

実際の組み込みソフトの開発では、
デバッグ用入出力機能からこのようなコードを実行できるようにすることで、
グローバルnew/deleteが使用するそれぞれのMPoolFixedインスタンスのメモリの調整ができるだろう。


## クラスnew/deleteのオーバーロード
「[グローバルnew/deleteのオーバーロードの実装](---)」で示したコードのロックを、
「割り込みディセーブル/イネーブル」に置き換えることで、リアルタイム性を保障することができるが、
この機構はある程度多くのメモリを必要とするため、
極めてメモリ制限の厳しいシステムでは使用が困難である場合もあるだろう。

そのような場合、非スタック上でのオブジェクト生成には、

* 限定的なクラスのみ、newによる動的な方法を用いる
* その他のクラスに対しては、[Singleton](---)や[Named Constructor](---)と同様な静的な方法を用いる

とし、グローバルnewを使用しないことが、より良いメモリ使用方法となり得る。

グローバルnewを使わずに動的にオブジェクトを生成するためには、

* プレースメントnewを使う
* クラス毎にnew/deleteをオーバーロードする

という2つの選択肢が考えられるが、すでに述べた理由によりプレースメントnewの使用は避けるべきである。
従って、その方法はクラス毎のnew/deleteのオーバーロードになる。

メモリ管理に「[固定長メモリプール](---)」で示したMPoolFixedを利用した実装例を以下に示す。

```cpp
    // @@@ example/dynamic_memory_allocation/class_new_delete_ut.cpp #0:0 begin
    // @@@ example/dynamic_memory_allocation/class_new_delete_ut.cpp #0:1 begin
    // @@@ example/dynamic_memory_allocation/class_new_delete_ut.cpp #0:2 begin
```

以下の単体テストが示す通り、
静的に定義したMPoolFixedインスタンスがオーバーロードしたnew/deleteから使われていることがわかる
(従ってグローバルnew/deleteは使われていないこともわかる)。

```cpp
    // @@@ example/dynamic_memory_allocation/class_new_delete_ut.cpp #1:0 begin -1
```

しかし、この方法ではnewのオーバーロードを行うクラス毎に、

```cpp
    // @@@ example/dynamic_memory_allocation/class_new_delete_ut.cpp #0:1 begin -1
```

を記述しなければならず、コードクローンの温床となってしまう。
これを避けるためには、
[CRTP(curiously recurring template pattern)](---)
を利用した下記のようなクラステンプレートを導入すれば良い。

```cpp
    // @@@ example/dynamic_memory_allocation/op_new.h #0:0 begin
```

このOpeNewを使用した「new/deleteのオーバーロードを持つ基底クラスとその一連の派生クラス」
の実装例を以下に示す。

```cpp
    // @@@ example/dynamic_memory_allocation/class_new_delete_ut.cpp #2:0 begin
```

OpNewをクラステンプレートとし、内部で利用しないテンプレートパラメータを宣言した理由は、
別のクラスからはOpNewの別インスタンスを使用できるようにするためである。

この方法は、コードが若干複雑にることを除けば、
「[グローバルnew/deleteのオーバーロード](---)」に比べ、優れているように見えてしまうかもしれないが、
下記のように、さらに派生クラスを定義してしまうとnewが失敗してしまうことがあるので注意が必要である。

```cpp
    // @@@ example/dynamic_memory_allocation/class_new_delete_ut.cpp #2:1 begin
```

なお、 下記のようなクラスをnew/deleteをオーバーロードしないすべのクラスの基底クラスとすることで、
偶発的にグローバルnewを使ってしまわないようにすることもできる。

```cpp
    // @@@ example/dynamic_memory_allocation/op_new_deleted.h #0:0 begin
```
```cpp
    // @@@ example/dynamic_memory_allocation/class_new_delete_ut.cpp #3:0 begin
```

この記述方法は、コードインスペクションの省力化にも繋がるため、
OpNewを使うプロジェクトには導入するべきだろう。


## STLコンテナのアロケーター
ここまで前提として来たような組み込みソフトにおいても、
その大部分のコードにリアルタイム性は不要であり、このような部分のコードにSTLコンテナが使用できれば、

* 開発効率が向上する
* 開発コード量が少なくなる

等のポジティブな影響を期待できることは多い。
STLコンテナはこういった状況に備えて、ユーザ定義のアロケータを使用できるように定義されている。
ここでは、アロケータの定義例や、その使い方を示す。

### STLコンテナ用アロケータ
アロケータの定義例を以下に示す。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator.h #0:0 begin
```

アロケータのパブリックなメンバやoperator ==、operator !=は、STLに従い定義している
([STL allocator](https://cpprefjp.github.io/reference/memory/allocator.html)参照)。

上記コードからわかるようにメモリの実際のアロケーションには、
これまでと同様にMPoolから派生したクラスを使用するが、
リアルタイム性は不要であるためメモリ効率が悪いMPoolFixedは使わない。
代わりに、可変長メモリを扱うためメモリ効率がよいMPoolVariabl
(「[可変長メモリプール](---)」参照)を使う。

### 可変長メモリプール
可変長メモリプールを生成するMPoolVariableの実装は下記のようになる
(全体は巻末の"[example/dynamic_memory_allocation/mpool_variable.h](---)"に掲載する)。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_variable.h #0:0 begin
    // @@@ example/dynamic_memory_allocation/mpool_variable.h #0:1 begin

        // 中略
        ...
    // @@@ example/dynamic_memory_allocation/mpool_variable.h #0:3 begin 0 -1
    // @@@ example/dynamic_memory_allocation/mpool_variable.h #0:4 begin
```

下記のようにMPoolVariable、
MPoolBasedAllocatorを使うことでnew char[]に対応するアロケータが定義できる。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #0:0 begin
```

下記の単体テストは、このアロケータを使うstd::stringオブジェクトの宣言方法と、
その振る舞いを示している。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #0:1 begin -1
```

この長い宣言は、下記のようにすることで簡潔に記述できるようになる。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #0:2 begin
```

下記のように宣言、定義することで、

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #1:0 begin
```

下記の単体テストが示す通り、std::vector\<int>にこのアロケータを使わせることもできる。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #1:1 begin -1
```

これまでの手法を組み合わせ下記のようにすることで、
std::stringと同等のオブジェクトを保持するstd::vectorを宣言することもできる。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #2:0 begin
```
```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #2:1 begin -1
```

しかし、下記に示すように、これまでの定義、
宣言のみではmpv_stringのnewにこのアロケータを使わせることはできない。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #3:1 begin -1
```

そうするためには、さらに下記のような定義が必要になる。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #3:0 begin
```

このようにすることで、下記に示すように期待した動きになる。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #3:2 begin -2
```

ただし、std::make_sharedを使用した場合、この関数のメモリアロケーションの最適化により、
下記に示すように期待した結果にならないため、注意が必要である。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #3:3 begin -2
```

newをオーバーロードしたクラスをstd::shared_ptrで管理する場合、下記のようにしなければならない。

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #3:4 begin -2
```

### デバッグ用イテレータ
[可変長メモリプール](---)を使用すると、
メモリのフラグメントによりアロケーションが失敗することがあり得る。
このような事態が発生している可能性がある場合、
アロケータが保持しているメモリの状態を表示させることがデバッグの第一歩となる。

下記のコードは、そのためのインターフェースを提供する。


```cpp
    // @@@ example/dynamic_memory_allocation/mpool_variable.h #0:0 begin

        // 中略
        ...
    // @@@ example/dynamic_memory_allocation/mpool_variable.h #0:2 begin 0 -1

        // 中略
        ...
    // @@@ example/dynamic_memory_allocation/mpool_variable.h #0:4 begin 0 -1
```

このインターフェースを下記のように使用することで、

```cpp
    // @@@ example/dynamic_memory_allocation/mpool_allocator_ut.cpp #4:0 begin -2
```

下記のようにmpv_allocator.header\_が保持するメモリの現在の状態を見ることができる
(これによるとmpv_allocatorが保持するメモリの先頭付近では多少フラグメントを起こしているが、
最後に大きなメモリブロックがあるため、全体としては問題ないレベルである)。

```
        0x7f073afe59d0:3
        0x7f073afe5a60:3
        0x7f073afe5ac0:3
        0x7f073afe5b70:3
        0x7f073afe5c50:11
        0x7f073afe5cb0:3
        0x7f073afe5e50:13
                     0:4018
```

「[グローバルnew/deleteのオーバーロードの実装](---)」でも述べたように、
デバッグ用入出力機能からこのような出力を得られるようにしておくべきである。


### エクセプション処理機構の変更
多くのコンパイラのエクセプション処理機構にはnew/deleteやmalloc/freeが使われているため、
リアルタイム性が必要な個所でエクセプション処理を行ってはならない。
そういった規制でプログラミングを行っていると、
リアルタイム性が不要な処理であるため使用しているSTLコンテナにすら、
既存のエクセプション処理機構を使わせたく無くなるものである。

コンパイラに[g++](---)や[clang++](---)を使っている場合、
下記関数を置き換えることでそういった要望を叶えることができる。

|関数                                         |機能                            |
|---------------------------------------------|--------------------------------|
|__cxa_allocate_exception(size_t thrown_size) |エクセプション処理用のメモリ確保|
|__cxa_free_exception(void\* thrown_exception)|上記で確保したメモリの解放      |

オープンソースである[static exception](https://github.com/ApexAI/static_exception)を使うことで、
上記2関数を置き換えることもできるが、この実装が複雑すぎると思うのであれば、
下記に示すような、これまで使用したMPoolFixedによる単純な実装を使うこともできる。

```cpp
    // @@@ example/dynamic_memory_allocation/exception_allocator_ut.cpp #0:0 begin
```

以下に単体テストを示す。

```cpp
    // @@@ example/dynamic_memory_allocation/exception_allocator_ut.cpp #1:0 begin -1
```

すでに述べたが、残念なことに、この方法はC++の標準外であるため、
これを適用できるコンパイラは限られている。
しかし、多くのコンパイラはこれと同様の拡張方法を備えているため、
安易にエクセプションやSTLコンテナを使用禁止することなく、安全に使用する方法を探るべきだろう。


