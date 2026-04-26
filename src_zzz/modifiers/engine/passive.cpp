#include "passive.hpp"
#include "engine/data.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/skillFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"


namespace Modifiers::Engine::Passive {
	template<auto Stats::ModsSheet::*location, class StatMember>
	struct FormulaMaker {
		struct Frm : Formula::FormulaBase<RetType<typename StatMember::RetType>> {
			StatMember stat;
			SheetMemberIdentifier member;
			using Ret = RetType<typename StatMember::RetType>;

			[[nodiscard]] Formula::NodeType<Ret> fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
				if (!context.source.loadout().engine) return Formula::ConstantBase<Ret>{};
				auto node = stat.resolve(std::invoke(location, context.source.loadout().engine->data->data.mods));
				if (!node.hasValue()) return Formula::ConstantBase<Ret>{};
				return node.fold(context, args);
			}

			[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
				if (!context.source.loadout().engine) return "";
				const auto &mod = stat.resolve(std::invoke(location, context.source.loadout().engine->data->data.mods));
				if (!mod.hasValue()) return "";
				return mod.print(context);
			}

			[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
				if (!context.source.loadout().engine) return Ret{};
				const auto &mod = stat.resolve(std::invoke(location, context.source.loadout().engine->data->data.mods));
				if (!mod.hasValue()) return Ret{};
				return mod.eval(context);
			}
		};
	};

	const Stats::Sheet<Formula::FloatNode> &base() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::base, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &initial() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::initial, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &combat() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::combat, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamInitial() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::teamInitial, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamCombat() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::teamCombat, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activeInitial() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::activeInitial, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activeCombat() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::activeCombat, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Skills<Formula::IntNode> &skills() {
		static auto ret = skillFactory<Formula::IntNode, FormulaMaker<&Stats::ModsSheet::skills, SkillMember<Stats::ModsSheet::_Skills>>::Frm>(SkillPointerFactory<Stats::ModsSheet::_Skills>{}, SkillNameFactory{});
		return ret;
	}
	const Skills<Formula::IntNode> &teamSkills() {
		static auto ret = skillFactory<Formula::IntNode, FormulaMaker<&Stats::ModsSheet::teamSkills, SkillMember<Stats::ModsSheet::_Skills>>::Frm>(SkillPointerFactory<Stats::ModsSheet::_Skills>{}, SkillNameFactory{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::enemy, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}, EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers::Engine::Passive