#pragma once

#include <string>
#include <string>
#include <iostream>

class WeakPtr;

struct Block {
    int strong = 0;
    int week = 0;
};

class SharedPtr {
    friend class WeakPtr;

public:
    SharedPtr() = default;
    SharedPtr(std::string* ptr) : obj_(ptr), counter_(new Block({1, 0})) {
    }

    SharedPtr(const WeakPtr& ptr);

    SharedPtr(const SharedPtr& other) {
        obj_ = other.obj_;
        counter_ = other.counter_;
        if (counter_) {
            ++(counter_->strong);
        }
    }

    SharedPtr(SharedPtr&& other) {
        obj_ = other.obj_;
        counter_ = other.counter_;
        other.obj_ = nullptr;
        other.counter_ = nullptr;
    }

    void Reset(std::string* pointer) {
        if (counter_ && counter_->strong == 1) {
            delete obj_;
            obj_ = pointer;
        } else {
            --(counter_->strong);
            obj_ = pointer;
            counter_ = new Block({1, 0});
        }
    }

    ~SharedPtr() {
        if (!counter_) {
            return;
        }
        if (--(counter_->strong) == 0) {
            delete obj_;
            if (counter_->week == 0) {
                delete counter_;
            }
        }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (other.obj_ == obj_ && other.counter_ == counter_) {
            return *this;
        }

        SharedPtr copy(other);
        std::swap(obj_, copy.obj_);
        std::swap(counter_, copy.counter_);
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (other.obj_ == obj_ && other.counter_ == counter_) {
            return *this;
        }
        std::swap(other.counter_, counter_);
        std::swap(other.obj_, obj_);
        return *this;
    }

    std::string& operator*() const {
        return *obj_;
    }

    std::string* operator->() const {
        return obj_;
    }

    std::string* Get() const {
        return obj_;
    }

private:
    std::string* obj_ = nullptr;
    Block* counter_ = nullptr;
};

class WeakPtr {
public:
    WeakPtr() = default;
    WeakPtr(const SharedPtr& ptr) : obj_(ptr.obj_), counter_(ptr.counter_) {
        if (counter_) {
            ++counter_->week;
        }
    }
    bool IsExpired() const {
        if (counter_) {
            return counter_->strong <= 0;
        }
        return true;
    }

    SharedPtr Lock() {
        SharedPtr result;
        if (!IsExpired()) {
            result.counter_ = counter_;
            result.obj_ = obj_;
            ++counter_->strong;
        }
        return result;
    }
    WeakPtr(const WeakPtr& other) : obj_(other.obj_), counter_(other.counter_) {
        if (counter_) {
            ++counter_->week;
        }
    }
    WeakPtr(WeakPtr&& other) : obj_(other.obj_), counter_(other.counter_) {
        other.obj_ = nullptr;
        other.counter_ = nullptr;
    }

    WeakPtr& operator=(const WeakPtr& other) {
        WeakPtr new_ptr(other);
        std::swap(obj_, new_ptr.obj_);
        std::swap(counter_, new_ptr.counter_);
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) {
        std::swap(other.counter_, counter_);
        std::swap(other.obj_, obj_);
        return *this;
    }

    ~WeakPtr() {
        if (counter_) {
            --counter_->week;
            if (counter_->week == 0 && counter_->strong == 0) {
                delete counter_;
            }
        }
    }

    std::string& operator*() const {
        return *obj_;
    }

    std::string* operator->() const {
        return obj_;
    }

    std::string* Get() const {
        return obj_;
    }

    Block* GetBlock() const {
        return counter_;
    }

private:
    std::string* obj_ = nullptr;
    Block* counter_ = nullptr;
};

SharedPtr::SharedPtr(const WeakPtr& ptr) : obj_(ptr.Get()), counter_(ptr.GetBlock()) {
    if (ptr.IsExpired()) {
        obj_ = nullptr;
    }
    if (counter_) {
        ++(counter_->strong);
    }
}
