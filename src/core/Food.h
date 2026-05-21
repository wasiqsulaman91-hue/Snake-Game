#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <random>
#include "ds/Grid.h"
#include "core/Constants.h"

class Food {
public:
    Food();


    template<std::size_t R, std::size_t C>
    void respawn(const Grid<R, C>& occupied) {
        std::vector<sf::Vector2i> free;
        free.reserve(R * C / 2);

        for (int r = 0; r < static_cast<int>(R); ++r)
            for (int c = 0; c < static_cast<int>(C); ++c)
                if (!occupied(r, c))
                    free.push_back({c, r});

        if (free.empty()) return;

        std::uniform_int_distribution<int> dist(0, static_cast<int>(free.size()) - 1);
        pos_ = free[dist(rng_)];
    }

    [[nodiscard]] sf::Vector2i pos() const { return pos_; }

private:
    sf::Vector2i pos_;
    std::mt19937 rng_{ std::random_device{}() };
};
