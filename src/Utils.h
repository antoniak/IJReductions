#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>
#include <map>
#include <chrono>
#include <algorithm>
#include <atomic>
#include <cassert>
#include <functional>
#include <thread>
#include <tuple>
// #include </home/exet4718/postgres/include/libpq-fe.h>

struct Interval {
    int start, end;
};

inline std::istream& operator>>(std::istream& input, Interval& interval) {
    input >> interval.start >> interval.end;
    return input;
}

namespace constants {
    static const std::string POINTS = "Points.tbl";
    static const std::string RECTANGLES = "Rectangles.tbl";
    static const std::string POINTS_T = "Points_t.tbl";
    static const std::string RECTANGLES_T = "Rectangles_T.tbl";
}

namespace types {
    template<class T> using Tuple = std::tuple<T, T>;
    template<class T> using Table = std::vector<Tuple<T>>;
}

namespace utils {
    static bool isDirectory(const char *path) {
        struct stat info;
        if(stat(path, &info) != 0) return false;
        else if(info.st_mode & S_IFDIR) return true;
        else return false;
    }

    static std::vector<std::string> split_string(std::string input, const char delimiter) {
        std::vector<std::string> elements;
        std::stringstream ss(input);
        std::string substring;
        
        while(getline(ss, substring, delimiter)) {
            elements.push_back(substring);
        }
        return elements;
    }
}
#endif //UTILS_H