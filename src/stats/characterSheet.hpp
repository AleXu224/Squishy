#pragma once

#include "Talents.hpp"
#include "characterBase.hpp"
#include "functional"
#include "stat.hpp"
#include "value.hpp"

namespace Stats {
	class CharacterSheet;
	using CharacterValue = Value<CharacterSheet>;

	class CharacterSheet {
		CharacterValue hp{0};
		CharacterValue hp_{0};
		CharacterValue baseHp{0};
		CharacterValue atk{0};
		CharacterValue atk_{0};
		CharacterValue baseAtk{0};
		CharacterValue additionalAtk{0};
		CharacterValue def{0};
		CharacterValue def_{0};
		CharacterValue baseDef{0};
		CharacterValue er{1};// Er starts at 100%
		CharacterValue em{0};
		CharacterValue cr{0.05};// All characters start with 5% crit rate except for Kokomi
		CharacterValue cd{0.5};
		CharacterValue hb{0};
		CharacterValue pyroDmg{0};
		CharacterValue hydroDmg{0};
		CharacterValue cryoDmg{0};
		CharacterValue electroDmg{0};
		CharacterValue dendroDmg{0};
		CharacterValue anemoDmg{0};
		CharacterValue geoDmg{0};
		CharacterValue physicalDmg{0};
		CharacterValue allDmg{0};

	public:
		Talents talents;
		unsigned short constellation{};
		unsigned short level{1};
		unsigned short ascension{0};

		explicit CharacterSheet(const Stats::CharacterBase &base) : talents{1, 1, 1} {
			addModifier(Stat::baseHp, [&base](const Stats::CharacterSheet &sheet) {
				return base.getHpAt(sheet.level, sheet.ascension);
			});
			addModifier(Stat::baseAtk, [&base](const Stats::CharacterSheet &sheet) {
				return base.getAtkAt(sheet.level, sheet.ascension);
			});
			addModifier(Stat::baseDef, [&base](const Stats::CharacterSheet &sheet) {
				return base.getDefAt(sheet.level, sheet.ascension);
			});
			addModifier(Stat::hp, [](const Stats::CharacterSheet &sheet) {
				return (sheet.getStat(Stat::hp_) + 1.0f) * sheet.getStat(Stat::baseHp);
			});
			addModifier(Stat::atk, [](const Stats::CharacterSheet &sheet) {
				return (sheet.getStat(Stat::atk_) + 1.0f) * sheet.getStat(Stat::baseAtk);
			});
			addModifier(Stat::def, [](const Stats::CharacterSheet &sheet) {
				return (sheet.getStat(Stat::def_) + 1.0f) * sheet.getStat(Stat::baseDef);
			});
		}

		void addModifier(Stat stat, const std::function<float(const Stats::CharacterSheet &statSheet)> &func) {
			getStatVal(stat).modifiers.push_back(func);
		}

		void addArtifactModifier(Stat stat, const std::function<float(const Stats::CharacterSheet &statSheet)> &func) {
			auto &statVal = getStatVal(stat);
			statVal.artifactModifiers.at(statVal.artifactModifiersCount++) = func;
			//		getStatVal(stat).artifactModifiers.push_back(func);
		}

		float getStat(const Stat &stat) const {
			return getStatVal(stat).calculateValue(*this);
		}

		float getStatFromElement(const Element &element) {
			return getStatFromElementVal(element).calculateValue(*this);
		}

		void clearArtifactModifiers() {
			//		for (int i = 0; i < 24; i++) {
			////			auto &statsheetVal = getStatVal(static_cast<Stat>(i));
			////			if (!statsheetVal.artifactModifiers.empty()) statsheetVal.artifactModifiers.clear();
			//			getStatVal(static_cast<Stat>(i)).artifactModifiersCount = 0;
			//		}

			// VERY VERY UGLY but it's a huge speedup
			// from ~56 ns to just ~9 ns
			hp.artifactModifiersCount = 0;
			hp_.artifactModifiersCount = 0;
			baseHp.artifactModifiersCount = 0;
			atk.artifactModifiersCount = 0;
			atk_.artifactModifiersCount = 0;
			baseAtk.artifactModifiersCount = 0;
			additionalAtk.artifactModifiersCount = 0;
			def.artifactModifiersCount = 0;
			def_.artifactModifiersCount = 0;
			baseDef.artifactModifiersCount = 0;
			er.artifactModifiersCount = 0;
			em.artifactModifiersCount = 0;
			cr.artifactModifiersCount = 0;
			cd.artifactModifiersCount = 0;
			hb.artifactModifiersCount = 0;
			pyroDmg.artifactModifiersCount = 0;
			hydroDmg.artifactModifiersCount = 0;
			cryoDmg.artifactModifiersCount = 0;
			electroDmg.artifactModifiersCount = 0;
			dendroDmg.artifactModifiersCount = 0;
			anemoDmg.artifactModifiersCount = 0;
			geoDmg.artifactModifiersCount = 0;
			physicalDmg.artifactModifiersCount = 0;
			allDmg.artifactModifiersCount = 0;
		}

