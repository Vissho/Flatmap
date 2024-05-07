#pragma once

#include <iterator>

#include <iostream>

#include <stdexcept>

#include <new>

#include <algorithm>

namespace fox {

    template <class Key, class T, class Compare = std::less<Key>>
    class FlatMap {
    public:
        using key_type = const Key;
        using mapped_type = T;
        using value_type = std::pair<key_type, mapped_type>;
        using reference = value_type&;
        using const_reference = std::pair<key_type, const mapped_type>&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    private:
        value_type* data_ = nullptr;
        size_t capacity_ = 0;
        size_t size_ = 0;
        Compare compare_;

    public:
        template <class K, class V>
        class Iterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = std::pair<const K, V>;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type*;
            using reference = value_type&;

            Iterator(pointer data) : data_(data)
            {
            }

            reference operator*()
            {
                return *data_;
            }

            pointer operator->()
            {
                return data_;
            }

            Iterator& operator++()
            {
                ++data_;
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator oldValue = *this;
                ++data_;
                return oldValue;
            }

            Iterator& operator--()
            {
                --data_;
                return *this;
            }

            Iterator operator--(int)
            {
                Iterator oldValue = *this;
                --data_;
                return oldValue;
            }

            Iterator operator+(difference_type n) const
            {
                return Iterator(data_ + n);
            }

            Iterator operator-(difference_type n) const
            {
                return Iterator(data_ - n);
            }

            difference_type operator-(Iterator other) const
            {
                return data_ - other.data_;
            }

            Iterator& operator+=(difference_type n)
            {
                data_ += n;
                return *this;
            }

            Iterator& operator-=(difference_type n)
            {
                data_ -= n;
                return *this;
            }

            reference operator[](difference_type n) const
            {
                return *(data_ + n);
            }

            friend bool operator==(const Iterator& lhs, const Iterator& rhs)
            {
                return (lhs.data_ == rhs.data_);
            }

            friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
            {
                return (lhs.data_ != rhs.data_);
            }

            friend bool operator<(const Iterator& lhs, const Iterator& rhs)
            {
                return (lhs.data_ < rhs.data_);
            }
            friend bool operator>(const Iterator& lhs, const Iterator& rhs)
            {
                return (lhs.data_ > rhs.data_);
            }
            friend bool operator<=(const Iterator& lhs, const Iterator& rhs)
            {
                return (lhs.data_ <= rhs.data_);
            }
            friend bool operator>=(const Iterator& lhs, const Iterator& rhs)
            {
                return (lhs.data_ >= rhs.data_);
            }

        private:
            pointer data_ = nullptr;
        };

        FlatMap() = default;

        FlatMap(size_t size) : capacity_(size), data_(nullptr), compare_()
        {
            if (size > 0) {
                data_ = static_cast<value_type*>(
                        ::operator new(size * sizeof(value_type)));
            }
        }

        template <typename InputIt>
        FlatMap(InputIt begin, InputIt end)
            : data_(nullptr), capacity_(std::distance(begin, end)), compare_()
        {
            if (capacity_ > 0) {
                data_ = static_cast<value_type*>(
                        ::operator new(capacity_ * sizeof(value_type)));

                size_t index = 0;
                try {
                    for (auto it = begin; it != end; ++it, ++index) {
                        new (&data_[index]) value_type(*it);
                    }
                } catch (const std::exception& e) {
                    for (size_t i = 0; i < size_; ++i) {
                        data_[i].~value_type();
                    }
                    delete[] data_;

                    std::cerr << e.what() << '\n';
                    throw std::out_of_range("FlatMap: out of range");
                }

                std::sort(
                        data_,
                        data_ + capacity_,
                        [this](const_reference lhs, const_reference rhs) {
                            return compare_(lhs.first, rhs.first);
                        });
            }
        }

