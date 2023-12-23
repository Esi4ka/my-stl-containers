#pragma once

#include <type_traits>
#include <exception>
#include <stdexcept>

template <class T>
class Optional {
public:
    Optional() {
        has_value_ = false;
    }

    Optional(const T &value) {
        new (data_) T(value);
        has_value_ = true;
    }

    Optional(T &&value) {
        new (data_) T(std::move(value));
        has_value_ = true;
    }

    Optional(const Optional &other) {
        if (other.has_value_) {
            //            auto copy = *other;
            /* ptr_ = */ new (data_) T(*other);
            has_value_ = true;
        } else {
            //            ptr_ = nullptr;
            has_value_ = false;
        }
    }

    Optional(Optional &&other) {
        if (other.has_value_) {
            /*ptr_ = */ new (data_) T(std::move(*other));
            has_value_ = true;
        } else {
            //            ptr_ = nullptr;
            has_value_ = false;
        }
    }

    Optional &operator=(const Optional &other) {
        if (this == &other) {
            return *this;
        }

        if (other.has_value_) {
            auto copy = *other;
            Reset();
            /*delete ptr_;*/
            /*ptr_ = */ new (data_) T(copy);
            has_value_ = true;
        } else {
            Reset();
            //            ptr_ = nullptr;
            has_value_ = false;
        }
        return *this;
    }

    Optional &operator=(Optional &&other) {
        if (this == &other) {
            return *this;
        }
        if (other.has_value_) {
            auto copy = std::move(*other);
            Reset();
            //            delete ptr_;
            /*ptr_ = */ new (data_) T(std::move(copy));
            has_value_ = true;
        } else {
            Reset();
            //            ptr_ = nullptr;
            has_value_ = false;
        }
        return *this;
    }

    // This constructor must not change other.HasValue()
    // Just move value (if present)

    ~Optional() {
        if (has_value_) {
            reinterpret_cast<Optional::Type *>(data_)->~T();
            //            has_value_ = false;
        }
    }

    // This method must not change other.HasValue()
    // Just move value (if present)

    T &operator*() & {
        if (has_value_) {
            return *reinterpret_cast<Optional::Type *>(data_);
        } else {
            throw std::runtime_error("");
        }
    }

    const T &operator*() const & {
        if (has_value_) {
            return *reinterpret_cast<const Optional::Type *>(data_);
        } else {
            throw std::runtime_error("");
        }
    }

    T &&operator*() && {
        if (has_value_) {
            return std::move(*reinterpret_cast<Optional::Type *>(data_));
        } else {
            throw std::runtime_error("");
        }
    }

    T *operator->() {
        return reinterpret_cast<Optional::Type *>(data_);
    }

    const T *operator->() const {
        return reinterpret_cast<const Optional::Type *>(data_);
    }

    T &Value() & {
        return *(*this);
    }

    const T &Value() const & {
        return *(*this);
    }

    T &&Value() && {
        return std::move(*(*this));
    }

    bool HasValue() const {
        return has_value_;
    }

    void Reset() {
        if (has_value_) {
            this->~Optional();
            has_value_ = false;
        }
    }

private:
    using Type = T;
    bool has_value_;
    alignas(alignof(T)) unsigned char data_[sizeof(T)];
    // std::aligned_storage_t<sizeof(T), alignof(T)> data_ptr_;
    // T* ptr_;
};