		void print() const {
			std::println("level: {}", level);
			std::println("ascension: {}", ascension);
			std::println("constellation: {}", constellation);
			std::println("normal lvl: {}", talents.normal);
			std::println("skill lvl: {}", talents.skill);
			std::println("burst lvl: {}", talents.burst);
			std::println("hp: {}", hp.calculateValue(*this));
			std::println("hp_: {}", hp_.calculateValue(*this));
			std::println("baseHp: {}", baseHp.calculateValue(*this));
			std::println("atk: {}", atk.calculateValue(*this));
			std::println("atk_: {}", atk_.calculateValue(*this));
			std::println("baseAtk: {}", baseAtk.calculateValue(*this));
			std::println("additionalAtk: {}", additionalAtk.calculateValue(*this));
			std::println("def: {}", def.calculateValue(*this));
			std::println("def_: {}", def_.calculateValue(*this));
			std::println("baseDef: {}", baseDef.calculateValue(*this));
			std::println("er: {}", er.calculateValue(*this));
			std::println("em: {}", em.calculateValue(*this));
			std::println("cr: {}", cr.calculateValue(*this));
			std::println("cd: {}", cd.calculateValue(*this));
			std::println("hb: {}", hb.calculateValue(*this));
			std::println("pyroDmg: {}", pyroDmg.calculateValue(*this));
			std::println("hydroDmg: {}", hydroDmg.calculateValue(*this));
			std::println("cryoDmg: {}", cryoDmg.calculateValue(*this));
			std::println("electroDmg: {}", electroDmg.calculateValue(*this));
			std::println("dendroDmg: {}", dendroDmg.calculateValue(*this));
			std::println("anemoDmg: {}", anemoDmg.calculateValue(*this));
			std::println("geoDmg: {}", geoDmg.calculateValue(*this));
			std::println("physicalDmg: {}", physicalDmg.calculateValue(*this));
			std::println("allDmg: {}", allDmg.calculateValue(*this));
		}

	private:
		CharacterValue &getStatFromElementVal(const Element &element) {
			switch (element) {
				case Element::pyro:
					return pyroDmg;
				case Element::hydro:
					return hydroDmg;
				case Element::cryo:
					return cryoDmg;
				case Element::electro:
					return electroDmg;
				case Element::dendro:
					return dendroDmg;
				case Element::anemo:
					return anemoDmg;
				case Element::geo:
					return geoDmg;
				case Element::physical:
					return physicalDmg;
			}
		}

		[[nodiscard]] CharacterValue &getStatVal(const Stat &stat) {
			switch (stat) {
				case Stat::hp:
					return hp;
				case Stat::hp_:
					return hp_;
				case Stat::baseHp:
					return baseHp;
				case Stat::atk:
					return atk;
				case Stat::atk_:
					return atk_;
				case Stat::baseAtk:
					return baseAtk;
				case Stat::additionalAtk:
					return additionalAtk;
				case Stat::def:
					return def;
				case Stat::def_:
					return def_;
				case Stat::baseDef:
					return baseDef;
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
					return pyroDmg;
				case Stat::hydroDmg:
					return hydroDmg;
				case Stat::cryoDmg:
					return cryoDmg;
				case Stat::electroDmg:
					return electroDmg;
				case Stat::dendroDmg:
					return dendroDmg;
				case Stat::anemoDmg:
					return anemoDmg;
				case Stat::geoDmg:
					return geoDmg;
				case Stat::physicalDmg:
					return physicalDmg;
				case Stat::allDmg:
					return allDmg;
			}
		}

		[[nodiscard]] const CharacterValue &getStatVal(const Stat &stat) const {
			switch (stat) {
				case Stat::hp:
					return hp;
				case Stat::hp_:
					return hp_;
				case Stat::baseHp:
					return baseHp;
				case Stat::atk:
					return atk;
				case Stat::atk_:
					return atk_;
				case Stat::baseAtk:
					return baseAtk;
				case Stat::additionalAtk:
					return additionalAtk;
				case Stat::def:
					return def;
				case Stat::def_:
					return def_;
				case Stat::baseDef:
					return baseDef;
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
					return pyroDmg;
				case Stat::hydroDmg:
					return hydroDmg;
				case Stat::cryoDmg:
					return cryoDmg;
				case Stat::electroDmg:
					return electroDmg;
				case Stat::dendroDmg:
					return dendroDmg;
				case Stat::anemoDmg:
					return anemoDmg;
				case Stat::geoDmg:
					return geoDmg;
				case Stat::physicalDmg:
					return physicalDmg;
				case Stat::allDmg:
					return allDmg;
			}
		}
	};
}// namespace Stats
