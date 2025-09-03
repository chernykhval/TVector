// Copyright 2025 Chernykh Valentin
#include <windows.h>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <utility>

#include "TVector.h"

void set_color(int text_color, int bg_color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg_color << 4) | text_color);
}

namespace TestSystem {
int count_success = 0, count_failed = 0;

void start_test(bool(*test)(), const char* name_of_test) {
    set_color(2, 0);
    std::cout << "[ RUN      ]";
    set_color(7, 0);

    std::cout << name_of_test << std::endl;

    bool status = test();

    if (status == true) {
        set_color(2, 0);
        std::cout << "[       OK ]" << std::endl;
        count_success++;
    } else {
        set_color(4, 0);
        std::cout << "[  FAILED  ]" << std::endl;
        count_failed++;
    }
    set_color(7, 0);
}

template <class T>
bool check_exp(const T& expected, const T& actual) {
    if (expected == actual) {
        return true;
    } else {
        std::cout << "Expected result is " << expected <<
            ", but actual is " << actual << "." << std::endl;
        return false;
    }
}

template <class T>
bool check_unexp(const T& unexpected, const T& actual) {
    if (unexpected != actual) {
        return true;
    } else {
        std::cout << "Unexpected result is " << unexpected <<
            ", but actual is " << actual << "." << std::endl;
        return false;
    }
}

void print_init_info() {
    set_color(2, 0);
    std::cout << "[==========] " << std::endl;
    set_color(7, 0);
}

void print_final_info() {
    set_color(2, 0);
    std::cout << "[==========] ";
    set_color(7, 0);
    std::cout << count_success + count_failed << " test" <<
        (count_success + count_failed > 1 ? "s" : "") << " ran." << std::endl;
    set_color(2, 0);
    std::cout << "[  PASSED  ] ";
    set_color(7, 0);
    std::cout << count_success << " test" <<
        (count_success > 1 ? "s" : "") << std::endl;
    if (count_failed > 0) {
        set_color(4, 0);
        std::cout << "[  FAILED  ] ";
        set_color(7, 0);
        std::cout << count_failed << " test" <<
            (count_failed > 1 ? "s." : ".") << std::endl;
    }
}
};  // namespace TestSystem

#pragma region TvectorTests

bool tvector_default_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec;

    if (vec.data() != nullptr || vec.begin() != vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(0), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(0), vec.capacity());
}
bool tvector_size_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec(25);

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(25), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec.capacity());
}
bool tvector_size_and_value_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec(25, 5);

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(25), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec.capacity());
}
bool tvector_copy_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1(25, 5);
    TVector<int> vec_2(vec_1);

    if (vec_1.data() == nullptr || vec_1.begin() == vec_1.end()) {
        actual_result = false;
    }

    if (vec_2.data() == nullptr || vec_2.begin() == vec_2.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(25), vec_1.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec_1.capacity()) &&
        TestSystem::check_exp(static_cast<size_t>(25), vec_2.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec_2.capacity());
}
bool tvector_move_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1(25, 5);
    TVector<int> vec_2(std::move(vec_1));

    if (vec_1.data() != nullptr || vec_1.begin() != vec_1.end()) {
        actual_result = false;
    }

    if (vec_2.data() == nullptr || vec_2.begin() == vec_2.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(0), vec_1.size()) &&
        TestSystem::check_exp(static_cast<size_t>(0), vec_1.capacity()) &&
        TestSystem::check_exp(static_cast<size_t>(25), vec_2.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec_2.capacity());
}
bool tvector_array_init() {
    bool actual_result = true;
    bool expected_result = true;
    int* array = new int[16] { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec(array, 16);

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(16), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec.capacity());
}
bool tvector_initialize_list_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(16), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec.capacity());
}

