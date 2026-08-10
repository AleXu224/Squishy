#include "kit.hpp"
#include "character/data.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/loadout.hpp"

namespace Modifiers::Character::Kit {
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

			void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step prevStep) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.stats.data.data->mods));
				if (!mod.hasValue()) return;
				Formula::Descriptor formulaDescriptor;
				mod.print(formulaDescriptor, context, prevStep);
				if constexpr (std::is_same_v<Ret, float>) {
					descriptor.add(std::format("Character Kit {}", member.getName()), {eval(context), member.isPercentage()}, std::move(formulaDescriptor));
				} else {
					descriptor.add(std::format("Character Kit {}", member.getName()), eval(context), std::move(formulaDescriptor));
				}
			}

			[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.stats.data.data->mods));
				if (!mod.hasValue()) return {};
				return mod.eval(context);
			}
		};
	};

	const Stats::Sheet<Formula::FloatNode> &preMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::preMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &postMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::postMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamPreMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::teamPreMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamPostMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::teamPostMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activePreMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::activePreMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activePostMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::activePostMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{});
		return ret;
	}
	const Talents<Formula::IntNode> &talents() {
		static auto ret = talentFactory<Formula::IntNode, FormulaMaker<&Stats::ModsSheet::talents, TalentMember<Stats::ModsSheet::_Talents>>::Frm>(TalentPointerFactory<Stats::ModsSheet::_Talents>{}, TalentNameFactory{});
		return ret;
	}
	const Talents<Formula::IntNode> &teamTalents() {
		static auto ret = talentFactory<Formula::IntNode, FormulaMaker<&Stats::ModsSheet::teamTalents, TalentMember<Stats::ModsSheet::_Talents>>::Frm>(TalentPointerFactory<Stats::ModsSheet::_Talents>{}, TalentNameFactory{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::enemy, EnemyMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}, EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers::Character::Kit