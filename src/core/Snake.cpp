#include "core/Snake.h"

Snake::Snake(int startX, int startY) {

    body_.push_back({startX,     startY});
    body_.push_back({startX - 1, startY});
    body_.push_back({startX - 2, startY});
}

void Snake::enqueueDirection(Direction d) {
    if (isOpposite(d, dir_)) return;
    inputBuf_.enqueue(d);
}

void Snake::update() {

    Direction next;
    if (inputBuf_.dequeue(next)) {
        if (!isOpposite(next, dir_)) dir_ = next;
    }


    sf::Vector2i head = headPos();
    switch (dir_) {
        case Direction::UP:    --head.y; break;
        case Direction::DOWN:  ++head.y; break;
        case Direction::LEFT:  --head.x; break;
        case Direction::RIGHT: ++head.x; break;
    }

    body_.push_front(head);

    if (pendingGrow_) {
        pendingGrow_ = false;
    } else {
        body_.pop_back();
    }
}

bool Snake::hasSelfCollision() const {
    const sf::Vector2i h = headPos();
    const auto* node = body_.front()->next;
    while (node) {
        if (node->data == h) return true;
        node = node->next;
    }
    return false;
}

void Snake::grow() { pendingGrow_ = true; }

sf::Vector2i Snake::headPos() const { return body_.front()->data; }

bool Snake::isOpposite(Direction a, Direction b) {
    return (a == Direction::UP    && b == Direction::DOWN)  ||
           (a == Direction::DOWN  && b == Direction::UP)    ||
           (a == Direction::LEFT  && b == Direction::RIGHT) ||
           (a == Direction::RIGHT && b == Direction::LEFT);
}