bool tvector_operator_copy_assign() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec_2;

    vec_2 = vec_1;

    if (vec_1.data() == nullptr || vec_1.begin() == vec_1.end()) {
        actual_result = false;
    }

    if (vec_2.data() == nullptr || vec_2.begin() == vec_2.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(16), vec_1.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec_1.capacity()) &&
        TestSystem::check_exp(static_cast<size_t>(16), vec_2.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec_2.capacity());
}
bool tvector_operator_move_assign() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec_2;

    vec_2 = std::move(vec_1);

    if (vec_1.data() != nullptr || vec_1.begin() != vec_1.end()) {
        actual_result = false;
    }

    if (vec_2.data() == nullptr || vec_2.begin() == vec_2.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(0), vec_1.size()) &&
        TestSystem::check_exp(static_cast<size_t>(0), vec_1.capacity()) &&
        TestSystem::check_exp(static_cast<size_t>(16), vec_2.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec_2.capacity());
}
bool tvector_operator_equality() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec_2 = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };

    actual_result = vec_1 == vec_2;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_operator_inequality() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec_2 = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 17 };

    actual_result = vec_1 != vec_2;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_operator_index_access() {
    const TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = vec_1[13];
    int expected_result = 14;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_operator_index_access_exception() {
    bool actual_result = true;
    bool expected_result = false;
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };

    int num;

    try {
        num = vec[17];
    }
    catch (std::out_of_range& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_const_operator_index_access() {
    const TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = vec_1[13];
    int expected_result = 14;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_const_operator_index_access_exception() {
    bool actual_result = true;
    bool expected_result = false;
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };

    try {
        int num = vec[17];
    }
    catch (std::out_of_range& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}

bool tvector_data() {
    const TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *vec.data();
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_const_data() {
    const TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *vec.data();
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_size() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    int actual_result = vec.size();
    int expected_result = 5;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_capacity() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    int actual_result = vec.capacity();
    int expected_result = 15;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_front() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    int actual_result = vec.front();
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_back() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    int actual_result = vec.back();
    int expected_result = 5;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_begin() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int>::Iterator it = vec.begin();

    int actual_result = *it;
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_end() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int>::Iterator it = vec.end();

    int actual_result = *(it-1);
    int expected_result = 16;

    return TestSystem::check_exp(expected_result, actual_result);
}

// bool tvector_push_back_copy_filled() {
//    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8,
//    9, 10, 11, 12, 13, 14, 15 };
//    int a = 16;
//    actual_result.push_back(a);
//    TVector<int> expected_result = { 1, 2, 3, 4, 5, 6, 7, 8,
//    9, 10, 11, 12, 13, 14, 15, 16 };
//
//    return TestSystem::check_exp(expected_result, actual_result) &&
//       TestSystem::check_exp(static_cast<size_t>(16), actual_result.size()) &&
//     TestSystem::check_exp(static_cast<size_t>(30), actual_result.capacity());
// }
// bool tvector_push_back_copy_empty() {
//    TVector<int> actual_result;
//    int a = 16;
//    actual_result.push_back(a);
//    TVector<int> expected_result = { 16 };
//
//    return TestSystem::check_exp(expected_result, actual_result) &&
//        TestSystem::check_exp(static_cast<size_t>(1), actual_result.size()) &&
//     TestSystem::check_exp(static_cast<size_t>(15), actual_result.capacity());
// }
// bool tvector_push_back_copy_empty() {
//    TVector<int> actual_result;
//    int a = 16;
//    actual_result.push_back(a);
//    TVector<int> expected_result = { 16 };
//
//    return TestSystem::check_exp(expected_result, actual_result) &&
//        TestSystem::check_exp(static_cast<size_t>(1), actual_result.size()) &&
//     TestSystem::check_exp(static_cast<size_t>(15), actual_result.capacity());
// }
// bool tvector_push_back_copy() {
//    return tvector_push_back_copy_filled() && tvector_push_back_copy_empty();
// }
bool tvector_push_back_copy_empty() {
    TVector<int> actual_result;
    int a = 16;
    actual_result.push_back(a);
    TVector<int> expected_result = { 16 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(1), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(15),
            actual_result.capacity());
}
bool tvector_push_back_copy_with_capacity() {
    TVector<int> actual_result = { 1, 2, 3 };
    int a = 4;
    size_t old_capacity = actual_result.capacity();
    size_t old_size = actual_result.size();

    actual_result.push_back(a);

    TVector<int> expected_result = { 1, 2, 3, 4 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(old_size + 1, actual_result.size()) &&
        TestSystem::check_exp(old_capacity, actual_result.capacity());
}
bool tvector_push_back_copy_full_capacity() {
    TVector<int> actual_result;
    for (int i = 0; i < static_cast<int>(actual_result.capacity()); ++i) {
        actual_result.push_back(i);
    }
    int a = 100;
    size_t old_capacity = actual_result.capacity();
    size_t old_size = actual_result.size();

    actual_result.push_back(a);

    TVector<int> expected_result;
    for (size_t i = 0; i < old_size; ++i)
        expected_result.push_back(static_cast<int>(i));
    expected_result.push_back(a);

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(old_size + 1, actual_result.size()) &&
        TestSystem::check_exp(true, actual_result.capacity() > old_capacity);
}
bool tvector_push_back_copy_large_vector() {
    TVector<int> actual_result;
    const int large_size = 1000000;
    for (int i = 0; i < large_size; ++i) {
        actual_result.push_back(i);
    }
    int a = 42;
    actual_result.push_back(a);

    bool size_ok = TestSystem::check_exp(static_cast<size_t>((large_size + 1)),
        actual_result.size());
    bool last_elem_ok = (actual_result[large_size] == a);
    return size_ok && last_elem_ok;
}
bool tvector_push_back_copy_after_shrink_to_fit() {
    TVector<int> actual_result;
    for (int i = 0; i < static_cast<int>(actual_result.capacity()); ++i) {
        actual_result.push_back(i);
    }
    actual_result.shrink_to_fit();

    size_t old_capacity = actual_result.capacity();
    size_t old_size = actual_result.size();

    int a = 999;
    actual_result.push_back(a);

    TVector<int> expected_result;
    for (size_t i = 0; i < old_size; ++i)
        expected_result.push_back(static_cast<int>(i));
    expected_result.push_back(a);

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(old_size + 1, actual_result.size()) &&
        TestSystem::check_exp(true, actual_result.capacity() > old_capacity);
}
bool tvector_push_back_copy() {
    return
        tvector_push_back_copy_empty() &&
        tvector_push_back_copy_with_capacity() &&
        tvector_push_back_copy_full_capacity() &&
        tvector_push_back_copy_after_shrink_to_fit();
}

bool tvector_push_back_move() {
    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15 };
    actual_result.push_back(16);
    TVector<int> expected_result = { 1, 2, 3, 4, 5, 6, 7, 8,
     9, 10, 11, 12, 13, 14, 15, 16 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(16), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30),
            actual_result.capacity());
}
bool tvector_push_front_copy() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    int a = 1;
    actual_result.push_front(a);
    TVector<int> expected_result = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(16), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30),
            actual_result.capacity());
}
bool tvector_push_front_move() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    actual_result.push_front(1);
    TVector<int> expected_result = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(16), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30),
            actual_result.capacity());
}
bool tvector_insert_copy() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int a = 12;
    actual_result.insert(actual_result.begin() + 2, a);
    TVector<int> expected_result = { 2, 3, 12, 4, 5, 6, 7, 8, 9, 10};

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(10), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(15),
            actual_result.capacity());
}
bool tvector_insert_elems_copy() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int a = 12;
    actual_result.insert(actual_result.begin() + 2, 7, a);
    TVector<int> expected_result = { 2, 3, 12, 12, 12, 12, 12,
        12, 12, 4, 5, 6, 7, 8, 9, 10 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(16), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30),
            actual_result.capacity());
}
bool tvector_insert_move() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    actual_result.insert(actual_result.begin() + 2, 12);
    TVector<int> expected_result = { 2, 3, 12, 4, 5, 6, 7, 8, 9, 10 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(10), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(15),
            actual_result.capacity());
}
bool tvector_emplace() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    actual_result.emplace(actual_result.begin() + 2, 12);
    TVector<int> expected_result = { 2, 3, 12, 4, 5, 6, 7, 8, 9, 10 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(10), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(15),
            actual_result.capacity());
}

bool tvector_pop_back_empty() {
    TVector<int> vec;

    bool actual_result = true;
    bool expected_result = false;

    try {
        vec.pop_back();
    }
    catch (std::exception& ex) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_pop_back_with_capacity() {
    TVector<int> actual_result = { 1, 2, 3 };
    size_t old_capacity = actual_result.capacity();
    size_t old_size = actual_result.size();

    actual_result.pop_back();

    TVector<int> expected_result = { 1, 2 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(old_size - 1, actual_result.size()) &&
        TestSystem::check_exp(old_capacity, actual_result.capacity());
}
bool tvector_pop_back_less_capacity() {
    TVector<int> actual_result;
    for (int i = 0; i < 16; ++i) {
        actual_result.push_back(i);
    }
    size_t old_capacity = actual_result.capacity();
    size_t old_size = actual_result.size();

    actual_result.pop_back();
    actual_result.pop_back();
    actual_result.pop_back();

    TVector<int> expected_result;
    for (size_t i = 0; i < 13; ++i)
        expected_result.push_back(static_cast<int>(i));

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(old_size - 3, actual_result.size()) &&
        TestSystem::check_exp(true, actual_result.capacity() < old_capacity);
}
bool tvector_pop_back_large_vector() {
    TVector<int> actual_result(1000000);
    const int large_size = 1000000;
    int a = 10;
    for (int i = 0; i < large_size; ++i) {
        actual_result.pop_back();
        if (a == i) {
            std::cout << i << std::endl;
            a *= 10;
        }
    }

    return TestSystem::check_exp(static_cast<size_t>(0), actual_result.size());
}
bool tvector_pop_back_after_shrink_to_fit() {
    TVector<int> actual_result(16);
    actual_result.shrink_to_fit();

    size_t old_capacity = actual_result.capacity();
    size_t old_size = actual_result.size();

    actual_result.pop_back();
    actual_result.pop_back();
    actual_result.pop_back();

    return TestSystem::check_exp(old_size - 3, actual_result.size()) &&
        TestSystem::check_exp(true, actual_result.capacity() < old_capacity);
}
bool tvector_pop_back() {
    return
        tvector_pop_back_empty() &&
        tvector_pop_back_with_capacity() &&
        tvector_pop_back_less_capacity() &&
        tvector_pop_back_after_shrink_to_fit();
}
bool tvector_pop_front() {
    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    TVector<int> expected_result = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

    actual_result.pop_front();
    actual_result.pop_front();
    actual_result.pop_front();
    actual_result.pop_front();
    actual_result.pop_front();
    actual_result.pop_front();

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(25), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30),
            actual_result.capacity());
}
bool tvector_erase() {
    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    TVector<int> expected_result = { 3, 4, 5, 6, 8, 9, 10, 12, 13, 14, 16, 17,
        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

    actual_result.erase(actual_result.begin());
    actual_result.erase(actual_result.end() - 1);
    actual_result.erase(actual_result.begin());
    actual_result.erase(actual_result.end() - 1);
    actual_result.erase(actual_result.begin() + 4);
    actual_result.erase(actual_result.begin() + 11);
    actual_result.erase(actual_result.begin() + 7);

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(24), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30),
            actual_result.capacity());
}

