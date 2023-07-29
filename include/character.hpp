#pragma once
#include "ICharacterData.hpp"
#include "statSheet.hpp"

namespace Squishy {
    struct Character {
        const ICharacterData &data;
        StatSheet sheet{};
        Nodes nodes{};

        Character(const ICharacterData &data) : data(data) {
            data.addStatSheetData(sheet);
            nodes = data.nodeSetup(sheet, data.talentMultipliers);
        }
    };
}