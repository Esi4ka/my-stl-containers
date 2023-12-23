#pragma once

#include <string>
#include <cstddef>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

struct Command {
    int ref_count = 0;
    std::vector<std::string> pointer;
};

class COWVector {
public:
    COWVector() {
        state_ = new Command;
        state_->ref_count = 1;
        state_->pointer = std::vector<std::string>();
    }

    ~COWVector() {
        --state_->ref_count;
        if (state_->ref_count <= 0) {
            delete state_;
        }
    }

    COWVector(const COWVector &other) : state_(other.state_) {
        state_->ref_count++;
    }

    COWVector &operator=(const COWVector &other) {
        if (state_ == other.state_) {
            return *this;
        }
        --state_->ref_count;
        if (state_->ref_count == 0) {
            this->~COWVector();
        }
        state_ = other.state_;
        ++state_->ref_count;
        return *this;
    }

    size_t Size() const {
        return state_->pointer.size();
    }

    void Resize(size_t size) {
        if (size == state_->pointer.size()) {
            return;
        }
        if (size == 0) {
            state_->pointer.resize(0);
            return;
        }
        if (state_->ref_count == 1) {
            state_->pointer.resize(size);
            return;
        }
        UpdateState(size);
    }

    const std::string &Get(size_t at) const {
        return state_->pointer.at(at);
    }

    const std::string &Back() const {
        return state_->pointer.back();
    }

    void PushBack(const std::string &value) {
        if (state_->ref_count == 1) {
            state_->pointer.push_back(value);
            return;
        }
        UpdateState(state_->pointer.size());
        state_->pointer.push_back(value);
    }

    void Set(size_t at, const std::string &value) {
        if (state_->ref_count == 1) {
            state_->pointer[at] = value;
            return;
        }
        UpdateState(state_->pointer.size());
        state_->pointer[at] = value;
    }

private:
    Command *state_ = nullptr;

    void UpdateState(size_t size) {
        --state_->ref_count;
        Command *new_state = new Command;
        new_state->pointer = std::vector<std::string>(size);

        for (size_t idx = 0; idx < state_->pointer.size(); ++idx) {
            new_state->pointer[idx] = state_->pointer[idx];
        }

        std::swap(new_state, state_);
        state_->ref_count = 1;
    }
};
