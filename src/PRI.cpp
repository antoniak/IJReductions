#include "Utils.h"
#include "PRI.h"
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>
#include "boost/tuple/tuple.hpp"
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/io/wkt/read.hpp>

namespace bg = boost::geometry;

template<typename T>
std::vector<std::vector<T>> SplitVector(const std::vector<T>& vec, size_t n)
{
    std::vector<std::vector<T>> outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(n, vec.size()); ++i)
    {
        end += (remain > 0) ? (length + !!(remain--)) : length;

        outVec.push_back(std::vector<T>(vec.begin() + begin, vec.begin() + end));

        begin = end;
    }

    return outVec;
}

template<class T>
void PRI::ReadTable(const std::string& path, Table<T>& table) {    
    std::ifstream input(path, std::ifstream::in);
    table.clear();
    int id = 0;
    T elem_0;
    T elem_1;
    while (input >> elem_0) {
        input >> elem_1;
        auto tuple = std::make_tuple(id, elem_0, elem_1);
        table.push_back(tuple);
        id++;
    }
    input.close();
}

void PRI::ReadGeom(const std::string& path, std::vector<std::string>& table) {
    std::ifstream input(path, std::ifstream::in);

    table.clear();
    for(std::string line; getline(input, line);){
        table.push_back(line);
    }
    input.close();
}


PRI::PRI(std::string path): PathToFiles(path) {

    ReadTable<int>(PathToFiles + POINTS, Points); // Read table with points.
    ReadTable<Interval>(PathToFiles + RECTANGLES, Rectangles); // Read table with rectangles.
    ReadGeom(PathToFiles + POINTS_GEOM, Points_geom); 
    ReadGeom(PathToFiles + POLYGONS_GEOM, Polygons_geom); 

    auto start_time = std::chrono::steady_clock::now();

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

    auto reduction_time = std::chrono::steady_clock::now();
    std::cerr << "Reduction Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(reduction_time-start_time).count() << " ms\n";

    // // Apply the reduction.
    boost::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> hash_table;
    for (Tuple<int>& tuple : Points) {
        int node_0 = SegmentTree_x->Query(std::get<1>(tuple));
        int node_1 = SegmentTree_y->Query(std::get<2>(tuple));

        for (int n_0 = node_0; n_0 > 0; n_0 /= 2) {
            for (int n_1 = node_1; n_1 > 0; n_1 /= 2) {
                hash_table.insert(std::make_pair(std::make_pair(n_0, n_1), std::get<0>(tuple)));
            }
        }
    }

    auto hash_time = std::chrono::steady_clock::now();
    std::cerr << "Hash Table Construction Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(hash_time-reduction_time).count() << " ms\n";

    typedef bg::model::point<double, 2, bg::cs::cartesian> Point;
    Point point;
    bg::model::ring<Point> polygon;

    long long count = 0;
    for (auto &tuple: Rectangles_t) {
        auto key = std::make_pair(std::get<0>(tuple), std::get<1>(tuple)); 
        auto range = hash_table.equal_range(key);

        for (auto it = range.first; it != range.second; ++it) {
            bg::read_wkt(Points_geom[it->second], point);
            bg::read_wkt("POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3))", polygon);
            bg::intersects(point, polygon);
            count++;
        }
    }

    auto join_time = std::chrono::steady_clock::now();
    std::cerr << "Join Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(join_time-reduction_time).count() << " ms\n";
}

PRI::~PRI() {}

