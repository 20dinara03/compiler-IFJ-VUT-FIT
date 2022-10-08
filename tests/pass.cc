#include <gtest/gtest.h>

extern "C"
{
#include "../src/common/queue.c"
#include "../src/scanner/token.c"
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    // EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}
