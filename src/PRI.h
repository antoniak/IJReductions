#ifndef PRI_H
#define PRI_H

#include "SegTree.h"
#include "Utils.h"

using namespace utils;
using namespace constants;
using namespace types;

class PRI {

public:
    explicit PRI(std::string path);
    ~PRI();

private:
    int threads;
    std::string pathToFiles;
    std::vector<std::tuple<int, int>> Points;
    std::vector<std::tuple<Interval, Interval>> Rectangles;
    SegTree *ST_x;
    SegTree *ST_y;
    std::vector<std::tuple<int, int , int, int, int, int>> Rectangles_t;
    std::vector<std::tuple<int, int, int, int>> Points_t;

    void ConstructSegmentTrees();
    void Reduce();
    template<class T> void Load(const std::string& path, types::Table<T>& table);
};

#endif //PRI_H