#pragma once

#include "character/instance.hpp"
#include "compiled/constant.hpp"
#include "formulaContext.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	[[nodiscard]] constexpr auto getElement(Misc::AttackSource attackSource, Utils::JankyOptional<Misc::Element> element, const Formula::Context &context) {
		if (element.has_value()) return element.value();
		switch (attackSource) {
			case Misc::AttackSource::charged:
				if (context.source.character.base.weaponType == Misc::WeaponType::bow) return context.source.character.base.element;
			case Misc::AttackSource::normal:
			case Misc::AttackSource::plunge:
				if (context.source.character.base.weaponType == Misc::WeaponType::catalyst) return context.source.character.base.element;
				return context.source.character.sheet.infusion.eval(context).value_or(Misc::Element::physical);
			case Misc::AttackSource::skill:
			case Misc::AttackSource::burst:
				return context.source.character.base.element;
		}
		std::unreachable();
	}

	struct ElementCount {
		::Misc::Element element;

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantInt(eval(context));
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} count {}", Utils::Stringify(element), eval(context));
		}

		[[nodiscard]] uint32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character) continue;
				if (character->loadout.character.base.element == element) ret++;
			}
			return ret;
		}
	};

	struct ElementCountOthers {
		::Misc::Element element;

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantInt(eval(context));
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} count {}", Utils::Stringify(element), eval(context));
		}

		[[nodiscard]] uint32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character || &character->loadout == &context.source) continue;
				if (character->loadout.character.base.element == element) ret++;
			}
			return ret;
		}
	};


	struct SameElementCount {
		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantInt(eval(context));
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Team same element count {}", eval(context));
		}

		[[nodiscard]] uint32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character || &character->loadout == &context.source) continue;
				if (character->loadout.character.base.element == context.source.character.base.element) ret++;
			}

			return ret;
		}
	};

	struct OtherElementCount {
		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantInt(eval(context));
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Team other element count {}", eval(context));
		}

		[[nodiscard]] uint32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character || &character->loadout == &context.source) continue;
				if (character->loadout.character.base.element != context.source.character.base.element) ret++;
			}

			return ret;
		}
	};

	struct PHECCount {
		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantInt(eval(context));
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Team other element count {}", eval(context));
		}

		[[nodiscard]] uint32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &character: context.team.characters) {
				if (!character || &character->loadout == &context.source) continue;
				switch (character->loadout.character.base.element) {
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

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantBool(eval(context));
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character {} ({})", Utils::Stringify(element), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.character.base.element == element;
		}
	};

	struct IsActiveCharacterElement {
		Misc::Element element;

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantBool(eval(context));
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character {} ({})", Utils::Stringify(element), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.active.character.base.element == element;
		}
	};
}// namespace Formula