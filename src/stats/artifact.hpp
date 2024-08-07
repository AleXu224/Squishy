#pragma once

#include "conditional/conditional.hpp"

namespace Stats {
    struct Artifact {
        Conditional::ArtifactMap conditionals{};
    };
}