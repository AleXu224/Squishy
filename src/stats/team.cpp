#include "team.hpp"
#include "formula/elemental.hpp"
#include "formula/operators.hpp"
#include "formula/option.hpp"
#include "formula/requirement.hpp"
#include "formula/requires.hpp"
#include "formula/teamCharacter.hpp"


Stats::Team::Team() : infusion(Formula::TeamInfusion{}), moonsignLevel(Formula::TeamMoonsignLevel{}) {
	using namespace Formula::Operators;

	auto IsActive = [](const Utils::HashedString &str) {
		return Formula::impl_IsActivePassive{.name = str};
	};

	// Pyro
	resonances.atk_.modifiers.at(0) = Formula::Requires{.requirement = Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount{.element = Misc::Element::pyro} >= 2, .ret = Formula::Constant{.value = 0.25f}};

	// Hydro
	resonances.hp_.modifiers.at(0) = Formula::Requires{.requirement = Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount{.element = Misc::Element::hydro} >= 2, .ret = Formula::Constant{.value = 0.25f}};

	// Cryo
	auto cryoCond = Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount{.element = Misc::Element::cryo} >= 2;
	auto cryoBuff = Formula::Requires{.requirement = cryoCond && IsActive("teamCryoCond"), .ret = Formula::Constant{.value = 0.15f}};
	resonances.cr.modifiers.at(0) = cryoBuff;

	// Geo
	auto geoCond = Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount{.element = Misc::Element::geo} >= 2;
	resonances.shield_.modifiers.at(0) = Formula::Requires{.requirement = geoCond, .ret = Formula::Constant{.value = 0.15f}};
	auto geoBuff1 = Formula::Requires{.requirement = geoCond && IsActive("teamGeoCond"), .ret = Formula::Constant{.value = 0.15f}};
	resonances.all.DMG.modifiers.at(0) = geoBuff1;
	auto geoBuff2 = Formula::Requires{.requirement = geoCond && IsActive("teamGeoCond") && IsActive("teamGeoCond2"), .ret = Formula::Constant{.value = -0.20f}};
	resonancesEnemy.resistance.geo.modifiers.at(0) = geoBuff2;

	// Dendro
	auto dendroCond = Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount{.element = Misc::Element::dendro} >= 2;
	resonances.em.modifiers.at(0) = Formula::Requires{
		.requirement = dendroCond,
		.ret = Formula::ConstantFlat{.value = 50.f}
			 + Formula::Requires{.requirement = IsActive("teamDendroCond"), .ret = Formula::ConstantFlat{.value = 30.f}}
			 + Formula::Requires{.requirement = IsActive("teamDendroCond2"), .ret = Formula::ConstantFlat{.value = 20.f}}
	};

	// Moonsign team effects
	auto moonsignCond = IsActive("teamLunarAscendantDream");
	auto moonsignBuff = Formula::Requires{
		.requirement = moonsignCond,
		.ret = Formula::NonMoonsignCharacterTeamBuff{},
	};
	resonances.allLunar.DMG.modifiers.at(0) = moonsignBuff;


	options.insert({
		Utils::HashedString("teamCryoCond"),
		Option::Boolean{
			.key = "teamCryoCond",
			.name = "Enemy is Frozen or affected by Cryo",
			.displayCondition = cryoCond,
			.mods{.preMod{.cr = cryoBuff}},
		},
	});
	options.insert({
		Utils::HashedString("teamGeoCond"),
		Option::Boolean{
			.key = "teamGeoCond",
			.name = "Character is protected by a shield",
			.displayCondition = geoCond,
			.mods{.preMod{.all{.DMG = geoBuff1}}},
		},
	});
	options.insert({
		Utils::HashedString("teamGeoCond2"),
		Option::Boolean{
			.key = "teamGeoCond2",
			.name = "After dealing damage to enemies",
			.displayCondition = geoCond && IsActive("teamGeoCond"),
			.mods{.enemy{.resistance{.geo = geoBuff2}}},
		},
	});
	options.insert({
		Utils::HashedString("teamDendroCond"),
		Option::Boolean{
			.key = "teamDendroCond",
			.name = "After triggering Burning, Quicken, or Bloom reactions",
			.displayCondition = dendroCond,
			.mods{.preMod{.em = Formula::Requires{.requirement = dendroCond && IsActive("teamDendroCond"), .ret = Formula::ConstantFlat{.value = 30.f}}}},
		},
	});
	options.insert({
		Utils::HashedString("teamDendroCond2"),
		Option::Boolean{
			.key = "teamDendroCond2",
			.name = "After triggering Aggravate, Spread, Hyperbloom, or Burgeon reactions",
			.displayCondition = dendroCond,
			.mods{.preMod{.em = Formula::Requires{.requirement = dendroCond && IsActive("teamDendroCond2"), .ret = Formula::ConstantFlat{.value = 20.f}}}},
		},
	});
	options.insert({
		Utils::HashedString("teamLunarAscendantDream"),
		Option::Boolean{
			.key = "teamLunarAscendantDream",
			.name = "Elemental Skill or Elemental Burst casted by a non-Moonsign character",
			.displayCondition = Requirement::ascendantGleam,
			.mods{.preMod{.allLunar{.DMG = moonsignBuff}}},
		},
	});
}
[[nodiscard]] bool Stats::Team::hasCharacter(::Character::InstanceKey key) const {
	for (const auto &character: characters) {
		if (!character) continue;
		if (character->instanceKey == key) return true;
	}
	return false;
}
