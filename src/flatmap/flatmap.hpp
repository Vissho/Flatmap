#pragma once

#include <iterator>

#include <iostream>

#include <stdexcept>

#include <new>

#include <algorithm>

#include <initializer_list>

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

    private:
        value_type* data_ = nullptr;
        size_t size_ = 0;
        Compare compare_;

    public:
        template <class K, class V>
        class Iterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = std::pair<K, V>;
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

        FlatMap(size_t size) : data_(nullptr), compare_()
        {
            if (size > 0) {
                data_ = static_cast<value_type*>(
                        ::operator new(size * sizeof(value_type)));
            }
        }

        template <typename InputIt>
        FlatMap(InputIt begin, InputIt end) : data_(nullptr), compare_()
        {
            const size_t dist = std::distance(begin, end);
            if (dist > 0) {
                data_ = static_cast<value_type*>(
                        ::operator new(dist * sizeof(value_type)));

                try {
                    for (auto iter = begin; iter != end; ++iter) {
                        insert(iter->first, iter->second);
                    }
                } catch (const std::exception& e) {
                    for (size_t i = 0; i < size_; ++i) {
                        data_[i].~value_type();
                    }
                    ::operator delete(data_);

                    std::cerr << e.what() << '\n';
                    throw std::out_of_range("FlatMap: out of range");
                }
            }
        }

        FlatMap(std::initializer_list<value_type> list)
            : data_(nullptr), compare_()
        {
            const size_t dist = list.size();
            if (dist > 0) {
                data_ = static_cast<value_type*>(
                        ::operator new(dist * sizeof(value_type)));

                try {
                    for (auto iter = list.begin(); iter != list.end(); ++iter) {
                        insert(iter->first, iter->second);
                    }
                } catch (const std::exception& e) {
                    for (size_t i = 0; i < size_; ++i) {
                        data_[i].~value_type();
                    }
                    ::operator delete(data_);

                    std::cerr << e.what() << '\n';
                    throw std::out_of_range("FlatMap: out of range");
                }
            }
        }

        ~FlatMap()
        {
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~value_type();
            }
            if (data_ != nullptr) {
                ::operator delete(data_);
            }
        }

        FlatMap(const FlatMap& other)
            : data_(static_cast<value_type*>(
                    ::operator new(other.size_ * sizeof(value_type)))),
              size_(other.size_)
        {
            for (size_t i = 0; i < size_; ++i) {
                new (&data_[i]) value_type(other.data_[i]);
            }
        }

        FlatMap& operator=(const FlatMap& other)
        {
            if (this != &other) {
                FlatMap temp(other);
                std::swap(data_, temp.data_);
                std::swap(size_, temp.size_);
            }
            return *this;
        }

        FlatMap(FlatMap&& other) noexcept
            : data_(other.data_), size_(other.size_)
        {
            other.data_ = nullptr;
            other.size_ = 0;
        }

        FlatMap& operator=(FlatMap&& other) noexcept
        {
            if (this != &other) {
                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~value_type();
                }
                ::operator delete(data_);
                data_ = other.data_;
                size_ = other.size_;

                other.data_ = nullptr;
                other.size_ = 0;
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

            auto* newData = static_cast<value_type*>(
                    ::operator new((size_ + 1) * sizeof(value_type)));

            const size_t insertIndex = iter - data_;

            for (size_t i = 0; i < insertIndex; ++i) {
                new (&newData[i]) value_type(std::move(data_[i]));
            }

            new (&newData[insertIndex]) value_type(key, mapped_type());

            for (size_t i = insertIndex; i < size_; ++i) {
                new (&newData[i + 1]) value_type(std::move(data_[i]));
            }

            for (size_t i = 0; i < size_; ++i) {
                data_[i].~value_type();
            }

            if (data_ != nullptr) {
                ::operator delete(data_);
            }

            ++size_;
            data_ = newData;

            return newData[insertIndex].second;
        }

        mapped_type& at(const key_type& key)
        {
            auto iter = find(key);

            if (iter != end()) {
                return iter->second;
            }

            throw std::out_of_range("Key not found in flatmap");
        }

        const mapped_type& at(const key_type& key) const
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

            throw std::out_of_range("Key not found in flatmap");
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

            if (data_ != nullptr) {
                ::operator delete(data_);
            }

            ++size_;
            data_ = newData;
        }

        void insert(const value_type& value)
        {
            insert(value.first, value.second);
        }

        void insert_or_assign(const Key& key, const T& value)
        {
            auto iter = std::lower_bound(
                    begin(),
                    end(),
                    key,
                    [this](const value_type& element, const Key& key) {
                        return compare_(element.first, key);
                    });

            if (iter != end() && !compare_(key, iter->first)) {
                iter->second = value;
            } else {
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

                if (data_ != nullptr) {
                    ::operator delete(data_);
                }

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
                auto* newData = static_cast<value_type*>(
                        ::operator new((size_ - 1) * sizeof(value_type)));

                const size_t insertIndex = iter - data_;

                for (size_t i = 0; i < insertIndex; ++i) {
                    new (&newData[i]) value_type(std::move(data_[i]));
                }

                for (size_t i = insertIndex + 1; i < size_; ++i) {
                    new (&newData[i - 1]) value_type(std::move(data_[i]));
                }

                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~value_type();
                }

                if (data_ != nullptr) {
                    ::operator delete(data_);
                }

                --size_;
                data_ = newData;

                return true;
            }

            return false;
        }

        iterator find(const key_type& key)
        {
            auto iter = std::lower_bound(
                    begin(),
                    end(),
                    key,
                    [this](const value_type& element, const Key& key) {
                        return compare_(element.first, key);
                    });

            if (iter != end() && !(compare_(key, iter->first))) {
                return iter;
            }

            return end();
        }

        bool contains(const Key& key)
        {
            return find(key) != end();
        }

        size_t size() const
        {
            return size_;
        }
    };

    template <typename K, typename T, typename Compare>
    std::ostream&
    operator<<(std::ostream& stream, const FlatMap<K, T, Compare>& flatMap)
    {
        for (const auto& pair : flatMap) {
            stream << pair.first << ' ' << pair.second << '\n';
        }
        return stream;
    }
}; // namespace fox