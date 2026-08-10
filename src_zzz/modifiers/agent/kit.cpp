#include "kit.hpp"
#include "agent/data.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/skillFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"


namespace Modifiers::Agent::Kit {
	template<auto Stats::ModsSheet::*location, class StatMember>
	struct FormulaMaker {
		struct Frm : Formula::FormulaBase<RetType<typename StatMember::RetType>> {
			StatMember stat;
			SheetMemberIdentifier member;
			using Ret = RetType<typename StatMember::RetType>;

			[[nodiscard]] Formula::NodeType<Ret> fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.stats.data.data->mods));
				if (!mod.hasValue()) return Formula::ConstantBase<Ret>{.value = {}};
				return mod.fold(context, args);
			}

			void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.stats.data.data->mods));
				if (!mod.hasValue()) return;
				// mod.print(descriptor, context);
				Formula::Descriptor formulaDescriptor;
				mod.print(formulaDescriptor, context, Formula::Step::none);
				if constexpr (std::is_same_v<Ret, float>) {
					descriptor.add(std::format("Agent Kit {}", member.getName()), {eval(context), member.isPercentage()}, std::move(formulaDescriptor));
				} else {
					descriptor.add(std::format("Agent Kit {}", member.getName()), eval(context), std::move(formulaDescriptor));
				}
			}

			[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.stats.data.data->mods));
				if (!mod.hasValue()) return {};
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
		static auto ret = enemyFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::enemy, EnemyMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}, EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers::Agent::Kit