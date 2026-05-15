#pragma once

#include "agent/data.hpp"
#include "agent/instance.hpp"
#include "node/abloomNode.hpp"
#include "node/node.hpp"
#include "stats/team.hpp"

namespace Formula {
	struct AbloomName : FormulaBase<std::string, Type::constant> {
		::Misc::Attribute attribute;
		size_t index;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return eval(context);
		}

		[[nodiscard]] std::string eval(const Context &context) const {
			auto &character = context.team.agents.at(index);
			if (!character) return "None";
			return std::format("{} Abloom", character->state.stats.data.name);
		}
	};

	inline void addAblooms(std::array<float, 5> multipliers, std::vector<Node::Types> &nodes) {
		for (const auto &[multiplier, attribute]: std::views::zip(multipliers, Misc::attributes)) {
			for (size_t i = 0; i < 3; i++) {
				nodes.emplace_back(Node::AbloomNode{
					.name = AbloomName{
						.attribute = attribute,
						.index = i,
					},
					.attribute = attribute,
					.multiplier = multiplier,
					.index = i,
				});
			}
		}
	}
}// namespace Formula