#pragma once
#include <memory>
#include <stdexcept>
#include <utility>
#include <algorithm>

template<typename T>
class my_vector {
public:
    my_vector() noexcept = default;

    explicit my_vector(size_t n, const T &val = T()) {
        reserve(n);
        for (size_t i = 0; i < n; ++i) {
            emplace_back(val);
        }
    }

    my_vector(const my_vector &other) {
        reserve(other.sz_);
        for (size_t i = 0; i < other.sz_; ++i) {
            emplace_back(other.data_[i]);
        }
    }

    my_vector(my_vector &&other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          sz_(std::exchange(other.sz_, 0)),
          cap_(std::exchange(other.cap_, 0)) {
    }

    my_vector &operator=(my_vector other) {
        swap(other);
        return *this;
    }

    ~my_vector() {
        clear();
        deallocate();
    }

    T &operator[](size_t i) { return data_[i]; }
    const T &operator[](size_t i) const { return data_[i]; }

    T &at(size_t i) {
        if (i >= sz_) throw std::out_of_range("my_vector::at");
        return data_[i];
    }

    const T &at(size_t i) const {
        if (i >= sz_) throw std::out_of_range("my_vector::at");
        return data_[i];
    }

    T &front() { return data_[0]; }
    const T &front() const { return data_[0]; }

    T &back() { return data_[sz_ - 1]; }
    const T &back() const { return data_[sz_ - 1]; }

    [[nodiscard]] size_t size() const { return sz_; }
    [[nodiscard]] size_t capacity() const { return cap_; }
    [[nodiscard]] bool empty() const { return sz_ == 0; }

    void reserve(size_t new_cap) {
        if (new_cap > cap_) reallocate(new_cap);
    }

    void resize(size_t new_size, const T &val = T()) {
        if (new_size < sz_) {
            for (size_t i = new_size; i < sz_; ++i) {
                std::allocator_traits<decltype(alloc_)>::destroy(alloc_, data_ + i);
            }
            sz_ = new_size;
        } else if (new_size > sz_) {
            reserve(new_size);
            for (size_t i = sz_; i < new_size; ++i) {
                std::allocator_traits<decltype(alloc_)>::construct(alloc_, data_ + i, val);
            }
            sz_ = new_size;
        }
    }

    void clear() {
        for (size_t i = 0; i < sz_; ++i) {
            std::allocator_traits<decltype(alloc_)>::destroy(alloc_, data_ + i);
        }
        sz_ = 0;
    }

    void push_back(const T &val) {
        if (sz_ == cap_) grow();
        std::allocator_traits<decltype(alloc_)>::construct(alloc_, data_ + sz_, val);
        ++sz_;
    }

    void push_back(T &&val) {
        if (sz_ == cap_) grow();
        std::allocator_traits<decltype(alloc_)>::construct(alloc_, data_ + sz_, std::move(val));
        ++sz_;
    }

    template<typename... Args>
    T &emplace_back(Args &&... args) {
        if (sz_ == cap_) grow();
        std::allocator_traits<decltype(alloc_)>::construct(alloc_, data_ + sz_, std::forward<Args>(args)...);
        return data_[sz_++];
    }

    void pop_back() {
        if (sz_ == 0) return;
        std::allocator_traits<decltype(alloc_)>::destroy(alloc_, data_ + sz_ - 1);
        --sz_;
    }

    void swap(my_vector &other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(sz_, other.sz_);
        swap(cap_, other.cap_);
    }

    T begin() { return data_; }
    T end() { return data_ + sz_; }
    const T * begin() const { return data_; }
    const T * end() const { return data_ + sz_; }

private:
    std::allocator<T> alloc_;
    T *data_ = nullptr;
    size_t sz_ = 0;
    size_t cap_ = 0;

    void deallocate() {
        if (data_) {
            std::allocator_traits<decltype(alloc_)>::deallocate(alloc_, data_, cap_);
        }
        data_ = nullptr;
        cap_ = 0;
    }

    void reallocate(size_t new_cap) {
        T *new_data = std::allocator_traits<decltype(alloc_)>::allocate(alloc_, new_cap);
        for (size_t i = 0; i < sz_; ++i) {
            std::allocator_traits<decltype(alloc_)>::construct(
                alloc_, new_data + i, std::move_if_noexcept(data_[i])
            );
            std::allocator_traits<decltype(alloc_)>::destroy(alloc_, data_ + i);
        }
        deallocate();
        data_ = new_data;
        cap_ = new_cap;
    }

    void grow() {
        size_t new_cap = cap_ ? cap_ * 2 : 1;
        reallocate(new_cap);
    }
};
