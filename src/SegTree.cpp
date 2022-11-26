#include <cmath>
#include "SegTree.h"

SegTree::SegTree(std::vector<int> e): elements(e){

    std::sort(elements.begin(), elements.end());

    std::vector<int>::iterator it;
    it = std::unique(elements.begin(), elements.end());
    elements.resize(std::distance(elements.begin(), it));
    
    height = ceil(log2(elements.size()));
    labels.resize(elements.size());
    for (int i = 1; i <= elements.size(); i++) { labels[i - 1] =  pow(2, height) - 1 + i; }

}

SegTree::~SegTree() {}

/**
 * Segment tree Query function.
 *
 * @param point A point.
 * @return The label of the leaf that corresponds to 'point'.
 */
int SegTree::Query(int point) {

    std::vector<int>::iterator low = std::lower_bound(elements.begin(), elements.end(), point);
    int index = low - elements.begin();
    return labels[index];

}

Interval SegTree::GetSegment(int node) {
    Interval segment;
    segment.start = node;
    segment.end = node;
    for (int i = 0; i < height - floor(log2(node)); i++) {
        segment.start = segment.start * 2;
        segment.end = segment.end * 2 + 1;
    }
    return segment;
}

/**
 * Segment tree Insert function. It takes as input an interval, a node, and a vector. 
 * It appends to the vector the labels of the nodes in the sub-tree rooted by the node 
 * that cover maximally the interval. 
 *
 * @param interval An interval.
 * @param node A node.
 * @param result A vector.
 */
void SegTree::Insert(Interval interval, int node, std::vector<int> &result) {

    if(node >= pow(2, height + 1)) { return; } 

    Interval segment = GetSegment(node);

    if (utils::isContainedTo(&segment, &interval)) { 
        result.push_back(node);
    } else { 
        Interval lc_segment = GetSegment(node * 2);
        Interval rc_segment = GetSegment(node * 2 + 1);

        if (intersectsWith(&lc_segment, &interval)) {
            Insert(interval, node * 2, result);
        }
        if (intersectsWith(&rc_segment, &interval)) {
            Insert(interval, node * 2 + 1, result);
        }        
    }
    return;
}

std::vector<int> SegTree::CanonicalPartition(Interval interval) {

    std::vector<int> canonical_partition;
    Interval inter;
    inter.start = Query(interval.start);
    inter.end = Query(interval.end);

    Insert(inter, 1, canonical_partition);
    return canonical_partition;

}


