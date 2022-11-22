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

int SegTree::Query(int point) {

    std::vector<int>::iterator low = std::lower_bound(elements.begin(), elements.end(), point);
    int index = low - elements.begin();
    return labels[index];

}

void SegTree::Insert(Interval interval, int node, std::vector<int> &result) {

    if(node >= pow(2, height + 1)) { return; }

    int interval_left, interval_right;
    int node_left = node, node_right = node;
    
    for (int i = 0; i < height - ceil(log2(node)); i++) {
        node_left = node_left*2;
        node_right = node_right*2 + 1;
    }

    interval_left = Query(interval.start);
    interval_right = Query(interval.end);

    if ((interval_left <= node_left) && (interval_right >= node_right)) {
        result.push_back(node);
    } else {
        if ((interval_left >= node_left) && (interval_left <= node_right)) {
            Insert(interval, node*2, result);
        }
        if ((interval_right >= node_left) && (interval_right <= node_right)) {
            Insert(interval, node*2 + 1, result);
        }
    }
    return;
}

std::vector<int> SegTree::CanonicalPartition(Interval interval) {

    std::vector<int> canonical_partition;
    Insert(interval, 1, canonical_partition);
    return canonical_partition;

}


