#pragma once

#include <iterator>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include <utility>
// #include <iostream>

class Vector {
public:
    Vector() : capacity_(0), size_(0), pointer_(nullptr) {
        // std::cout << "Constructor! \n";
    }

    ~Vector() {
        // std::cout << "Destructor! \n";
        delete[] pointer_;
        //        if (pointer_ != nullptr) {
        //            delete[] pointer_;
        //        }
    }

    Vector(size_t input_size) {
        pointer_ = new int[input_size];
        capacity_ = input_size;
        size_ = input_size;
        for (size_t ind = 0; ind < input_size; ++ind) {
            pointer_[ind] = 0;
        }
        // std::cout << "Constructor! \n";
    }

    Vector(std::initializer_list<int> input) {
        if (std::empty(input)) {
            size_ = 0;
            capacity_ = 0;
            pointer_ = nullptr;
            return;
        }
        capacity_ = input.size();
        // std::cout << "initializer list size is " << capacity_ << std::endl;
        pointer_ = new int[capacity_];
        size_ = capacity_;
        int ind = 0;
        for (auto el : input) {
            pointer_[ind] = el;
            ++ind;
        }
        // std::cout << "Constructor! \n";
    }

    Vector(const Vector &rhs) {
        if (rhs.capacity_ == 0) {
            capacity_ = 0;
            size_ = 0;
            pointer_ = nullptr;
            return;
        }
        capacity_ = rhs.capacity_;
        size_ = rhs.size_;
        pointer_ = new int[capacity_];
        for (size_t ind = 0; ind < size_; ++ind) {
            pointer_[ind] = rhs.pointer_[ind];
        }
        // std::cout << "copy constructor! \n";
    }

    Vector &operator=(const Vector &rhs) {
        // std::cout << "call operator = \n";
        if (this == &rhs) {
            // std::cout << "they are equal! \n";
            return *this;
        }
        delete[] pointer_;
        pointer_ = new int[rhs.capacity_];
        for (size_t ind = 0; ind < rhs.size_; ++ind) {
            pointer_[ind] = rhs.pointer_[ind];
        }
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        return *this;
    }

    Vector(Vector &&rhs)
        : capacity_(rhs.capacity_),
        size_(rhs.size_),
        pointer_(std::exchange(rhs.pointer_, nullptr)) {
        rhs.capacity_ = 0;
        rhs.size_ = 0;
        // rhs.pointer_ = nullptr;
        // std::cout << "copy && constructor! \n";
    }

    Vector &operator=(Vector &&rhs) {
        if (this == &rhs) {
            return *this;
        }
        delete[] pointer_;
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        pointer_ = std::exchange(rhs.pointer_, nullptr);
        rhs.capacity_ = 0;
        rhs.size_ = 0;
        // rhs.pointer_ = nullptr;
        return *this;
    }

    const int &operator[](size_t index) const {
        return pointer_[index];
    }

    int &operator[](size_t index) {
        return pointer_[index];
    }

    size_t Capacity() const {
        // std::cout << "capacity = " << capacity_ << '\n';
        return capacity_;
    }

    void Swap(Vector &other) {
        std::swap(other.pointer_, pointer_);
        std::swap(other.size_, size_);
        std::swap(other.capacity_, capacity_);
    }

    void PushBack(int element) {
        if (capacity_ == 0) {
            pointer_ = new int[1];
            capacity_ = 1;
            pointer_[0] = element;
            size_ = 1;
            return;
        } else if (capacity_ == size_) {
            // std::cout << "capacity == size \n";
            capacity_ *= 2;
            int *ptr = new int[capacity_];
            for (size_t ind = 0; ind < size_; ++ind) {
                ptr[ind] = pointer_[ind];
            }
            std::swap(ptr, pointer_);
            delete[] ptr;
            // PrintVectorAfterPush();
        }
        ++size_;
        // std::cout << "pushed and realloc \n";
        // std::cout << "size= " << size_ << " capacity = " << capacity_ << '\n';
        pointer_[size_ - 1] = element;
    }

    void PopBack() {
        if (size_ != 0) {
            --size_;
        }
    }

    void Clear() {
        size_ = 0;
    }

    void Reserve(size_t capacity) {
        if (capacity <= capacity_) {
            return;
        }
        int *tmp_ptr = new int[capacity];
        std::swap(tmp_ptr, pointer_);
        delete[] tmp_ptr;
        capacity_ = capacity;
    }

    class Iterator {
        int *ptr_;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = int;
        using reference = int &;
        using pointer = int *;
        using difference_type = ptrdiff_t;

        Iterator() : ptr_(nullptr){};

        Iterator(int *ptr) : ptr_(ptr){};

        Iterator(const Iterator &rhs) : ptr_(rhs.ptr_){};

        Iterator &operator=(const Iterator &rhs) {
            if (ptr_ == rhs.ptr_) {
                return *this;
            }
            ptr_ = rhs.ptr_;
            return *this;
        }

        reference operator*() const {
            return *ptr_;
        }

        pointer operator->() const {
            return ptr_;
        }

        Iterator &operator++() {
            ptr_++;
            return *this;
        }

        Iterator &operator--() {
            ptr_--;
            return *this;
        }

        Iterator operator++(int) {
            Iterator res = *this;
            ++(*this);
            return res;
        }

        Iterator operator--(int) {
            Iterator res = *this;
            --(*this);
            return res;
        }

        Iterator &operator+=(const int &rhs) {
            ptr_ += rhs;
            return *this;
        }

        Iterator &operator-=(const int &rhs) {
            ptr_ -= rhs;
            return *this;
        }

        Iterator &operator+=(difference_type &rhs) {
            ptr_ += rhs;
            return *this;
        }

        Iterator &operator-=(difference_type &rhs) {
            ptr_ -= rhs;
            return *this;
        }

        difference_type operator-(const Iterator &it) const {
            return this->ptr_ - it.ptr_;
        }

        Iterator operator+(const difference_type &diff) const {
            return Iterator(ptr_ + diff);
        }

        Iterator operator-(const difference_type &diff) const {
            return Iterator(ptr_ - diff);
        }

        friend Iterator operator+(const int &number, const Iterator &iter) {
            return Iterator(iter.ptr_ + number);
        }

        friend Iterator operator+(difference_type &number, const Iterator &iter) {
            return Iterator(iter.ptr_ + number);
        }

        reference operator[](const difference_type &offset) const {
            return *(*this + offset);
        }

        auto operator<=>(const Iterator &) const = default;
    };

    Iterator begin() {
        return Iterator(&pointer_[0]);
    }

    Iterator end() {
        return Iterator(&pointer_[size_]);
    }

    size_t Size() const {
        return size_;
    }

private:
    size_t capacity_ = 0;
    size_t size_ = 0;
    int *pointer_ = nullptr;
};
