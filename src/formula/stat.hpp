#pragma once

#include "formulaContext.hpp"
#include "modifiers/total/total.hpp"
#include "stats/stat.hpp"
#include "step.hpp"
#include "utility"

namespace Formula {
	[[nodiscard]] constexpr auto EvalStat(auto &&sheet, ::Stat stat, auto &&evalFunc) {
		switch (stat) {
			case ::Stat::hp:
				return evalFunc(sheet.hp);
			case ::Stat::hp_:
				return evalFunc(sheet.hp_);
			case ::Stat::baseHp:
				return evalFunc(sheet.baseHp);
			case ::Stat::atk:
				return evalFunc(sheet.atk);
			case ::Stat::atk_:
				return evalFunc(sheet.atk_);
			case ::Stat::baseAtk:
				return evalFunc(sheet.baseAtk);
			case ::Stat::additionalAtk:
				return evalFunc(sheet.additionalAtk);
			case ::Stat::def:
				return evalFunc(sheet.def);
			case ::Stat::def_:
				return evalFunc(sheet.def_);
			case ::Stat::baseDef:
				return evalFunc(sheet.baseDef);
			case ::Stat::er:
				return evalFunc(sheet.er);
			case ::Stat::em:
				return evalFunc(sheet.em);
			case ::Stat::cr:
				return evalFunc(sheet.cr);
			case ::Stat::cd:
				return evalFunc(sheet.cd);
			case ::Stat::hb:
				return evalFunc(sheet.hb);
			case ::Stat::incHb:
				return evalFunc(sheet.incHb);
			case ::Stat::shield_:
				return evalFunc(sheet.shield_);
			case ::Stat::pyroDmg:
				return evalFunc(sheet.pyro.DMG);
			case ::Stat::hydroDmg:
				return evalFunc(sheet.hydro.DMG);
			case ::Stat::cryoDmg:
				return evalFunc(sheet.cryo.DMG);
			case ::Stat::electroDmg:
				return evalFunc(sheet.electro.DMG);
			case ::Stat::dendroDmg:
				return evalFunc(sheet.dendro.DMG);
			case ::Stat::anemoDmg:
				return evalFunc(sheet.anemo.DMG);
			case ::Stat::geoDmg:
				return evalFunc(sheet.geo.DMG);
			case ::Stat::physicalDmg:
				return evalFunc(sheet.physical.DMG);
			case ::Stat::allDmg:
				return evalFunc(sheet.all.DMG);
		}
		std::unreachable();
	}
	struct Stat {
		::Stat stat;


		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			return EvalStat(Modifiers::total, stat, [&](auto &&stat) {
				return stat.print(context, prevStep);
			});
		}

		[[nodiscard]] float eval(const Context &context) const {
			return EvalStat(Modifiers::total, stat, [&](auto &&stat) {
				return stat.eval(context);
			});
		}
	};
}// namespace Formula