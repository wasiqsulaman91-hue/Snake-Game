#pragma once
#include <array>
#include <cstddef>

template<typename T, std::size_t CAP>
class CircularQueue {
    std::array<T, CAP> buf_{};
    std::size_t        head_ = 0;
    std::size_t        tail_ = 0;
    std::size_t        size_ = 0;

public:

    bool enqueue(const T& val) {
        if (size_ == CAP) return false;
        buf_[tail_] = val;
        tail_       = (tail_ + 1) % CAP;
        ++size_;
        return true;
    }


    bool dequeue(T& out) {
        if (size_ == 0) return false;
        out   = buf_[head_];
        head_ = (head_ + 1) % CAP;
        --size_;
        return true;
    }

    [[nodiscard]] bool        empty() const { return size_ == 0;   }
    [[nodiscard]] bool        full()  const { return size_ == CAP; }
    [[nodiscard]] std::size_t size()  const { return size_;        }
};
