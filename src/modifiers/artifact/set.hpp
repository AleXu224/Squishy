#pragma once

#include "artifact/set.hpp"
#include "formula/compiled/constantOrValue.hpp"
#include "formula/formulaContext.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"


namespace Modifiers::Artifact::Set {
	using namespace Formula::Operators;
	template<auto sheet1, auto sheet2>
	struct SetFormula {
		[[nodiscard]] auto compile(const Formula::Context &context) const {
			return (sheet1 + sheet2).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (sheet1 + sheet2).print(context, prevStep);
		}

		using Ret = RetType<sheet1>;

		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			return (sheet1 + sheet2).eval(context);
		}
	};

	template<auto Stats::ModsSheet::*location, std::optional<Stats::ArtifactBonus> Stats::Artifact::*location2, class StatMember>
	struct SheetFormulaMaker {
		template<StatMember stat>
		struct Frm {
			using Ret = RetTypeMember<stat>;
			using CompiledRet = decltype(stat.resolve(std::invoke(location, std::invoke(location2, std::declval<const Formula::Context &>().source.artifact).value().bonusPtr.mods)).compile(std::declval<const Formula::Context &>()));

			[[nodiscard]] Formula::Compiled::ConstantOr<Ret, CompiledRet> compile(const Formula::Context &context) const {
				const auto &bonus = std::invoke(location2, context.source.artifact);
				if (!bonus) return {
					.val = Formula::Compiled::Constant<Ret>{0},
				};
				const auto &val = bonus.value();
				const auto &mod = stat.resolve(std::invoke(location, val.bonusPtr.mods));
				if (!mod.hasValue()) return {.val = Formula::Compiled::Constant<Ret>{0}};
				return {
					.val = mod.compile(context),
				};
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

	static constexpr StatFactory<SheetFormulaMaker<&Stats::ModsSheet::preMod, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> sheet1PreMods;
	static constexpr StatFactory<SheetFormulaMaker<&Stats::ModsSheet::postMod, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> sheet1PostMods;
	static constexpr StatFactory<SheetFormulaMaker<&Stats::ModsSheet::teamPreMod, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> sheet1TeamPreMods;
	static constexpr StatFactory<SheetFormulaMaker<&Stats::ModsSheet::teamPostMod, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> sheet1TeamPostMods;
	static constexpr TalentFactory<SheetFormulaMaker<&Stats::ModsSheet::talents, &Stats::Artifact::bonus1, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}> sheet1Talents;
	static constexpr TalentFactory<SheetFormulaMaker<&Stats::ModsSheet::teamTalents, &Stats::Artifact::bonus1, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}> sheet1TeamTalents;
	static constexpr EnemyFactory<SheetFormulaMaker<&Stats::ModsSheet::enemy, &Stats::Artifact::bonus1, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm, EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}> sheet1Enemy;

	static constexpr StatFactory<SheetFormulaMaker<&Stats::ModsSheet::preMod, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> sheet2PreMods;
	static constexpr StatFactory<SheetFormulaMaker<&Stats::ModsSheet::postMod, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> sheet2PostMods;
	static constexpr StatFactory<SheetFormulaMaker<&Stats::ModsSheet::teamPreMod, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> sheet2TeamPreMods;
	static constexpr StatFactory<SheetFormulaMaker<&Stats::ModsSheet::teamPostMod, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> sheet2TeamPostMods;
	static constexpr TalentFactory<SheetFormulaMaker<&Stats::ModsSheet::talents, &Stats::Artifact::bonus2, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}> sheet2Talents;
	static constexpr TalentFactory<SheetFormulaMaker<&Stats::ModsSheet::teamTalents, &Stats::Artifact::bonus2, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}> sheet2TeamTalents;
	static constexpr EnemyFactory<SheetFormulaMaker<&Stats::ModsSheet::enemy, &Stats::Artifact::bonus2, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm, EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}> sheet2Enemy;

	static constexpr StatFactory<SetFormula, sheet1PreMods, sheet2PreMods> preMods;
	static constexpr StatFactory<SetFormula, sheet1PostMods, sheet2PostMods> postMods;
	static constexpr StatFactory<SetFormula, sheet1TeamPreMods, sheet2TeamPreMods> teamPreMods;
	static constexpr StatFactory<SetFormula, sheet1TeamPostMods, sheet2TeamPostMods> teamPostMods;
	static constexpr TalentFactory<SetFormula, sheet1Talents, sheet2Talents> talents;
	static constexpr TalentFactory<SetFormula, sheet1TeamTalents, sheet2TeamTalents> teamTalents;
	static constexpr EnemyFactory<SetFormula, sheet1Enemy, sheet2Enemy> enemy;
}// namespace Modifiers::Artifact::Set