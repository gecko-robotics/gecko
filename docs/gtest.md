# gtest

- test suite: group of related tests
- test case: a program with a particular input and verified output results

## Asserts

`ASSERT_*` will exit on failure, while `EXPECT_*` will continue to reveal
more errors.

- ASSERT_TRUE: true
- ASSERT_FALSE: false
- ASSERT_EQ: a == b
- ASSERT_NE: a != b
- ASSERT_LT: a < b
- ASSERT_LE: a <= b
- ASSERT_GT: a > b
- ASSERT_GE: a >= b
- ASSERT_THROW(statement, exception_type)
- ASSERT_ANY_THROW(statement)
- ASSERT_NO_THROW(statement)

## Basics

```c
TEST(TestSuite, TestCase) {
    ...
}
```
