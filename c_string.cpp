#include "c_string.hpp"

#include "test_runner.hpp"

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
    RUN_TEST(tr, TestStrNCat);
    RUN_TEST(tr, TestStrLen);
    RUN_TEST(tr, TestStrCmp);
    RUN_TEST(tr, TestStrNCmp);
    RUN_TEST(tr, TestStrChr);
    RUN_TEST(tr, TestStrRChr);
    RUN_TEST(tr, TestStrSpn);
    RUN_TEST(tr, TestStrCSpn);
    RUN_TEST(tr, TestStrPbrk);
    RUN_TEST(tr, TestStrStr);
    RUN_TEST(tr, TestStrTok);
    RUN_TEST(tr, TestMemChr);
    RUN_TEST(tr, TestMemCmp);
    RUN_TEST(tr, TestMemSet);
    RUN_TEST(tr, TestMemCpy);
    RUN_TEST(tr, TestMemMove);

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

void TestStrNCat() {
  constexpr char str1[]{"The behavior is undefined "};
  constexpr size_t str1_sz{sizeof str1};
  constexpr char str2[]{"if the destination array does not have enough space"};
  constexpr size_t str2_sz{sizeof str2};

  string str{str1};
  str.resize(size(str) + str2_sz - 1);
  char* str_data{data(str)};
  string str_ref{string(str1) + str2};

  const char* result{
      StrNCat(str_data, str2, str2_sz)};  // str2_sz includes '\0'
  ASSERT_EQUAL(str, str_ref)
  ASSERT_EQUAL(result, str_data)

  str.back() = '-';
  str[str1_sz - 1] = '\0';
  str_ref.back() = '-';
  result = StrNCat(str_data, str2, str2_sz - 2);
  ASSERT_EQUAL(str, str_ref)
  ASSERT_EQUAL(result, str_data)

  str.assign(size(str), '\0');
  str_ref.assign(size(str), '\0');
  result = StrNCat(str_data, str2, 0);
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

void TestStrNCmp() {
  constexpr char str1[]{"pointer to the null-terminated string"};
  string str2{"pointer to the string"};
  const size_t matches{15};
  char* rhs{data(str2)};

  int result{StrNCmp(str1, rhs, sizeof str1)};
  ASSERT(result < 0)

  result = StrNCmp(str1, rhs, matches);
  ASSERT_EQUAL(result, 0)

  rhs[1] = 'a';
  result = StrNCmp(str1, rhs, matches);
  ASSERT(result > 0)

  rhs[1] = 'z';
  result = StrNCmp(str1, rhs, matches);
  ASSERT(result < 0)
}

void TestStrChr() {
  const char str1[]{"HIJKLMN"};
  const char str2[1]{};

  const void* pos{StrChr(str1, 'M')};
  ASSERT_EQUAL(pos, static_cast<const void*>(str1 + 5))

  pos = StrChr(str1, 'Z');
  ASSERT_EQUAL(pos, nullptr)

  pos = StrChr(str2, 'A');
  ASSERT_EQUAL(pos, nullptr)

  pos = StrChr(str2, '\0');
  ASSERT_EQUAL(pos, str2)
}

void TestStrRChr() {
  const char str1[]{"OPQRSOT"};
  const char str2[1]{};

  const void* pos{StrRChr(str1, 'O')};
  ASSERT_EQUAL(pos, static_cast<const void*>(str1 + 5))

  pos = StrRChr(str1, 'Z');
  ASSERT_EQUAL(pos, nullptr)

  pos = StrRChr(str1, '\0');
  ASSERT_EQUAL(pos, static_cast<const void*>(str1 + 7))

  pos = StrRChr(str2, '\0');
  ASSERT_EQUAL(pos, str2)
}

void TestStrSpn() {
  constexpr auto* str1{"abcde312$#@"};
  constexpr char str2[1]{};
  constexpr auto* low_alpha{"qwertyuiopasdfghjklzxcvbnm"};
  constexpr auto* digits{"1234567890"};

  size_t span_sz{StrSpn(str1, low_alpha)};
  ASSERT_EQUAL(span_sz, 5)

  span_sz = StrSpn(str1, digits);
  ASSERT_EQUAL(span_sz, 0)

  span_sz = StrSpn(str2, digits);
  ASSERT_EQUAL(span_sz, 0)
}

void TestStrCSpn() {
  constexpr auto* str1{"abcde312$#@"};
  constexpr char str2[1]{};
  constexpr auto* invalid{"*$#"};
  constexpr auto* digits{"1234567890"};

  size_t span_sz{StrCSpn(str1, invalid)};
  ASSERT_EQUAL(span_sz, 8)

  span_sz = StrCSpn(str1, digits);
  ASSERT_EQUAL(span_sz, 5)

  span_sz = StrCSpn(str2, digits);
  ASSERT_EQUAL(span_sz, 0)
}

void TestStrPbrk() {
  constexpr auto* str1{"Hello world, friend of mine!"};
  constexpr char str2[1]{};
  constexpr auto* sep{" ,!"};

  constexpr size_t matches[]{5, 11, 12, 19, 22, 27};
  constexpr size_t matches_cnt{sizeof matches / sizeof(size_t)};

  size_t matches_idx{0};
  for (const char* current = str1;; ++current) {
    current = StrPbrk(current, sep);
    if (!current) {
      ASSERT_EQUAL(matches_idx, matches_cnt)
      break;
    }
    ASSERT_EQUAL(static_cast<size_t>(current - str1), matches[matches_idx])
    ++matches_idx;
  }

  const char* result{StrPbrk(str2, sep)};
  ASSERT_EQUAL(result, nullptr)
}

void TestStrStr() {
  constexpr auto* str1{"Pops a doubleword (POPFD) from the top of the stack"};
  constexpr char str2[1]{};

  const char* result{StrStr(str1, "POPFD")};
  ASSERT_EQUAL(static_cast<const void*>(result),
               static_cast<const void*>(str1 + 19))

  result = StrStr(str1, "PUSHFD");
  ASSERT_EQUAL(static_cast<const void*>(result), nullptr)

  result = StrStr(str2, "");
  ASSERT_EQUAL(static_cast<const void*>(result), static_cast<const void*>(str2))
}

void TestStrTok() {
  char str1[]{"    A little, little bird came down the walk"};
  char str2[]{"SingleToken"};
  char str3[]{"   "};
  constexpr auto* sep{" ,"};
  const char* tokens[]{"A",    "little", "little", "bird",
                       "came", "down",   "the",    "walk"};

  size_t token_idx{0};
  const char* token{StrTok(str1, sep)};
  while (token) {
    ASSERT_EQUAL(string_view{token}, tokens[token_idx++])
    token = StrTok(nullptr, sep);
  }

  const char* single_token{StrTok(str2, sep)};
  ASSERT_EQUAL(single_token, string_view{"SingleToken"})

  const char* no_tokens{StrTok(str3, sep)};
  ASSERT_EQUAL(static_cast<const void*>(no_tokens), nullptr)
}

void TestMemChr() {
  const char str[]{"ABCDEFG"};
  constexpr size_t str_sz{sizeof str};

  const void* pos{MemChr(str, 'D', str_sz)};
  ASSERT_EQUAL(pos, static_cast<const void*>(str + 3))

  pos = MemChr(str, 'Z', str_sz);
  ASSERT_EQUAL(pos, nullptr)

  pos = MemChr(str, 'A', 0);
  ASSERT_EQUAL(pos, nullptr)
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

void TestMemMove() {
  vector vec1{7, 42, 1, 2, 3, 4, 5, 10, 9};
  const size_t vec_sz{size(vec1)};
  vector<int> vec2(vec_sz);
  int* dst{data(vec2)};

  const void* result{MemMove(dst, data(vec1), vec_sz * sizeof(int))};
  ASSERT_EQUAL(result, dst)
  ASSERT_EQUAL(vec2, vec1)

  rotate(begin(vec1), begin(vec1) + 2, end(vec1));
  // 1, 2, 3, 4, 5, 10, 9, 42, 7

  // dst + count < src
  result = MemMove(dst, dst + 2, (vec_sz - 2) * sizeof(int));
  // 1, 2, 3, 4, 5, 10, 9, 10, 9
  ASSERT_EQUAL(result, dst)
  ASSERT(equal(begin(vec2), end(vec2) - 2, begin(vec1)))

   // src + count < dst
  result = MemMove(dst + 2, dst, (vec_sz - 2) * sizeof(int));
  // 1, 2, 1, 2, 3, 4, 5, 10, 9
  ASSERT_EQUAL(result, dst + 2)
  ASSERT(equal(begin(vec2) + 2, end(vec2), begin(vec1)))
}