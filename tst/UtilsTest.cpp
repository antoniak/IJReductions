#include "gtest/gtest.h"
#include "Utils.h"

using namespace types;
using namespace utils;

TEST(utils, isDirectory) {

    std::string dir = "../tst/examples";
    EXPECT_EQ(isDirectory(dir.c_str()), true);

    std::string dir_ = ".";
    EXPECT_EQ(isDirectory(dir_.c_str()), true);

    std::string dir__ = "../";
    EXPECT_EQ(isDirectory(dir__.c_str()), true);

    std::string dir___ = "examples";
    EXPECT_EQ(isDirectory(dir___.c_str()), false);

}