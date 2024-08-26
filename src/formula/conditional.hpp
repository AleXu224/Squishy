#pragma once

#include "conditional/conditional.hpp"
#include "fmt/core.h"
#include "intermediary.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	[[nodiscard]] inline const Conditional::TypesMap &_getConditionalLocation(Conditional::Location location, const Stats::Loadout &source) {
		switch (location) {
			case Conditional::Location::normal:
				return source.character.conditionals.normal;
			case Conditional::Location::charged:
				return source.character.conditionals.charged;
			case Conditional::Location::plunge:
				return source.character.conditionals.plunge;
			case Conditional::Location::skill:
				return source.character.conditionals.skill;
			case Conditional::Location::burst:
				return source.character.conditionals.burst;
			case Conditional::Location::passive1:
				return source.character.conditionals.passive1;
			case Conditional::Location::passive2:
				return source.character.conditionals.passive2;
			case Conditional::Location::constellation1:
				return source.character.conditionals.constellation1;
			case Conditional::Location::constellation2:
				return source.character.conditionals.constellation2;
			case Conditional::Location::constellation4:
				return source.character.conditionals.constellation4;
			case Conditional::Location::constellation6:
				return source.character.conditionals.constellation6;
			case Conditional::Location::weapon:
				return source.weapon.conditionals;
			case Conditional::Location::artifact:
				return source.artifact.conditionals;
		}
		std::unreachable();
	}

	template<FloatFormula T>
	struct Conditional {
		::Conditional::Location location{};
		Utils::HashedString name;
		T val;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			auto cond = ::Conditional::getBool(_getConditionalLocation(location, context.source), name);
			return fmt::format("{}", cond ? fmt::format("{} {}", name.str, val.print(context, Step::none)) : "");
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			auto cond = ::Conditional::getBool(_getConditionalLocation(location, context.source), name);
			if (cond)
				return val.eval(context);

			return 0.f;
		}
	};
	struct ConditionalBool {
		::Conditional::Location location{};
		Utils::HashedString name;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			auto cond = eval(context);
			return fmt::format("{}", cond);
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return ::Conditional::getBool(_getConditionalLocation(location, context.source), name);
		}
	};
	struct ConditionalValue {
		::Conditional::Location location{};
		Utils::HashedString name;
		float defaultValue = 0.f;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			auto cond = ::Conditional::getFloat(_getConditionalLocation(location, context.source), name, defaultValue);
			return fmt::format("{} {}", name.str, cond);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			auto cond = ::Conditional::getFloat(_getConditionalLocation(location, context.source), name, defaultValue);
			return cond;
		}
	};
}// namespace Formula