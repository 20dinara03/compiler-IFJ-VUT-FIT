#include <gtest/gtest.h>
#include <iostream>
extern "C"
{
#include "../../../src/scanner/realscanner.h"
}

class ScannerFixture : public ::testing::Test
{
public:
    scanner_t *scanner;

protected:
    ScannerFixture()
    {
        scanner = init_scanner();
    }

    ~ScannerFixture() override
    {
        scanner->free(scanner);
    }
};