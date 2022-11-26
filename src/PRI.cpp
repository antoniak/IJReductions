#include "Utils.h"
#include "PRI.h"

PRI::PRI(std::string path): PathToFiles(path) {

ReadTable(PathToFiles + POINTS, Points); // Read table with points.
ReadTable(PathToFiles + RECTANGLES, Rectangles); // Read table with rectangles.

// Construct a segment trees.
std::vector<int> values_x;
for (auto& tuple : Rectangles) {
    values_x.push_back(std::get<1>(tuple).start);
    values_x.push_back(std::get<1>(tuple).end);
}    
SegmentTree_x = new SegTree(values_x); // Segment tree for the intervals in x-axis.

std::vector<int> values_y;
for (auto& tuple : Rectangles) {
    values_y.push_back(std::get<2>(tuple).start);
    values_y.push_back(std::get<2>(tuple).end);
}   
SegmentTree_y = new SegTree(values_y); // Segment tree for the intervals in y-axis.

// Apply the reduction.
for (auto& tuple : Points) {
    int node_0 = SegmentTree_x->Query(std::get<1>(tuple));
    int node_1 = SegmentTree_y->Query(std::get<2>(tuple));

    for (int n_0 = node_0; n_0 > 0; n_0 /= 2) {
        for (int n_1 = node_1; n_1 > 0; n_1 /= 2) {
            auto t = std::make_tuple(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple), n_0, n_1);
            Points_t.push_back(t);
        }
    }
}

for (auto& tuple : Rectangles) {
    std::vector<int> nodes_0 = SegmentTree_x->CanonicalPartition(std::get<1>(tuple));
    std::vector<int> nodes_1 = SegmentTree_y->CanonicalPartition(std::get<2>(tuple));
    for (int i = 0; i < nodes_0.size(); i++) {
        for(int j = 0; j < nodes_1.size(); j++) {
            auto t = std::make_tuple(std::get<0>(tuple), std::get<1>(tuple).start, std::get<1>(tuple).end, std::get<2>(tuple).start, std::get<2>(tuple).end, nodes_0[i], nodes_1[j]);
            Rectangles_t.push_back(t);
        }
    }
}

std::ofstream points_file(PathToFiles + POINTS_T);
std::stringstream ss;
for(auto t: Points_t) {
    ss << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << " " << std::get<3>(t) << " " << std::get<4>(t) << "\n";        
}
points_file << ss.str();
points_file.close();
ss.str("");

std::ofstream rectangles_file(PathToFiles + RECTANGLES_T);
for(auto t: Rectangles_t) {
    ss << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << " " << std::get<3>(t) << " " << std::get<4>(t) << " " << std::get<5>(t) << " " << std::get<6>(t)  << "\n";        
}
rectangles_file << ss.str();
rectangles_file.close();
ss.str("");
}

PRI::~PRI() {}

template<class T>
void PRI::ReadTable(const std::string& path, Table<T>& table) {    
    std::ifstream input(path, std::ifstream::in);
    table.clear();
    unsigned int id;
    T elem_0;
    T elem_1;
    while (input >> id) {
        input >> elem_0;
        input >> elem_1;
        Tuple<T> tuple = std::make_tuple(id, elem_0, elem_1);
        table.push_back(tuple);
    }
    input.close();
}