bool tvector_erase_empty() {
    TVector<int> vec;
    bool actual_result = true;
    bool expected_result = false;

    try {
        vec.erase(vec.begin());
    }
    catch (const std::exception& ex) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}

bool comp(int a, int b) {
    return a > b || a > 10;
}
bool tvector_test() {
    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
        31, 32, 33, 34, 35, 36 };
    TVector<int> expected_result = { 2, 3, 4, 5, 7, 8, 9, 11, 222, 12, 13,
        14, 15, 16, 18, 20, 21, 333, 22, 23, 24, 25, 26, 27, 28, 30,
        31, 111, 32, 33, 34, 35 };

    actual_result.pop_back();
    actual_result.pop_front();
    actual_result.erase(actual_result.begin() + 8);
    actual_result.erase(actual_result.begin() + 4);
    actual_result.erase(actual_result.begin() + 25);
    actual_result.erase(actual_result.begin() + 15);
    actual_result.erase(actual_result.begin() + 13);
    actual_result.insert(actual_result.begin() + 25, 111);
    actual_result.insert(actual_result.begin() + 8, 222);
    actual_result.insert(actual_result.begin() + 17, 333);

    //  shuffle(actual_result);

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(32), actual_result.size()) &&
        TestSystem::check_exp(static_cast<size_t>(45),
            actual_result.capacity());
}

bool tvector_iterator_init() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    bool actual_result = true;
    bool expected_result = true;

    if (*vec.begin() != 1) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_dereference_operator() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    bool actual_result = true;
    bool expected_result = true;

    try {
        TVector<int>::Iterator it = vec.begin();

        if (*it != 1) {
            actual_result = false;
            std::cout << "Dereference failed."
                         " Expected 1, got " << *it << std::endl;
        }

        *it = 10;

        if (vec[0] != 10) {
            actual_result = false;
            std::cout << "Failed to modify element"
                         " through dereference." << std::endl;
        }

        ++it;

        if (*it != 2) {
            actual_result = false;
            std::cout << "Dereference after increment"
                         " failed. Expected 2, got " << *it << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_dereference_operator_empty() {
    TVector<int> vec;
    bool actual_result = true;
    bool expected_result = false;

    try {
        TVector<int>::Iterator it = vec.end();
        int value = *it;
    }
    catch (const std::exception& ex) {
        actual_result = false;
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_dereference_operator_out_of_range() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    bool actual_result = true;
    bool expected_result = false;

    try {
        TVector<int>::Iterator it = vec.end();
        int value = *it;
    }
    catch (const std::exception& ex) {
        actual_result = false;
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_arrow_operator() {
    TVector<int> vect(1, 5);
    TVector<TVector<int>> vec(1, vect);
    bool actual_result = true;
    bool expected_result = true;

    try {
        if (vec.begin()->front() != 5) {
            actual_result = false;
            std::cout << "operator-> returned"
                         " wrong pointer." << vec.begin()->size() << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_assign() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int>::Iterator it_1 = vec.end();
    it_1 = vec.begin();

    if (*it_1 != 1) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_left_increment() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *(++vec.begin());
    int expected_result = 2;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_right_increment() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
     9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *(vec.begin()++);
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_left_decrement() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
     9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *(--vec.end());
    int expected_result = 16;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_right_decrement() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
     9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *((vec.end()-1)--);
    int expected_result = 16;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_add() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    int actual_result = *(vec.begin() + 2);
    int expected_result = 3;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_add_out_off_range_exception() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    bool actual_result = true;
    bool expected_result = false;

    try {
        vec.begin() + 6;
    }
    catch (const std::exception& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    try {
        actual_result = true;
        vec.begin() + (-1);
    }
    catch (const std::exception& ex) {
        actual_result = false;
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_sub() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    int actual_result = *(vec.end() - 2);
    int expected_result = 4;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_sub_out_off_range_exception() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    bool actual_result = true;
    bool expected_result = false;

    try {
        vec.end() - 6;
    }
    catch (const std::exception& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    try {
        actual_result = true;
        vec.end() - (-1);
    }
    catch (const std::exception& ex) {
        actual_result = false;
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_add_assign() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    TVector<int>::Iterator bg = vec.begin();
    bool actual_result = true;
    bool expected_result = true;

    if (*bg != 1)
        actual_result = false;

    bg += 2;

    if (*bg != 3)
        actual_result = false;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_add_assign_out_off_range_exception() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    TVector<int>::Iterator bg = vec.begin();
    bool actual_result = true;
    bool expected_result = false;

    try {
        bg = vec.begin();
        bg += 6;
    }
    catch (const std::exception& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    try {
        actual_result = true;
        bg = vec.begin();
        bg += (-1);
    }
    catch (const std::exception& ex) {
        actual_result = false;
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_sub_assign() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    TVector<int>::Iterator ed = vec.end();
    bool actual_result = true;
    bool expected_result = true;

    if (*(ed - 1) != 5)
        actual_result = false;

    ed -= 3;

    if (*ed != 3)
        actual_result = false;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_sub_assign_out_off_range_exception() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    TVector<int>::Iterator ed = vec.end();
    bool actual_result = true;
    bool expected_result = false;

    try {
        ed = vec.end();
        ed -= 6;
    }
    catch (const std::exception& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    try {
        actual_result = true;
        ed = vec.end();
        ed -= (-1);
    }
    catch (const std::exception& ex) {
        actual_result = false;
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_operator_equality() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };

    TVector<int>::Iterator it_1 = vec.begin();
    TVector<int>::Iterator it_2 = vec.begin();

    bool actual_result = it_1 == it_2;
    bool expected_result = true;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_operator_inequality() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };

    TVector<int>::Iterator it_1 = vec.begin();
    TVector<int>::Iterator it_2 = vec.end();

    bool actual_result = it_1 != it_2;
    bool expected_result = true;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_operator_iterator_difference() {
    TVector<int> vec = { 1, 2, 3, 4, 5 };
    vec.pop_front();
    TVector<int>::Iterator it_1 = vec.begin();
    TVector<int>::Iterator it_2 = vec.end();

    int actual_result = it_2 - it_1;
    int expected_result = 4;

    return TestSystem::check_exp(expected_result, actual_result);
}

bool tvector_clear() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    vec.clear();
    bool actual_result = true;
    bool expected_result = true;

    if (vec.data() == nullptr || vec.begin() != vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(0), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(15), vec.capacity());
}
bool tvector_shrink_to_fit() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16 };
    vec.shrink_to_fit();
    bool actual_result = true;
    bool expected_result = true;

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(16), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(16), vec.capacity());
}
bool tvector_resize_1() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31, 32 };

    vec.pop_back();
    vec.pop_front();
    vec.erase(vec.begin() + 8);
    vec.erase(vec.begin() + 4);
    vec.erase(vec.begin() + 25);
    vec.erase(vec.begin() + 15);
    vec.erase(vec.begin() + 13);
    vec.insert(vec.begin() + 25, 111);
    vec.insert(vec.begin() + 8, 222);
    vec.insert(vec.begin() + 17, 333);
    vec.resize(32);
    bool actual_result = true;
    bool expected_result = true;

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }


    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(32), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(45), vec.capacity());
}
bool tvector_resize_2() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, 26, 27, 28, 29, 30, 31, 32 };

    vec.pop_back();
    vec.pop_front();
    vec.erase(vec.begin() + 8);
    vec.erase(vec.begin() + 4);
    vec.erase(vec.begin() + 25);
    vec.erase(vec.begin() + 15);
    vec.erase(vec.begin() + 13);
    vec.insert(vec.begin() + 25, 111);
    vec.insert(vec.begin() + 8, 222);
    vec.insert(vec.begin() + 17, 333);
    vec.resize(25);
    bool actual_result = true;
    bool expected_result = true;

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(25), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(30), vec.capacity());
}
bool tvector_resize_3() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
        22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };

    vec.pop_back();
    vec.pop_front();
    vec.erase(vec.begin() + 8);
    vec.erase(vec.begin() + 4);
    vec.erase(vec.begin() + 25);
    vec.erase(vec.begin() + 15);
    vec.erase(vec.begin() + 13);
    vec.insert(vec.begin() + 25, 111);
    vec.insert(vec.begin() + 8, 222);
    vec.insert(vec.begin() + 17, 333);
    vec.resize(5);
    bool actual_result = true;
    bool expected_result = true;

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(5), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(15), vec.capacity());
}
bool tvector_shuffle() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    vec.pop_back();
    vec.pop_back();
    vec.pop_front();
    vec.erase(vec.begin() + 2);
    shuffle(vec);

    bool actual_result = true;
    bool expected_result = true;

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp(static_cast<size_t>(6), vec.size()) &&
        TestSystem::check_exp(static_cast<size_t>(15), vec.capacity());
}

