#include "vector.h"
#include <cassert>
#include <iostream>
#include <string>
#include <utility>

using std::cout;

void test_basic_int() {
    my_vector<int> v;
    assert(v.empty());

    v.push_back(10);
    v.push_back(20);
    v.emplace_back(30);
    assert(v.size() == 3);
    assert(v[0] == 10 && v[1] == 20 && v[2] == 30);

    v.pop_back();
    assert(v.size() == 2);
    assert(v.back() == 20);

    v.resize(5, 99);
    assert(v.size() == 5);
    assert(v[4] == 99);

    v.clear();
    assert(v.empty());
}

void test_strings() {
    my_vector<std::string> s;
    s.push_back("hello");
    s.emplace_back("world");
    assert(s.size() == 2);
    assert(s.front() == "hello");
    assert(s.back() == "world");

    my_vector<std::string> copy = s;
    assert(copy.size() == 2 && copy[1] == "world");

    my_vector<std::string> moved = std::move(s);
    assert(moved.size() == 2);
}

void test_iterators() {
    my_vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i);
    int sum = 0;
    for (auto x: v) sum += x;
    assert(sum == 10);
}

void test_at_exceptions() {
    my_vector<int> v(2, 1);
    bool thrown = false;
    try {
        v.at(3);
    } catch (const std::out_of_range &) {
        thrown = true;
    }
    assert(thrown);
}

int main() {
    test_basic_int();
    test_strings();
    test_iterators();
    test_at_exceptions();
    cout << "SUCCESS\n";
}
