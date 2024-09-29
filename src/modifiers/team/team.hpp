#pragma once

#include "character/instance.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/artifact/set.hpp"
#include "modifiers/character/kit.hpp"
#include "modifiers/weapon/passive.hpp"
#include "stats/team.hpp"


namespace Modifiers::Team {
	using namespace Formula::Operators;
	template<auto characterStat, auto weaponStat, auto artifactStat>
	struct Frm {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (characterStat + weaponStat + artifactStat).print(context, prevStep);
		}

		using Ret = RetType<characterStat>;

		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			Ret total = 0;
			for (const auto &character: context.team.characters) {
				total += character.transform([&](const std::reference_wrapper<::Character::Instance> &character) {
									  auto newContext = context.withSource(character.get().loadout);
									  return (characterStat + weaponStat + artifactStat).eval(newContext);
								  })
							 .value_or(Ret{});
			}
			return total;
		}
	};

	static constexpr StatFactory<Frm, Character::Kit::teamPreMods, Weapon::Passive::teamPreMods, Artifact::Set::teamPreMods> preMods;
	static constexpr StatFactory<Frm, Character::Kit::teamPostMods, Weapon::Passive::teamPostMods, Artifact::Set::teamPostMods> postMods;
	static constexpr TalentFactory<Frm, Character::Kit::teamTalents, Weapon::Passive::teamTalents, Artifact::Set::teamTalents> talents;
	static constexpr EnemyFactory<Frm, Character::Kit::enemy, Weapon::Passive::enemy, Artifact::Set::enemy> enemy;
}// namespace Modifiers::Team