bool find_chet(int a) {
    return a %2 == 0;
}
bool tvector_search_begin() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    vec.pop_front();
    int actual_result = search_begin(vec, find_chet);
    int expected_result = 0;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_search_end() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    vec.pop_front();
    vec.pop_back();
    int actual_result = search_end(vec, find_chet);
    int expected_result = 6;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_search_all() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    vec.pop_front();
    vec.pop_back();

    bool actual_result = true;
    bool expected_result = true;

    int* searched_result = search_all(vec, find_chet);
    int* need_result = new int[4] {0, 2, 4, 6};

    for (int i = 0; i < 4; i++) {
        if (searched_result[i] != need_result[i])
            actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}

bool sort_check(uint64_t a, uint64_t b) {
    return a < b;
}

bool tvector_sort() {
    TVector<uint64_t> expected_result(100000000);
    std::cout << "vectors memory" << std::endl;
    TVector<uint64_t> actual_result(100000000);
    uint64_t a = 10;

    for (uint64_t i = 0; i < 100000000; i++) {
        expected_result[i] = i;

        if (i == a) {
            std::cout << i << std::endl;
            a *= 10;
        }
    }

    a = 10;

    for (uint64_t i = 0; i < 100000000; i++) {
        actual_result[100000000 - 1 - i] = i;

        if (i == a) {
            std::cout << i << std::endl;
            a *= 10;
        }
    }
    std::cout << "vectors created" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    tv_sort(actual_result, sort_check);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Sort waste " << duration.count()
    << " milliseconds." << std::endl;

    return TestSystem::check_exp(expected_result, actual_result);
}
#pragma endregion

#pragma region AdditionalTVectorTests

// Test size_and_value_init with edge cases
bool tvector_size_value_init_zero() {
    bool actual_result = true;
    bool expected_result = false;
    try {
        TVector<int> vec(0, 5);
    }
    catch (const std::exception& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
        actual_result = false;
    }
    return TestSystem::check_exp(expected_result, actual_result);
}

bool tvector_size_value_init_large() {
    TVector<int> vec(1000, 42);
    bool all_values_correct = true;
    for (size_t i = 0; i < vec.size() && all_values_correct; ++i) {
        if (vec[i] != 42) {
            all_values_correct = false;
        }
    }
    return TestSystem::check_exp(static_cast<size_t>(1000), vec.size()) &&
           TestSystem::check_exp(static_cast<size_t>(1005), vec.capacity()) &&
           TestSystem::check_exp(true, all_values_correct);
}

// Test assignment operators with edge cases
bool tvector_self_assignment() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int> original = vec;
    vec = vec;  // Self-assignment
    return TestSystem::check_exp(original, vec);
}

bool tvector_assignment_different_sizes() {
    TVector<int> vec1 = {1, 2, 3};
    TVector<int> vec2 = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    vec1 = vec2;
    return TestSystem::check_exp(vec2, vec1);
}

  // Test states() method
/*bool tvector_states_access() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    State* states = vec.states();
    bool all_busy = true;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (states[i] != Busy) {
            all_busy = false;
            break;
        }
    }
    return TestSystem::check_exp(true, all_busy);
}

bool tvector_states_after_erase() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    vec.erase(vec.begin() + 2);
    State* states = vec.states();
    // Check that there's at least one deleted state
    bool has_deleted = false;
    for (size_t i = 0; i < vec.capacity(); ++i) {
        if (states[i] == Deleted) {
            has_deleted = true;
            break;
        }
    }
    return TestSystem::check_exp(true, has_deleted);
}

// Test used() method
bool tvector_used_vs_size() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    vec.erase(vec.begin() + 2);
    return TestSystem::check_exp(vec.used() - 1, vec.size());
}
*/
// Test front() and back() with edge cases
bool tvector_front_exception() {
    TVector<int> vec;
    bool caught_exception = false;
    try {
        vec.front();
    } catch (const std::runtime_error&) {
        caught_exception = true;
    }
    return TestSystem::check_exp(true, caught_exception);
}

