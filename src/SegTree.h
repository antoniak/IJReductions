#ifndef SEGTREE_H
#define SEGTREE_H

#include "Utils.h"

using namespace utils;

class SegTree{

public:

    explicit SegTree(std::vector<int> e);
    ~SegTree();

    std::vector<int> labels;
    std::vector<int> elements;
    int height;

    std::vector<int> CanonicalPartition(Interval interval);
    int Query(int point);

private:

    void Insert(Interval interval, int node, std::vector<int> &result);
    Interval GetSegment(int node);
};



#endif //SEGTREE_H