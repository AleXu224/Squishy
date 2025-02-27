#pragma once

#include "reaction.hpp"

namespace Reaction::List {
	static inline std::vector<const Reaction::Additive *> additiveList{
		&Reaction::List::Additive::aggravate,
		&Reaction::List::Additive::spread,
	};

	static inline std::vector<const Reaction::Amplifying *> amplifyingList{
		&Reaction::List::Amplifying::forwardVape,
		&Reaction::List::Amplifying::reverseVape,
		&Reaction::List::Amplifying::forwardMelt,
		&Reaction::List::Amplifying::reverseMelt,
	};

	[[nodiscard]] std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> fromNodeReaction(Misc::NodeReaction reaction) {
		switch (reaction) {
			case Misc::NodeReaction::none:
				return &Reaction::List::none;
			case Misc::NodeReaction::forwardVape:
				return &Reaction::List::Amplifying::forwardVape;
			case Misc::NodeReaction::forwardMelt:
				return &Reaction::List::Amplifying::forwardMelt;
			case Misc::NodeReaction::reverseVape:
				return &Reaction::List::Amplifying::reverseVape;
			case Misc::NodeReaction::reverseMelt:
				return &Reaction::List::Amplifying::reverseMelt;
			case Misc::NodeReaction::aggravate:
				return &Reaction::List::Additive::aggravate;
			case Misc::NodeReaction::spread:
				return &Reaction::List::Additive::spread;
		}
	}
}// namespace Reaction::List