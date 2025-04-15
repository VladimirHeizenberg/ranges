#pragma once
#include <string>

class Dir {
public:
    Dir(std::string path, bool recursive = true)
    : path_(path)
    , recursive_(recursive) {}
private:
    bool recursive_;
    std::string path_;
};