bool tvector_back_exception() {
    TVector<int> vec;
    bool caught_exception = false;
    try {
        vec.back();
    } catch (const std::runtime_error&) {
        caught_exception = true;
    }
    return TestSystem::check_exp(true, caught_exception);
}

bool tvector_front_back_after_operations() {
    TVector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vec.pop_front();
    vec.pop_back();
    vec.erase(vec.begin() + 2);
    return TestSystem::check_exp(2, vec.front()) &&
           TestSystem::check_exp(9, vec.back());
}

// Test at() method
/*bool tvector_at_valid_index() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    return TestSystem::check_exp(3, vec.at(2));
}

bool tvector_at_invalid_index() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    bool caught_exception = false;
    try {
        vec.at(10);
    } catch (const std::out_of_range&) {
        caught_exception = true;
    }
    return TestSystem::check_exp(true, caught_exception);
}*/

// Test assign() method
/*bool tvector_assign_method() {
    TVector<int> vec1 = {1, 2, 3};
    TVector<int> vec2 = {10, 20, 30, 40, 50};
    vec1.assign(vec2);
    return TestSystem::check_exp(vec2, vec1);
}*/

// Test is_empty() method
bool tvector_is_empty_true() {
    TVector<int> vec;
    return TestSystem::check_exp(true, vec.is_empty());
}

bool tvector_is_empty_false() {
    TVector<int> vec = {1, 2, 3};
    return TestSystem::check_exp(false, vec.is_empty());
}

bool tvector_is_empty_after_clear() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    vec.clear();
    return TestSystem::check_exp(true, vec.is_empty());
}

bool tvector_is_empty_after_all_erased() {
    TVector<int> vec = {1, 2, 3};
    vec.erase(vec.begin());
    vec.erase(vec.begin());
    vec.erase(vec.begin());
    return TestSystem::check_exp(true, vec.is_empty());
}

// Test resize with value parameter
bool tvector_resize_with_value() {
    TVector<int> vec = {1, 2, 3};
    vec.resize(6);
    bool correct_values = true;
    if (vec.size() != 6) correct_values = false;
    return TestSystem::check_exp(true, correct_values);
}

// Test iterator boundary conditions
bool tvector_iterator_begin_end_empty() {
    TVector<int> vec;
    return TestSystem::check_exp(true, vec.begin() == vec.end());
}

bool tvector_iterator_arithmetic_edge_cases() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int>::Iterator it = vec.begin();

    // Test adding 0
    TVector<int>::Iterator it_plus_zero = it + 0;
    bool add_zero_ok = (it == it_plus_zero);

    // Test subtracting 0
    TVector<int>::Iterator it_minus_zero = it - 0;
    bool sub_zero_ok = (it == it_minus_zero);

    return TestSystem::check_exp(true, add_zero_ok) &&
           TestSystem::check_exp(true, sub_zero_ok);
}

// Test iterator with deleted elements
bool tvector_iterator_skip_deleted() {
    TVector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vec.erase(vec.begin() + 2);  // Delete element at index 2 (value 3)
    vec.erase(vec.begin() + 4);  // Delete element at index 4 (value 6)

    // Iterate through and collect values
    TVector<int> collected;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        collected.push_back(*it);
    }

    TVector<int> expected = {1, 2, 4, 5, 7, 8, 9, 10};
    return TestSystem::check_exp(expected, collected);
}

// Test memory management after many operations
bool tvector_memory_management_stress() {
    TVector<int> vec;

    // Fill vector
    for (int i = 0; i < 100; ++i) {
        vec.push_back(i);
    }

    // Remove every third element
    for (int i = 99; i >= 0; i -= 3) {
        if (i < static_cast<int>(vec.size())) {
            vec.erase(vec.begin() + i);
        }
    }

    // Add some more elements
    for (int i = 1000; i < 1020; ++i) {
        vec.push_back(i);
    }

    // Check that size is reasonable and vector is still functional
    bool size_reasonable = vec.size() > 50 && vec.size() < 150;
    bool can_access_elements = true;
    try {
        vec.front();
        vec.back();
        vec[0];
        vec[vec.size() - 1];
    } catch (...) {
        can_access_elements = false;
    }

    return TestSystem::check_exp(true, size_reasonable) &&
           TestSystem::check_exp(true, can_access_elements);
}

// Test copy constructor with deleted elements
bool tvector_copy_with_deleted_elements() {
    TVector<int> vec1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vec1.erase(vec1.begin() + 2);
    vec1.erase(vec1.begin() + 4);
    vec1.erase(vec1.begin() + 6);

    TVector<int> vec2(vec1);  // Copy constructor

    return TestSystem::check_exp(vec1, vec2) &&
           TestSystem::check_exp(vec1.size(), vec2.size());
}

// Test move constructor edge cases
bool tvector_move_from_empty() {
    TVector<int> vec1;
    TVector<int> vec2(std::move(vec1));

    return TestSystem::check_exp(static_cast<size_t>(0), vec1.size()) &&
           TestSystem::check_exp(static_cast<size_t>(0), vec1.capacity()) &&
           TestSystem::check_exp(static_cast<size_t>(0), vec2.size()) &&
           TestSystem::check_exp(static_cast<size_t>(0), vec2.capacity()) &&
           TestSystem::check_exp(true, vec1.data() == nullptr) &&
           TestSystem::check_exp(true, vec2.data() == nullptr);
}

// Test initializer list with duplicates
bool tvector_initializer_list_duplicates() {
    TVector<int> vec = {5, 5, 5, 5, 5};
    bool all_same = true;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] != 5) {
            all_same = false;
            break;
        }
    }
    return TestSystem::check_exp(true, all_same) &&
           TestSystem::check_exp(static_cast<size_t>(5), vec.size());
}

// Test edge cases for push operations
bool tvector_push_to_full_capacity() {
    TVector<int> vec(15, 1);  // Fill to exact capacity step
    vec.push_back(2);
    return TestSystem::check_exp(static_cast<size_t>(16), vec.size()) &&
           TestSystem::check_exp(static_cast<size_t>(30), vec.capacity()) &&
           TestSystem::check_exp(2, vec.back());
}

// Test multiple insertions at same position
bool tvector_multiple_insertions() {
    TVector<int> vec = {1, 5};
    auto it = vec.begin() + 1;
    vec.insert(it, 2);
    vec.insert(it, 3);
    vec.insert(it, 4);

    TVector<int> expected = {1, 4, 3, 2, 5};
    return TestSystem::check_exp(expected, vec);
}

// Test emplace with complex construction
bool tvector_emplace_construction() {
    TVector<int> vec = {1, 2, 4, 5};
    vec.emplace(vec.begin() + 2, 3);

    TVector<int> expected = {1, 2, 3, 4, 5};
    return TestSystem::check_exp(expected, vec);
}

