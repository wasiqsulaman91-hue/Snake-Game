#include "core/Game.h"
#include <algorithm>

Game::Game()
    : renderer_(window_),
      snake_(GRID_COLS / 2, GRID_ROWS / 2)
{
    window_.setFramerateLimit(144);
    reset();
}

void Game::run() {
    sf::Clock clock;
    while (window_.isOpen()) {
        const float dt = clock.restart().asSeconds();
        processEvents();
        if (state_ == GameState::PLAYING) update(dt);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = window_.pollEvent()) {


        if (event->is<sf::Event::Closed>())
            window_.close();


        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {


            if (state_ == GameState::PLAYING) {
                if (key->code == sf::Keyboard::Key::Up    || key->code == sf::Keyboard::Key::W)
                    snake_.enqueueDirection(Direction::UP);
                if (key->code == sf::Keyboard::Key::Down  || key->code == sf::Keyboard::Key::S)
                    snake_.enqueueDirection(Direction::DOWN);
                if (key->code == sf::Keyboard::Key::Left  || key->code == sf::Keyboard::Key::A)
                    snake_.enqueueDirection(Direction::LEFT);
                if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D)
                    snake_.enqueueDirection(Direction::RIGHT);
            }


            if (key->code == sf::Keyboard::Key::Enter) {
                if (state_ == GameState::MENU || state_ == GameState::GAME_OVER) {
                    reset();
                    state_ = GameState::PLAYING;
                }
            }
            if (key->code == sf::Keyboard::Key::P || key->code == sf::Keyboard::Key::Escape) {
                if (state_ == GameState::PLAYING)       state_ = GameState::PAUSED;
                else if (state_ == GameState::PAUSED)   state_ = GameState::PLAYING;
            }
        }
    }
}

void Game::update(float dt) {
    tickTimer_ += dt;
    if (tickTimer_ < tickRate_) return;
    tickTimer_ = 0.f;

    snake_.update();

    const sf::Vector2i head = snake_.headPos();


    if (head.x < 0 || head.x >= GRID_COLS ||
        head.y < 0 || head.y >= GRID_ROWS) {
        state_ = GameState::GAME_OVER;
        highScore_ = std::max(highScore_, score_);
        return;
    }


    if (snake_.hasSelfCollision()) {
        state_ = GameState::GAME_OVER;
        highScore_ = std::max(highScore_, score_);
        return;
    }


    if (head == food_.pos()) {
        score_    += 10;
        tickRate_  = std::max(MIN_TICK, tickRate_ - SPEED_INC);
        snake_.grow();
        buildOccupancyGrid();
        food_.respawn(occupancy_);
    }

    buildOccupancyGrid();
}

void Game::render() {
    renderer_.clear();
    renderer_.drawGrid();
    renderer_.drawFood(food_);
    renderer_.drawSnake(snake_);
    renderer_.drawPanel(state_, score_, highScore_, snake_.length(), tickRate_);

    switch (state_) {
        case GameState::MENU:      renderer_.drawMenu();           break;
        case GameState::PAUSED:    renderer_.drawPause();          break;
        case GameState::GAME_OVER: renderer_.drawGameOver(score_); break;
        default: break;
    }

    renderer_.display();
}

void Game::reset() {
    snake_     = Snake(GRID_COLS / 2, GRID_ROWS / 2);
    tickRate_  = BASE_TICK;
    tickTimer_ = 0.f;
    score_     = 0;
    buildOccupancyGrid();
    food_.respawn(occupancy_);
}

void Game::buildOccupancyGrid() {
    occupancy_.reset(false);
    for (const auto& seg : snake_.body())
        occupancy_(seg.y, seg.x) = true;
}
