#pragma once
#include <string>

#include <filesystem>
#include "Range.h"

template<class Iter>
class DirIterator {
public:
    DirIterator(const Iter& it1)
    : it(it1) {}

    DirIterator& operator++() {
        ++it;
        return *this;
    }

    DirIterator operator++(int) {
        auto copy(*this);
        ++it;
        return copy;
    }

    std::filesystem::directory_entry& operator*() {
        return *it;
    }

    bool operator==(const DirIterator& other) const {
        return it == other.it;
    }

    bool operator!=(const DirIterator& other) const {
        return !(*this == other);
    }
private:
    Iter it;
};

class Dir : public Pipe {
public:
    Dir(std::string path, bool recursive = true)
    : path_(std::move(path))
    , recursive_(recursive) {}

    auto begin() {
        if (recursive_) {
            return DirIterator(
                std::filesystem::recursive_directory_iterator(path_)
            );
        } else {
            return DirIterator(
                std::filesystem::recursive_directory_iterator(path_)
            );
        }
    }
private:
    bool recursive_;
    std::string path_;
};