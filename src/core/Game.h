#pragma once
#include <SFML/Graphics.hpp>
#include "core/Snake.h"
#include "core/Food.h"
#include "core/Constants.h"
#include "core/GameState.h"
#include "ds/Grid.h"
#include "ui/Renderer.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void reset();
    void buildOccupancyGrid();


    sf::RenderWindow window_{
        sf::VideoMode({static_cast<unsigned>(WINDOW_W),
                       static_cast<unsigned>(WINDOW_H)}),
        "Snake - DSA Edition"
    };

    Renderer renderer_;
    Snake    snake_;
    Food     food_;

    Grid<GRID_ROWS, GRID_COLS, bool> occupancy_;

    GameState state_     = GameState::MENU;
    int       score_     = 0;
    int       highScore_ = 0;
    float     tickTimer_ = 0.f;
    float     tickRate_  = BASE_TICK;
};
