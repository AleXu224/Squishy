#pragma once

#include "fmt/core.h"
#include "option/option.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Formula {
	struct impl_IsActive {
		Utils::HashedString name;

		[[nodiscard]] Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", name.str);
		}

		[[nodiscard]] bool eval(const Context &context) const {
			if (context.overrides != nullptr) {
				if (auto opt = context.overrides->getOption(Utils::hashCombine(context.source.instanceKey, name.hash)); opt.has_value()) {
					return std::visit(
						Utils::overloaded{
							[](const bool &active) {
								return active;
							},
							[](const std::optional<uint8_t> &currentIndex) {
								return currentIndex.has_value();
							},
						},
						opt->get().value
					);
				}
			}

			return ::Option::getBool(context.source.options, name);
		}
	};

	struct impl_IsActivePassive {
		Utils::HashedString name;

		[[nodiscard]] Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", name.str);
		}

		[[nodiscard]] bool eval(const Context &context) const {
			if (context.overrides != nullptr) {
				if (auto opt = context.overrides->getOption(Utils::hashCombine(0, name.hash)); opt.has_value()) {
					return std::visit(
						Utils::overloaded{
							[](const bool &active) {
								return active;
							},
							[](const std::optional<uint8_t> &currentIndex) {
								return currentIndex.has_value();
							},
						},
						opt->get().value
					);
				}
			}

			return ::Option::getBool(context.team.options, name);
		}
	};

	struct impl_GetFloat {
		Utils::HashedString name;
		float defaultValue = 0.f;

		[[nodiscard]] Compiled::FloatNode compile(const Context &context) const {
			return Compiled::ConstantFloat{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto &option = ::Option::getValueListOption(context.source.options, name);

			if (context.overrides != nullptr) {
				if (auto opt = context.overrides->getOption(Utils::hashCombine(context.source.instanceKey, name.hash)); opt.has_value()) {
					auto index = std::get<std::optional<uint8_t>>(opt->get().value);
					if (index.has_value()) {
						return static_cast<float>(option.values.at(index.value()));
					} else {
						return defaultValue;
					}
				}
			}

			return option.getValue().value_or(defaultValue);
		}
	};

	struct impl_GetInt {
		Utils::HashedString name;
		uint32_t defaultValue = 0.f;

		[[nodiscard]] Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			auto &option = ::Option::getValueListOption(context.source.options, name);

			if (context.overrides != nullptr) {
				if (auto opt = context.overrides->getOption(Utils::hashCombine(context.source.instanceKey, name.hash)); opt.has_value()) {
					auto index = std::get<std::optional<uint8_t>>(opt->get().value);
					if (index.has_value()) {
						return option.values.at(index.value());
					} else {
						return defaultValue;
					}
				}
			}

			return option.getValue().value_or(defaultValue);
		}
	};

	struct impl_GetIndex {
		Utils::HashedString name;
		uint32_t defaultValue = 0;

		[[nodiscard]] Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			if (context.overrides != nullptr) {
				if (auto opt = context.overrides->getOption(Utils::hashCombine(context.source.instanceKey, name.hash)); opt.has_value()) {
					return std::get<std::optional<uint8_t>>(opt->get().value).value_or(defaultValue);
				}
			}

			return ::Option::getIndex(context.source.options, name, defaultValue);
		}
	};
}// namespace Formula