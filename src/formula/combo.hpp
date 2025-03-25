#pragma once

#include "fmt/core.h"
#include "formula/node.hpp"
#include "formulaContext.hpp"
#include <vector>

namespace Formula {
	struct Combo {
		struct Entry {
			float multiplier;
			std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> reaction;
			FloatNode node;
		};
		std::vector<Entry> nodes;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Combo {}", eval(context));
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			float total = 0;
			for (const auto &node: nodes) {
				total += (node.multiplier * node.node.eval(context.withReaction(node.reaction)));
			}

			return total;
		}
	};
}// namespace Formula