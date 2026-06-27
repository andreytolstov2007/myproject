#include <iostream>
#include "02test.h"
#include "datas/DynamicArray.h"

static int tests_passed = 0;
static int tests_failed = 0;

static void Register_test(void (*test_func)(), const std::string &test_name) {
    try {
        test_func();
        tests_passed ++;
        std::cout << "[PASS] " << test_name << std::endl;
    }
    catch (const Errors &err) {
        tests_failed ++;
        std::cout << "[FAIL] " << test_name << " " << err.Message() << std::endl;
    }
}

#define ASSERT_EQ(x, y) \
    if ((x) != (y)) { throw Errors("ASSERT_EQ failed"); }

#define ASSERT_THROWS(expr, exc_type) \
    { \
        bool caught = false; \
        try { expr; } \
        catch (const exc_type&) { caught = true; } \
        catch (const Errors&) {} \
        if (caught == false) { throw Errors("ASSERT_THROWS failed"); } \
    }

// ==================== DynamicArray_tests ====================

static void Test_on_konstructor_without_parametors() {
    DynamicArray<int> m;

    ASSERT_EQ(m.Get_length(), 0);
    ASSERT_EQ(m.Get_allocated_length(), 16);
}

static void Test_on_konstructor_with_length_error() {
    ASSERT_THROWS(DynamicArray<int> m(-5), IndexOutOfRange);
}

static void Test_on_konstructor_with_length() {
    DynamicArray<int> m(100);

    ASSERT_EQ(m.Get_length(), 0);
    ASSERT_EQ(m.Get_allocated_length(), 100);
}

static void Test_on_zero() {
    DynamicArray<int> m(7);
    m.Zero();

    ASSERT_EQ(m.Get_length(), 7);
    ASSERT_EQ(m.Get_allocated_length(), 7);
    for (int i = 0; i < m.Get_length(); i ++) {
        ASSERT_EQ(m.Get(i), 0);
    }
}

static void Test_on_konstructor_with_length_and_array() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);

    ASSERT_EQ(m.Get_length(), 4);
    ASSERT_EQ(m.Get_allocated_length(), 4);
    for (int i = 0; i < m.Get_length(); i ++) {
        ASSERT_EQ(m.Get(i), data[i]);
    }
}

static void Test_on_konstructor_with_length_and_array_with_erreo_1() {
    short data[4] = {7,5,-3,2};
    ASSERT_THROWS(DynamicArray<short> m(data, -5), IndexOutOfRange);
}

static void Test_on_konstructor_with_length_and_array_with_erreo_2() {
    short *data = nullptr;
    ASSERT_THROWS(DynamicArray<short> m(data, 10), NullPointerError);
}

static void Test_on_copy_konstructor() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);
    DynamicArray<short> m2 = m;

    ASSERT_EQ(m2.Get_length(), 4);
    ASSERT_EQ(m2.Get_allocated_length(), 4);
    for (int i = 0; i < m2.Get_length(); i ++) {
        ASSERT_EQ(m2.Get(i), m.Get(i));
    }
}

static void Test_on_operator_equals() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m2(data, 4);
    short data2[5] = {-1,1,3,5,7};
    DynamicArray<short> m(data2, 5);
    m2 = m;

    ASSERT_EQ(m2.Get_length(), 5);
    ASSERT_EQ(m2.Get_allocated_length(), 5);
    for (int i = 0; i < m2.Get_length(); i ++) {
        ASSERT_EQ(m2.Get(i), m.Get(i));
    }
}

static void Test_on_resize_up_length() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);
    m.Resize(7);

    ASSERT_EQ(m.Get_length(), 4);
    ASSERT_EQ(m.Get_allocated_length(), 7);
    for (int i = 0; i < m.Get_length(); i ++) {
        ASSERT_EQ(m.Get(i), data[i]);
    }
}

static void Test_on_resize_down_length() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);
    m.Resize(2);

    ASSERT_EQ(m.Get_length(), 2);
    ASSERT_EQ(m.Get_allocated_length(), 2);
    for (int i = 0; i < m.Get_length(); i ++) {
        ASSERT_EQ(m.Get(i), data[i]);
    }
}

static void Test_on_resize_with_error() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);

    ASSERT_THROWS(m.Resize(-1);, IndexOutOfRange);
}

static void Test_on_get() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);

    ASSERT_EQ(m.Get(0), 7);
    ASSERT_EQ(m.Get(1), 5);
    ASSERT_EQ(m.Get(2), -3);
    ASSERT_EQ(m.Get(3), 2);
}

static void Test_on_get_with_error() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);

    ASSERT_THROWS(m.Get(-1), IndexOutOfRange);
    ASSERT_THROWS(m.Get(4), IndexOutOfRange);
}

static void Test_on_set() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);
    m.Set(0, 16);
    m.Set(2, 25);

    ASSERT_EQ(m.Get(0), 16);
    ASSERT_EQ(m.Get(1), 5);
    ASSERT_EQ(m.Get(2), 25);
    ASSERT_EQ(m.Get(3), 2);
}

static void Test_on_set_with_errors() {
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);

    ASSERT_THROWS(m.Set(-1, 0), IndexOutOfRange);
    ASSERT_THROWS(m.Set(4, 567), IndexOutOfRange);
}

static void Test_on_append() { //-
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);

    ASSERT_THROWS(m.Set(-1, 0), IndexOutOfRange);
    ASSERT_THROWS(m.Set(4, 567), IndexOutOfRange);
}

static void Test_on_prepend() {  //-
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);

    ASSERT_THROWS(m.Set(-1, 0), IndexOutOfRange);
    ASSERT_THROWS(m.Set(4, 567), IndexOutOfRange);
}