// Test shrink_to_fit edge cases
bool tvector_shrink_to_fit_empty() {
    TVector<int> vec;
    vec.shrink_to_fit();
    return TestSystem::check_exp(static_cast<size_t>(0), vec.size()) &&
           TestSystem::check_exp(static_cast<size_t>(0), vec.capacity());
}

bool tvector_shrink_to_fit_after_deletions() {
    TVector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vec.erase(vec.begin() + 2);
    vec.erase(vec.begin() + 4);
    vec.shrink_to_fit();

    return TestSystem::check_exp(static_cast<size_t>(8), vec.size()) &&
           TestSystem::check_exp(static_cast<size_t>(8), vec.capacity());
}

// Test resize edge cases
bool tvector_resize_to_zero() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    vec.resize(0);
    return TestSystem::check_exp(static_cast<size_t>(0), vec.size()) &&
           TestSystem::check_exp(true, vec.is_empty());
}

bool tvector_resize_same_size() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    size_t old_capacity = vec.capacity();
    vec.resize(5);
    return TestSystem::check_exp(static_cast<size_t>(5), vec.size()) &&
           TestSystem::check_exp(old_capacity, vec.capacity());
}

// Test operator[] with deleted elements
bool tvector_index_access_with_deletions() {
    TVector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vec.erase(vec.begin() + 2);  // Remove 3
    vec.erase(vec.begin() + 4);  // Remove 6 (now at index 4)

    // After deletions: {1, 2, 4, 5, 7, 8, 9, 10}
    return TestSystem::check_exp(1, vec[0]) &&
           TestSystem::check_exp(2, vec[1]) &&
           TestSystem::check_exp(4, vec[2]) &&
           TestSystem::check_exp(5, vec[3]) &&
           TestSystem::check_exp(7, vec[4]);
}

// Test const operations
bool tvector_const_operations() {
    const TVector<int> vec = {1, 2, 3, 4, 5};
    bool all_tests_pass = true;

    // Test const data()
    if (*vec.data() != 1) all_tests_pass = false;

    // Test const operator[]
    if (vec[2] != 3) all_tests_pass = false;

    // Test const size(), capacity()
    if (vec.size() != 5) all_tests_pass = false;
    if (vec.capacity() != 15) all_tests_pass = false;

    return TestSystem::check_exp(true, all_tests_pass);
}

// Test iterator comparison operators
bool tvector_iterator_comparisons() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    auto it1 = vec.begin();
    auto it2 = vec.begin() + 2;
    auto it3 = vec.end();

    return TestSystem::check_exp(true, it1 < it2) &&
           TestSystem::check_exp(true, it2 > it1) &&
           TestSystem::check_exp(true, it1 <= it2) &&
           TestSystem::check_exp(true, it2 >= it1) &&
           TestSystem::check_exp(true, it2 < it3) &&
           TestSystem::check_exp(false, it1 > it3);
}

// Test iterator indexing operator[]
bool tvector_iterator_index_operator() {
    TVector<int> vec = {10, 20, 30, 40, 50};
    auto it = vec.begin();

    return TestSystem::check_exp(10, it[0]) &&
           TestSystem::check_exp(20, it[1]) &&
           TestSystem::check_exp(30, it[2]) &&
           TestSystem::check_exp(40, it[3]) &&
           TestSystem::check_exp(50, it[4]);
}

bool tvector_iterator_index_operator_exception() {
    TVector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    bool caught_exception = false;

    try {
        int value = it[10];
    } catch (const std::runtime_error&) {
        caught_exception = true;
    }

    return TestSystem::check_exp(true, caught_exception);
}

// Test large vector operations
bool tvector_large_operations() {
    const size_t large_size = 10000;
    TVector<int> vec;

    // Fill with large amount of data
    for (size_t i = 0; i < large_size; ++i) {
        vec.push_back(static_cast<int>(i));
    }

    // Verify size and some elements
    bool size_ok = vec.size() == large_size;
    bool first_ok = vec[0] == 0;
    bool last_ok = vec[large_size - 1] == static_cast<int>(large_size - 1);
    bool middle_ok = vec[large_size / 2] == static_cast<int>(large_size / 2);

    return TestSystem::check_exp(true, size_ok) &&
           TestSystem::check_exp(true, first_ok) &&
           TestSystem::check_exp(true, last_ok) &&
           TestSystem::check_exp(true, middle_ok);
}

// Test exception safety
bool tvector_exception_safety() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    bool all_exceptions_caught = true;

    // Test invalid index access
    try {
        vec[100];
        all_exceptions_caught = false;
    } catch (const std::out_of_range&) {}

    // Test iterator out of range
    try {
        vec.begin() + 100;
        all_exceptions_caught = false;
    } catch (const std::out_of_range&) {}

    // Test dereferencing end()
    try {
        *(vec.end());
        all_exceptions_caught = false;
    } catch (const std::out_of_range&) {}

    return TestSystem::check_exp(true, all_exceptions_caught);
}

// Test capacity management
bool tvector_capacity_management() {
    TVector<int> vec;
    size_t prev_capacity = 0;
    bool capacity_increases = true;

    // Fill vector and check capacity increases
    for (int i = 0; i < 50; ++i) {
        if (vec.capacity() < prev_capacity) {
            capacity_increases = false;
            break;
        }
        prev_capacity = vec.capacity();
        vec.push_back(i);
    }

    return TestSystem::check_exp(true, capacity_increases);
}

// Test equality with different internal states
bool tvector_equality_after_operations() {
    TVector<int> vec1 = {1, 2, 3, 4, 5};
    TVector<int> vec2 = {0, 1, 2, 3, 4, 5, 6};

    // Modify vec2 to have same logical content as vec1
    vec2.pop_front();  // Remove 0
    vec2.pop_back();   // Remove 6

    return TestSystem::check_exp(vec1, vec2);
}

// Test search functions edge cases
bool search_predicate_none_match(int x) { return x > 1000; }
bool search_predicate_all_match(int x) { return x >= 0; }

bool tvector_search_no_matches() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    int result_begin = search_begin(vec, search_predicate_none_match);
    int result_end = search_end(vec, search_predicate_none_match);

    return TestSystem::check_exp(-1, result_begin) &&
           TestSystem::check_exp(-1, result_end);
}

bool tvector_search_all_match() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    int result_begin = search_begin(vec, search_predicate_all_match);
    int result_end = search_end(vec, search_predicate_all_match);

    return TestSystem::check_exp(0, result_begin) &&
           TestSystem::check_exp(4, result_end);
}

// Test sorting with edge cases
bool sort_descending(int a, int b) { return a > b; }
bool sort_scending(int a, int b) { return a < b; }

bool tvector_sort_already_sorted() {
    TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int> expected = vec;
    tv_sort(vec, sort_scending);
    return TestSystem::check_exp(expected, vec);
}

bool tvector_sort_reverse_order() {
    TVector<int> vec = {5, 4, 3, 2, 1};
    tv_sort(vec, sort_scending);
    TVector<int> expected = {1, 2, 3, 4, 5};
    return TestSystem::check_exp(expected, vec);
}

