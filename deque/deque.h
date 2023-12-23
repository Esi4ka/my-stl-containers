#pragma once

#pragma once

#include <cstddef>
#include <initializer_list>
// #include <deque>
#include <iostream>
// #include <array> ????
#include <algorithm>
#include <exception>
#include <stdexcept>

class Chunk {
public:
    Chunk() {
        ptr_ = new int[chunk_size_];
    }

    ~Chunk() {
        if (ptr_ != nullptr) {
            delete[] ptr_;
        }
    }

    Chunk(int value) : Chunk() {
        for (size_t ind = 0; ind < chunk_size_; ++ind) {
            ptr_[ind] = value;
        }
        //        for (size_t ind = 0; ind < chunk_size_; ++ind) {
        //            std::cout << ptr_[ind] << " ";
        //        }
        //        std::cout << '\n';
    }

    int& operator[](size_t index) {
        return ptr_[index];
    }

    int operator[](size_t index) const {
        return ptr_[index];
    }

    Chunk(const Chunk& other) : Chunk() {
        for (size_t ind = 0; ind < chunk_size_; ++ind) {
            ptr_[ind] = other.ptr_[ind];
        }
        //        for (size_t ind = 0; ind < chunk_size_; ++ind) {
        //            std::cout << ptr_[ind] << " ";
        //        }
        //        std::cout << '\n';
    }

    Chunk& operator=(Chunk& other) {
        //        std::cout << "caal operator = \n";
        delete[] ptr_;
        ptr_ = new int[chunk_size_];
        for (size_t ind = 0; ind < chunk_size_; ++ind) {
            ptr_[ind] = other.ptr_[ind];
        }
        return *this;
    }

    Chunk& operator=(int value) {
        delete[] ptr_;
        ptr_ = new int[chunk_size_];
        for (size_t ind = 0; ind < chunk_size_; ++ind) {
            ptr_[ind] = value;
        }
        return *this;
    }

private:
    const size_t chunk_size_ = 512;
    int* ptr_ = nullptr;
};

class Deque {
private:
    size_t size_ = 0;
    const size_t chunk_size_ = 512;
    Chunk** data_ptr_ = nullptr;
    size_t start_ = 0;
    size_t number_chunks_ = 0;

    auto DeallocateFuncFront(size_t old_numb_chunks) {
        auto resently_allocated_memory = new Chunk*[number_chunks_ + old_numb_chunks];
        if (data_ptr_ != nullptr) {
            for (size_t ind = 0; ind < number_chunks_; ++ind) {
                resently_allocated_memory[old_numb_chunks + ind] = data_ptr_[ind];
            }
            delete[] data_ptr_;
        }

        for (size_t ind = 0; ind < old_numb_chunks; ++ind) {
            resently_allocated_memory[ind] = new Chunk(0);
        }

        return resently_allocated_memory;
    }

    auto DeallocateFunc(size_t old_numb_chunks) {
        auto resently_allocated_memory = new Chunk*[number_chunks_ + old_numb_chunks];
        // copy memory
        if (data_ptr_ != nullptr) {
            for (size_t ind = 0; ind < number_chunks_; ++ind) {
                resently_allocated_memory[ind] = data_ptr_[ind];
            }
            delete[] data_ptr_;
        }
        for (size_t ind = number_chunks_; ind < number_chunks_ + old_numb_chunks; ++ind) {
            resently_allocated_memory[ind] = new Chunk(0);
        }
        return resently_allocated_memory;
    }

public:
    Deque() = default;

    Deque(size_t size) : size_(size), data_ptr_(nullptr), start_(0) {
        bool flag = size_ % chunk_size_ != 0;
        number_chunks_ = size_ / chunk_size_ + flag;
        data_ptr_ = new Chunk*[number_chunks_];

        for (size_t ind = 0; ind < number_chunks_; ++ind) {
            data_ptr_[ind] = new Chunk(0);
        }
    }

