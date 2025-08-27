#include <iostream>
#include <windows.h>
#include <stdexcept>
#include <algorithm>

#include "TVector.h"
#include <chrono>

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
        }
        else {
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
        }
        else {
            std::cout << "Expected result is " << expected << ", but actual is " << actual << "." << std::endl;
            return false;
        }
    }

    template <class T>
    bool check_unexp(const T& unexpected, const T& actual) {
        if (unexpected != actual) {
            return true;
        }
        else {
            std::cout << "Unexpected result is " << unexpected << ", but actual is " << actual << "." << std::endl;
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
        std::cout << count_success + count_failed << " test" << (count_success + count_failed > 1 ? "s" : "") << " ran." << std::endl;
        set_color(2, 0);
        std::cout << "[  PASSED  ] ";
        set_color(7, 0);
        std::cout << count_success << " test" << (count_success > 1 ? "s" : "") << std::endl;
        if (count_failed > 0) {
            set_color(4, 0);
            std::cout << "[  FAILED  ] ";
            set_color(7, 0);
            std::cout << count_failed << " test" << (count_failed > 1 ? "s." : ".") << std::endl;
        }
    }
};

/*
для добавления нового теста сюда добавить функцию с данными следующего теста
в формате
bool имя_тестовой_функции() {
    1. создать тестовые данные
    2. создать правильный ответ для этих тестовых данных
    3. выполнить тестируемый метод
    4. вернуть результат сравнения ожидаемого и получившегося результатов:
    return TestSystem::check(expected_result, actual_result);
}
*/

#pragma region TvectorTests

bool tvector_default_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec;

    if (vec.data() != nullptr || vec.begin() != vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)0, vec.size()) &&
        TestSystem::check_exp((size_t)0, vec.capacity());
}
bool tvector_size_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec(25);

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)25, vec.size()) &&
        TestSystem::check_exp((size_t)30, vec.capacity());
}
bool tvector_size_and_value_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec(25, 5);

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)25, vec.size()) &&
        TestSystem::check_exp((size_t)30, vec.capacity());
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
        TestSystem::check_exp((size_t)25, vec_1.size()) &&
        TestSystem::check_exp((size_t)30, vec_1.capacity()) &&
        TestSystem::check_exp((size_t)25, vec_2.size()) &&
        TestSystem::check_exp((size_t)30, vec_2.capacity());
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
        TestSystem::check_exp((size_t)0, vec_1.size()) &&
        TestSystem::check_exp((size_t)0, vec_1.capacity()) &&
        TestSystem::check_exp((size_t)25, vec_2.size()) &&
        TestSystem::check_exp((size_t)30, vec_2.capacity());
}
bool tvector_array_init() {
    bool actual_result = true;
    bool expected_result = true;
    int* array = new int[16] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec(array, 16);

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)16, vec.size()) &&
        TestSystem::check_exp((size_t)30, vec.capacity());
}
bool tvector_initialize_list_init() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)16, vec.size()) &&
        TestSystem::check_exp((size_t)30, vec.capacity());
}

