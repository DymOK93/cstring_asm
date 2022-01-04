#include "c_string.hpp"

#include "test_runner.h"

#include <iostream>
#include <string_view>
#include <vector>

using namespace std;

int main() {
  try {
    TestRunner tr;
    RUN_TEST(tr, TestStrCpy);
    RUN_TEST(tr, TestStrNCpy);
    RUN_TEST(tr, TestStrLen);
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
  ASSERT_EQUAL(buffer[last_pos], '\n');
}

void TestStrLen() {
  constexpr auto* str1{
      "Eat some more of these soft French rolls, and have some tea"};
  ASSERT_EQUAL(StrLen(str1), string_view{str1}.size())
  constexpr char str2[1]{};
  ASSERT_EQUAL(StrLen(str2), string_view{str2}.size())
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