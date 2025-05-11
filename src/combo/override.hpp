#pragma once
#include "character/instance.hpp"
#include "formula/formulaContext.hpp"
#include "option.hpp"
#include "stats/team.hpp"


namespace Combo {
	[[nodiscard]] inline uint32_t pushOverrides(const Formula::Context &context, const std::vector<::Combo::Option> &overrides) {
		uint32_t count = 0;
		for (const auto &override: overrides) {
			for (const auto &character: context.team.characters) {
				if (!character) continue;
				if (character->instanceKey != override.key) continue;
				if (auto it = character->state.options.find(override.hash); it != character->state.options.end()) {
					decltype(::Combo::Option::value) val;
					std::visit(
						Utils::overloaded{
							[&](::Option::Boolean &opt) {
								val = opt.active;
								opt.active = std::get<bool>(override.value);
							},
							[&](::Option::ValueList &opt) {
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
		return count;
	}

	inline void popOverrides(const Formula::Context &context, uint32_t count) {
		for (uint32_t i = 0; i < count; i++) {
			auto &override = context.optionStore->back();
			for (const auto &character: context.team.characters) {
				if (!character) continue;
				if (character->instanceKey != override.key) continue;
				std::visit(
					Utils::overloaded{
						[&](bool val) {
							// std::println("restoring {}", val);
							std::get<::Option::Boolean>(character->state.options.at(override.hash)).active = val;
						},
						[&](std::optional<uint8_t> val) {
							// std::println("restoring {}", val.value_or(255));
							std::get<::Option::ValueList>(character->state.options.at(override.hash)).currentIndex = val;
						},
					},
					override.value
				);
			}
			context.optionStore->pop_back();
		}
	}
}// namespace Combo