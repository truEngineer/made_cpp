#ifndef MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H
#define MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H

#include "xor_sum.h"
#include "binary.h"

template <size_t Count, size_t Idx, size_t Head, size_t... Tail>
struct FindHeapIdx {
    static constexpr ssize_t value = GetBit<Idx, Head>::value ? Count : FindHeapIdx<Count + 1, Idx, Tail...>::value;
};

template <size_t Count, size_t Idx, size_t Head>
struct FindHeapIdx<Count, Idx, Head> {
    static constexpr ssize_t value = GetBit<Idx, Head>::value ? Count : -1;
};

template <size_t Head, size_t... Tail>
struct FindMaxNum {
    static constexpr size_t value = Head > FindMaxNum<Tail...>::value ? Head : FindMaxNum<Tail...>::value;
};

template <size_t Head>
struct FindMaxNum<Head> {
    static constexpr size_t value = Head;
};

template <size_t... Heaps>
struct Game {
    static constexpr size_t xor_sum = XorSum<Heaps...>::value;
    static constexpr bool first_player_wins = xor_sum;
    static constexpr ssize_t first_move_heap_index = FindHeapIdx<0, Log2<xor_sum>::value - 1, Heaps...>::value;
    static constexpr size_t max_val = FindMaxNum<Heaps...>::value;
    static constexpr ssize_t first_move_rocks_count = max_val - (xor_sum ^ max_val);
};

#endif /// MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H.