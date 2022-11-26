#ifndef PRI_H
#define PRI_H

#include "SegTree.h"
#include "Utils.h"

using namespace utils;

// Constrants 
static const std::string POINTS = "Points.tbl";
static const std::string RECTANGLES = "Rectangles.tbl";
static const std::string POINTS_T = "Points_t.tbl";
static const std::string RECTANGLES_T = "Rectangles_T.tbl";

// Types
template<class T> using Tuple = std::tuple<unsigned int, T, T>;
template<class T> using Table = std::vector<Tuple<T>>;

class PRI {

public:
    explicit PRI(std::string path);
    ~PRI();

private:
    int threads;
    std::string PathToFiles;
    Table<int> Points;
    Table<Interval> Rectangles;
    SegTree *SegmentTree_x;
    SegTree *SegmentTree_y;
    std::vector<std::tuple<int, int, int, int, int, int, int>> Rectangles_t;
    std::vector<std::tuple<int, int, int, int, int>> Points_t;

    // void ConstructSegmentTrees();
    // void Reduce();
    template<class T> void ReadTable(const std::string& path, Table<T>& table);
};

#endif //PRI_H