#include <gtest/gtest.h>

extern "C"
{
#include "../../src/common/queue.c"
#include "../../src/scanner/token.c"
}

TEST(Token, Init)
{
    token_t *token = init_token();

    EXPECT_TRUE(token->type == WHITE_SPACE);
}

TEST(Token, Insert_1)
{
    token_t *token = init_token();

    push_char_in_token(token,'a');
    push_char_in_token(token,'a');
    push_char_in_token(token,'a');

    EXPECT_TRUE(true);
}
