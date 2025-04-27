#pragma once

#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"


namespace Modifiers::Character::Kit {
	const Stats::Sheet<Formula::FloatNode> &preMods();
	const Stats::Sheet<Formula::FloatNode> &postMods();
	const Stats::Sheet<Formula::FloatNode> &teamPreMods();
	const Stats::Sheet<Formula::FloatNode> &teamPostMods();
	const Talents<Formula::IntNode> &talents();
	const Talents<Formula::IntNode> &teamTalents();
	const Stats::EnemySheet<Formula::FloatNode> &enemy();
}// namespace Modifiers::Character::Kit