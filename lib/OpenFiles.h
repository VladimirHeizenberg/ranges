#pragma once

#include <fstream>
#include <optional>
#include <filesystem>

#include "Range.h"
#include "Transform.h"

class OpenFiles : public Pipe {
public:
    template<class Range>
    auto operator()(Range&& range) {
        auto function = [](const std::filesystem::path& path) {
            std::ifstream stream(path);
            if (!stream.is_open()) {
                throw std::runtime_error("File " + path.string() + " not found");
            }
            return std::move(stream);
        };
        return range | Transform(function);
    }
};