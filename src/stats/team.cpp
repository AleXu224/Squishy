#include "team.hpp"
#include "formula/element.hpp"
#include "formula/operators.hpp"
#include "formula/option.hpp"
#include "formula/requires.hpp"
#include "formula/teamCharacter.hpp"


Stats::Team::Team() : infusion(Formula::TeamInfusion{}) {
	using namespace Formula::Operators;

	using IsActive = Formula::impl_IsActive<Stats::Team>;

	// Pyro
	resonances.atk_.modifiers.at(0) = Formula::Requires(Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount(Misc::Element::pyro) >= 2, Formula::Constant(0.25f));

	// Hydro
	resonances.hp_.modifiers.at(0) = Formula::Requires(Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount(Misc::Element::hydro) >= 2, Formula::Constant(0.25f));

	// Cryo
	auto cryoCond = Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount(Misc::Element::cryo) >= 2;
	auto cryoBuff = Formula::Requires(cryoCond && IsActive("teamCryoCond"), Formula::Constant(0.15f));
	resonances.cr.modifiers.at(0) = cryoBuff;

	// Geo
	auto geoCond = Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount(Misc::Element::geo) >= 2;
	resonances.shield_.modifiers.at(0) = Formula::Requires(geoCond, Formula::Constant(0.15f));
	auto geoBuff1 = Formula::Requires(geoCond && IsActive("teamGeoCond"), Formula::Constant(0.15f));
	resonances.all.DMG.modifiers.at(0) = geoBuff1;
	auto geoBuff2 = Formula::Requires(geoCond && IsActive("teamGeoCond") && IsActive("teamGeoCond2"), Formula::Constant(-0.20f));
	resonancesEnemy.resistance.geo.modifiers.at(0) = geoBuff2;

	// Dendro
	auto dendroCond = Formula::TeamCharacterCount{} >= 4 && Formula::ElementCount(Misc::Element::dendro) >= 2;
	resonances.em.modifiers.at(0) = Formula::Requires(
		dendroCond,
		Formula::ConstantFlat(50.f)
			+ Formula::Requires(IsActive("teamDendroCond"), Formula::ConstantFlat(30.f))
			+ Formula::Requires(IsActive("teamDendroCond2"), Formula::ConstantFlat(20.f))
	);

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
			.mods{.preMod{.geo{.DMG = geoBuff1}}},
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
			.mods{.preMod{.em = Formula::Requires(dendroCond && IsActive("teamDendroCond"), Formula::ConstantFlat(30.f))}},
		},
	});
	options.insert({
		Utils::HashedString("teamDendroCond2"),
		Option::Boolean{
			.key = "teamDendroCond2",
			.name = "After triggering Aggravate, Spread, Hyperbloom, or Burgeon reactions",
			.displayCondition = dendroCond,
			.mods{.preMod{.em = Formula::Requires(dendroCond && IsActive("teamDendroCond2"), Formula::ConstantFlat(20.f))}},
		},
	});
}