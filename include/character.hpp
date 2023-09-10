#pragma once
#include "ICharacterData.hpp"
#include "IWeaponData.hpp"
#include "artifact.hpp"
#include "statSheet.hpp"
#include <array>
#include <functional>
#include <memory>

namespace Squishy {
    struct Character {
        const ICharacterData &data;
        std::reference_wrapper<const IWeaponData> weaponData;
        StatSheet sheet{};
        ICharacterData::Conditionals conditionals{};
        IWeaponData::Conditionals weaponConditionals{};
        Nodes nodes{};
        std::vector<Node> weaponNodes{};
        std::array<std::weak_ptr<Artifact>, 5> artifacts{};

        Character(const ICharacterData &data, const IWeaponData &weaponData) : data(data), weaponData(weaponData) {
            conditionals = data.conditionalsSetup(sheet);
            data.addStatSheetData(sheet);
            data.modsSetup(sheet, data.talentMultipliers, conditionals);
            weaponConditionals = weaponData.conditionalsSetup(sheet);
            weaponData.addStatSheetData(sheet);
            weaponData.modsSetup(sheet, weaponConditionals);
            nodes = data.nodeSetup(sheet, data.talentMultipliers, conditionals);
            weaponNodes = weaponData.nodeSetup(sheet, weaponConditionals);
        }

        void update() {
            sheet.stats.clear();
            data.addStatSheetData(sheet);
            data.modsSetup(sheet, data.talentMultipliers, conditionals);
            weaponData.get().addStatSheetData(sheet);
			weaponData.get().modsSetup(sheet, weaponConditionals);
			nodes = data.nodeSetup(sheet, data.talentMultipliers, conditionals);
			weaponNodes = weaponData.get().nodeSetup(sheet, weaponConditionals);
		}

        void updateWeapon(const IWeaponData &weaponData) {
            this->weaponData = weaponData;
            weaponConditionals = weaponData.conditionalsSetup(sheet);
            update();
        }
    };
}