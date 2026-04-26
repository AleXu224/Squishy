#include "set.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/skillFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/disc.hpp"
#include "stats/loadout.hpp"


namespace Modifiers::Disc::Set {
	using namespace Formula::Operators;
	template<class Ret>
	struct SetFormula : Formula::FormulaBase<RetType<Ret>> {
		Ret sheet1;
		Ret sheet2;
		Ret sheet3;
		[[nodiscard]] Formula::NodeType<RetType<Ret>> fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = sheet1
					 + sheet2
					 + sheet3;
			if (&context.active == &context.source && args.enableGates) {
				return SetFormula{
					.sheet1 = sheet1,
					.sheet2 = sheet2,
					.sheet3 = sheet3,
				};
			}
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (sheet1 + sheet2 + sheet3).print(context, prevStep);
		}

		[[nodiscard]] constexpr RetType<Ret> eval(const Formula::Context &context) const {
			return sheet1.eval(context)
				 + sheet2.eval(context)
				 + sheet3.eval(context);
		}
	};

	template<auto Stats::ModsSheet::*location, std::optional<Stats::DiscBonus> Stats::Disc::*location2, class StatMember>
	struct SheetFormulaMaker {
		struct Frm : Formula::FormulaBase<RetType<typename StatMember::RetType>> {
			StatMember stat;
			using Ret = RetType<typename StatMember::RetType>;

			[[nodiscard]] Formula::NodeType<Ret> fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
				const auto &bonus = std::invoke(location2, context.source.loadout().disc);
				if (!bonus) return Formula::ConstantBase<Ret>{.value = {}};
				const auto &val = bonus.value();
				const auto &mod = stat.resolve(std::invoke(location, val.bonusPtr->mods));
				if (!mod.hasValue()) return Formula::ConstantBase<Ret>{.value = {}};
				return mod.fold(context, args);
			}

			[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
				const auto &bonus = std::invoke(location2, context.source.loadout().disc);
				if (!bonus) return std::string();
				const auto &val = bonus.value();
				const auto &mod = stat.resolve(std::invoke(location, val.bonusPtr->mods));
				if (!mod.hasValue()) return std::string();
				return mod.print(context);
			}

			[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
				const auto &bonus = std::invoke(location2, context.source.loadout().disc);
				if (!bonus) return Ret{};
				const auto &val = bonus.value();
				const auto &mod = stat.resolve(std::invoke(location, val.bonusPtr->mods));
				if (!mod.hasValue()) return Ret{};
				return mod.eval(context);
			}
		};
	};

	const Stats::Sheet<Formula::FloatNode> &sheet1Base() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::base, &Stats::Disc::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1Initial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::initial, &Stats::Disc::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1Combat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::combat, &Stats::Disc::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1TeamInitial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamInitial, &Stats::Disc::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1TeamCombat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamCombat, &Stats::Disc::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1ActiveInitial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::activeInitial, &Stats::Disc::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1ActiveCombat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::activeCombat, &Stats::Disc::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Skills<Formula::IntNode> &sheet1Skills() {
		static auto ret = skillFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::skills, &Stats::Disc::bonus1, SkillMember<Stats::ModsSheet::_Skills>>::Frm>(SkillPointerFactory<Stats::ModsSheet::_Skills>{});
		return ret;
	}
	const Skills<Formula::IntNode> &sheet1TeamSkills() {
		static auto ret = skillFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::teamSkills, &Stats::Disc::bonus1, SkillMember<Stats::ModsSheet::_Skills>>::Frm>(SkillPointerFactory<Stats::ModsSheet::_Skills>{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &sheet1Enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::enemy, &Stats::Disc::bonus1, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{});
		return ret;
	}

	const Stats::Sheet<Formula::FloatNode> &sheet2Base() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::base, &Stats::Disc::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2Initial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::initial, &Stats::Disc::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2Combat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::combat, &Stats::Disc::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2TeamInitial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamInitial, &Stats::Disc::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2TeamCombat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamCombat, &Stats::Disc::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2ActiveInitial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::activeInitial, &Stats::Disc::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2ActiveCombat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::activeCombat, &Stats::Disc::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Skills<Formula::IntNode> &sheet2Skills() {
		static auto ret = skillFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::skills, &Stats::Disc::bonus2, SkillMember<Stats::ModsSheet::_Skills>>::Frm>(SkillPointerFactory<Stats::ModsSheet::_Skills>{});
		return ret;
	}
	const Skills<Formula::IntNode> &sheet2TeamSkills() {
		static auto ret = skillFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::teamSkills, &Stats::Disc::bonus2, SkillMember<Stats::ModsSheet::_Skills>>::Frm>(SkillPointerFactory<Stats::ModsSheet::_Skills>{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &sheet2Enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::enemy, &Stats::Disc::bonus2, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{});
		return ret;
	}

	const Stats::Sheet<Formula::FloatNode> &sheet3Base() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::base, &Stats::Disc::bonus3, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet3Initial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::initial, &Stats::Disc::bonus3, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet3Combat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::combat, &Stats::Disc::bonus3, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet3TeamInitial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamInitial, &Stats::Disc::bonus3, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet3TeamCombat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamCombat, &Stats::Disc::bonus3, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet3ActiveInitial() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::activeInitial, &Stats::Disc::bonus3, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet3ActiveCombat() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::activeCombat, &Stats::Disc::bonus3, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Skills<Formula::IntNode> &sheet3Skills() {
		static auto ret = skillFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::skills, &Stats::Disc::bonus3, SkillMember<Stats::ModsSheet::_Skills>>::Frm>(SkillPointerFactory<Stats::ModsSheet::_Skills>{});
		return ret;
	}
	const Skills<Formula::IntNode> &sheet3TeamSkills() {
		static auto ret = skillFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::teamSkills, &Stats::Disc::bonus3, SkillMember<Stats::ModsSheet::_Skills>>::Frm>(SkillPointerFactory<Stats::ModsSheet::_Skills>{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &sheet3Enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::enemy, &Stats::Disc::bonus3, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{});
		return ret;
	}

	const Stats::Sheet<Formula::FloatNode> &base() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1Base(), sheet2Base(), sheet3Base());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &initial() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1Initial(), sheet2Initial(), sheet3Initial());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &combat() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1Combat(), sheet2Combat(), sheet3Combat());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamInitial() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1TeamInitial(), sheet2TeamInitial(), sheet3TeamInitial());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamCombat() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1TeamCombat(), sheet2TeamCombat(), sheet3TeamCombat());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activeInitial() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1ActiveInitial(), sheet2ActiveInitial(), sheet3ActiveInitial());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activeCombat() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1ActiveCombat(), sheet2ActiveCombat(), sheet3ActiveCombat());
		return ret;
	}
	const Skills<Formula::IntNode> &skills() {
		static auto ret = skillFactory<Formula::IntNode, SetFormula<Formula::IntNode>>(sheet1Skills(), sheet2Skills(), sheet3Skills());
		return ret;
	}
	const Skills<Formula::IntNode> &teamSkills() {
		static auto ret = skillFactory<Formula::IntNode, SetFormula<Formula::IntNode>>(sheet1TeamSkills(), sheet2TeamSkills(), sheet3TeamSkills());
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1Enemy(), sheet2Enemy(), sheet3Enemy());
		return ret;
	}
}// namespace Modifiers::Disc::Set