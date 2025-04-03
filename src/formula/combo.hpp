#pragma once

#include "character/instance.hpp"
#include "combo/option.hpp"
#include "fmt/core.h"
#include "formula/node.hpp"
#include "formulaContext.hpp"
#include "stats/team.hpp"
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

	template<class T>
	struct ComboOptionOverride {
		std::vector<::Combo::Option> overrides;
		T node;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Combo {}", eval(context));
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			if (!context.optionStore || overrides.empty()) return node.eval(context);
			uint32_t count = 0;
			for (const auto &override: overrides) {
				for (const auto &character: context.team.characters) {
					if (!character) continue;
					if (character->instanceKey != override.key) continue;
					if (auto it = character->loadout.options.find(override.hash); it != character->loadout.options.end()) {
						decltype(::Combo::Option::value) val;
						std::visit(
							Utils::overloaded{
								[&](Option::Boolean &opt) {
									val = opt.active;
									opt.active = std::get<bool>(override.value);
								},
								[&](Option::ValueList &opt) {
									// std::println("pushing {}, using {}", opt.currentIndex.value_or(255), std::get<std::optional<uint8_t>>(override.value).value_or(255));
									val = opt.currentIndex;
									opt.currentIndex = std::get<std::optional<uint8_t>>(override.value);
								},
							},
							it->second
						);
						context.optionStore->emplace_back(::Combo::Option{
							.key = override.key,
							.hash = override.hash,
							.value = val,
						});
						count++;
					}
				}
			}

			auto ret = node.eval(context);

			for (uint32_t i = 0; i < count; i++) {
				auto &override = context.optionStore->back();
				for (const auto &character: context.team.characters) {
					if (!character) continue;
					if (character->instanceKey != override.key) continue;
					std::visit(
						Utils::overloaded{
							[&](bool val) {
								// std::println("restoring {}", val);
								std::get<::Option::Boolean>(character->loadout.options.at(override.hash)).active = val;
							},
							[&](std::optional<uint8_t> val) {
								// std::println("restoring {}", val.value_or(255));
								std::get<::Option::ValueList>(character->loadout.options.at(override.hash)).currentIndex = val;
							},
						},
						override.value
					);
				}
				context.optionStore->pop_back();
			}

			return ret;
		}
	};
}// namespace Formula