#include "cut/cut.hpp"
#include "cppregex/Rule.h"

USING_CUM_NS

FIXTURE(RuleSpec) {
  TEST("empty string") {
    ASSERT_TRUE(match(val(""), ""));
    ASSERT_FALSE(match(val(""), "a"));
  }

  TEST("one character") {
    ASSERT_TRUE(match(val('c'), "c"));
    ASSERT_FALSE(match(val('c'), "b"));
    ASSERT_FALSE(match(val('c'), ""));
  }

  TEST("one character: escaped") {
    ASSERT_TRUE(match(val('\n'), "\n"));
    ASSERT_FALSE(match(val('\n'), "\t"));
    ASSERT_FALSE(match(val('\n'), ""));
  }

  TEST("any character") {
    ASSERT_TRUE(match(any(), "a"));
  }

  TEST("simple string") {
    ASSERT_TRUE(match(val("abc"), "abc"));
    ASSERT_FALSE(match(val("abc"), ""));
    ASSERT_FALSE(match(val("abc"), "bcd"));
  }

  TEST("sequence: char + char") {
    ASSERT_TRUE(match(sequence(val('a'), val('b')), "ab"));
    ASSERT_FALSE(match(sequence(val('b'), val('a')), "ab"));
  }

  TEST("sequence: string + string") {
    ASSERT_TRUE(match(sequence(val("abc"), val("bcd")), "abcbcd"));
    ASSERT_FALSE(match(sequence(val("bcd"), val("abc")), "abcbcd"));
  }

  TEST("sequence: string + char") {
    ASSERT_TRUE(match(sequence(val("abc"), val("d")), "abcd"));
    ASSERT_TRUE(match(sequence(val('d'), val("abc")), "dabc"));
  }

  TEST("alternative: char | char") {
    auto rule = alternative(val('c'), val('d'));

    ASSERT_TRUE(match(rule, "c"));
    ASSERT_TRUE(match(rule, "d"));
  }

  TEST("alternative: string | string") {
    auto rule = alternative(val("abc"), val("bcd"));

    ASSERT_TRUE(match(rule, "abc"));
    ASSERT_TRUE(match(rule, "bcd"));
  }

  TEST("alternative: string | char") {
    auto rule = alternative(val("abc"), val('\n'));

    ASSERT_TRUE(match(rule, "abc"));
    ASSERT_TRUE(match(rule, "\n"));
  }

  TEST("zero or more(greedy, non-backoff): a*b") {
    auto rule = sequence(zero_or_more(val('a')), val('b'));

    ASSERT_TRUE(match(rule, "b"));
    ASSERT_TRUE(match(rule, "ab"));
    ASSERT_TRUE(match(rule, "aab"));
    ASSERT_FALSE(match(rule, "c"));
  }

  TEST("zero or more(greedy, non-backoff): a*b") {
    auto rule = sequence(zero_or_more(val('a')), val('b'));

    ASSERT_TRUE(match(rule, "b"));
    ASSERT_TRUE(match(rule, "ab"));
    ASSERT_TRUE(match(rule, "aab"));
    ASSERT_FALSE(match(rule, "c"));
    ASSERT_FALSE(match(rule, "ac"));
  }

  TEST("one or more(greedy, non-backoff): b+cde") {
    auto rule = sequence(one_or_more(val('b')), val("cde"));

    ASSERT_FALSE(match(rule, "cde"));
    ASSERT_TRUE(match(rule, "bcde"));
    ASSERT_TRUE(match(rule, "bbcde"));
    ASSERT_TRUE(match(rule, "bbcdef"));
  }

  TEST("one or more(greedy, backoff): (b|d)+bbc") {
    auto rule = sequence(
        one_or_more(alternative(val('b'), val('d'))),
        val("bbc"));

    ASSERT_TRUE(match(rule, "bbbc"));
    ASSERT_TRUE(match(rule, "dbbc"));
  }

  TEST("oneof('abc'): [bd]+bbc") {
    auto rule = sequence(
        one_or_more(oneof("bd")),
        val("bbc"));

    ASSERT_TRUE(match(rule, "bbbc"));
    ASSERT_TRUE(match(rule, "dbbc"));
  }

  TEST("one or more(greedy, non-backoff): b+cde$") {
    auto rule = eof(sequence(one_or_more(val('b')), val("cde")));

    ASSERT_FALSE(match(rule, "cde"));
    ASSERT_TRUE(match(rule, "bcde"));
    ASSERT_TRUE(match(rule, "bbcde"));
    ASSERT_FALSE(match(rule, "bbcdef"));
  }

  TEST("optinal(greedy, non-backoff): b?cde") {
    auto rule = sequence(optional(val('b')), val("cde"));

    ASSERT_TRUE(match(rule, "cde"));
    ASSERT_TRUE(match(rule, "bcde"));
    ASSERT_TRUE(match(rule, "bcdef"));
  }
};
