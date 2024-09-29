#pragma once

#include "formulaContext.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	[[nodiscard]] constexpr auto getElement(Misc::AttackSource attackSource, Utils::JankyOptional<Misc::Element> element, const Formula::Context &context) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
			case Misc::AttackSource::charged:
			case Misc::AttackSource::plunge:
				return element.value_or(context.target.character.sheet.infusion.eval(context).value_or(Misc::Element::physical));
			case Misc::AttackSource::skill:
			case Misc::AttackSource::burst:
				return element.value_or(context.target.character.base.element);
		}
		std::unreachable();
	}
}// namespace Formula