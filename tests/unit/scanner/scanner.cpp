#include <gtest/gtest.h>
extern "C" {
#include "../../../src/scanner/realscanner.h"
}

class ScannerFixture : public ::testing::Test{
    public:
    scanner_t* scanner;
    protected:
    ScannerFixture(){
        scanner = init_scanner();
    }

    ~ScannerFixture() override{
        scanner->free(scanner);
    }
};

TEST_F(ScannerFixture,testing){
    dup2(3,STDOUT_FILENO);
    EXPECT_EQ(2,1)<<"Hello";
}