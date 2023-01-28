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

struct Interval {
    int start, end;
};

inline std::istream& operator>>(std::istream& input, Interval& interval) {
    input >> interval.start >> interval.end;
    return input;
}

namespace utils {
    static bool isDirectory(const char *path) {
        struct stat info;
        if(stat(path, &info) != 0) return false;
        else if(info.st_mode & S_IFDIR) return true;
        else return false;
    }

    static bool isContainedTo(const Interval *n, const Interval *m) {
        if ((m->start <= n->start) && (m->end >= n->end)){
            return true;
        }
        return false;
    }

    static bool intersectsWith(const Interval *n, const Interval *m) {
        if(((n->start >= m->start) && (n->start <= m->end)) || ((m->start >= n->start) && (m->start <= n->end))) {
            return true;
        }
        return false;
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