bool tvector_operator_copy_assign() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec_2;

    vec_2 = vec_1;

    if (vec_1.data() == nullptr || vec_1.begin() == vec_1.end()) {
        actual_result = false;
    }

    if (vec_2.data() == nullptr || vec_2.begin() == vec_2.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)16, vec_1.size()) &&
        TestSystem::check_exp((size_t)30, vec_1.capacity()) &&
        TestSystem::check_exp((size_t)16, vec_2.size()) &&
        TestSystem::check_exp((size_t)30, vec_2.capacity());
}
bool tvector_operator_move_assign() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec_2;

    vec_2 = std::move(vec_1);

    if (vec_1.data() != nullptr || vec_1.begin() != vec_1.end()) {
        actual_result = false;
    }

    if (vec_2.data() == nullptr || vec_2.begin() == vec_2.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)0, vec_1.size()) &&
        TestSystem::check_exp((size_t)0, vec_1.capacity()) &&
        TestSystem::check_exp((size_t)16, vec_2.size()) &&
        TestSystem::check_exp((size_t)30, vec_2.capacity());
}
bool tvector_operator_equality() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec_2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    actual_result = vec_1 == vec_2;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_operator_inequality() {
    bool actual_result = true;
    bool expected_result = true;
    TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int> vec_2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17 };

    actual_result = vec_1 != vec_2;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_operator_index_access() {
    const TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = vec_1[13];
    int expected_result = 14;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_operator_index_access_exception() {
    bool actual_result = true;
    bool expected_result = false;
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

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
    const TVector<int> vec_1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = vec_1[13];
    int expected_result = 14;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_const_operator_index_access_exception() {
    bool actual_result = true;
    bool expected_result = false;
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

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
    const TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *vec.data();
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_const_data() {
    const TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
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
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int>::Iterator it = vec.begin();

    int actual_result = *it;
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_end() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int>::Iterator it = vec.end();

    int actual_result = *(it-1);
    int expected_result = 16;

    return TestSystem::check_exp(expected_result, actual_result);
}

//bool tvector_push_back_copy_filled() {
//    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//    int a = 16;
//    actual_result.push_back(a);
//    TVector<int> expected_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
//
//    return TestSystem::check_exp(expected_result, actual_result) && 
//        TestSystem::check_exp((size_t)16, actual_result.size()) &&
//        TestSystem::check_exp((size_t)30, actual_result.capacity());
//}
//bool tvector_push_back_copy_empty() {
//    TVector<int> actual_result;
//    int a = 16;
//    actual_result.push_back(a);
//    TVector<int> expected_result = { 16 };
//
//    return TestSystem::check_exp(expected_result, actual_result) &&
//        TestSystem::check_exp((size_t)1, actual_result.size()) &&
//        TestSystem::check_exp((size_t)15, actual_result.capacity());
//}
//bool tvector_push_back_copy_empty() {
//    TVector<int> actual_result;
//    int a = 16;
//    actual_result.push_back(a);
//    TVector<int> expected_result = { 16 };
//
//    return TestSystem::check_exp(expected_result, actual_result) &&
//        TestSystem::check_exp((size_t)1, actual_result.size()) &&
//        TestSystem::check_exp((size_t)15, actual_result.capacity());
//}
//bool tvector_push_back_copy() {
//    return tvector_push_back_copy_filled() && tvector_push_back_copy_empty();
//}
bool tvector_push_back_copy_empty() {
    TVector<int> actual_result;
    int a = 16;
    actual_result.push_back(a);
    TVector<int> expected_result = { 16 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)1, actual_result.size()) &&
        TestSystem::check_exp((size_t)15, actual_result.capacity());
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
    for (int i = 0; i < (int)actual_result.capacity(); ++i) {
        actual_result.push_back(i);
    }
    int a = 100;
    size_t old_capacity = actual_result.capacity();
    size_t old_size = actual_result.size();

    actual_result.push_back(a);

    TVector<int> expected_result;
    for (size_t i = 0; i < old_size; ++i) expected_result.push_back((int)i);
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

    bool size_ok = TestSystem::check_exp((size_t)(large_size + 1), actual_result.size());
    bool last_elem_ok = (actual_result[large_size] == a);
    return size_ok && last_elem_ok;
}
bool tvector_push_back_copy_after_shrink_to_fit() {
    TVector<int> actual_result;
    for (int i = 0; i < (int)actual_result.capacity(); ++i) {
        actual_result.push_back(i);
    }
    actual_result.shrink_to_fit();

    size_t old_capacity = actual_result.capacity();
    size_t old_size = actual_result.size();

    int a = 999;
    actual_result.push_back(a);

    TVector<int> expected_result;
    for (size_t i = 0; i < old_size; ++i) expected_result.push_back((int)i);
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
    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    actual_result.push_back(16);
    TVector<int> expected_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)16, actual_result.size()) &&
        TestSystem::check_exp((size_t)30, actual_result.capacity());
}
bool tvector_push_front_copy() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int a = 1;
    actual_result.push_front(a);
    TVector<int> expected_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)16, actual_result.size()) &&
        TestSystem::check_exp((size_t)30, actual_result.capacity());
}
bool tvector_push_front_move() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    actual_result.push_front(1);
    TVector<int> expected_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)16, actual_result.size()) &&
        TestSystem::check_exp((size_t)30, actual_result.capacity());
}
bool tvector_insert_copy() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int a = 12;
    actual_result.insert(actual_result.begin() + 2, a);
    TVector<int> expected_result = { 2, 3, 12, 4, 5, 6, 7, 8, 9, 10};

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)10, actual_result.size()) &&
        TestSystem::check_exp((size_t)15, actual_result.capacity());
}
bool tvector_insert_elems_copy() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int a = 12;
    actual_result.insert(actual_result.begin() + 2, 7, a);
    TVector<int> expected_result = { 2, 3, 12, 12, 12, 12, 12, 12, 12, 4, 5, 6, 7, 8, 9, 10 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)16, actual_result.size()) &&
        TestSystem::check_exp((size_t)30, actual_result.capacity());
}
bool tvector_insert_move() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    actual_result.insert(actual_result.begin() + 2, 12);
    TVector<int> expected_result = { 2, 3, 12, 4, 5, 6, 7, 8, 9, 10 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)10, actual_result.size()) &&
        TestSystem::check_exp((size_t)15, actual_result.capacity());
}
bool tvector_emplace() {
    TVector<int> actual_result = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    actual_result.emplace(actual_result.begin() + 2, 12);
    TVector<int> expected_result = { 2, 3, 12, 4, 5, 6, 7, 8, 9, 10 };

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)10, actual_result.size()) &&
        TestSystem::check_exp((size_t)15, actual_result.capacity());
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
    for (size_t i = 0; i < 13; ++i) expected_result.push_back((int)i);

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

    return TestSystem::check_exp((size_t)(0), actual_result.size());
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
    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
    TVector<int> expected_result = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

    actual_result.pop_front();
    actual_result.pop_front();
    actual_result.pop_front();
    actual_result.pop_front();
    actual_result.pop_front();
    actual_result.pop_front();

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)25, actual_result.size()) &&
        TestSystem::check_exp((size_t)30, actual_result.capacity());
}
bool tvector_erase() {
    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
    TVector<int> expected_result = { 3, 4, 5, 6, 8, 9, 10, 12, 13, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

    actual_result.erase(actual_result.begin());
    actual_result.erase(actual_result.end() - 1);
    actual_result.erase(actual_result.begin());
    actual_result.erase(actual_result.end() - 1);
    actual_result.erase(actual_result.begin() + 4);
    actual_result.erase(actual_result.begin() + 11);
    actual_result.erase(actual_result.begin() + 7);

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)24, actual_result.size()) &&
        TestSystem::check_exp((size_t)30, actual_result.capacity());
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
    TVector<int> actual_result = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 };
    TVector<int> expected_result = { 2, 3, 4, 5, 7, 8, 9, 11, 222, 12, 13, 14, 15, 16, 18, 20, 21, 333, 22, 23, 24, 25, 26, 27, 28, 30, 31, 111, 32, 33, 34, 35 };
    
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

    //shuffle(actual_result);

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)32, actual_result.size()) &&
        TestSystem::check_exp((size_t)45, actual_result.capacity());
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
            std::cout << "Dereference failed. Expected 1, got " << *it << std::endl;
        }

        *it = 10;

        if (vec[0] != 10) {
            actual_result = false;
            std::cout << "Failed to modify element through dereference." << std::endl;
        }

        ++it;

        if (*it != 2) {
            actual_result = false;
            std::cout << "Dereference after increment failed. Expected 2, got " << *it << std::endl;
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
            std::cout << "operator-> returned wrong pointer." << vec.begin()->size() << std::endl;
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
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    TVector<int>::Iterator it_1 = vec.end();
    it_1 = vec.begin();

    if (*it_1 != 1) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_left_increment() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *(++vec.begin());
    int expected_result = 2;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_right_increment() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *(vec.begin()++);
    int expected_result = 1;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_left_decrement() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int actual_result = *(--vec.end());
    int expected_result = 16;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_iterator_right_decrement() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
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
bool tvector_iterator_add_out_off_range_exeption() {
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
bool tvector_iterator_sub_out_off_range_exeption() {
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
bool tvector_iterator_add_assign_out_off_range_exeption() {
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
bool tvector_iterator_sub_assign_out_off_range_exeption() {
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
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    vec.clear();
    bool actual_result = true;
    bool expected_result = true;

    if (vec.data() == nullptr || vec.begin() != vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)0, vec.size()) &&
        TestSystem::check_exp((size_t)15, vec.capacity());
}
bool tvector_shrink_to_fit() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    vec.shrink_to_fit();
    bool actual_result = true;
    bool expected_result = true;

    if (vec.data() == nullptr || vec.begin() == vec.end()) {
        actual_result = false;
    }

    return TestSystem::check_exp(expected_result, actual_result) &&
        TestSystem::check_exp((size_t)16, vec.size()) &&
        TestSystem::check_exp((size_t)16, vec.capacity());
}
bool tvector_resize_1() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };

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
        TestSystem::check_exp((size_t)32, vec.size()) &&
        TestSystem::check_exp((size_t)45, vec.capacity());
}
bool tvector_resize_2() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };

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
        TestSystem::check_exp((size_t)25, vec.size()) &&
        TestSystem::check_exp((size_t)30, vec.capacity());
}
bool tvector_resize_3() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };

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
        TestSystem::check_exp((size_t)5, vec.size()) &&
        TestSystem::check_exp((size_t)15, vec.capacity());
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
        TestSystem::check_exp((size_t)6, vec.size()) &&
        TestSystem::check_exp((size_t)15, vec.capacity());
}

