#pragma once

#include <iterator>
#include <iostream>
#include <stdexcept>
#include <map>
#include <unordered_map>
// #include <functional>
#include <limits.h>

namespace fox {

    template <class T>
    class MyVector {
    private:
        T* arr;
        size_t current_capacity;
        size_t current_size;

    public:
        typedef T* iterator;
        typedef const T* const_iterator;

        iterator begin()
        {
            return arr;
        }
        iterator end()
        {
            return arr + current_size;
        }
        const_iterator cbegin() const
        {
            return arr;
        }
        const_iterator cend() const
        {
            return arr + current_size;
        }

        MyVector() : arr(nullptr), current_capacity(0), current_size(0)
        {
        }

        explicit MyVector(size_t size)
            : arr(new T[size]), current_capacity(size), current_size(size)
        {
        }

        MyVector(const MyVector& other)
            : arr(new T[other.current_capacity]),
              current_capacity(other.current_capacity),
              current_size(other.current_size)
        {
            for (size_t i = 0; i < current_size; ++i) {
                arr[i] = other.arr[i];
            }
        }

        MyVector& operator=(const MyVector& other)
        {
            if (this != &other) {
                delete[] arr;
                arr = new T[other.current_capacity];
                current_capacity = other.current_capacity;
                current_size = other.current_size;
                for (size_t i = 0; i < current_size; ++i) {
                    arr[i] = other.arr[i];
                }
            }
            return *this;
        }

        MyVector(MyVector&& other)
            : arr(other.arr),
              current_capacity(other.current_capacity),
              current_size(other.current_size)
        {
            other.arr = nullptr;
            other.current_capacity = 0;
            other.current_size = 0;
        }

        MyVector& operator=(MyVector&& other)
        {
            if (this != &other) {
                delete[] arr;
                arr = other.arr;
                current_capacity = other.current_capacity;
                current_size = other.current_size;
                other.arr = nullptr;
                other.current_capacity = 0;
                other.current_size = 0;
            }
            return *this;
        }

        ~MyVector()
        {
            delete[] arr;
        }

        void push_back(const T& value)
        {
            if (current_size >= current_capacity) {
                size_t new_capacity
                        = current_capacity == 0 ? 1 : current_capacity * 2;
                reserve(new_capacity);
            }
            for (int i = current_size + 1; i > 1; --i) {
                if (std::less<T>{}(arr[i - 1], value)) {
                    arr[i] = value;
                    current_size++;
                    break;
                } else {
                    arr[i] = std::move(arr[i - 1]);
                    if (i == 1) {
                        arr[0] = value;
                        current_size++;
                        break;
                    }
                }
            }
        }

        void pop_back()
        {
            if (current_size > 0) {
                --current_size;
            } else {
                throw std::out_of_range("pop_back called on an empty vector");
            }
        }

        T& at(size_t index)
        {
            if (index < current_size) {
                return arr[index];
            } else {
                throw std::out_of_range("index out of range");
            }
        }

        size_t size() const
        {
            return current_size;
        }

        size_t capacity() const
        {
            return current_capacity;
        }

        void reserve(size_t new_capacity)
        {
            if (new_capacity > current_capacity) {
                T* new_arr = new T[new_capacity];
                for (size_t i = 0; i < current_size; ++i) {
                    new_arr[i] = std::move(arr[i]);
                }
                current_capacity = new_capacity;
                delete[] arr;
                arr = new_arr;
            }
        }

        const T& operator[](size_t index) const
        {
            if (index < current_size) {
                return arr[index];
            } else {
                throw std::out_of_range("index out of range");
            }
        }

        const T& front() const
        {
            if (current_size > 0) {
                return arr[0];
            } else {
                throw std::out_of_range("front called on an empty vector");
            }
        }

        const T& back() const
        {
            if (current_size > 0) {
                return arr[current_size - 1];
            } else {
                throw std::out_of_range("back called on an empty vector");
            }
        }

        void clear()
        {
            current_size = 0;
        }

        void swap(MyVector& other)
        {
            std::swap(arr, other.arr);
            std::swap(current_capacity, other.current_capacity);
            std::swap(current_size, other.current_size);
        }

        T* data()
        {
            return arr;
        }

        bool empty() const
        {
            return current_size == 0;
        }

        void insert(size_t index, const T& value)
        {
            if (index < current_size) {
                arr[index] = value;
            } else {
                throw std::out_of_range("index out of range");
            }
        }

        void erase(size_t index)
        {
            if (index < current_size) {
                for (size_t i = index; i < current_size - 1; ++i) {
                    arr[i] = std::move(arr[i + 1]);
                }
                --current_size;
            } else {
                throw std::out_of_range("index out of range");
            }
        }

        size_t find(const T& value) const
        {
            for (size_t i = 0; i < current_size; ++i) {
                if (arr[i] == value) {
                    return i;
                }
            }
            return -1;
        }
    };

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
            : keys(new Key[size]), values(new T[size]), capacity(size), size(0)
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

        // FlatMap(const FlatMap<Key, T, Compare>& other) :
        //
        // {
        // }

        // FlatMap& oerator= (const FlatMap& other)
        // {
        // }

        // FlatMap(FlatMap&& other) :
        // {
        // }

        // FlatMap& oerator= (FlatMap && other)
        // {
        // }

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