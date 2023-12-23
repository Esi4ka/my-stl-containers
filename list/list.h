#pragma once

#include <cstddef>
#include <iterator>
#include <iostream>

struct BaseListNode {
    BaseListNode *prev = this;
    BaseListNode *next = this;

    virtual ~BaseListNode() = default;
};

template <typename T>
struct ListNode : BaseListNode {
    ListNode(const T &val) : value(val) {
        prev = nullptr;
        next = nullptr;
    };

    ListNode(T &&val) : value(std::move(val)) {
        prev = nullptr;
        next = nullptr;
    }

    T value;

    ~ListNode() = default;
};

template <class T>
class List {
private:
    void Unlink(BaseListNode *node) {
        if (node == base_) {
            return;
        }
        auto prev_node = node->prev;
        auto next_node = node->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        --size_;
        node->prev = nullptr;
        node->next = nullptr;
        delete node;
        // std::cout << "size after unlink = " << size_ << std::endl;
    }

    void LinkAfter(BaseListNode *target, BaseListNode *after) {
        auto old_next = target->next;
        target->next = after;
        after->prev = target;
        after->next = old_next;
        old_next->prev = after;
        // target -> after -> old_next
        ++size_;
    }

public:
    List() {
        base_ = new BaseListNode;
        size_ = 0;
    }

    size_t Size() const {
        return size_;
    }

    bool IsEmpty() const {
        if (!Size()) {
            return true;
        }
        return false;
    }

    void PushBack(const T &value) {
        auto new_node = new ListNode<T>(value);
        LinkAfter(base_->prev, new_node);
    }

    void PushBack(T &&value) {
        auto new_node = new ListNode<T>(std::move(value));
        LinkAfter(base_->prev, new_node);
    }

    void PopBack() {
        auto old_last_node = base_->prev;
        Unlink(old_last_node);
    }

    void PopFront() {
        // std::cout << "call pop front! \n";
        // std::cout << "inital size = " << size_ << '\n';
        auto old_first_node = base_->next;
        Unlink(old_first_node);
    }

    void PushFront(const T &value) {
        // std::cout << "call push front! \n";
        // std::cout << "inital size = " << size_ << '\n';
        auto new_node = new ListNode<T>(value);
        LinkAfter(base_, new_node);
    }

    void PushFront(T &&value) {
        auto new_node = new ListNode<T>(std::move(value));
        LinkAfter(base_, new_node);
    }

    T &Back() {
        auto pointer = base_->prev;
        return dynamic_cast<ListNode<T> *>(pointer)->value;
    }

    const T &Back() const {
        auto pointer = base_->prev;
        return dynamic_cast<ListNode<T> *>(pointer)->value;
    }

    T &Front() {
        auto pointer = base_->next;
        return dynamic_cast<ListNode<T> *>(pointer)->value;
    }

    const T &Front() const {
        auto pointer = base_->next;
        return dynamic_cast<ListNode<T> *>(pointer)->value;
    }

    List(const List &other) : size_(0) {
        // std::cout << "call constructor & " << std::endl;
        base_ = new BaseListNode;
        auto current_push_node = other.base_->next;
        for (; current_push_node != other.base_;) {
            // std::cout << "pointer = " << current_push_node << std::endl;
            // std::cout << "*pointer = " << dynamic_cast<ListNode<T>*>
            // (current_push_node)->value << std::endl;
            T value = dynamic_cast<ListNode<T> *>(current_push_node)->value;
            PushBack(value);
            current_push_node = current_push_node->next;
        }
        // std::cout << "size = " << size_ << '\n';
    };

    List(List &&other) : base_(other.base_), size_(other.size_) {
        // std::cout << "call constructor && " << std::endl;
        other.base_ = new BaseListNode;
        other.size_ = 0;
    }

    ~List() {
        while (base_->next != base_) {
            PopBack();
        }
        base_->prev = nullptr;
        base_->next = nullptr;
        delete base_;
        size_ = 0;
    }

    List &operator=(const List &other) {
        // std::cout << "call operator = \n";
        if (&other == this) {
            return *this;
        }

        this->~List();
        // std::cout << "size after destructor = " << size_ << std::endl;
        // std::cout << "done destructor! \n";
        size_ = 0;
        // std::cout << "size = " << size_ << '\n';
        base_ = new BaseListNode;
        // std::cout << "base = " << base_ << '\n';
        auto current_push_node = other.base_->next;
        for (; current_push_node != other.base_;) {
            // std::cout << "pointer = " << current_push_node << '\n';
            T value = dynamic_cast<ListNode<T> *>(current_push_node)->value;
            PushBack(value);
            current_push_node = current_push_node->next;
        }
        return *this;
    }

    List &operator=(List &&other) {
        // std::cout << "call operator && = \n";
        if (&other == this) {
            return *this;
        }
        this->~List();
        size_ = other.size_;
        base_ = other.base_;
        other.base_ = new BaseListNode;
        other.size_ = 0;
        return *this;
    }

    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        Iterator() = default;

        Iterator(BaseListNode *ptr) {
            ptr_ = ptr;
        };

        T &operator*() const {
            return dynamic_cast<ListNode<T> *>(ptr_)->value;
        }

        BaseListNode *Get() const {
            return ptr_;
        }

        T *operator->() const {
            return &dynamic_cast<ListNode<T> *>(ptr_)->value;
        }

        Iterator &operator++() {
            ptr_ = ptr_->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator res = *this;
            ptr_ = ptr_->next;
            return res;
        }

        Iterator &operator--() {
            ptr_ = ptr_->prev;
            return *this;
        }

        Iterator operator--(int) {
            Iterator res = *this;
            ptr_ = ptr_->prev;
            return res;
        }

        bool operator==(const Iterator &rhs) const {
            if (ptr_ == rhs.ptr_) {
                return true;
            }
            return false;
        }

        bool operator!=(const Iterator &rhs) const {
            if (ptr_ != rhs.ptr_) {
                return true;
            }
            return false;
        }

    private:
        BaseListNode *ptr_ = nullptr;
    };

    void Erase(Iterator ptr) {
        Unlink(ptr.Get());
    }

    Iterator Begin() {
        return Iterator(base_->next);
    }

    Iterator End() {
        return Iterator(base_);
    }

private:
    BaseListNode *base_;
    size_t size_;
};

template <class T>
List<T>::Iterator begin(List<T> &list) {
    return list.Begin();
}

template <class T>
List<T>::Iterator end(List<T> &list) {
    return list.End();
}
