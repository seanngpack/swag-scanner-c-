#include <gtest/gtest.h>
#include "FileHandler.h"

using namespace testing;


class FileHandlerFixture : public ::testing::Test {


protected:
    file::FileHandler *handler;

    virtual void SetUp() {
        handler = new file::FileHandler;
    }

    virtual void TearDown() {
        delete handler;
    }
};


/**
 * Test the get current scan folder method with an invalid path name.
 * Expecting invalid argument errors.
 */
TEST_F(FileHandlerFixture, TestGetCurrentScanFolderInvalidPath) {
    ASSERT_THROW(file::FileHandler *test = new file::FileHandler("swagggg"),
            std::invalid_argument);
}

/**
 * Test to see if the method sets the correct current scan folder.
 * Again, if there are folders 1->10 in the valid directory, then the current
 * scan folder should be 11.
 * NOTE: This test is flawed because when I change the default folder this test
 * will break. Figure out a solution later to address this.
 */
TEST_F(FileHandlerFixture, TestGetCurrentScanFolder) {
    EXPECT_EQ(handler->get_current_scan_folder(),
              "/Users/seanngpack/Programming Stuff/Projects/scanner_files/17");
}

/**
 * Test if the file handler is smart enough to make the current scan directory "1" if
 * the directory entereed is empty.
 */
TEST_F(FileHandlerFixture, TestGetcurrentScanFolderEmptyCase) {
    file::FileHandler *test = new file::FileHandler(
            "/Users/seanngpack/Programming Stuff/Projects/scanner_files/testing/empty");
    EXPECT_EQ(test->get_current_scan_folder(),
              "/Users/seanngpack/Programming Stuff/Projects/scanner_files/testing/empty/1");
}