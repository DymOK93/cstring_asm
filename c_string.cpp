﻿#include "c_string.hpp"

#include "test_runner.h"

#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>

using namespace std;

int main() {
  try {
    TestRunner tr;
    RUN_TEST(tr, TestStrCpy);
    RUN_TEST(tr, TestStrNCpy);
    RUN_TEST(tr, TestStrCat);
    RUN_TEST(tr, TestStrLen);
    RUN_TEST(tr, TestStrCmp);
    RUN_TEST(tr, TestMemCmp);
    RUN_TEST(tr, TestMemSet);
    RUN_TEST(tr, TestMemCpy);

  } catch (...) {
    return 1;
  }
  return 0;
}

void TestStrCpy() {
  constexpr char str1[]{"C++ is your best choice"};
  char buffer[sizeof str1];

  const char* result{StrCpy(buffer, str1)};
  ASSERT_EQUAL(buffer, string_view{str1})
  const char* ref{std::strcpy(buffer, str1)};
  ASSERT_EQUAL(ref, result)

  constexpr char str2[1]{};
  result = StrCpy(buffer, str2);
  ASSERT_EQUAL(buffer, string_view{str2})
  ref = std::strcpy(buffer, str2);
  ASSERT_EQUAL(ref, result)
}

void TestStrNCpy() {
  constexpr char str1[]{"Welcome to the x64 assembler world"};
  constexpr size_t str1_sz{sizeof str1};
  char buffer[str1_sz];

  const char* result{StrNCpy(buffer, str1, str1_sz)};
  ASSERT_EQUAL(buffer, string_view{str1})
  const char* ref{std::strncpy(buffer, str1, str1_sz)};
  ASSERT_EQUAL(result, ref)

  constexpr size_t last_pos{str1_sz - 1};
  buffer[last_pos] = '\n';
  result = StrNCpy(buffer, str1, last_pos);
  ASSERT_EQUAL(string_view(buffer, last_pos), string_view(str1, last_pos))
  ref = std::strncpy(buffer, str1, last_pos);
  ASSERT_EQUAL(result, ref)
  ASSERT_EQUAL(buffer[last_pos], '\n')
}

void TestStrCat() {
  constexpr auto* str1{"At the assembly-code level, "};
  constexpr char str2[]{"two forms of this instruction are allowed"};
  string str{str1};
  str.resize(size(str) + sizeof str2 - 1);
  char* str_data{data(str)};
  const string str_ref{string(str1) + str2};

  const char* result{StrCat(str_data, str2)};
  ASSERT_EQUAL(str, str_ref)
  ASSERT_EQUAL(result, str_data)
}

void TestStrLen() {
  constexpr auto* str1{
      "Eat some more of these soft French rolls, and have some tea"};
  ASSERT_EQUAL(StrLen(str1), string_view{str1}.size())

  constexpr char str2[1]{};
  ASSERT_EQUAL(StrLen(str2), string_view{str2}.size())
}

void TestStrCmp() {
  constexpr auto* str1{"The no-operands form of the instruction"};
  string str2{str1};
  char* rhs{data(str2)};

  int result{StrCmp(str1, rhs)};
  ASSERT_EQUAL(result, 0)

  rhs[2] = 'a';
  result = StrCmp(str1, rhs);
  ASSERT(result > 0)

  rhs[2] = 'z';
  result = StrCmp(str1, rhs);
  ASSERT(result < 0)
}

void TestMemCmp() {
  const string str1{"JSON for Modern C++ version 3.10.5"};
  string str2{str1};
  const size_t str_sz{size(str1)};
  const char* lhs{data(str1)};
  char* rhs{data(str2)};

  int result{MemCmp(lhs, rhs, str_sz)};
  ASSERT_EQUAL(result, 0)

  rhs[3] = 'A';
  result = MemCmp(lhs, rhs, str_sz);
  ASSERT(result > 0)

  rhs[3] = 'Z';
  result = MemCmp(lhs, rhs, str_sz);
  ASSERT(result < 0)
}

void TestMemSet() {
  vector<unsigned char> vec1(115, 0);
  unsigned char* dst{data(vec1)};
  const size_t vec_sz{size(vec1)};

  const vector<unsigned char> vec2(vec_sz, 42);
  const void* result{MemSet(dst, 42, vec_sz)};
  ASSERT_EQUAL(vec1, vec2)
  ASSERT_EQUAL(result, dst)

  result = MemSet(dst, 0, vec_sz);
  ASSERT_EQUAL(accumulate(begin(vec1), end(vec1), 0), 0)
  ASSERT_EQUAL(result, dst)
}

void TestMemCpy() {
  vector vec1{1, 2, 3, 4, 5, 10, 9, 8, 7};
  const size_t vec_sz{size(vec1)};
  vector<int> vec2(vec_sz);
  int* dst{data(vec2)};

  const void* result{MemCpy(dst, data(vec1), vec_sz * sizeof(int))};
  ASSERT_EQUAL(result, dst)
  ASSERT_EQUAL(vec2, vec1)
}