    ~Deque() {
        if (data_ptr_ != nullptr) {
            for (size_t ind = 0; ind < number_chunks_; ++ind) {
                delete data_ptr_[ind];
            }
            delete[] data_ptr_;
        }

        // in valid state
        start_ = 0;
        data_ptr_ = nullptr;
        number_chunks_ = 0;
        size_ = 0;
    }

    void Clear() {
        this->~Deque();
    }

    size_t Size() const {
        return size_;
    }

    void PopBack() {
        if (size_ > 0) {
            --size_;
        } else {
            throw std::runtime_error("pop from empty deque!");
        }
    }

    void PopFront() {
        if (size_ > 0) {
            --size_;
            ++start_;
        } else {
            throw std::runtime_error("pop from empty deque!");
        }
    }

    int& operator[](size_t index) {
        size_t ind = (start_ + index) % chunk_size_;
        size_t ind2 = (start_ + index) / chunk_size_;
        return (*data_ptr_[ind2])[ind];
    }

    int operator[](size_t index) const {
        size_t ind = (start_ + index) % chunk_size_;
        size_t ind2 = (start_ + index) / chunk_size_;
        return (*data_ptr_[ind2])[ind];
    }

    Deque(const std::initializer_list<int>& list) : size_(list.size()), data_ptr_(nullptr) {
        bool flag = size_ % chunk_size_ != 0;
        number_chunks_ = size_ / chunk_size_ + flag;

        // allocate
        data_ptr_ = new Chunk*[number_chunks_];
        for (size_t ind = 0; ind < number_chunks_; ++ind) {
            data_ptr_[ind] = new Chunk();
        }

        size_t iterator = 0;
        for (const auto& elem : list) {
            (*this)[iterator] = elem;
            ++iterator;
        }
        //        for (size_t ind = 0; ind < chunk_size_; ++ind) {
        //            std::cout << ptr_[ind] << " ";
        //        }
        //        std::cout << '\n';
    }

    void Swap(Deque& other) {
        std::swap(size_, other.size_);
        std::swap(start_, other.start_);
        std::swap(data_ptr_, other.data_ptr_);
        std::swap(number_chunks_, other.number_chunks_);
    }

    Deque(const Deque& other)
        : size_(other.size_),
          data_ptr_(nullptr),
          start_(other.start_),
          number_chunks_(other.number_chunks_) {
        data_ptr_ = new Chunk*[number_chunks_];
        for (size_t i = 0; i < number_chunks_; ++i) {
            data_ptr_[i] = new Chunk(*(other.data_ptr_[i]));
        }
    }

    Deque& operator=(const Deque& other) {
        if (this == &other) {
            return *this;
        }
        Clear();
        data_ptr_ = new Chunk*[other.number_chunks_];
        for (size_t ind = 0; ind < other.number_chunks_; ++ind) {
            data_ptr_[ind] = new Chunk(*(other.data_ptr_[ind]));
        }
        size_ = other.size_;
        start_ = other.start_;
        number_chunks_ = other.number_chunks_;
        return *this;
    }

    Deque(Deque&& other) : Deque() {
        // Clear();
        Swap(other);
    }

    Deque& operator=(Deque&& other) {
        // std::cout << "call = move operator! \n";
        Clear();
        Swap(other);
        return *this;
    }

    void PushBack(int value) {
        if (start_ + size_ != chunk_size_ * number_chunks_) {
            (*this)[size_] = value;
            ++size_;
            return;
        }
        size_t old_number_chunks = ((number_chunks_ != 0) ? number_chunks_ : 1);
        data_ptr_ = DeallocateFunc(old_number_chunks);
        (*this)[size_] = value;
        ++size_;
        number_chunks_ += old_number_chunks;
    }

    void PushFront(int value) {
        if (start_ != 0) {
            --start_;
            (*this)[0] = value;
            ++size_;
            return;
        }

        size_t old_number_chunks = ((number_chunks_ != 0) ? number_chunks_ : 1);

        start_ += old_number_chunks * chunk_size_;
        data_ptr_ = DeallocateFuncFront(old_number_chunks);
        number_chunks_ += old_number_chunks;

        --start_;
        (*this)[0] = value;
        ++size_;
    }
};
