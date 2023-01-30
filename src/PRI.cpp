#include "Utils.h"
#include "PRI.h"

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

template<class T>
void PRI::ReadGeom(const std::string& path, std::vector<T>& table) {
    std::ifstream input(path, std::ifstream::in);
    T object;
    table.clear();
    for(std::string line; getline(input, line);){
        boost::erase_all(line, "\\");
        boost::geometry::read_wkt(line, object);
        boost::geometry::correct(object);
        table.push_back(object);
    }
    input.close();
}


PRI::PRI(std::string path): PathToFiles(path) {

    ReadTable<int>(PathToFiles + POINTS, Points); // Read table with points.
    ReadTable<Interval>(PathToFiles + RECTANGLES, Rectangles); // Read table with rectangles.
    ReadGeom<types::Point>(PathToFiles + POINTS_GEOM, Points_geom); 
    ReadGeom<types::MultiPolygon>(PathToFiles + POLYGONS_GEOM, Polygons_geom); 

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

    // Apply the reduction.
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

    long long count = 0;
    for (auto &tuple: Rectangles_t) {
        auto key = std::make_pair(std::get<0>(tuple), std::get<1>(tuple)); 
        auto range = hash_table.equal_range(key);

        for (auto it = range.first; it != range.second; ++it) {
            if(boost::geometry::within(Points_geom[it->second], Polygons_geom[std::get<2>(tuple)])) {
                count++;
            }
        }
    }

    std::cout << "Result: " << count << std::endl;

    auto join_time = std::chrono::steady_clock::now();
    std::cerr << "Join Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(join_time-hash_time).count() << " ms\n";
}

PRI::~PRI() {}

