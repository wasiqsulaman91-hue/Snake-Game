#pragma once
#include <SFML/Graphics.hpp>
#include "core/Snake.h"
#include "core/Food.h"
#include "core/GameState.h"
#include "core/Constants.h"

class Renderer {
public:
    explicit Renderer(sf::RenderWindow& window);

    void clear();
    void display();

    void drawGrid();
    void drawSnake(const Snake& snake);
    void drawFood(const Food& food);
    void drawPanel(GameState state, int score, int highScore,
                   int length, float tickRate);

    void drawMenu();
    void drawPause();
    void drawGameOver(int score);

private:
    sf::RenderWindow& win_;
    sf::Font          font_;
    sf::Clock         animClock_;

    sf::RectangleShape rectShape_;
    sf::CircleShape    circShape_;

    void drawCenteredText(const std::string& str, unsigned charSize,
                          sf::Color col, float cx, float cy);
    void drawOverlay(sf::Color col);
    void drawSeparator(float y, float alpha = 40.f);

    static const sf::Color COL_BG;
    static const sf::Color COL_GRID;
    static const sf::Color COL_HEAD;
    static const sf::Color COL_BODY;
    static const sf::Color COL_TAIL;
    static const sf::Color COL_FOOD;
    static const sf::Color COL_PANEL;
    static const sf::Color COL_ACCENT;
    static const sf::Color COL_TEXT;
    static const sf::Color COL_DIM;
};
