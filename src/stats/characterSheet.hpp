#pragma once

#include "Talents.hpp"
#include "misc/attackSource.hpp"
#include "stat.hpp"
#include "value.hpp"


namespace Stats {
	struct Sheet;
	using CharacterValue = Stats::Value<Stats::Sheet>;

	struct Skill {
		CharacterValue DMG{};
		CharacterValue additiveDMG{};
		CharacterValue multiplicativeDMG{};
		CharacterValue critRate{};
		CharacterValue critDMG{};
	};
	struct CharacterSheet {
		CharacterValue hp{};
		CharacterValue hp_{};
		CharacterValue atk{};
		CharacterValue atk_{};
		CharacterValue additionalAtk{};
		CharacterValue def{};
		CharacterValue def_{};
		// CharacterValue er{1};// Er starts at 100%
		CharacterValue er{.value = 1.f};
		CharacterValue em{};
		// CharacterValue cr{0.05};// All characters start with 5% crit rate except for Kokomi
		CharacterValue cr{.value = 0.05f};
		// CharacterValue cd{0.5};
		CharacterValue cd{.value = 0.5f};
		CharacterValue hb{};
		Skill pyro{};
		Skill hydro{};
		Skill cryo{};
		Skill electro{};
		Skill dendro{};
		Skill anemo{};
		Skill geo{};
		Skill physical{};
		Skill all{};

		Skill normal{};
		Skill charged{};
		Skill plunge{};
		Skill skill{};
		Skill burst{};

		Talents talents{};
		unsigned short constellation{};
		unsigned short level{1};
		unsigned short ascension{0};

		void init(Stats::Sheet &sheet);

		// static inline void print(Stats::Sheet &sheet) {
		// 	std::println("level: {}", level);
		// 	std::println("ascension: {}", ascension);
		// 	std::println("constellation: {}", constellation);
		// 	std::println("normal lvl: {}", talents.normal);
		// 	std::println("skill lvl: {}", talents.skill);
		// 	std::println("burst lvl: {}", talents.burst);
		// 	std::println("hp: {}", hp.getTotal(*this));
		// 	std::println("hp_: {}", hp_.getTotal(*this));
		// 	std::println("baseHp: {}", baseHp.getTotal(*this));
		// 	std::println("atk: {}", atk.getTotal(*this));
		// 	std::println("atk_: {}", atk_.getTotal(*this));
		// 	std::println("baseAtk: {}", baseAtk.getTotal(*this));
		// 	std::println("additionalAtk: {}", additionalAtk.getTotal(*this));
		// 	std::println("def: {}", def.getTotal(*this));
		// 	std::println("def_: {}", def_.getTotal(*this));
		// 	std::println("baseDef: {}", baseDef.getTotal(*this));
		// 	std::println("er: {}", er.getTotal(*this));
		// 	std::println("em: {}", em.getTotal(*this));
		// 	std::println("cr: {}", cr.getTotal(*this));
		// 	std::println("cd: {}", cd.getTotal(*this));
		// 	std::println("hb: {}", hb.getTotal(*this));
		// 	std::println("pyroDmg: {}", pyro.DMG.getTotal(*this));
		// 	std::println("hydroDmg: {}", hydro.DMG.getTotal(*this));
		// 	std::println("cryoDmg: {}", cryo.DMG.getTotal(*this));
		// 	std::println("electroDmg: {}", electro.DMG.getTotal(*this));
		// 	std::println("dendroDmg: {}", dendro.DMG.getTotal(*this));
		// 	std::println("anemoDmg: {}", anemo.DMG.getTotal(*this));
		// 	std::println("geoDmg: {}", geo.DMG.getTotal(*this));
		// 	std::println("physicalDmg: {}", physical.DMG.getTotal(*this));
		// 	std::println("allDmg: {}", all.DMG.getTotal(*this));
		// }

		[[nodiscard]] Skill &fromElement(const Misc::Element &element) {
			switch (element) {
				case Misc::Element::pyro:
					return pyro;
				case Misc::Element::hydro:
					return hydro;
				case Misc::Element::cryo:
					return cryo;
				case Misc::Element::electro:
					return electro;
				case Misc::Element::dendro:
					return dendro;
				case Misc::Element::anemo:
					return anemo;
				case Misc::Element::geo:
					return geo;
				case Misc::Element::physical:
					return physical;
			}
		}

		[[nodiscard]] CharacterValue &fromStat(const Stat &stat) {
			switch (stat) {
				case Stat::hp:
					return hp;
				case Stat::hp_:
					return hp_;
				case Stat::atk:
					return atk;
				case Stat::atk_:
					return atk_;
				case Stat::additionalAtk:
					return additionalAtk;
				case Stat::def:
					return def;
				case Stat::def_:
					return def_;
				case Stat::er:
					return er;
				case Stat::em:
					return em;
				case Stat::cr:
					return cr;
				case Stat::cd:
					return cd;
				case Stat::hb:
					return hb;
				case Stat::pyroDmg:
					return pyro.DMG;
				case Stat::hydroDmg:
					return hydro.DMG;
				case Stat::cryoDmg:
					return cryo.DMG;
				case Stat::electroDmg:
					return electro.DMG;
				case Stat::dendroDmg:
					return dendro.DMG;
				case Stat::anemoDmg:
					return anemo.DMG;
				case Stat::geoDmg:
					return geo.DMG;
				case Stat::physicalDmg:
					return physical.DMG;
				case Stat::allDmg:
					return all.DMG;
			}
		}

		[[nodiscard]] Skill fromAttackSource(const Misc::AttackSource &attackSource) {
			switch (attackSource) {
				case Misc::AttackSource::normal:
					return normal;
				case Misc::AttackSource::charged:
					return charged;
				case Misc::AttackSource::plunge:
					return plunge;
				case Misc::AttackSource::skill:
					return skill;
				case Misc::AttackSource::burst:
					return burst;
			}
		}
	};
}// namespace Stats
