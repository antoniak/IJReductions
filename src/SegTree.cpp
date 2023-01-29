#include <cmath>
#include "SegTree.h"

SegTree::SegTree(std::vector<int> e): elements(e){

    std::sort(elements.begin(), elements.end());

    std::vector<int>::iterator it;
    it = std::unique(elements.begin(), elements.end());
    elements.resize(std::distance(elements.begin(), it));
    
    height = ceil(log2(elements.size()));
    max_node = pow(2, height + 1);
    
    labels.resize(elements.size());
    for (int i = 0; i < elements.size(); i++) { labels[i] = pow(2, height) + i; }

    root_segment.start = pow(2, height);
    root_segment.end = (2*pow(2, height)) - 1;

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
void SegTree::Insert(Interval interval, int node, int x, int y, std::vector<int> &result) {

    if(node >= max_node) { return; } 

    Interval segment;
    segment.start = x;
    segment.end = y;

    if (utils::isContainedTo(&segment, &interval)) { 
        result.push_back(node);
    } else { 
        Interval lc_segment;
        lc_segment.start = x;
        lc_segment.end = x + ((y - x) / 2);
        Interval rc_segment;
        rc_segment.start = x + ((y - x) / 2) + 1;
        rc_segment.end = y;

        if (intersectsWith(&lc_segment, &interval)) {
            Insert(interval, node * 2, lc_segment.start, lc_segment.end, result);
        }
        if (intersectsWith(&rc_segment, &interval)) {
            Insert(interval, node * 2 + 1, rc_segment.start, rc_segment.end, result);
        }        
    }
    return;
}

std::vector<int> SegTree::CanonicalPartition(Interval interval) {

    std::vector<int> canonical_partition;
    Interval inter;
    inter.start = Query(interval.start);
    inter.end = Query(interval.end);

    Insert(inter, 1, root_segment.start, root_segment.end, canonical_partition);
    return canonical_partition;

}
