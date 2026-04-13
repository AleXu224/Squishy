#include "team.hpp"
#include "character/instance.hpp"
#include "formula/teamCharacter.hpp"
#include "modifiers/artifact/set.hpp"
#include "modifiers/character/kit.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "modifiers/weapon/passive.hpp"
#include "stats/team.hpp"


namespace Modifiers::Team {
	using namespace Formula::Operators;
	template<class T>
	struct Frm : Formula::FormulaBase<RetType<T>> {
		T characterStat;
		T weaponStat;
		T artifactStat;
		using Ret = RetType<T>;
		[[nodiscard]] Formula::NodeType<Ret> fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto formula = characterStat + weaponStat + artifactStat;
			Formula::NodeType<Ret> ret = Formula::TeamCharacter{.index = 0, .formula = formula}
									   + Formula::TeamCharacter{.index = 1, .formula = formula}
									   + Formula::TeamCharacter{.index = 2, .formula = formula}
									   + Formula::TeamCharacter{.index = 3, .formula = formula};

			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			auto combined = characterStat + weaponStat + artifactStat;
			return (Formula::TeamCharacter{.index = 0, .formula = combined}
					+ Formula::TeamCharacter{.index = 1, .formula = combined}
					+ Formula::TeamCharacter{.index = 2, .formula = combined}
					+ Formula::TeamCharacter{.index = 3, .formula = combined})
				.print(context, prevStep);
		}


		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			Ret total = 0;
			for (const auto &character: context.team.characters) {
				if (!character) continue;
				auto newContext = context.withSource(character->state);
				total += characterStat.eval(newContext)
					   + weaponStat.eval(newContext)
					   + artifactStat.eval(newContext);
			}
			return total;
		}
	};

	struct FrmActive : Formula::FormulaBase<float> {
		Formula::FloatNode characterStat;
		Formula::FloatNode weaponStat;
		Formula::FloatNode artifactStat;

		using Ret = float;
		[[nodiscard]] Formula::NodeType<Ret> fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			Formula::NodeType<Ret> ret = Formula::ConstantBase<Ret>{};
			auto activeCharacter = context.team.characters.at(context.team.activeCharacterIndex);
			if (!activeCharacter || activeCharacter->instanceKey != context.source.instanceKey) return ret;

			auto formula = characterStat + weaponStat + artifactStat;
			ret = Formula::TeamCharacter{.index = 0, .formula = formula}
				+ Formula::TeamCharacter{.index = 1, .formula = formula}
				+ Formula::TeamCharacter{.index = 2, .formula = formula}
				+ Formula::TeamCharacter{.index = 3, .formula = formula};
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			auto combined = characterStat + weaponStat + artifactStat;
			return Formula::ActiveCharacterFilter{
				.formula = Formula::TeamCharacter{.index = 0, .formula = combined}
						 + Formula::TeamCharacter{.index = 1, .formula = combined}
						 + Formula::TeamCharacter{.index = 2, .formula = combined}
						 + Formula::TeamCharacter{.index = 3, .formula = combined},
			}
				.print(context, prevStep);
		}


		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			Ret total = 0;
			auto activeCharacter = context.team.characters.at(context.team.activeCharacterIndex);
			if (!activeCharacter || activeCharacter->instanceKey != context.source.instanceKey) return 0;

			for (const auto &character: context.team.characters) {
				if (!character) continue;
				auto newContext = context.withSource(character->state);
				total += characterStat.eval(newContext)
					   + weaponStat.eval(newContext)
					   + artifactStat.eval(newContext);
			}
			return total;
		}
	};

	const Stats::Sheet<Formula::FloatNode> &preMods() {
		static auto ret = statFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Character::Kit::teamPreMods(), Weapon::Passive::teamPreMods(), Artifact::Set::teamPreMods());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &postMods() {
		static auto ret = statFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Character::Kit::teamPostMods(), Weapon::Passive::teamPostMods(), Artifact::Set::teamPostMods());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activePreMods() {
		static auto ret = statFactory<Formula::FloatNode, FrmActive>(Character::Kit::activePreMods(), Weapon::Passive::activePreMods(), Artifact::Set::activePreMods());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activePostMods() {
		static auto ret = statFactory<Formula::FloatNode, FrmActive>(Character::Kit::activePostMods(), Weapon::Passive::activePostMods(), Artifact::Set::activePostMods());
		return ret;
	}
	const Talents<Formula::IntNode> &talents() {
		static auto ret = talentFactory<Formula::IntNode, Frm<Formula::IntNode>>(Character::Kit::teamTalents(), Weapon::Passive::teamTalents(), Artifact::Set::teamTalents());
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Character::Kit::enemy(), Weapon::Passive::enemy(), Artifact::Set::enemy());
		return ret;
	}
}// namespace Modifiers::Team