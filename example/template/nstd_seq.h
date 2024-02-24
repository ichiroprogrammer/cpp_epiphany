#pragma once

namespace Nstd {
// @@@ sample begin 0:0

// パラメータパック展開ヘルパクラス
template <size_t... Ns>
struct index_sequence {
};

// index_sequence<0, 1, 2, ...>を作るためのクラステンプレート
// make_index_sequence<3>
// -> make_index_sequence<2, 2>
// -> make_index_sequence<1, 1, 2>
// -> make_index_sequence<0, 0, 1, 2>
// -> index_sequence<0, 1, 2>
template <size_t N, size_t... Ns>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, Ns...> {
};

template <size_t... Ns>
struct make_index_sequence<0, Ns...> : index_sequence<Ns...> {
};

// @@@ sample end
}  // namespace Nstd
