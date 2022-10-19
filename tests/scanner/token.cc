#include <gtest/gtest.h>

extern "C"
{
#include "../../src/common/queue.h"
#include "../../src/scanner/token.h"
}

TEST(Token, Init)
{
    token_t *token = init_token();

    EXPECT_TRUE(token->type == WHITE_SPACE)<< "Expected values are not equal";

    free_token(token);

    EXPECT_EQ(token,nullptr) << "free_token was unable to free token with length " << 0;
}

TEST(Token, Insert_Easy)
{
    token_t *token = init_token();

    token->text = nullptr;

    push_char_in_token(token, 'a');
    push_char_in_token(token, 'a');
    push_char_in_token(token, 'a');

    EXPECT_TRUE(strcmp(token->text, "aaa") == 0) << "Expected values are not equal";

    free_token(token);

    EXPECT_EQ(token,nullptr) << "free_token was unable to free token with length " << 3;
}

TEST(Token, Insert_Hard)
{
    token_t *token = init_token();
    const int MAX_SIZE = 10000;

    token->text = nullptr;

    for (int i = 0; i < MAX_SIZE; i++)
    {
        push_char_in_token(token, 'a');
    }

    char *a = new char[MAX_SIZE + 1];
    memset(a, 'a', MAX_SIZE);

    EXPECT_TRUE(strlen(token->text) == MAX_SIZE && strcmp(token->text, a) == 0) << "Expected values are not equal";

    free_token(token);

    EXPECT_EQ(token,nullptr) << "free_token was unable to free token with length " << MAX_SIZE;
}