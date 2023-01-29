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
    int max_node;
    Interval root_segment;

    std::vector<int> CanonicalPartition(Interval interval);
    int Query(int point);

private:

    void Insert(Interval interval, int node, int x, int y, std::vector<int> &result);
    Interval GetSegment(int node);
};

#endif //SEGTREE_H