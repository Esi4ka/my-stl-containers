#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <iostream>

struct Node {
    std::string key;
    std::string value;
};

class LruCache {
public:
    explicit LruCache(size_t max_size) : capacity_(max_size) {
    }

    LruCache(const LruCache &) = delete;

    LruCache &operator=(const LruCache &) = delete;

    void Set(const std::string &key, const std::string &value) {
        if (data_.find(key) != data_.end()) {
            std::list<Node>::iterator current_key_iter = data_.find(key)->second;
            Node current_node = *current_key_iter;
            Node new_node(key, value);
            current_node.value = value;
            time_used_.erase(current_key_iter);
            time_used_.emplace_back(current_node);
            current_key_iter = time_used_.end();
            --current_key_iter;
            data_[key] = current_key_iter;
        } else {
            time_used_.push_back({key, value});
            std::list<Node>::iterator current_key_iter = time_used_.end();
            --current_key_iter;
            data_.insert({key, current_key_iter});
            // std::cout << "we push new value" << key << " " << value << "\n";
            if (data_.size() > capacity_) {
                // std::cout << "need to delete LRU value \n";
                auto node_to_erase = time_used_.front();
                time_used_.pop_front();
                // std::cout << "it is " << value.key << '\n';
                data_.erase(node_to_erase.key);
            }
        }
    }

    bool Get(const std::string &key, [[maybe_unused]] std::string *value) {
        if (data_.find(key) != data_.end()) {
            std::list<Node>::iterator current_key_iter = data_.find(key)->second;
            Node current_node = *current_key_iter;
            time_used_.erase(current_key_iter);
            time_used_.push_back(current_node);
            current_key_iter = time_used_.end();
            --current_key_iter;
            data_[key] = current_key_iter;
            // std::cout << "kurrent inserted k-v" << key << value << '\n';
            // std::cout << "wia iter " << current_key_iter->value  <<
            // " "<< current_key_iter->key << '\n';
            if (value != nullptr) {
                *value = current_key_iter->value;
            }
            // std::cout << "value = " << value << '\n';
            return true;
        } else {
            // std::cout << "we don't find key " << key << '\n';
            return false;
        }
    }

private:
    size_t capacity_;
    std::unordered_map<std::string, std::list<Node>::iterator> data_;
    std::list<Node> time_used_;
};