static void Test_on_insert_at() { //-
    short data[4] = {7,5,-3,2};
    DynamicArray<short> m(data, 4);

    ASSERT_THROWS(m.Set(-1, 0), IndexOutOfRange);
    ASSERT_THROWS(m.Set(4, 567), IndexOutOfRange);
}

static void Test_on_pop() {
    short data[11] = {7,5,-3,2, 8 ,3 ,6 ,2, -3, 8,2};
    DynamicArray<short> m(data, 11);
    m.Pop(2);

    ASSERT_EQ(m.Get_length(), 10);
    ASSERT_EQ(m.Get_allocated_length(), 11);
    ASSERT_EQ(m.Get(0), 7);
    ASSERT_EQ(m.Get(1), 5);
    for (long i = 2; i < m.Get_length(); i ++) {
        ASSERT_EQ(m.Get(i), data[i + 1]);
    }
}

static void Test_on_last_remove() {
    short data[11] = {7,5,-3,2, 8 ,3 ,6 ,2, -3, 8,2};
    DynamicArray<short> m(data, 11);
    Option<long> x = m.last_remove(8);
    Option<long> y = m.last_remove(7);
    Option<long> z = m.last_remove(2);

    ASSERT_EQ(m.Get_length(), 8);
    ASSERT_EQ(m.Get_allocated_length(), 11);
    ASSERT_EQ(x.Get(), 9);
    ASSERT_EQ(y.Get(), 0);
    ASSERT_EQ(z.Get(), 8);

    ASSERT_EQ(m.Get(0), 5);
    ASSERT_EQ(m.Get(1), -3);
    ASSERT_EQ(m.Get(2), 2);
    ASSERT_EQ(m.Get(3), 8);
    ASSERT_EQ(m.Get(4), 3);
    ASSERT_EQ(m.Get(5), 6);
    ASSERT_EQ(m.Get(6), 2);
    ASSERT_EQ(m.Get(7), -3);
}

static void Test_on_last_remove_empty() {
    short data[11] = {7,5,-3,2, 8 ,3 ,6 ,2, -3, 8,2};
    DynamicArray<short> m(data, 11);
    Option<long> x = m.last_remove(4);

    ASSERT_EQ(x.Is_none(), true);
}

static void Test_on_first_remove() {
    short data[11] = {7,5,-3,2, 8 ,3 ,6 ,2, -3, 8,2};
    DynamicArray<short> m(data, 11);
    Option<long> x = m.first_remove(8);
    Option<long> y = m.first_remove(7);
    Option<long> z = m.first_remove(2);
    Option<long> t = m.first_remove(2);

    ASSERT_EQ(m.Get_length(), 7);
    ASSERT_EQ(m.Get_allocated_length(), 11);
    ASSERT_EQ(x.Get(), 4);
    ASSERT_EQ(y.Get(), 0);
    ASSERT_EQ(z.Get(), 2);
    ASSERT_EQ(t.Get(), 4);

    ASSERT_EQ(m.Get(0), 5);
    ASSERT_EQ(m.Get(1), -3);
    ASSERT_EQ(m.Get(2), 3);
    ASSERT_EQ(m.Get(3), 6);
    ASSERT_EQ(m.Get(4), -3);
    ASSERT_EQ(m.Get(5), 8);
    ASSERT_EQ(m.Get(6), 2);
}

static void Test_on_first_remove_empty() {
    short data[11] = {7,5,-3,2, 8 ,3 ,6 ,2, -3, 8,2};
    DynamicArray<short> m(data, 11);
    Option<long> x = m.first_remove(-78);

    ASSERT_EQ(x.Is_none(), true);
}

// ==================== Run_tests ====================

void Run_tests() {
    Register_test(Test_on_konstructor_without_parametors, "Массив по умолчанию.");
    Register_test(Test_on_konstructor_with_length_error, "Массив заданной длины с неправильным индексом.");
    Register_test(Test_on_konstructor_with_length, "Массив заданной длины с правильным индексом.");
    Register_test(Test_on_zero, "Инициализация массива нулевыми элементами.");
    Register_test(Test_on_konstructor_with_length_and_array, "Конструктор с входным массивом и его длиной.");
    Register_test(Test_on_konstructor_with_length_and_array_with_erreo_1, "Конструктор с входным массивом и его длиной, но с индексом меньшим нуля");
    Register_test(Test_on_konstructor_with_length_and_array_with_erreo_2, "Конструктор с входным массивом и его длиной, но с лшибкой на указатель.");
    Register_test(Test_on_copy_konstructor, "Копирующий конструктор.");
    Register_test(Test_on_operator_equals, "Оператор равно.");
    Register_test(Test_on_resize_up_length, "Увеличение размера массива.");
    Register_test(Test_on_operator_equals, "Уменьшение размера массива.");
    Register_test(Test_on_get, "Значение элемента в массиве по индексу.");
    Register_test(Test_on_get_with_error, "Значение элемента в массиве по индексу, который меньше 0 и больше его размера.");
    Register_test(Test_on_set, "Изменения значения по индексу.");
    Register_test(Test_on_set_with_errors, "Изменения значения по индексу меньшим нуля.");

    Register_test(Test_on_pop, "Удаление элемента по индексу.");
    Register_test(Test_on_last_remove, "Удаление элемента по значению по последнему вхождению.");
    Register_test(Test_on_last_remove_empty, "Удаление элемента, которого нет в массиве, по значению по последнему вхождению.");
    Register_test(Test_on_first_remove, "Удаление элемента по значению по первому вхождению.");
    Register_test(Test_on_first_remove_empty, "Удаление элемента, которого нет в массиве, по значению по первому вхождению.");

    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;

    if (tests_failed == 0) {
        std::cout << "All tests passed!" << std::endl;
    }
}