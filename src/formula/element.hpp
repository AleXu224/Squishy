#pragma once

#include "character/instance.hpp"
#include "compiled/constant.hpp"
#include "formulaContext.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	[[nodiscard]] constexpr auto getElement(const Utils::JankyOptional<Misc::AttackSource> &attackSource, Utils::JankyOptional<Misc::Element> element, const Formula::Context &context) {
		if (element.has_value()) return element.value();
		if (!attackSource.has_value()) return element.value_or(Misc::Element::physical);
		switch (attackSource.value()) {
			case Misc::AttackSource::charged:
				if (context.source.stats.base.weaponType == Misc::WeaponType::bow) return context.source.stats.base.element;
				[[fallthrough]];
			case Misc::AttackSource::normal:
			case Misc::AttackSource::plunge:
				if (context.source.stats.base.weaponType == Misc::WeaponType::catalyst) return context.source.stats.base.element;
				return context.source.stats.sheet.infusion.eval(context).value_or(Misc::Element::physical);
			case Misc::AttackSource::skill:
			case Misc::AttackSource::burst:
				return context.source.stats.base.element;
		}
		std::unreachable();
	}

	struct ElementCount {
		::Misc::Element element;

		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} count {}", Utils::Stringify(element), eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character) continue;
				if (character->state.stats.base.element == element) ret++;
			}
			return ret;
		}
	};

	struct ElementCountOthers {
		::Misc::Element element;

		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} count {}", Utils::Stringify(element), eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character || &character->state == &context.source) continue;
				if (character->state.stats.base.element == element) ret++;
			}
			return ret;
		}
	};


	struct SameElementCount {
		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Team same element count {}", eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character || &character->state == &context.source) continue;
				if (character->state.stats.base.element == context.source.stats.base.element) ret++;
			}

			return ret;
		}
	};

	struct OtherElementCount {
		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Team other element count {}", eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character || &character->state == &context.source) continue;
				if (character->state.stats.base.element != context.source.stats.base.element) ret++;
			}

			return ret;
		}
	};

	struct PHECCount {
		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Team other element count {}", eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character || &character->state == &context.source) continue;
				switch (character->state.stats.base.element) {
					case Misc::Element::pyro:
					case Misc::Element::hydro:
					case Misc::Element::electro:
					case Misc::Element::cryo:
						ret++;
					default:
						continue;
				}
			}

			return ret;
		}
	};

	struct IsCharacterElement {
		Misc::Element element;

		[[nodiscard]] inline Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character {} ({})", Utils::Stringify(element), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.stats.base.element == element;
		}
	};

	struct IsActiveCharacterElement {
		Misc::Element element;

		[[nodiscard]] inline Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character {} ({})", Utils::Stringify(element), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.active.stats.base.element == element;
		}
	};
}// namespace Formula