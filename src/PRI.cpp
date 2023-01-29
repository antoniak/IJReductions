#include "Utils.h"
#include "PRI.h"
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>
#include "boost/tuple/tuple.hpp"

template<class T>
void PRI::ReadTable(const std::string& path, Table<T>& table) {    
    std::ifstream input(path, std::ifstream::in);
    table.clear();
    int id;
    T elem_0;
    T elem_1;
    while (input >> id) {
        input >> elem_0;
        input >> elem_1;
        auto tuple = std::make_tuple(id, elem_0, elem_1);
        table.push_back(tuple);
    }
    input.close();
}

PRI::PRI(std::string path): PathToFiles(path) {

    ReadTable<int>(PathToFiles + POINTS, Points); // Read table with points.
    ReadTable<Interval>(PathToFiles + RECTANGLES, Rectangles); // Read table with rectangles.

    // Construct a segment trees.
    std::vector<int> values_x;
    for (Tuple<Interval>& tuple : Rectangles) {
        values_x.push_back(std::get<1>(tuple).start);
        values_x.push_back(std::get<1>(tuple).end);
    }    
    SegmentTree_x = new SegTree(values_x); // Segment tree for the intervals in x-axis.

    std::vector<int> values_y;
    for (Tuple<Interval>& tuple : Rectangles) {
        values_y.push_back(std::get<2>(tuple).start);
        values_y.push_back(std::get<2>(tuple).end);
    }   
    SegmentTree_y = new SegTree(values_y); // Segment tree for the intervals in y-axis.

    // // Apply the reduction.
    std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> hash_table;
    for (Tuple<int>& tuple : Points) {
        int node_0 = SegmentTree_x->Query(std::get<1>(tuple));
        int node_1 = SegmentTree_y->Query(std::get<2>(tuple));

        for (int n_0 = node_0; n_0 > 0; n_0 /= 2) {
            for (int n_1 = node_1; n_1 > 0; n_1 /= 2) {
                hash_table.insert(std::make_pair(std::make_tuple(n_0, n_1), std::get<0>(tuple)));
            }
        }
    }

    for (Tuple<Interval>& tuple : Rectangles) {
        std::vector<int> nodes_0 = SegmentTree_x->CanonicalPartition(std::get<1>(tuple));
        std::vector<int> nodes_1 = SegmentTree_y->CanonicalPartition(std::get<2>(tuple));
        for (int i = 0; i < nodes_0.size(); i++) {
            for(int j = 0; j < nodes_1.size(); j++) {
                auto t = std::make_tuple(nodes_0[i], nodes_1[j], std::get<0>(tuple));
                Rectangles_t.push_back(t);
            }
        }
    }

    long long count = 0;
    for (auto &tuple: Rectangles_t) {
        auto key = std::make_pair(std::get<0>(tuple), std::get<1>(tuple)); 
        auto range = hash_table.equal_range(key);
        for (auto it = range.first; it != range.second; ++it) {
            count++;
            // std::cout << it->first << ' ' << it->second << '\n';
        }
    }

    // return count;

}

PRI::~PRI() {}

