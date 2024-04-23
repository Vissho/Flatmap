#pragma once

#include <iterator>

#include <iostream>

#include <stdexcept>

#include <map>

#include <unordered_map>

#include <limits.h>

namespace fox {

    template <class Key, class T>
    class FlatMap {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<key_type, mapped_type>;
        using reference = std::pair<const key_type&, mapped_type&>;
        using const_reference = std::pair<const key_type&, const mapped_type&>;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    private:
        Key* keys = nullptr;
        T* values = nullptr;
        size_t capacity = 0;
        size_t size = 0;

    public:
        template <class K, class V>
        class Iterator {
        private:
            K* keys = nullptr;
            V* values = nullptr;
            size_t size = 0;
            size_t it = 0;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = std::pair<const K, V>;
            using difference_type = std::ptrdiff_t;
            using pointer = std::pair<const K*, V*>;
            using reference = std::pair<const K&, V&>;

            Iterator(Key* ikeys, T* ivalues, size_t isize, size_t iter)
                : keys(ikeys), values(ivalues), size(isize), it(iter)
            {
            }

            value_type operator*()
            {
                return std::make_pair(keys[it], values[it]);
            }

            Iterator& operator++()
            {
                ++it;
                if (it == ULLONG_MAX) {
                    throw(std::out_of_range("Iterator out of range"));
                }
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator tmp = *this;
                ++it;
                if (it == ULLONG_MAX) {
                    throw(std::out_of_range("Iterator out of range"));
                }
                return tmp;
            }

            Iterator& operator--()
            {
                --it;
                if (it < 0) {
                    throw(std::out_of_range("Iterator out of range"));
                }
                return *this;
            }

            Iterator operator--(int)
            {
                Iterator tmp = *this;
                --it;
                if (it < 0) {
                    throw(std::out_of_range("Iterator out of range"));
                }
                return tmp;
            }

            friend bool operator==(const Iterator& lhs, const Iterator& rhs)
            {
                return (lhs.it == rhs.it && lhs.size == rhs.size
                        && lhs.keys == rhs.keys && lhs.values == rhs.values);
            }

            friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
            {
                return (lhs.it != rhs.it && lhs.size != rhs.size
                        && lhs.keys != rhs.keys && lhs.values != rhs.values);
            }
        };

        FlatMap() = default;

        FlatMap(size_t size)
            : keys(new Key[size]), values(new T[size]), capacity(size)
        {
        }

        FlatMap(std::map<Key, T> map) : FlatMap(map.size())
        {
            for (const auto& [key, value] : map) {
                insert(key, value);
            }
        }

        FlatMap(std::unordered_map<Key, T> map) : FlatMap(map.size())
        {
            for (const auto& [key, value] : map) {
                insert(key, value);
            }
        }

        FlatMap(std::initializer_list<value_type> list) : FlatMap(list.size())
        {
            for (const auto& [key, value] : list) {
                insert(key, value);
            }
        }

        ~FlatMap()
        {
            delete[] keys;
            delete[] values;
        }

        FlatMap(const FlatMap& other)
        {
            for (const auto& [key, value] : other) {
                insert(key, value);
            }
        }

        FlatMap& operator=(const FlatMap& other)
        {
            if (this != &other) {
                for (const auto& [key, value] : other) {
                    insert(key, value);
                }
                return *this;
            }
        }

        FlatMap(FlatMap&& other) noexcept
            : keys(other.keys),
              values(other.values),
              capacity(other.capacity),
              size(other.size)
        {
            other.keys = nullptr;
            other.values = nullptr;
            other.capacity = 0;
            other.size = 0;
        }

        FlatMap& operator=(FlatMap&& other) noexcept
        {
            keys = other.keys;
            values = other.values;
            capacity = other.capacity;
            size = other.size;
            other.keys = nullptr;
            other.values = nullptr;
            other.capacity = 0;
            other.size = 0;
            return *this;
        }

