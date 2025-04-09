#pragma once

#include <fstream>
#include <optional>

#include "Range.h"
#include "Transform.h"

class OpenFiles : public Pipe {
    template<class Range>
    auto operator()(const Range& range) {
        auto function = [](const std::filesystem::path path) -> std::optional<std::ifstream> {
            std::ifstream stream(path);
            if (!stream.is_open()) {
                return std::nullopt;
            }
            return stream;
        };
        return TransformRange(range, function);
    }
};