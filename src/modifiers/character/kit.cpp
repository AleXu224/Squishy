#include "kit.hpp"
#include "character/data.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/loadout.hpp"

namespace Modifiers::Character::Kit {
	template<auto Stats::ModsSheet::*location, class StatMember>
	struct FormulaMaker {
		struct Frm {
			StatMember stat;
			using Ret = RetType<typename StatMember::RetType>;

			[[nodiscard]] Formula::Compiled::NodeType<Ret> compile(const Formula::Context &context) const {
				const auto &mod = stat.resolve(std::invoke(location, context.source.character.data.data.mods));
				if (!mod.hasValue()) return Formula::Compiled::Constant<Ret>{};
				return mod.compile(context);
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

	const Stats::Sheet<Formula::FloatNode> &preMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::preMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &postMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::postMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamPreMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::teamPreMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &teamPostMods() {
		static auto ret = statFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::teamPostMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm>(StatPointerFactory<Stats::ModsSheet::_Sheet>{});
		return ret;
	}
	const Talents<Formula::IntNode> &talents() {
		static auto ret = talentFactory<Formula::IntNode, FormulaMaker<&Stats::ModsSheet::talents, TalentMember<Stats::ModsSheet::_Talents>>::Frm>(TalentPointerFactory<Stats::ModsSheet::_Talents>{});
		return ret;
	}
	const Talents<Formula::IntNode> &teamTalents() {
		static auto ret = talentFactory<Formula::IntNode, FormulaMaker<&Stats::ModsSheet::teamTalents, TalentMember<Stats::ModsSheet::_Talents>>::Frm>(TalentPointerFactory<Stats::ModsSheet::_Talents>{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, FormulaMaker<&Stats::ModsSheet::enemy, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm>(EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{});
		return ret;
	}
}// namespace Modifiers::Character::Kit