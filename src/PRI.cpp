#include "Utils.h"
#include "PRI.h"

PRI::PRI(std::string path): pathToFiles(path) {

// Load Points in memory 
std::ifstream input_points(pathToFiles + POINTS);
if (!input_points) {
    std::cout << "Error: " << pathToFiles << POINTS << " does not exist.";
    exit(1);
}
Load(pathToFiles + POINTS, Points);

// Load Rectangles in memory
std::ifstream input_polygons(pathToFiles + RECTANGLES);
if (!input_polygons) {
    std::cout << "Error: " << pathToFiles << RECTANGLES << " does not exist.";
    exit(1);
}
Load(pathToFiles + RECTANGLES, Rectangles);

// Construct two segment trees for the Rectangles
ConstructSegmentTrees();

Reduce();

std::ofstream points_file(pathToFiles + POINTS_T);
std::stringstream ss;
for(auto t: Points_t) {
    ss << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << " " << std::get<3>(t) << "\n";        
}
points_file << ss.str();
points_file.close();
ss.str("");

std::ofstream rectangles_file(pathToFiles + RECTANGLES_T + FILE_EXT);
for(auto t: Rectangles_t) {
    ss << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << " " << std::get<3>(t) << " " << std::get<4>(t) << " " << std::get<5>(t) << "\n";        
}
rectangles_file << ss.str();
rectangles_file.close();
ss.str("");
}

PRI::~PRI() {}

void PRI::ConstructSegmentTrees() {

    std::vector<int> values_0;
    for (auto& tuple : Rectangles) {
        values_0.push_back(tuple[0].x);
        values_0.push_back(tuple[0].y);
    }    

    st_x = new SegTree(values_0);

    std::vector<int> values_1;
    for (auto& tuple : Rectangles) {
        values_1.push_back(tuple[1].x);
        values_1.push_back(tuple[1].y);
    }
    
    st_y = new SegTree(values_1);

}

void PRI::Reduce() {
    
    for (auto& tuple : Points) {
        int node_0 = st_x->Query(tuple[0].x);
        int node_1 = st_y->Query(tuple[1].x);
        for (int n_0 = node_0; n_0 > 0; n_0 /= 2) {
            for (int n_1 = node_1; n_1 > 0; n_1 /= 2) {
                auto t = std::make_tuple(n_0, n_1, tuple[0].x, tuple[1].x);
                Points_t.push_back(t);
            }
        }
    }

    for (auto& tuple : Rectangles) {
        std::vector<int> nodes_0 = st_x->CanonicalPartition(tuple[0]);
        std::vector<int> nodes_1 = st_y->CanonicalPartition(tuple[0]);
        for (int i = 0; i < nodes_0.size(); i++) {
            for(int j = 0; j < nodes_1.size(); j++) {
                auto t = std::make_tuple(nodes_0[i], nodes_1[j], tuple[0].x, tuple[0].y, tuple[1].x, tuple[1].y);
                Rectangles_t.push_back(t);
            }
        }
    }
}

template<class T>
void PRI::Load(const std::string& path, Table<T>& table) {
    table.clear();
    std::ifstream input(path);
    if (!input) {
        std::cout << "Error: " << path + " does not exist." << std::endl;
        exit(1);
    }

    T elem_0;
    T elem_1;
    while (input >> elem_0) {
        input >> elem_1;
        Tuple<T> tuple = std::make_tuple(elem_0, elem_1);
        table.push_back(tuple);
    }
}
