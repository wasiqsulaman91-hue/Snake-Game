#pragma once
#include <SFML/System/Vector2.hpp>
#include "ds/DoublyLinkedList.h"
#include "ds/CircularQueue.h"

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Snake {
public:
    explicit Snake(int startX, int startY);


    void enqueueDirection(Direction d);


    void update();


    void grow();


    [[nodiscard]] bool hasSelfCollision() const;


    [[nodiscard]] const DoublyLinkedList<sf::Vector2i>& body() const { return body_; }
    [[nodiscard]] sf::Vector2i headPos()          const;
    [[nodiscard]] Direction    currentDirection() const { return dir_; }
    [[nodiscard]] int          length()           const { return static_cast<int>(body_.size()); }

private:
    DoublyLinkedList<sf::Vector2i> body_;
    CircularQueue<Direction, 4>    inputBuf_;
    Direction                      dir_         = Direction::RIGHT;
    bool                           pendingGrow_ = false;

    [[nodiscard]] static bool isOpposite(Direction a, Direction b);
};
