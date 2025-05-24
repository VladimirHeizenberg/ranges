#include "Range.h"
#include <filesystem>

class DirIterator {
public:
    using value_type = std::filesystem::directory_entry;
    using simple_it = std::filesystem::directory_iterator;
    using recursive_it = std::filesystem::recursive_directory_iterator;

    DirIterator() : recursive_(false), simple_it_(), recursive_it_() {}

    DirIterator(const std::filesystem::path& path, bool recursive)
    : recursive_(recursive) {
        if (recursive_) {
            recursive_it_ = recursive_it(path);
            path_ = *recursive_it_;
        }
        else {
            simple_it_ = simple_it(path);
            path_ = *simple_it_;
        }
    }

    const std::filesystem::path& operator*() const {
        if (recursive_) return path_;
        else return path_;
    }

    DirIterator& operator++() {
        if (recursive_) ++recursive_it_;
        else ++simple_it_;
        return *this;
    }

    bool operator==(const DirIterator& other) const {
        if (recursive_) return recursive_it_ == other.recursive_it_;
        else return simple_it_ == other.simple_it_;
    }

    bool operator!=(const DirIterator& other) const {
        return !(*this == other);
    }

private:
    bool recursive_;
    std::filesystem::path path_;
    simple_it simple_it_;
    recursive_it recursive_it_;
};

class Dir {
public:
    using value_type = std::filesystem::directory_entry;
    Dir(const std::string& path, bool recursive = false)
        : path_(path), recursive_(recursive) {}

    DirIterator begin() const {
        return DirIterator(path_, recursive_);
    }

    DirIterator end() const {
        return DirIterator{};
    }

private:
    std::filesystem::path path_;
    bool recursive_;
};
