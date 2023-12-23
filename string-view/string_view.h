#pragma once
#include <iostream>
#include <string>
#include <cstring>

class StringView {
public:
    StringView(const char* str, size_t size) {
        if (size == 0) {
            size_ = strlen(str);
        } else if (size > strlen(str)) {
            size_ = strlen(str);
        } else {
            size_ = size;
        }
        data_pointer_ = str;
    }

    StringView(const char* str) : StringView(str, 0) {
    }

    char operator[](size_t ind) const {
        return *(data_pointer_ + ind);
    }

    const char* Data() const {
        return data_pointer_;
    }

    size_t Size() const {
        return size_;
    }

    StringView(const std::string& str) : StringView(str, 0) {
    }

    StringView(const std::string& str, size_t start_pos) {
        size_ = str.size() - start_pos;
        data_pointer_ = &str[start_pos];
    }

    StringView(const std::string& str, size_t start_pos, size_t size) {
        if (size == std::string::npos) {
            size_ = str.size() - start_pos;
        } else {
            size_ = size;
        }
        data_pointer_ = &str[start_pos];
    }

private:
    const char* data_pointer_;
    size_t size_;
};
