#pragma once

#include "character/data.hpp"
#include "formula/compiled/constantOrValue.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"


namespace Modifiers::Character::Kit {
	template<auto Stats::ModsSheet::*location, class StatMember>
	struct FormulaMaker {
		template<StatMember stat>
		struct Frm {
			using Ret = RetTypeMember<stat>;

			using CompileRet = Formula::Compiled::ConstantOr<Ret, Formula::Compiled::NodeType<Ret>>;
			[[nodiscard]] CompileRet compile(const Formula::Context &context) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.character.data.data.mods));
				if (!mod.hasValue()) return CompileRet(Formula::Compiled::Constant<Ret>{});
				return CompileRet(mod.compile(context));
			}

			[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.character.data.data.mods));
				if (!mod.hasValue()) return "";
				return mod.print(context);
			}

			[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.character.data.data.mods));
				if (!mod.hasValue()) return 0.f;
				return mod.eval(context);
			}
		};
	};

	static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::preMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> preMods;
	static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::postMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> postMods;
	static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::teamPreMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> teamPreMods;
	static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::teamPostMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}> teamPostMods;
	static constexpr TalentFactory<FormulaMaker<&Stats::ModsSheet::talents, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}> talents;
	static constexpr TalentFactory<FormulaMaker<&Stats::ModsSheet::teamTalents, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}> teamTalents;
	static constexpr EnemyFactory<FormulaMaker<&Stats::ModsSheet::enemy, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm, EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}> enemy;
}// namespace Modifiers::Character::Kit