        Iterator<Key, T> begin() const
        {
            return Iterator<Key, T>(keys, values, size, 0);
        }
        Iterator<Key, T> end() const
        {
            return Iterator<Key, T>(keys, values, size, size);
        }
        Iterator<const Key, T> cbegin() const
        {
            return Iterator<const Key, T>(keys, values, size, 0);
        }
        Iterator<const Key, T> cend() const
        {
            return Iterator<const Key, T>(keys, values, size, size);
        }

        Iterator<Key, T> rbegin() const
        {
            return std::reverse_iterator<Iterator<Key, T>>(
                    keys, values, size, size);
        }
        Iterator<Key, T> rend() const
        {
            return std::reverse_iterator<Iterator<Key, T>>(
                    keys, values, size, 0);
        }

        Iterator<const Key, T> crbegin() const
        {
            return std::reverse_iterator<Iterator<const Key, T>>(
                    keys, values, size, size);
        }
        Iterator<const Key, T> crend() const
        {
            return std::reverse_iterator<Iterator<const Key, T>>(
                    keys, values, size, 0);
        }

        Iterator<const Key, T> operator[](const Key& key)
        {
            return find(key);
        }

        Iterator<const Key, T> at(const Key& key)
        {
            return find(key);
        }

        bool empty() const
        {
            return size == 0;
        }

        void insert(const Key& key, const T& value)
        {
            if (size + 1 >= capacity) {
                resize(capacity * 2);
            }

            keys[size] = key;
            values[size] = value;
            size++;

            // for (size_t i = size - 1; i > 0; --i) {
            // if (std::less<Key>{}(keys[i], key)) {
            // keys[i + 1] = key;
            // values[i + 1] = value;
            // size++;
            // break;
            // } else {
            // keys[i] = std::move(keys[i + 1]);
            // values[i] = std::move(values[i + 1]);
            // if (i == 0) {
            // keys[0] = key;
            // values[0] = value;
            // size++;
            // break;
            // }
            // }
            // }
        }

        void insert_or_assign(const Key& key, const T& value)
        {
            if (contains(key)) {
                for (size_t i = 0; i < size; ++i) {
                    if (keys[i] == key) {
                        values[i] = value;
                        break;
                    }
                }
            } else {
                insert(key, value);
            }
        }

        void erase(size_t index)
        {
            if (index < size) {
                for (size_t i = index; i < size - 1; ++i) {
                    keys[i] = std::move(keys[i + 1]);
                    values[i] = std::move(values[i + 1]);
                }
                --size;
            } else {
                throw std::out_of_range("index out of range");
            }
        }

        void erase(const Key& key)
        {
            for (size_t i = 0; i < size; ++i) {
                if (keys[i] == key) {
                    erase(i);
                    break;
                }
            }
        }

        Iterator<const Key, T> find(const Key& key)
        {
            for (size_t i = 0; i < size; ++i) {
                if (keys[i] == key) {
                    return Iterator<const Key, T>(keys, values, size, i);
                }
            }
            return cend();
        }

        bool contains(const Key& key)
        {
            return find(key) != cend();
        }

        void resize(size_t new_capacity)
        {
            Key* new_keys = new Key[new_capacity];
            T* new_values = new T[new_capacity];
            for (size_t i = 0; i < size; ++i) {
                new_keys[i] = std::move(keys[i]);
                new_values[i] = std::move(values[i]);
            }
            capacity = new_capacity;
            delete[] keys;
            delete[] values;
            keys = new_keys;
            values = new_values;
        }
    };

    // template <class Key, class T>
    // std::ostream& operator<<(std::ostream& stream, FlatMap<Key, T> FlatMap)
    // {
    //     for (const auto& [key, value] : FlatMap) {
    //         stream << key << ' ' << value << '\n';
    //     }
    //     return stream;
    // }
}; // namespace fox