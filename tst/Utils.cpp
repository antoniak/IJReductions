#include "gtest/gtest.h"
#include "Utils.h"

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


TEST(utils, isContainedTo) {

    Interval interval_1, interval_2;
    interval_1.start = 10;
    interval_1.end = 1000000;
    interval_2.start = 20;
    interval_2.end = 30;

    EXPECT_EQ(isContainedTo(&interval_2, &interval_1), true);
    EXPECT_EQ(isContainedTo(&interval_1, &interval_2), false);

    interval_2.start = 1;
    interval_2.end = 30;

    EXPECT_EQ(isContainedTo(&interval_2, &interval_1), false);

    interval_2.start = 20;
    interval_2.end = 2000000;

    EXPECT_EQ(isContainedTo(&interval_2, &interval_1), false);

    interval_2.start = 10;
    interval_2.end = 2000000;

    EXPECT_EQ(isContainedTo(&interval_2, &interval_1), false);

}


TEST(utils, intersectsWith) {

    Interval interval_1, interval_2;
    interval_1.start = 10;
    interval_1.end = 1000000;
    interval_2.start = 20;
    interval_2.end = 30;

    EXPECT_EQ(intersectsWith(&interval_2, &interval_1), true);
    EXPECT_EQ(intersectsWith(&interval_1, &interval_2), true);

    interval_2.start = 1;
    interval_2.end = 30;

    EXPECT_EQ(intersectsWith(&interval_2, &interval_1), true);

    interval_2.start = 20;
    interval_2.end = 2000000;

    EXPECT_EQ(intersectsWith(&interval_2, &interval_1), true);

    interval_2.start = 10;
    interval_2.end = 2000000;

    EXPECT_EQ(intersectsWith(&interval_2, &interval_1), true);

    interval_2.start = 2000000;
    interval_2.end = 2000010;

    EXPECT_EQ(intersectsWith(&interval_2, &interval_1), false);

}