bool tvector_sort_with_deletions() {
    TVector<int> vec = {5, 4, 3, 2, 1, 6, 7, 8};
    vec.erase(vec.begin() + 2);  // Remove 3
    vec.erase(vec.begin() + 4);  // Remove 1

    tv_sort(vec, sort_scending);

    // Should be sorted but may have gaps due to deleted elements
    bool is_sorted = true;
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i-1]) {
            is_sorted = false;
            break;
        }
    }

    return TestSystem::check_exp(true, is_sorted);
}

// Test shuffle preserves elements
bool tvector_shuffle_preserves_elements() {
    TVector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    TVector<int> original = vec;

    shuffle(vec);

    // Sort both vectors to compare content
    tv_sort(vec, sort_scending);
    tv_sort(original, sort_scending);

    return TestSystem::check_exp(original, vec) &&
           TestSystem::check_exp(original.size(), vec.size());
}

// Test performance-related aspects
bool tvector_performance_push_back() {
    TVector<int> vec;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 100000; ++i) {
        vec.push_back(i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Check that it completed and has correct size
    bool completed = vec.size() == 100000;
    std::cout << "Push back 100k elements took " << duration.count()
    << " microseconds" << std::endl;

    return TestSystem::check_exp(true, completed);
}

#pragma endregion

#pragma region ConstIteratorTests

bool tvector_const_iterator_begin() {
    const TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int>::ConstIterator it = vec.begin();

    int actual_result = *it;
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}

bool tvector_const_iterator_end() {
    const TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int>::ConstIterator it = vec.end();
    int actual_result = *(it - 1);
    int expected_result = 5;

    return TestSystem::check_exp(expected_result, actual_result);
}

bool tvector_const_iterator_dereference() {
    const TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int>::ConstIterator it = vec.begin();

    int actual_result = *it;
    int expected_result = 1;

    // Проверяем, что нельзя изменить значение через const_iterator
    bool cannot_modify = true;
    // *it = 10; // Эта строка должна вызывать ошибку компиляции

    return TestSystem::check_exp(expected_result, actual_result) &&
           TestSystem::check_exp(true, cannot_modify);
}

bool tvector_const_iterator_increment() {
    const TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int>::ConstIterator it = vec.begin();

    ++it;
    int actual_result = *it;
    int expected_result = 2;

    return TestSystem::check_exp(expected_result, actual_result);
}

bool tvector_const_iterator_decrement() {
    const TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int>::ConstIterator it = vec.end();

    --it;
    int actual_result = *it;
    int expected_result = 5;

    return TestSystem::check_exp(expected_result, actual_result);
}

bool tvector_const_iterator_comparison() {
    const TVector<int> vec = {1, 2, 3, 4, 5};
    TVector<int>::ConstIterator begin_it = vec.begin();
    TVector<int>::ConstIterator end_it = vec.end();

    bool not_equal = begin_it != end_it;
    bool equal = begin_it == begin_it;

    return TestSystem::check_exp(true, not_equal) &&
           TestSystem::check_exp(true, equal);
}

bool tvector_const_iterator_range_based_for() {
    const TVector<int> vec = {1, 2, 3, 4, 5};
    int sum = 0;

    for (const int& value : vec) {
        sum += value;
    }

    return TestSystem::check_exp(15, sum);
}

bool tvector_const_iterator_after_modification() {
    TVector<int> vec = {1, 2, 3};
    const TVector<int>& const_ref = vec;

    // Сохраняем итераторы до модификации
    auto old_begin = const_ref.begin();
    auto old_end = const_ref.end();

    // Модифицируем вектор
    vec.push_back(4);

    // Проверяем, что итераторы остались валидными
    int sum = 0;
    for (auto it = const_ref.begin(); it != const_ref.end(); ++it) {
        sum += *it;
    }

    return TestSystem::check_exp(10, sum); // 1+2+3+4=10
}

#pragma endregion

int main() {
    TestSystem::print_init_info();
    TestSystem::start_test(tvector_default_init, "default_init");
    TestSystem::start_test(tvector_size_init, "size_init");
    TestSystem::start_test(tvector_copy_init, "copy_init");
    TestSystem::start_test(tvector_move_init, "move_init");
    TestSystem::start_test(tvector_array_init, "array_init");
    TestSystem::start_test(tvector_initialize_list_init,
        "initialize_list_init");

    TestSystem::start_test(tvector_operator_copy_assign,
        "operator_copy_assign");
    TestSystem::start_test(tvector_operator_move_assign,
        "operator_move_assign");
    TestSystem::start_test(tvector_operator_equality, "operator_equality");
    TestSystem::start_test(tvector_operator_inequality, "operator_inequality");
    TestSystem::start_test(tvector_operator_index_access,
        "operator_index_access");
    TestSystem::start_test(tvector_operator_index_access_exception,
        "operator_index_access_exception");
    TestSystem::start_test(tvector_const_operator_index_access,
        "const_operator_index_access");
    TestSystem::start_test(tvector_const_operator_index_access_exception,
        "const_operator_index_access_exception");

    TestSystem::start_test(tvector_data, "get_data");
    TestSystem::start_test(tvector_const_data, "get_const_data");
    TestSystem::start_test(tvector_size, "get_size");
    TestSystem::start_test(tvector_capacity, "get_capacity");
    TestSystem::start_test(tvector_front, "get_front");
    TestSystem::start_test(tvector_back, "get_back");
    TestSystem::start_test(tvector_begin, "begin");
    TestSystem::start_test(tvector_end, "end");

    TestSystem::start_test(tvector_iterator_init, "iterator_init");
    TestSystem::start_test(tvector_iterator_dereference_operator,
        "iterator_dereference_operator");
    TestSystem::start_test(tvector_iterator_dereference_operator_empty,
        "iterator_dereference_operator_empty");
    TestSystem::start_test(tvector_iterator_dereference_operator_out_of_range,
        "iterator_dereference_operator_out_of_range");
    TestSystem::start_test(tvector_iterator_arrow_operator, "iterator_arrow");
    TestSystem::start_test(tvector_iterator_assign, "iterator_assign");
    TestSystem::start_test(tvector_iterator_right_increment,
        "iterator_right_increment");
    TestSystem::start_test(tvector_iterator_left_increment,
        "iterator_left_increment");
    TestSystem::start_test(tvector_iterator_right_decrement,
        "iterator_right_decrement");
    TestSystem::start_test(tvector_iterator_left_decrement,
        "iterator_left_decrement");
    TestSystem::start_test(tvector_iterator_add, "iterator_add");
    TestSystem::start_test(tvector_iterator_add_out_off_range_exception,
        "insert_add_out_off_range_exception");
    TestSystem::start_test(tvector_iterator_sub, "iterator_sub");
    TestSystem::start_test(tvector_iterator_sub_out_off_range_exception,
        "insert_sub_out_off_range_exception");
    TestSystem::start_test(tvector_iterator_add_assign, "iterator_add_assign");
    TestSystem::start_test(tvector_iterator_add_assign_out_off_range_exception,
        "iterator_add_assign_out_off_range_exception");
    TestSystem::start_test(tvector_iterator_sub_assign, "iterator_sub_assign");
    TestSystem::start_test(tvector_iterator_sub_assign_out_off_range_exception,
        "iterator_sub_assign_out_off_range_exception");
    TestSystem::start_test(tvector_iterator_operator_equality,
        "iterator_operator_equality");
    TestSystem::start_test(tvector_iterator_operator_inequality,
        "iterator_operator_inequality");
    TestSystem::start_test(tvector_iterator_operator_iterator_difference,
        "iterator_operator_iterator_difference");

    TestSystem::start_test(tvector_push_back_copy, "push_back_copy");
    TestSystem::start_test(tvector_push_back_move, "push_back_move");
    TestSystem::start_test(tvector_push_front_copy, "push_back_copy");
    TestSystem::start_test(tvector_push_front_move, "push_back_move");
    TestSystem::start_test(tvector_insert_elems_copy, "insert_elems_copy");
    TestSystem::start_test(tvector_insert_copy, "insert_copy");
    TestSystem::start_test(tvector_insert_move, "insert_move");
    TestSystem::start_test(tvector_emplace, "emplace");
    TestSystem::start_test(tvector_pop_back, "pop_back");
    TestSystem::start_test(tvector_pop_front, "pop_front");
    TestSystem::start_test(tvector_erase, "erase");

    TestSystem::start_test(tvector_clear, "clear");
    TestSystem::start_test(tvector_shrink_to_fit, "shrink_to_fit");
    TestSystem::start_test(tvector_resize_1, "resize_1");
    TestSystem::start_test(tvector_resize_2, "resize_2");
    TestSystem::start_test(tvector_resize_3, "resize_3");
    TestSystem::start_test(tvector_shuffle, "shuffle");
    TestSystem::start_test(tvector_search_begin, "search_begin");
    TestSystem::start_test(tvector_search_end, "search_end");
    TestSystem::start_test(tvector_search_all, "search_all");

    TestSystem::start_test(tvector_test, "test");

    // TestSystem::start_test(tvector_sort, "sort");
    TestSystem::start_test(tvector_push_back_copy, "push_back_copy");
    TestSystem::start_test(tvector_pop_back, "pop_back");
    TestSystem::start_test(tvector_erase_empty, "erase_empty");
    // TestSystem::start_test(tvector_pop_back_large_vector,
    // "pop_back_large_vector");
    TestSystem::print_final_info();

    std::cout << "Block of AI generated Tests" << std::endl;
    TestSystem::start_test(tvector_size_value_init_zero,
        "size_value_init_zero");
    TestSystem::start_test(tvector_size_value_init_large,
        "size_value_init_large");
    TestSystem::start_test(tvector_self_assignment,
        "self_assignment");
    TestSystem::start_test(tvector_assignment_different_sizes,
        "assignment_different_sizes");
    TestSystem::start_test(tvector_front_exception, "front_exception");
    TestSystem::start_test(tvector_back_exception, "back_exception");
    TestSystem::start_test(tvector_front_back_after_operations,
        "front_back_after_operations");
    TestSystem::start_test(tvector_is_empty_true, "is_empty_true");
    TestSystem::start_test(tvector_is_empty_false, "is_empty_false");
    TestSystem::start_test(tvector_is_empty_after_clear,
        "is_empty_after_clear");
    TestSystem::start_test(tvector_is_empty_after_all_erased,
        "is_empty_after_all_erased");
    TestSystem::start_test(tvector_iterator_begin_end_empty,
        "iterator_begin_end_empty");
    TestSystem::start_test(tvector_iterator_arithmetic_edge_cases,
     "iterator_arithmetic_edge_cases");
    TestSystem::start_test(tvector_iterator_skip_deleted,
     "iterator_skip_deleted");
    TestSystem::start_test(tvector_memory_management_stress,
     "memory_management_stress");
    TestSystem::start_test(tvector_copy_with_deleted_elements,
     "copy_with_deleted_elements");
    TestSystem::start_test(tvector_move_from_empty, "move_from_empty");
    TestSystem::start_test(tvector_initializer_list_duplicates,
     "initializer_list_duplicates");
    TestSystem::start_test(tvector_push_to_full_capacity,
     "push_to_full_capacity");
    TestSystem::start_test(tvector_multiple_insertions, "multiple_insertions");
    TestSystem::start_test(tvector_emplace_construction,
     "emplace_construction");
    TestSystem::start_test(tvector_shrink_to_fit_empty, "shrink_to_fit_empty");
    TestSystem::start_test(tvector_shrink_to_fit_after_deletions,
     "shrink_to_fit_after_deletions");
    TestSystem::start_test(tvector_resize_to_zero, "resize_to_zero");
    TestSystem::start_test(tvector_resize_same_size, "resize_same_size");
    TestSystem::start_test(tvector_index_access_with_deletions,
     "index_access_with_deletions");
    TestSystem::start_test(tvector_const_operations, "const_operations");
    TestSystem::start_test(tvector_iterator_comparisons,
     "iterator_comparisons");
    TestSystem::start_test(tvector_iterator_index_operator,
     "iterator_index_operator");
    TestSystem::start_test(tvector_iterator_index_operator_exception,
     "iterator_index_operator_exception");
    TestSystem::start_test(tvector_large_operations, "large_operations");
    TestSystem::start_test(tvector_exception_safety, "exception_safety");
    TestSystem::start_test(tvector_capacity_management, "capacity_management");
    TestSystem::start_test(tvector_equality_after_operations,
     "equality_after_operations");
    TestSystem::start_test(tvector_search_no_matches, "search_no_matches");
    TestSystem::start_test(tvector_search_all_match, "search_all_match");
    TestSystem::start_test(tvector_sort_already_sorted, "sort_already_sorted");
    TestSystem::start_test(tvector_sort_reverse_order, "sort_reverse_order");
    TestSystem::start_test(tvector_sort_with_deletions, "sort_with_deletions");
    TestSystem::start_test(tvector_shuffle_preserves_elements,
     "shuffle_preserves_elements");
    TestSystem::start_test(tvector_performance_push_back,
        "performance_push_back");

    TestSystem::start_test(tvector_const_iterator_begin,
     "const_iterator_begin");
    TestSystem::start_test(tvector_const_iterator_end,
     "const_iterator_end");
    TestSystem::start_test(tvector_const_iterator_dereference,
     "tvector_const_iterator_dereference");
    TestSystem::start_test(tvector_const_iterator_increment,
     "tvector_const_iterator_increment");
    TestSystem::start_test(tvector_const_iterator_decrement,
     "tvector_const_iterator_decrement");
    TestSystem::start_test(tvector_const_iterator_comparison,
     "tvector_const_iterator_comparison");
    TestSystem::start_test(tvector_const_iterator_range_based_for,
     "tvector_const_iterator_range_based_for");
    TestSystem::start_test(tvector_const_iterator_after_modification,
     "tvector_const_iterator_after_modification");

    TestSystem::print_final_info();


    return 0;
}
