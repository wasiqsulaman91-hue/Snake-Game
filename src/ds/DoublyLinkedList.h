#pragma once
#include <cstddef>

template<typename T>
class DoublyLinkedList {
public:

    struct Node {
        T     data;
        Node* prev = nullptr;
        Node* next = nullptr;
        explicit Node(const T& val) : data(val) {}
    };

private:
    Node*       head_  = nullptr;
    Node*       tail_  = nullptr;
    std::size_t size_  = 0;

public:

    DoublyLinkedList() = default;

    ~DoublyLinkedList() { clear(); }


    DoublyLinkedList(const DoublyLinkedList&)            = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    DoublyLinkedList(DoublyLinkedList&& o) noexcept
        : head_(o.head_), tail_(o.tail_), size_(o.size_)
    { o.head_ = o.tail_ = nullptr; o.size_ = 0; }

    DoublyLinkedList& operator=(DoublyLinkedList&& o) noexcept {
        if (this != &o) {
            clear();
            head_ = o.head_; tail_ = o.tail_; size_ = o.size_;
            o.head_ = o.tail_ = nullptr; o.size_ = 0;
        }
        return *this;
    }


    void push_front(const T& val) {
        Node* n = new Node(val);
        if (!head_) { head_ = tail_ = n; }
        else        { n->next = head_; head_->prev = n; head_ = n; }
        ++size_;
    }

    void push_back(const T& val) {
        Node* n = new Node(val);
        if (!tail_) { head_ = tail_ = n; }
        else        { tail_->next = n; n->prev = tail_; tail_ = n; }
        ++size_;
    }

    void pop_back() {
        if (!tail_) return;
        Node* old = tail_;
        tail_ = tail_->prev;
        if (tail_) tail_->next = nullptr;
        else       head_ = nullptr;
        delete old;
        --size_;
    }

    void pop_front() {
        if (!head_) return;
        Node* old = head_;
        head_ = head_->next;
        if (head_) head_->prev = nullptr;
        else       tail_ = nullptr;
        delete old;
        --size_;
    }

    void clear() { while (head_) pop_front(); }


    [[nodiscard]] std::size_t size()  const { return size_;          }
    [[nodiscard]] bool        empty() const { return size_ == 0;     }
    Node*       front()               { return head_; }
    const Node* front() const         { return head_; }
    Node*       back()                { return tail_; }
    const Node* back()  const         { return tail_; }


    struct Iterator {
        Node* cur;
        explicit Iterator(Node* n) : cur(n) {}
        T&         operator*()  { return cur->data; }
        Iterator&  operator++() { cur = cur->next; return *this; }
        bool operator!=(const Iterator& o) const { return cur != o.cur; }
    };
    struct ConstIterator {
        const Node* cur;
        explicit ConstIterator(const Node* n) : cur(n) {}
        const T&      operator*()  const { return cur->data; }
        ConstIterator& operator++()      { cur = cur->next; return *this; }
        bool operator!=(const ConstIterator& o) const { return cur != o.cur; }
    };
    Iterator      begin()        { return Iterator(head_); }
    Iterator      end()          { return Iterator(nullptr); }
    ConstIterator begin()  const { return ConstIterator(head_); }
    ConstIterator end()    const { return ConstIterator(nullptr); }
};