        ~FlatMap()
        {
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~value_type();
            }
            delete[] data_;
        }

        FlatMap(const FlatMap& other)
            : data_(new value_type[other.size_]),
              size_(other.size_),
              capacity_(other.size_)
        {
            std::copy(other.data_, other.data_ + other.size_, data_);
        }

        FlatMap& operator=(const FlatMap& other)
        {
            if (this != &other) {
                FlatMap temp(other);
                std::swap(data_, temp.data_);
                std::swap(size_, temp.size_);
                std::swap(capacity_, temp.capacity_);
            }
            return *this;
        }

        FlatMap(FlatMap&& other) noexcept
            : data_(other.data_), capacity_(other.capacity_), size_(other.size_)
        {
            other.data_ = nullptr;
            other.capacity_ = 0;
            other.size_ = 0;
        }

        FlatMap& operator=(FlatMap&& other) noexcept
        {
            if (this != &other) {
                delete[] data_;
                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;

                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }
            return *this;
        }

        using iterator = Iterator<key_type, mapped_type>;
        using const_iterator = Iterator<key_type, const mapped_type>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        iterator begin() const
        {
            return iterator(data_);
        }
        iterator end() const
        {
            return iterator(data_ + size_);
        }
        const_iterator cbegin() const
        {
            return const_iterator(data_);
        }
        const_iterator cend() const
        {
            return const_iterator(data_ + size_);
        }

        reverse_iterator rbegin() const
        {
            return reverse_iterator(data_ + size_);
        }
        reverse_iterator rend() const
        {
            return reverse_iterator(data_);
        }

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(data_ + size_);
        }
        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(data_);
        }

        T& operator[](const Key& key)
        {
            auto iter = std::lower_bound(
                    begin(),
                    end(),
                    key,
                    [this](const value_type& element, const Key& key) {
                        return compare_(element.first, key);
                    });

            if (iter != end() && !(compare_(key, iter->first))) {
                return iter->second;
            }

            if (size_ >= capacity_) {
                resize(capacity_ * 2);
            }

            auto* newData = static_cast<value_type*>(
                    ::operator new((size_ + 1) * sizeof(value_type)));

            std::uninitialized_copy(begin(), iter, newData);

            new (newData + (iter - data_)) value_type(key, T());

            std::uninitialized_copy(iter, end(), newData + (iter - data_) + 1);

            for (size_t i = 0; i < size_; ++i) {
                data_[i].~value_type();
            }
            delete[] data_;

            ++size_;
            data_ = newData;

            return newData[iter - data_].second;
        }

        T& at(const Key& key)
        {
            auto iter = find(key);
            if (iter == cend()) {
                throw(std::out_of_range("Key not found"));
            }
            return iter.second;
        }

        bool empty() const
        {
            return size_ == 0;
        }

        void insert(const Key& key, const T& value)
        {
            auto iter = std::lower_bound(
                    begin(),
                    end(),
                    key,
                    [this](const value_type& element, const Key& key) {
                        return compare_(element.first, key);
                    });

            if (iter != end() && !(compare_(key, iter->first))) {
                throw(std::invalid_argument("Key already exists"));
            }

            if (size_ >= capacity_) {
                resize(capacity_ * 2);
            }

            auto* newData = static_cast<value_type*>(
                    ::operator new((size_ + 1) * sizeof(value_type)));

            const size_t insertIndex = iter - data_;

            for (size_t i = 0; i < insertIndex; ++i) {
                new (&newData[i]) value_type(std::move(data_[i]));
            }

            new (&newData[insertIndex]) value_type(key, value);

            for (size_t i = insertIndex; i < size_; ++i) {
                new (&newData[i + 1]) value_type(std::move(data_[i]));
            }

            for (size_t i = 0; i < size_; ++i) {
                data_[i].~value_type();
            }
            delete[] data_;

            ++size_;
            data_ = newData;
        }

        void insert_or_assign(const Key& key, const T& value)
        {
            auto iter = std::lower_bound(begin(), end(), key, compare_);
            if (iter != end() && !compare_(key, iter->first)) {
                iter->second = value;
            } else {
                if (size_ >= capacity_) {
                    resize(capacity_ * 2);
                }

                auto* newData = static_cast<value_type*>(
                        ::operator new((size_ + 1) * sizeof(value_type)));

                std::uninitialized_copy(begin(), iter, newData);

                new (newData + (iter - data_)) value_type(key, value);

                std::uninitialized_copy(
                        iter, end(), newData + (iter - data_) + 1);

                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~value_type();
                }
                delete[] data_;

                ++size_;
                data_ = newData;
            }
        }

        bool erase(const Key& key)
        {
            auto iter = std::lower_bound(
                    begin(),
                    end(),
                    key,
                    [this](const value_type& element, const Key& key) {
                        return compare_(element.first, key);
                    });

            if (iter != end() && !(compare_(key, iter->first))) {
                std::move(iter + 1, end(), iter);
                --size_;
                data_[size_].~value_type();
                return true;
            }

            return false;
        }

        iterator erase(iterator pos)
        {
            if (pos >= begin() && pos < end()) {
                std::move(pos + 1, end(), pos);
                --size_;
                data_[size_].~value_type();
                return pos;
            }

            return end();
        }

        iterator erase(iterator first, iterator last)
        {
            if (first >= begin() && last <= end() && first <= last) {
                difference_type range = std::distance(first, last);
                std::move(last, end(), first);

                while (range > 0) {
                    --size_;
                    data_[size_].~value_type();
                    --range;
                }

                return first;
            }

            return end();
        }

        iterator find(const Key& key)
        {
            for (size_t i = 0; i < size_; ++i) {
                if (data_[i].first == key) {
                    return iterator(data_, size_, i);
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
            auto* new_data = new value_type[new_capacity];

            for (size_t i = 0; i < size_; ++i) {
                new (new_data + i) value_type(std::move(data_[i]));
            }
            capacity_ = new_capacity;
            delete[] data_;
            data_ = new_data;
        }
    };

    // template <class Key, class T>
    // std::ostream& operator<<(std::ostream& stream, FlatMap<Key, T>
    // FlatMap)
    // {
    //     for (const auto& [key, value] : FlatMap) {
    //         stream << key << ' ' << value << '\n';
    //     }
    //     return stream;
    // }
}; // namespace fox