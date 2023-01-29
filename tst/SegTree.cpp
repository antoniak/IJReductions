#include "gtest/gtest.h"
#include "SegTree.h"

TEST(SegTree, Query){

    std::vector<int> data;
    data.push_back(10);       
    data.push_back(100);       
    data.push_back(100);     
    data.push_back(1000);    
    data.push_back(10000);    
    data.push_back(10000);   
    data.push_back(100000);  
    data.push_back(1000000); 
    
    SegTree seg_tree = SegTree(data);

    EXPECT_EQ(seg_tree.Query(100000), 12);
    EXPECT_EQ(seg_tree.Query(10), 8);
    EXPECT_EQ(seg_tree.Query(10), 8);
    EXPECT_EQ(seg_tree.Query(0), 8);
    EXPECT_EQ(seg_tree.Query(100000000), 0);

}

TEST(SegTree, CanonicalPartition){
    std::vector<int> data;
    data.push_back(10);       
    data.push_back(100);       
    data.push_back(100);     
    data.push_back(1000);    
    data.push_back(10000);    
    data.push_back(10000);   
    data.push_back(100000);  
    data.push_back(1000000); 
    
    SegTree seg_tree = SegTree(data);

    Interval interval;
    interval.start = 10;
    interval.end = 10;
    EXPECT_EQ(seg_tree.CanonicalPartition(interval).size(), 1);
    EXPECT_EQ(seg_tree.CanonicalPartition(interval)[0], 8);

    interval.start = 10;
    interval.end = 1000000;
    EXPECT_EQ(seg_tree.CanonicalPartition(interval).size(), 2);
    EXPECT_EQ(seg_tree.CanonicalPartition(interval)[0], 2);
    EXPECT_EQ(seg_tree.CanonicalPartition(interval)[1], 6);

    interval.start = 100;
    interval.end = 100000;
    EXPECT_EQ(seg_tree.CanonicalPartition(interval).size(), 3);
    EXPECT_EQ(seg_tree.CanonicalPartition(interval)[0], 9);
    EXPECT_EQ(seg_tree.CanonicalPartition(interval)[1], 5);
    EXPECT_EQ(seg_tree.CanonicalPartition(interval)[2], 12);

    interval.start = 1000;
    interval.end = 10000;
    EXPECT_EQ(seg_tree.CanonicalPartition(interval).size(), 1);
    EXPECT_EQ(seg_tree.CanonicalPartition(interval)[0], 5);

}

TEST(SegTree, Build){

    std::vector<int> data;
    for (int i = 0; i < 1000; i++) { data.push_back(i);}
    
    SegTree seg_tree = SegTree(data);
    EXPECT_EQ(ceil(log2(data.size())), 10);
    EXPECT_EQ(seg_tree.Query(0), pow(2, 10));
    EXPECT_EQ(seg_tree.Query(231), pow(2, 10) + 231);

}

