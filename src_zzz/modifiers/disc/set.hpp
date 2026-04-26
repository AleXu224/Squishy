#pragma once

#include "disc/set.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"


namespace Modifiers::Disc::Set {
	const Stats::Sheet<Formula::FloatNode> &sheet1Base();
	const Stats::Sheet<Formula::FloatNode> &sheet1Initial();
	const Stats::Sheet<Formula::FloatNode> &sheet1Combat();
	const Stats::Sheet<Formula::FloatNode> &sheet1TeamInitial();
	const Stats::Sheet<Formula::FloatNode> &sheet1TeamCombat();
	const Stats::Sheet<Formula::FloatNode> &sheet1ActiveInitial();
	const Stats::Sheet<Formula::FloatNode> &sheet1ActiveCombat();
	const Skills<Formula::IntNode> &sheet1Skills();
	const Skills<Formula::IntNode> &sheet1TeamSkills();
	const Stats::EnemySheet<Formula::FloatNode> &sheet1Enemy();

	const Stats::Sheet<Formula::FloatNode> &sheet2Base();
	const Stats::Sheet<Formula::FloatNode> &sheet2Initial();
	const Stats::Sheet<Formula::FloatNode> &sheet2Combat();
	const Stats::Sheet<Formula::FloatNode> &sheet2TeamInitial();
	const Stats::Sheet<Formula::FloatNode> &sheet2TeamCombat();
	const Stats::Sheet<Formula::FloatNode> &sheet2ActiveInitial();
	const Stats::Sheet<Formula::FloatNode> &sheet2ActiveCombat();
	const Skills<Formula::IntNode> &sheet2Skills();
	const Skills<Formula::IntNode> &sheet2TeamSkills();
	const Stats::EnemySheet<Formula::FloatNode> &sheet2Enemy();

	const Stats::Sheet<Formula::FloatNode> &sheet3Base();
	const Stats::Sheet<Formula::FloatNode> &sheet3Initial();
	const Stats::Sheet<Formula::FloatNode> &sheet3Combat();
	const Stats::Sheet<Formula::FloatNode> &sheet3TeamInitial();
	const Stats::Sheet<Formula::FloatNode> &sheet3TeamCombat();
	const Stats::Sheet<Formula::FloatNode> &sheet3ActiveInitial();
	const Stats::Sheet<Formula::FloatNode> &sheet3ActiveCombat();
	const Skills<Formula::IntNode> &sheet3Skills();
	const Skills<Formula::IntNode> &sheet3TeamSkills();
	const Stats::EnemySheet<Formula::FloatNode> &sheet3Enemy();

	const Stats::Sheet<Formula::FloatNode> &base();
	const Stats::Sheet<Formula::FloatNode> &initial();
	const Stats::Sheet<Formula::FloatNode> &combat();
	const Stats::Sheet<Formula::FloatNode> &teamInitial();
	const Stats::Sheet<Formula::FloatNode> &teamCombat();
	const Stats::Sheet<Formula::FloatNode> &activeInitial();
	const Stats::Sheet<Formula::FloatNode> &activeCombat();
	const Skills<Formula::IntNode> &skills();
	const Skills<Formula::IntNode> &teamSkills();
	const Stats::EnemySheet<Formula::FloatNode> &enemy();
}// namespace Modifiers::Disc::Set