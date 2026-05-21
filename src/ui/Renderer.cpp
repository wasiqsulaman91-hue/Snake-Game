#include "ui/Renderer.h"
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdint>

const sf::Color Renderer::COL_BG     {  13,  17,  23 };
const sf::Color Renderer::COL_GRID   {  22,  30,  40 };
const sf::Color Renderer::COL_HEAD   {  80, 250, 123 };
const sf::Color Renderer::COL_BODY   {   0, 180,  80 };
const sf::Color Renderer::COL_TAIL   {   0,  80,  35 };
const sf::Color Renderer::COL_FOOD   { 255,  85,  85 };
const sf::Color Renderer::COL_PANEL  {  17,  24,  34 };
const sf::Color Renderer::COL_ACCENT {  80, 250, 123 };
const sf::Color Renderer::COL_TEXT   { 220, 220, 220 };
const sf::Color Renderer::COL_DIM    {  90, 110, 130 };

Renderer::Renderer(sf::RenderWindow& window) : win_(window) {
    bool loaded = font_.openFromFile("assets/fonts/Roboto-Regular.ttf");
    if (!loaded) {
        loaded = font_.openFromFile("C:/Windows/Fonts/consola.ttf");
        if (!loaded) font_.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
}

void Renderer::clear()   { win_.clear(COL_BG); }
void Renderer::display() { win_.display();     }

void Renderer::drawGrid() {
    rectShape_.setFillColor(COL_GRID);
    rectShape_.setSize({1.f, static_cast<float>(WINDOW_H)});
    for (int c = 0; c <= GRID_COLS; ++c) {
        rectShape_.setPosition({static_cast<float>(c * CELL_SIZE), 0.f});
        win_.draw(rectShape_);
    }
    rectShape_.setSize({static_cast<float>(GAME_W), 1.f});
    for (int r = 0; r <= GRID_ROWS; ++r) {
        rectShape_.setPosition({0.f, static_cast<float>(r * CELL_SIZE)});
        win_.draw(rectShape_);
    }
}

void Renderer::drawSnake(const Snake& snake) {
    const auto& body = snake.body();
    const int   len  = snake.length();
    int         idx  = 0;

    for (const auto& seg : body) {
        const float t = (len > 1) ? static_cast<float>(idx) / (len - 1) : 0.f;


        auto lerp8 = [](uint8_t a, uint8_t b, float f) {
            return static_cast<uint8_t>(static_cast<float>(a) + (static_cast<float>(b) - static_cast<float>(a)) * f);
        };

        sf::Color col;
        if (idx == 0) {
            col = COL_HEAD;
        } else if (t < 0.5f) {
            float f = t * 2.f;
            col = { lerp8(COL_HEAD.r, COL_BODY.r, f),
                    lerp8(COL_HEAD.g, COL_BODY.g, f),
                    lerp8(COL_HEAD.b, COL_BODY.b, f) };
        } else {
            float f = (t - 0.5f) * 2.f;
            col = { lerp8(COL_BODY.r, COL_TAIL.r, f),
                    lerp8(COL_BODY.g, COL_TAIL.g, f),
                    lerp8(COL_BODY.b, COL_TAIL.b, f) };
        }

        const float px  = static_cast<float>(seg.x * CELL_SIZE);
        const float py  = static_cast<float>(seg.y * CELL_SIZE);
        const float pad = (idx == 0) ? 1.f : 2.f;
        const float sz  = CELL_SIZE - 2.f * pad;

        rectShape_.setSize({sz, sz});
        rectShape_.setPosition({px + pad, py + pad});
        rectShape_.setFillColor(col);
        win_.draw(rectShape_);

        if (idx == 0) {
            sf::CircleShape eye(2.2f);
            eye.setFillColor(COL_BG);
            const auto d = snake.currentDirection();
            float ex1, ey1, ex2, ey2;
            if (d == Direction::RIGHT || d == Direction::LEFT) {
                float xOff = (d == Direction::RIGHT) ? 12.f : 4.f;
                ex1 = px + xOff; ey1 = py + 4.f;
                ex2 = px + xOff; ey2 = py + 12.f;
            } else {
                float yOff = (d == Direction::DOWN) ? 12.f : 4.f;
                ex1 = px + 4.f;  ey1 = py + yOff;
                ex2 = px + 12.f; ey2 = py + yOff;
            }
            eye.setPosition({ex1, ey1}); win_.draw(eye);
            eye.setPosition({ex2, ey2}); win_.draw(eye);
        }
        ++idx;
    }
}

void Renderer::drawFood(const Food& food) {
    const float t     = animClock_.getElapsedTime().asSeconds();
    const float pulse = 1.f + 0.18f * std::sin(t * 4.5f);
    const float base  = (CELL_SIZE / 2.f - 3.f) * pulse;

    const float cx = static_cast<float>(food.pos().x * CELL_SIZE + CELL_SIZE / 2);
    const float cy = static_cast<float>(food.pos().y * CELL_SIZE + CELL_SIZE / 2);

    sf::CircleShape glow(base + 5.f);
    glow.setFillColor({255, 85, 85, 35});
    glow.setOrigin({base + 5.f, base + 5.f});
    glow.setPosition({cx, cy});
    win_.draw(glow);

    sf::CircleShape mid(base + 2.f);
    mid.setFillColor({255, 85, 85, 70});
    mid.setOrigin({base + 2.f, base + 2.f});
    mid.setPosition({cx, cy});
    win_.draw(mid);

    circShape_.setRadius(base);
    circShape_.setFillColor(COL_FOOD);
    circShape_.setOrigin({base, base});
    circShape_.setPosition({cx, cy});
    win_.draw(circShape_);

    sf::CircleShape spec(base * 0.28f);
    spec.setFillColor({255, 200, 200, 200});
    spec.setOrigin({spec.getRadius(), spec.getRadius()});
    spec.setPosition({cx - base * 0.3f, cy - base * 0.3f});
    win_.draw(spec);
}

void Renderer::drawPanel(GameState , int score, int highScore,
                          int length, float tickRate) {
    rectShape_.setSize({static_cast<float>(PANEL_W), static_cast<float>(WINDOW_H)});
    rectShape_.setPosition({static_cast<float>(GAME_W), 0.f});
    rectShape_.setFillColor(COL_PANEL);
    win_.draw(rectShape_);

    rectShape_.setSize({3.f, static_cast<float>(WINDOW_H)});
    rectShape_.setPosition({static_cast<float>(GAME_W), 0.f});
    rectShape_.setFillColor(COL_ACCENT);
    win_.draw(rectShape_);

    const float cx = GAME_W + PANEL_W / 2.f;
    float y = 28.f;

    drawCenteredText("Noodle of Doom", 24, COL_ACCENT, cx, y);        y += 18.f;
    drawSeparator(y);                                          y += 18.f;

    drawCenteredText("SCORE", 11, COL_DIM, cx, y);            y += 20.f;
    drawCenteredText(std::to_string(score), 30, COL_TEXT, cx, y); y += 52.f;

    drawCenteredText("Best", 11, COL_DIM, cx, y);             y += 20.f;
    drawCenteredText(std::to_string(highScore), 25, COL_ACCENT, cx, y); y += 44.f;
    drawSeparator(y);                                          y += 18.f;


    int spd = static_cast<int>((BASE_TICK - tickRate) / (BASE_TICK - MIN_TICK) * 100.f);
    spd = std::clamp(spd, 0, 100);
    drawCenteredText("SPEED", 11, COL_DIM, cx, y);            y += 20.f;
    drawCenteredText(std::to_string(spd) + " %", 22, COL_TEXT, cx, y); y += 16.f;

    const float barW = PANEL_W - 30.f;
    const float barX = GAME_W + 15.f;
    rectShape_.setSize({barW, 6.f});
    rectShape_.setPosition({barX, y});
    rectShape_.setFillColor({40, 55, 65});
    win_.draw(rectShape_);
    rectShape_.setSize({barW * spd / 100.f, 6.f});
    rectShape_.setFillColor(COL_ACCENT);
    win_.draw(rectShape_);
    y += 32.f;

    drawSeparator(y); y += 18.f;

    drawCenteredText("", 11, COL_DIM, cx, y);         y += 20.f;
}

void Renderer::drawOverlay(sf::Color col) {
    rectShape_.setSize({static_cast<float>(GAME_W), static_cast<float>(WINDOW_H)});
    rectShape_.setPosition({0.f, 0.f});
    rectShape_.setFillColor(col);
    win_.draw(rectShape_);
}

void Renderer::drawMenu() {
    drawOverlay({0, 0, 0, 175});
    const float cx = GAME_W / 2.f;
    drawCenteredText("Noodle of Doom",        68, COL_ACCENT, cx, 145.f);
    drawCenteredText("", 16, COL_DIM,   cx, 222.f);
    drawSeparator(265.f);
    drawCenteredText("""Enter"" to start", 20, COL_TEXT, cx, 310.f);

    const int n = 7;
    auto lerp8 = [](uint8_t a, uint8_t b, float t) {
        return static_cast<uint8_t>(static_cast<float>(a) + (static_cast<float>(b) - static_cast<float>(a)) * t);
    };
    for (int i = 0; i < n; ++i) {
        float f = static_cast<float>(i) / (n - 1);
        sf::Color c { lerp8(COL_HEAD.r, COL_TAIL.r, f),
                      lerp8(COL_HEAD.g, COL_TAIL.g, f),
                      lerp8(COL_HEAD.b, COL_TAIL.b, f) };
        rectShape_.setSize({16.f, 16.f});
        rectShape_.setPosition({cx - n * 9.f + i * 18.f, 430.f});
        rectShape_.setFillColor(c);
        win_.draw(rectShape_);
    }
}

void Renderer::drawPause() {
    drawOverlay({0, 0, 0, 145});
    const float cx = GAME_W / 2.f;
    drawCenteredText("Hey!",38, COL_ACCENT, cx, 235.f);
    drawCenteredText("I was just about to win",25, COL_ACCENT, cx, 285.f);
    drawCenteredText("P to resume", 18, COL_TEXT,  cx, 325.f);
}

void Renderer::drawGameOver(int score) {
    drawOverlay({0, 0, 0, 185});
    const float cx = GAME_W / 2.f;
    drawCenteredText("Skill Issue",               54, COL_FOOD,  cx, 195.f);
    drawCenteredText("Score: " + std::to_string(score), 28, COL_TEXT, cx, 268.f);
    drawCenteredText("""Enter"" to restart", 18, COL_DIM,  cx, 332.f);
}

void Renderer::drawCenteredText(const std::string& str, unsigned charSize,
                                 sf::Color col, float cx, float cy) {
    sf::Text text(font_, str, charSize);
    text.setFillColor(col);
    const sf::FloatRect b = text.getLocalBounds();
    text.setOrigin({b.position.x + b.size.x / 2.f,
                    b.position.y + b.size.y / 2.f});
    text.setPosition({cx, cy});
    win_.draw(text);
}

void Renderer::drawSeparator(float y, float) {
    rectShape_.setSize({static_cast<float>(PANEL_W - 30), 1.f});
    rectShape_.setPosition({static_cast<float>(GAME_W + 15), y});
    rectShape_.setFillColor({40, 55, 70, 200});
    win_.draw(rectShape_);
}
