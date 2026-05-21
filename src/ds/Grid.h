#pragma once
#include <array>
#include <cstddef>

template<std::size_t ROWS, std::size_t COLS, typename T = bool>
class Grid {
    std::array<std::array<T, COLS>, ROWS> data_{};

public:
    Grid() { reset(); }

    void reset(T val = T{}) {
        for (auto& row : data_)
            row.fill(val);
    }


    T&       at(int r, int c)       { return data_[r][c]; }
    const T& at(int r, int c) const { return data_[r][c]; }

    T&       operator()(int r, int c)       { return data_[r][c]; }
    const T& operator()(int r, int c) const { return data_[r][c]; }

    static constexpr std::size_t rows() { return ROWS; }
    static constexpr std::size_t cols() { return COLS; }
};
