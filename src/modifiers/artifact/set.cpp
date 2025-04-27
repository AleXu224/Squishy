#include "set.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/artifact.hpp"
#include "stats/loadout.hpp"

namespace Modifiers::Artifact::Set {
	using namespace Formula::Operators;
	template<class Ret>
	struct SetFormula {
		Ret sheet1;
		Ret sheet2;
		[[nodiscard]] auto compile(const Formula::Context &context) const {
			using namespace Formula::Compiled::Operators;
			return sheet1.compile(context)
				 + sheet2.compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (sheet1 + sheet2).print(context, prevStep);
		}

		[[nodiscard]] constexpr RetType<Ret> eval(const Formula::Context &context) const {
			return sheet1.eval(context)
				 + sheet2.eval(context);
		}
	};

	template<auto Stats::ModsSheet::*location, std::optional<Stats::ArtifactBonus> Stats::Artifact::*location2, class StatMember>
	struct SheetFormulaMaker {
		struct Frm {
			StatMember stat;
			using Ret = RetType<typename StatMember::RetType>;

			[[nodiscard]] Formula::Compiled::NodeType<Ret> compile(const Formula::Context &context) const {
				const auto &bonus = std::invoke(location2, context.source.artifact);
				if (!bonus) return Formula::Compiled::Constant<Ret>{};
				const auto &val = bonus.value();
				const auto &mod = stat.resolve(std::invoke(location, val.bonusPtr.mods));
				if (!mod.hasValue()) return Formula::Compiled::Constant<Ret>{};
				return mod.compile(context);
			}

			[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
				const auto &bonus = std::invoke(location2, context.source.artifact);
				if (!bonus) return std::string();
				const auto &val = bonus.value();
				const auto &mod = stat.resolve(std::invoke(location, val.bonusPtr.mods));
				if (!mod.hasValue()) return std::string();
				return mod.print(context);
			}

			[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
				const auto &bonus = std::invoke(location2, context.source.artifact);
				if (!bonus) return Ret{};
				const auto &val = bonus.value();
				const auto &mod = stat.resolve(std::invoke(location, val.bonusPtr.mods));
				if (!mod.hasValue()) return Ret{};
				return mod.eval(context);
			}
		};
	};

	const Stats::Sheet<Formula::FloatNode> &sheet1PreMods() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::preMod, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1PostMods() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::postMod, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1TeamPreMods() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamPreMod, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet1TeamPostMods() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamPostMod, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Talents<Formula::IntNode> &sheet1Talents() {
		static auto ret = talentFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::talents, &Stats::Artifact::bonus1, TalentMember<Stats::ModsSheet::_Talents>>::Frm>(TalentPointerFactory<Stats::ModsSheet::_Talents>{});
		return ret;
	}
	const Talents<Formula::IntNode> &sheet1TeamTalents() {
		static auto ret = talentFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::teamTalents, &Stats::Artifact::bonus1, TalentMember<Stats::ModsSheet::_Talents>>::Frm>(TalentPointerFactory<Stats::ModsSheet::_Talents>{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &sheet1Enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::enemy, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{});
		return ret;
	}

	const Stats::Sheet<Formula::FloatNode> &sheet2PreMods() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::preMod, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2PostMods() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::postMod, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2TeamPreMods() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamPreMod, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &sheet2TeamPostMods() {
		static auto ret = statFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::teamPostMod, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Talents<Formula::IntNode> &sheet2Talents() {
		static auto ret = talentFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::talents, &Stats::Artifact::bonus2, TalentMember<Stats::ModsSheet::_Talents>>::Frm>(TalentPointerFactory<Stats::ModsSheet::_Talents>{});
		return ret;
	}
	const Talents<Formula::IntNode> &sheet2TeamTalents() {
		static auto ret = talentFactory<Formula::IntNode, SheetFormulaMaker<&Stats::ModsSheet::teamTalents, &Stats::Artifact::bonus2, TalentMember<Stats::ModsSheet::_Talents>>::Frm>(TalentPointerFactory<Stats::ModsSheet::_Talents>{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &sheet2Enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, SheetFormulaMaker<&Stats::ModsSheet::enemy, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{});
		return ret;
	}

	const Stats::Sheet<Formula::FloatNode> &preMods() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1PreMods(), sheet2PreMods());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &postMods() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1PostMods(), sheet2PostMods());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamPreMods() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1TeamPreMods(), sheet2TeamPreMods());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamPostMods() {
		static auto ret = statFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1TeamPostMods(), sheet2TeamPostMods());
		return ret;
	}
	const Talents<Formula::IntNode> &talents() {
		static auto ret = talentFactory<Formula::IntNode, SetFormula<Formula::IntNode>>(sheet1Talents(), sheet2Talents());
		return ret;
	}
	const Talents<Formula::IntNode> &teamTalents() {
		static auto ret = talentFactory<Formula::IntNode, SetFormula<Formula::IntNode>>(sheet1TeamTalents(), sheet2TeamTalents());
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, SetFormula<Formula::FloatNode>>(sheet1Enemy(), sheet2Enemy());
		return ret;
	}
}// namespace Modifiers::Artifact::Set