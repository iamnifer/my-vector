// @iamnifer
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

template <typename T>
class MyVector {
public:
    MyVector() : size_(0), capacity_(0), data_(nullptr) {
    }
    explicit MyVector(size_t size) : size_(size), capacity_(size), data_(new T[size]) {
    }
    MyVector(size_t size, const T &value_) : MyVector(size) {
        for (size_t i = 0; i < size; i++) {
            data_[i] = value_;
        }
    }
    MyVector(std::initializer_list<T> init) : MyVector(init.size()) {
        clear();
        for (const T &value : init) {
            push_back(value);
        }
    }
    MyVector(const MyVector &other) : MyVector(other.size()) {
        // data_ = new T[capacity_];
        for (size_t i = 0; i < size_; i++) {
            data_[i] = other.data_[i];
        }
    }
    MyVector(MyVector &&other)
        : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    MyVector &operator=(const MyVector &other) {
        if (this == &other) {
            return *this;
        }
        resize(other.size_);
        for (size_t i = 0; i < size_; i++) {
            data_[i] = other[i];
        }
        return *this;
    }
    MyVector &operator=(MyVector &&other) {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
        return *this;
    }

    MyVector &operator=(std::initializer_list<T> init) {
        clear();
        for (const T &value : init) {
            push_back(value);
        }
        return *this;
    }

    ~MyVector() {
        // std::cout << "called\n";
        delete[] data_;
    }

    size_t size() const {
        return size_;
    }
    void clear() {
        size_ = 0;
    }
    bool empty() {
        return size_ == 0;
    }
    void push_back(const T &value) {
        if (size_ == capacity_) {
            reallocate();
        }
        data_[size_++] = value;
    }
    template <typename... Args>
    void emplace_back(Args... args) {
        push_back(T(std::forward<Args>(args)...));
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("pop back in an empty vector");
        }
        --size_;
    }
    void resize(size_t new_size) {
        if (new_size <= size_) {
            size_ = new_size;
            return;
        }
        while (size_ < new_size) {
            push_back(T());
        }
    }
    T &back() {
        if (empty()) {
            throw std::out_of_range("back in an empty vector");
        }
        return data_[size_ - 1];
    }

    T &operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }
    const T &operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    T *begin() {
        return data_;
    }
    T *end() {
        return data_ + size_;
    }

private:
    bool is_pw_of2(size_t val) {
        return (val & -val) == val;
    }
    void reallocate() {
        size_t new_cap = capacity_ + 1;
        while (!is_pw_of2(new_cap)) {
            ++new_cap;
        }
        T *new_data_ = new T[new_cap];
        try {
            for (size_t i = 0; i < size_; i++) {
                new_data_[i] = data_[i];
            }
        } catch (...) {
            delete[] new_data_;
            throw std::runtime_error("error while copying in reallocate()");
        }

        delete[] data_;
        data_ = new_data_;
        capacity_ = new_cap;
    }
    size_t size_, capacity_;
    T *data_;
};
