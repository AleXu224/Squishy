#pragma once

#include "formula/base.hpp"
#include "option/option.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Formula {
	struct impl_IsActive : FormulaBase<bool, Type::constant> {
		Utils::HashedString name;

		[[nodiscard]] std::string print(const Context &, Step) const {
			return std::format("{}", name.str);
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
							[](const ::Combo::ComboFloatOption &value) {
								return value.value != 0.f;
							},
						},
						opt->get().value
					);
				}
			}

			return ::Option::getBool(context.source.options, name);
		}
	};

	struct impl_IsActivePassive : FormulaBase<bool, Type::constant> {
		Utils::HashedString name;

		[[nodiscard]] std::string print(const Context &, Step) const {
			return std::format("{}", name.str);
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
							[](const ::Combo::ComboFloatOption &value) {
								return value.value != 0.f;
							},
						},
						opt->get().value
					);
				}
			}

			return ::Option::getBool(context.team.options, name);
		}
	};

	struct impl_GetFloat : FormulaBase<float, Type::constant> {
		Utils::HashedString name;
		float defaultValue = 0.f;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] float eval(const Context &context) const {
			if (context.overrides != nullptr) {
				if (auto opt = context.overrides->getOption(Utils::hashCombine(context.source.instanceKey, name.hash)); opt.has_value()) {
					return std::visit(
						Utils::overloaded{
							[](const bool &active) {
								return active ? 1.f : 0.f;
							},
							[&](const std::optional<uint8_t> &currentIndex) -> float {
								auto &values = std::get<Option::ValueList>(context.source.options.at(name.hash)).values;
								if (currentIndex.has_value() && currentIndex.value() < values.size()) {
									return values.at(currentIndex.value());
								} else {
									return defaultValue;
								}
							},
							[](const ::Combo::ComboFloatOption &value) {
								return value.value;
							},
						},
						opt->get().value
					);
				}
			}

			return std::visit(
				Utils::overloaded{
					[&](const Option::Boolean &opt) -> float {
						return opt.active ? 1.f : 0.f;
					},
					[&](const Option::ValueList &opt) -> float {
						return opt.getValue().value_or(defaultValue);
					},
					[&](const Option::ValueSlider &opt) -> float {
						return opt.getValue();
					},
				},
				context.source.options.at(name.hash)
			);
		}
	};

	struct impl_GetInt : FormulaBase<int32_t, Type::constant> {
		Utils::HashedString name;
		uint32_t defaultValue = 0.f;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("{} {}", name.str, eval(context));
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

	struct impl_GetIndex : FormulaBase<int32_t, Type::constant> {
		Utils::HashedString name;
		uint32_t defaultValue = 0;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("{} {}", name.str, eval(context));
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