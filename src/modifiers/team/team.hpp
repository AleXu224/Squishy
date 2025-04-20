#pragma once

#include "character/instance.hpp"
#include "formula/compiled/constantOrValue.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/artifact/set.hpp"
#include "modifiers/character/kit.hpp"
#include "modifiers/weapon/passive.hpp"
#include "stats/team.hpp"


namespace Modifiers::Team {
	using namespace Formula::Operators;
	template<auto characterStat, auto weaponStat, auto artifactStat>
	struct Frm {
		using Ret = RetType<characterStat>;
		using CompiledRet = decltype((characterStat + weaponStat + artifactStat).compile(std::declval<const Formula::Context &>()));

		[[nodiscard]] auto compile(const Formula::Context &context) const {
			Formula::Compiled::TeamConstantOr<Ret, CompiledRet> ret{};
			for (auto [val, character]: std::views::zip(ret.vals, context.team.characters)) {
				if (!character) continue; // Val is a constant of 0 by default, no need to do anything
				auto newContext = context.withSource(character->loadout);
				val = (characterStat + weaponStat + artifactStat).compile(newContext);
			}
			return ret;
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (characterStat + weaponStat + artifactStat).print(context, prevStep);
		}


		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			Ret total = 0;
			for (const auto &character: context.team.characters) {
				if (!character) continue;
				auto newContext = context.withSource(character->loadout);
				total += (characterStat + weaponStat + artifactStat).eval(newContext);
			}
			return total;
		}
	};

	static constexpr StatFactory<Frm, Character::Kit::teamPreMods, Weapon::Passive::teamPreMods, Artifact::Set::teamPreMods> preMods;
	static constexpr StatFactory<Frm, Character::Kit::teamPostMods, Weapon::Passive::teamPostMods, Artifact::Set::teamPostMods> postMods;
	static constexpr TalentFactory<Frm, Character::Kit::teamTalents, Weapon::Passive::teamTalents, Artifact::Set::teamTalents> talents;
	static constexpr EnemyFactory<Frm, Character::Kit::enemy, Weapon::Passive::enemy, Artifact::Set::enemy> enemy;
}// namespace Modifiers::Team