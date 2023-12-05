#pragma once
#include <defs.hpp>
#include <queue>
#include <map>
#include <unordered_map>

namespace util::collections {

/*
* CappedQueue is a simple queue but when the size exceeds a limit, it removes the oldest element
*/

template <typename T, size_t MaxSize>
class CappedQueue {
public:
    void push(const T& element) {
        if (queue.size() >= MaxSize) {
            queue.pop();
        }

        queue.push(element);
    }

    size_t size() const {
        return queue.size();
    }

    bool empty() const {
        return queue.empty();
    }

    void clear() {
        queue = {}; // queue does not have .clear() for whatever funny reason
    }

    T front() const {
        return queue.front();
    }

    T back() const {
        return queue.back();
    }

    std::vector<T> extract() const {
        std::vector<T> out;

        std::queue<T> qcopy(queue);

        while (!qcopy.empty()) {
            out.push_back(qcopy.front());
            qcopy.pop();
        }

        return out;
    }
protected:
    std::queue<T> queue;
};

template <typename K, typename V>
std::vector<K> mapKeys(const std::map<K, V>& map) {
    std::vector<K> out;
    for (const auto& [key, _] : map) {
        out.push_back(key);
    }

    return out;
}

template <typename K, typename V>
std::vector<K> mapKeys(const std::unordered_map<K, V>& map) {
    std::vector<K> out;
    for (const auto& [key, _] : map) {
        out.push_back(key);
    }

    return out;
}

template <typename K, typename V>
std::vector<V> mapValues(const std::map<K, V>& map) {
    std::vector<V> out;
    for (const auto& [_, value] : map) {
        out.push_back(value);
    }

    return out;
}

template <typename K, typename V>
std::vector<V> mapValues(const std::unordered_map<K, V>& map) {
    std::vector<V> out;
    for (const auto& [_, value] : map) {
        out.push_back(value);
    }

    return out;
}

/* borrowed versions of the above functions */


template <typename K, typename V>
std::vector<K*> mapKeysBorrowed(std::map<K, V>& map) {
    std::vector<K*> out;
    for (auto& pair : map) {
        out.push_back(&pair.first);
    }

    return out;
}

template <typename K, typename V>
std::vector<K*> mapKeysBorrowed(std::unordered_map<K, V>& map) {
    std::vector<K*> out;
    for (auto& pair : map) {
        out.push_back(&pair.first);
    }

    return out;
}

template <typename K, typename V>
std::vector<V*> mapValuesBorrowed(std::map<K, V>& map) {
    std::vector<V*> out;
    for (auto& pair : map) {
        out.push_back(&pair.second);
    }

    return out;
}

template <typename K, typename V>
std::vector<V*> mapValuesBorrowed(std::unordered_map<K, V>& map) {
    std::vector<V*> out;
    for (auto& pair : map) {
        out.push_back(&pair.second);
    }

    return out;
}

template <typename T>
std::vector<std::pair<size_t, T&>> enumerate(std::vector<T>& vec) {
    std::vector<std::pair<size_t, T&>> out;
    out.reserve(vec.size());

    for (size_t i = 0; i < vec.size(); i++) {
        out.emplace_back(i, vec[i]);
    }

    return out;
}

};