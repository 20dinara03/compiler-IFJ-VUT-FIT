#include <gtest/gtest.h>
extern "C" {
#include "../../../src/parser/parser.h"
#include "../../../src/common/program.h"
}

const char* UT_VARIABLE_IDENTIFIER = "XXX_UT_VARIABLE_IDENTIFIER_XXX";
const char* UT_FUNCTION_IDENTIFIER = "XXX_UT_FUNCTION_IDENTIFIER_XXX";

struct generator_t {
    token_t** tokens;
    int size;
    int index;
};

generator_t* generator{};
program_t test;

token_t* mockNextToken() {
    if (generator->index >= generator->size)
        return nullptr;
    test.scanner->current_token = generator->tokens[generator->index++];
    return test.scanner->current_token;
}

void mockFreeToken(scanner_t* token) {};

class ParsingTestRaw : public ::testing::Test {
protected:

    void SetUp() override {
        test.scanner = init_scanner();
        test.scanner->free_token = mockFreeToken;
        generator = (generator_t*) malloc(sizeof(generator_t));
        generator->size = generator->index = 0;
        test.parser = init_parser(test.scanner);
        test.scanner->get_next_token = mockNextToken;
    }

    static void freeGenerator() {
        for (int i = 0; i < generator->size; i++) {
            free(generator->tokens[i]);
        }
        free(generator->tokens);
        free(generator);
    }

    void TearDown() override {
        printf("TearDown");
        test.parser->free(test.parser);
        freeGenerator();
    }

    static size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
    {
        size_t pos = txt.find( ch );
        size_t initialPos = 0;
        strs.clear();

        // Decompose statement
        while( pos != std::string::npos ) {
            strs.push_back( txt.substr( initialPos, pos - initialPos ) );
            initialPos = pos + 1;

            pos = txt.find( ch, initialPos );
        }

        // Add the last one
        strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

        return strs.size();
    }

    static void patchScanner(const char* input) {
        std::vector<std::string> v;
        split(input, v, ' ');

        generator->size = (int) v.size();
        if (generator->size == 0)
            return;

        if (generator->tokens == NULL)
            generator->tokens = (token_t**) malloc(sizeof(token_t*) * generator->size);
        else
            generator->tokens = (token_t**) realloc(generator->tokens, sizeof(token_t*) * generator->size);

        for (int i = 0; i < generator->size; i++) {
            char* token = (char*) v[i].c_str();
            generator->tokens[i] = init_token();
            if (strcmp(token, UT_VARIABLE_IDENTIFIER) == 0) {
                types_t type = VARIABLE_IDENTIFIER;
                generator->tokens[i]->type = type;
                generator->tokens[i]->text = (char*)"big_boobs";
            } else if (strcmp(token, UT_FUNCTION_IDENTIFIER) == 0) {
                types_t type = IDENTIFIER;
                generator->tokens[i]->type = type;
                generator->tokens[i]->text = (char*)"small_boobs"; // common they are boobs too
            } else {
                generator->tokens[i]->type = KEYWORD; // KEYWORD is never used
                generator->tokens[i]->text = token;
            }
        }
        mockNextToken();
    }
};

TEST_F(ParsingTestRaw, test_standart_php_header) {
    patchScanner("<?php declare ( strict_types = 1 ) ;");
    assert(parseProg(::test.parser));
}

TEST_F(ParsingTestRaw, false_test_standart_php_header) {
    patchScanner("<?php declare ( strict_types = 1 ) "); // no `;` at the end
    assert(!parseProg(::test.parser));
}
