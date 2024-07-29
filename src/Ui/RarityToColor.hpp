#pragma once

#include "../Rarity.hpp"
#include "color.hpp"
#include "unordered_map"

namespace Util {
    const std::unordered_map<Rarity, squi::Color> rarityToColor = {
        {Rarity::fiveStar, squi::Color(188, 105, 49)}
    };
}

