#pragma once

#include <functional>
#include <optional>
#include <iostream>
#include <fstream>
#include <string>
#include <atomic>
#include <list>
#include <vector>
#include <mutex>
#include <shared_mutex>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class Map {
public:
    Map(int num_buckets = 19, Hash hash_ = Hash{});

    Map(Map const&) = delete;
    Map& operator=(Map const&) = delete;

    bool insert(Key const& key, Value const& val);
    bool assign(Key const& key, Value const& val);
    bool remove(Key const& key);
    std::optional<Value> get(Key const& key) const;
    void print(std::string const& file_name) const;
    size_t size() const;

private:
    struct data {
        Key key;
        Value value;
        data(Key k, Value v) : key(k), value(v) {}
    };

    Hash hash;
    int num_buckets;
    int num_elements;
    mutable std::mutex mut_num_elements;
    std::vector<std::list<data>> buckets;
    mutable std::vector<std::shared_mutex> mutexi;
};

template <typename Key, typename Value, typename Hash>
Map<Key, Value, Hash>::Map(int num_buckets, Hash hash_) {
    this->num_buckets = num_buckets;
    num_elements = 0;
    hash = hash_;
    buckets.resize(num_buckets);
    mutexi = std::vector<std::shared_mutex>(num_buckets);
}

template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::insert(Key const& key, Value const& val) {
    int i = hash(key) % num_buckets;
    std::lock_guard<std::shared_mutex> loc(mutexi[i]);
    for (auto& item : buckets[i]) {
        if (item.key == key)
            return false;
    }
    std::lock_guard l_nm(mut_num_elements);
    buckets[i].emplace_back(key, val);
    num_elements++;
    return true;
}

template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::assign(Key const& key, Value const& val) {
    int i = hash(key) % num_buckets;
    std::lock_guard<std::shared_mutex> loc(mutexi[i]);
    for (auto& item : buckets[i]) {
        if (item.key == key) {
            item.value = val;
            return true;
        }
    }
    return false;
}

template <typename Key, typename Value, typename Hash>
bool Map<Key, Value, Hash>::remove(Key const& key) {
    int i = hash(key) % num_buckets;
    std::lock_guard<std::shared_mutex> loc(mutexi[i]);
    for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it) {
        if (it->key == key) {
            std::lock_guard lg(mut_num_elements);
            buckets[i].erase(it);
            num_elements--;
            return true;
        }
    }
    return false;
}

template <typename Key, typename Value, typename Hash>
std::optional<Value> Map<Key, Value, Hash>::get(Key const& key) const {
    int i = hash(key) % num_buckets;
    std::shared_lock<std::shared_mutex> loc(mutexi[i]);
    for (const auto& item : buckets[i]) {
        if (item.key == key)
            return item.value;
    }
    return std::nullopt;
}

template <typename Key, typename Value, typename Hash>
void Map<Key, Value, Hash>::print(std::string const& file_name) const {
    std::ofstream out(file_name);
    std::vector<std::shared_lock<std::shared_mutex>> locks;
    locks.reserve(num_buckets);

    for (int i = 0; i < num_buckets; ++i) {
        locks.emplace_back(mutexi[i]);
        out << "Bucket " << i << ": ";
        for (const auto& pair : buckets[i])
            out << "(" << pair.key << "," << pair.value << ") ";
        out << '\n';
    }
}

template <typename Key, typename Value, typename Hash>
size_t Map<Key, Value, Hash>::size() const {
    std::lock_guard lg(mut_num_elements);
    return num_elements;
}