bool find_chet(int a) {
    return a %2 == 0;
}
bool tvector_serach_begin() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    vec.pop_front();
    int actual_result = search_begin(vec, find_chet);
    int expected_result = 0;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_serach_end() {
    TVector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    vec.pop_front();
    vec.pop_back();
    int actual_result = search_end(vec, find_chet);
    int expected_result = 6;

    return TestSystem::check_exp(expected_result, actual_result);
}
bool tvector_serach_all() {
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

bool sort_check(unsigned long long a, unsigned long long b) {
    return a < b;
}

bool tvector_sort() {
    TVector<unsigned long long> expected_result(100000000);
    std::cout << "vectors memory" << std::endl;
    TVector<unsigned long long> actual_result(100000000);
    unsigned long long a = 10;

    for (unsigned long long i = 0; i < 100000000; i++) {
        expected_result[i] = i;

        if (i == a) {
            std::cout << i << std::endl;
            a *= 10;
        }
    }

    a = 10;

    for (unsigned long long i = 0; i < 100000000; i++) {
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
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Sort waste " << duration.count() << " milliseconds." << std::endl;

    return TestSystem::check_exp(expected_result, actual_result);
}
#pragma endregion

int main() {
    TestSystem::print_init_info();
    TestSystem::start_test(tvector_default_init, "default_init");
    TestSystem::start_test(tvector_size_init, "size_init");
    TestSystem::start_test(tvector_copy_init, "copy_init");
    TestSystem::start_test(tvector_move_init, "move_init");
    TestSystem::start_test(tvector_array_init, "array_init");
    TestSystem::start_test(tvector_initialize_list_init, "initialize_list_init");

    TestSystem::start_test(tvector_operator_copy_assign, "operator_copy_assign");
    TestSystem::start_test(tvector_operator_move_assign, "operator_move_assign");
    TestSystem::start_test(tvector_operator_equality, "operator_equality");
    TestSystem::start_test(tvector_operator_inequality, "operator_inequality");
    TestSystem::start_test(tvector_operator_index_access, "operator_index_access");
    TestSystem::start_test(tvector_operator_index_access_exception, "operator_index_access_exception");
    TestSystem::start_test(tvector_const_operator_index_access, "const_operator_index_access");
    TestSystem::start_test(tvector_const_operator_index_access_exception, "const_operator_index_access_exception");

    TestSystem::start_test(tvector_data, "get_data");
    TestSystem::start_test(tvector_const_data, "get_const_data");
    TestSystem::start_test(tvector_size, "get_size");
    TestSystem::start_test(tvector_capacity, "get_capacity");
    TestSystem::start_test(tvector_front, "get_front");
    TestSystem::start_test(tvector_back, "get_back");
    TestSystem::start_test(tvector_begin, "begin");
    TestSystem::start_test(tvector_end, "end");

    TestSystem::start_test(tvector_iterator_init, "iterator_init");
    TestSystem::start_test(tvector_iterator_dereference_operator, "iterator_dereference_operator");
    TestSystem::start_test(tvector_iterator_dereference_operator_empty, "iterator_dereference_operator_empty");
    TestSystem::start_test(tvector_iterator_dereference_operator_out_of_range, "iterator_dereference_operator_out_of_range");
    TestSystem::start_test(tvector_iterator_arrow_operator, "iterator_arrow");
    TestSystem::start_test(tvector_iterator_assign, "iterator_assign");
    TestSystem::start_test(tvector_iterator_right_increment, "iterator_right_increment");
    TestSystem::start_test(tvector_iterator_left_increment, "iterator_left_increment");
    TestSystem::start_test(tvector_iterator_right_decrement, "iterator_right_decrement");
    TestSystem::start_test(tvector_iterator_left_decrement, "iterator_left_decrement");
    TestSystem::start_test(tvector_iterator_add, "iterator_add");
    TestSystem::start_test(tvector_iterator_add_out_off_range_exeption, "insert_add_out_off_range_exeption");
    TestSystem::start_test(tvector_iterator_sub, "iterator_sub");
    TestSystem::start_test(tvector_iterator_sub_out_off_range_exeption, "insert_sub_out_off_range_exeption");
    TestSystem::start_test(tvector_iterator_add_assign, "iterator_add_assign");
    TestSystem::start_test(tvector_iterator_add_assign_out_off_range_exeption, "iterator_add_asiign_out_off_range_exeption");
    TestSystem::start_test(tvector_iterator_sub_assign, "iterator_sub_assign");
    TestSystem::start_test(tvector_iterator_sub_assign_out_off_range_exeption, "iterator_sub_asiign_out_off_range_exeption");
    TestSystem::start_test(tvector_iterator_operator_equality, "iterator_operator_equality");
    TestSystem::start_test(tvector_iterator_operator_inequality, "iterator_operator_inequality");
    TestSystem::start_test(tvector_iterator_operator_iterator_difference, "iterator_operator_iterator_difference");

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
    TestSystem::start_test(tvector_serach_begin, "serach_begin");
    TestSystem::start_test(tvector_serach_end, "serach_end");
    TestSystem::start_test(tvector_serach_all, "serach_all");

    TestSystem::start_test(tvector_test, "test");

    //TestSystem::start_test(tvector_sort, "sort");
    TestSystem::start_test(tvector_push_back_copy, "push_back_copy");
    TestSystem::start_test(tvector_pop_back, "pop_back");
    TestSystem::start_test(tvector_erase_empty, "erase_empty");
    //TestSystem::start_test(tvector_pop_back_large_vector, "pop_back_large_vector");
    TestSystem::print_final_info();

    return 0;
}
