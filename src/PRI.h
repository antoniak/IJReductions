#ifndef PRI_H
#define PRI_H

#include "SegTree.h"
#include "Utils.h"
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>

using namespace utils;
using namespace types;


// Constrants 
static const std::string POINTS = "Points.tbl";
static const std::string RECTANGLES = "Rectangles.tbl";
static const std::string POINTS_GEOM = "Points_geom.tbl";
static const std::string POLYGONS_GEOM = "Polygons_geom.tbl";

// Types
template<class T> using Tuple = std::tuple<int, T, T>;
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
    std::vector<std::tuple<int, int, int>> Rectangles_t;
    std::vector<std::tuple<int, int, int, int, int>> Points_t;
    std::vector<types::Point> Points_geom;
    std::vector<types::MultiPolygon> Polygons_geom;

    template<class T> void ReadTable(const std::string& path, Table<T>& table);
    template<class T> void ReadGeom(const std::string& path, std::vector<T>& table);
};

#endif //PRI_H