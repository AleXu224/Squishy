#pragma once

#include "formula/formulaContext.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"
#include "weapon/data.hpp"


namespace Modifiers::Weapon::Passive {
	template<auto Stats::ModsSheet::*location, class StatMember>
	struct FormulaMaker {
		template<StatMember stat, SheetMemberIdentifier member>
		struct Frm {
			[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
				const auto &mod = stat.resolve(std::invoke(location, context.target.weapon.data.data.mods));
				if (!mod.hasValue()) return "";
				return mod.print(context);
			}

			using Ret = RetTypeMember<stat>;

			[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
				const auto &mod = stat.resolve(std::invoke(location, context.target.weapon.data.data.mods));
				if (!mod.hasValue()) return Ret{};
				return mod.eval(context);
			}
		};
	};

	static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::preMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> preMods;
	static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::postMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> postMods;
	static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::teamPreMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> teamPreMods;
	static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::teamPostMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> teamPostMods;
	static constexpr TalentFactory<FormulaMaker<&Stats::ModsSheet::talents, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}, TalentNameFactory{}> talents;
	static constexpr TalentFactory<FormulaMaker<&Stats::ModsSheet::teamTalents, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}, TalentNameFactory{}> teamTalents;
	static constexpr EnemyFactory<FormulaMaker<&Stats::ModsSheet::enemy, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm, EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}, EnemyNameFactory{}> enemy;
}// namespace Modifiers::Weapon::Passive