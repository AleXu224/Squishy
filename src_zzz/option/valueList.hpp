#pragma once

#include "formula/node.hpp"
#include "node/node.hpp"
#include "stats/sheet.hpp"
#include "string_view"
#include "utils/hashedString.hpp"
#include <optional>
#include <variant>
#include <vector>


namespace Option {
	struct ValueList {
		Utils::HashedString key;
		std::string_view prefix;
		bool teamBuff = false;
		std::optional<size_t> currentIndex = std::nullopt;
		Formula::BoolNode displayCondition{};
		std::vector<std::variant<std::string, Formula::NodeType<std::string>>> values;
		Stats::ModsSheet mods{};
		std::vector<Node::Types> nodes{};

		[[nodiscard]] std::optional<std::string> getValue(const Formula::Context &ctx, std::optional<size_t> index = std::nullopt) const {
			std::optional<std::variant<std::string, Formula::NodeType<std::string>>> value;
			auto overloadedIndex = index ? index : currentIndex;
			if (overloadedIndex.has_value() && overloadedIndex.value() < values.size()) {
				value = values.at(overloadedIndex.value());
			}

			std::optional<std::string> ret;

			if (value.has_value()) {
				std::visit(
					Utils::overloaded{
						[&](const std::string &val) {
							ret = val;
						},
						[&](const Formula::NodeType<std::string> &val) {
							ret = val.eval(ctx);
						},
					},
					value.value()
				);
			}

			return ret;
		}
	};
}// namespace Option
