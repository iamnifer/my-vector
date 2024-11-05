// @iamnifer
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include "myvector.h"

template <typename T>
void print(std::string name, const MyVector<T> &v) {
    std::cout << name << ": {";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << (i ? " " : "") << v[i];
    }
    std::cout << "}" << std::endl;
}

#define print(v) print(#v, v)
#define test(fun) \
    std::cout << "test " << #fun << std::endl, fun(), std::cout << std::string(50, '-') << std::endl

void basic_constructors() {
    MyVector<int> v1;
    print(v1);
    MyVector<int> v2(3, 2);
    print(v2);
}

void copy_cons_assign() {
    MyVector<int> v1 = {1, 2, 3};
    auto v2(v1);
    v2.pop_back();
    print(v1);
    print(v2);
    v1 = v2;
    v1.pop_back();
    print(v1);
    print(v2);
}

void move_cons_assign() {
    MyVector<int> v1 = {1, 2, 3};
    auto v2(std::move(v1));
    v2.pop_back();
    print(v1);
    print(v2);
    v1 = std::move(v2);
    v1.pop_back();
    print(v1);
    print(v2);
}

void init_list() {
    MyVector<int> v = {1, 2, 3};
    print(v);
    v = {3, 4};
    print(v);
}

void range_based_for_works() {
    MyVector<int> v = {1, 2, 3};
    std::cout << "elements: ";
    for (auto i : v) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    std::cout << "2x elements: ";
    for (auto &i : v) {
        i *= 2;
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

void basic_methods() {
    MyVector<int> v1 = {1, 2, 3};
    print(v1);
    std::cout << "size: " << v1.size() << std::endl;
    std::cout << "is empty? " << v1.empty() << std::endl;
    v1.clear();
    std::cout << "is empty? " << v1.empty() << std::endl;
    v1.resize(5);
    print(v1);

    MyVector<std::string> v2;
    v2.push_back("he");
    v2.emplace_back(2, 'l');
    v2.emplace_back("o");
    print(v2);
    std::cout << "v2 back: " << v2.back() << std::endl;
}

void sorting_works() {
    MyVector<int> v2 = {3, 1, 4, 1};
    print(v2);
    MyVector<int> sorted_v2(v2);
    std::sort(sorted_v2.begin(), sorted_v2.end());
    print(sorted_v2);
    print(v2);
}

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

struct sus {
    sus() : val(3, 3) {}
    std::vector<int> val;
};

void big_random_methods() {
    MyVector<sus> a, b;
    int its = 10000;
    for (int i = 0; i < its; ++i) {
        switch (std::uniform_int_distribution<int>(1, 6)(rng)) {
            case 1:
                a.emplace_back();
                break;
            case 2:
                if (!a.empty()) {
                    a.pop_back();
                }
                break;
            case 3:
                a = std::move(b); break;
            case 4:
                a = b; break;
            case 5:
                std::swap(a, b); break;
            case 6:
                a.resize(rng() % 100); break;
        }
    }
}

constexpr int throw_cnt = 5;
struct amogus {
    amogus(int x = 0) : val(x) {
        if (++total >= throw_cnt) {
            throw std::runtime_error("O _ O");
        }
    }
    amogus& operator=(const amogus& other) {
        val = other.val;
        if (++total >= throw_cnt) {
            throw std::runtime_error("T w T");
        }
        return *this;
    }
    static int total;
    int val;
};
int amogus::total = 0;


void constructor_or_copying_throws() {
    amogus::total = 0;
    try {
        MyVector<amogus> v(1);
        v.push_back(2);
    } catch (...) {
        std::cout << "ok\n";
    }
}

void type_throws_in_constructor() {
    amogus::total = 0;
    try {
        MyVector<amogus> v(3, 1);
    } catch(...) {
        std::cout << "threw\n";
    }
    amogus::total = 0;
    try {
        MyVector<amogus> v = {1, 2};
    } catch(...) {
        std::cout << "threw\n";
    }
}

int main() {
    // some random tests
    test(basic_constructors);
    test(copy_cons_assign);
    test(move_cons_assign);
    test(init_list);
    test(range_based_for_works);
    test(basic_methods);
    test(sorting_works);
    test(big_random_methods);
    test(constructor_or_copying_throws);
    test(type_throws_in_constructor);
}
