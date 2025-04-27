#pragma once

#include "artifact/set.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"


namespace Modifiers::Artifact::Set {
	const Stats::Sheet<Formula::FloatNode> &sheet1PreMods();
	const Stats::Sheet<Formula::FloatNode> &sheet1PostMods();
	const Stats::Sheet<Formula::FloatNode> &sheet1TeamPreMods();
	const Stats::Sheet<Formula::FloatNode> &sheet1TeamPostMods();
	const Talents<Formula::IntNode> &sheet1Talents();
	const Talents<Formula::IntNode> &sheet1TeamTalents();
	const Stats::EnemySheet<Formula::FloatNode> &sheet1Enemy();

	const Stats::Sheet<Formula::FloatNode> &sheet2PreMods();
	const Stats::Sheet<Formula::FloatNode> &sheet2PostMods();
	const Stats::Sheet<Formula::FloatNode> &sheet2TeamPreMods();
	const Stats::Sheet<Formula::FloatNode> &sheet2TeamPostMods();
	const Talents<Formula::IntNode> &sheet2Talents();
	const Talents<Formula::IntNode> &sheet2TeamTalents();
	const Stats::EnemySheet<Formula::FloatNode> &sheet2Enemy();

	const Stats::Sheet<Formula::FloatNode> &preMods();
	const Stats::Sheet<Formula::FloatNode> &postMods();
	const Stats::Sheet<Formula::FloatNode> &teamPreMods();
	const Stats::Sheet<Formula::FloatNode> &teamPostMods();
	const Talents<Formula::IntNode> &talents();
	const Talents<Formula::IntNode> &teamTalents();
	const Stats::EnemySheet<Formula::FloatNode> &enemy();
}// namespace Modifiers::Artifact::Set