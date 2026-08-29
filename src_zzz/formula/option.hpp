#pragma once

#include "formula/base.hpp"
#include "option/option.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Formula {
	[[nodiscard]] inline const Combo::Option *matchingOverride(const Combo::Overrides *overrides, uint64_t hash, const Option::Types &definition) {
		if (overrides == nullptr) return nullptr;

		auto opt = overrides->getOption(hash);
		if (!opt.has_value()) return nullptr;

		bool matches = std::visit(
			Utils::overloaded{
				[&definition](const bool &) {
					return std::holds_alternative<Option::Boolean>(definition);
				},
				[&definition](const std::optional<uint8_t> &) {
					return std::holds_alternative<Option::ValueList>(definition);
				},
				[&definition](const ::Combo::ComboFloatOption &) {
					return std::holds_alternative<Option::ValueSlider>(definition);
				},
			},
			opt->get().value
		);

		return matches ? &opt->get() : nullptr;
	};

	struct impl_IsActive : FormulaBase<bool, Type::constant> {
		Utils::HashedString name;

		void print(Descriptor &descriptor, const Context &context, Step) const {
			descriptor.add(std::string{name.str}, std::string{eval(context) ? "active" : "inactive"});
		}

		[[nodiscard]] bool eval(const Context &context) const {
			if (auto *opt = matchingOverride(context.overrides, Utils::hashCombine(context.source.instanceKey, name.hash), context.source.options->at(name.hash))) {
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
					opt->value
				);
			}

			return ::Option::getBool(*context.source.options, name);
		}
	};

	struct impl_IsActivePassive : FormulaBase<bool, Type::constant> {
		Utils::HashedString name;

		void print(Descriptor &descriptor, const Context &context, Step) const {
			descriptor.add(std::string{name.str}, std::string{eval(context) ? "active" : "inactive"});
		}

		[[nodiscard]] bool eval(const Context &context) const {
			if (auto *opt = matchingOverride(context.overrides, Utils::hashCombine(0, name.hash), context.team.options->at(name.hash))) {
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
					opt->value
				);
			}

			return ::Option::getBool(*context.team.options, name);
		}
	};

	struct impl_GetFloat : FormulaBase<float, Type::constant> {
		Utils::HashedString name;
		float defaultValue = 0.f;

		void print(Descriptor &descriptor, const Context &context, Step) const {
			descriptor.add(std::string{name.str}, {eval(context), false});
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto &definition = context.source.options->at(name.hash);
			if (auto *opt = matchingOverride(context.overrides, Utils::hashCombine(context.source.instanceKey, name.hash), definition)) {
				return std::visit(
					Utils::overloaded{
						[](const bool &active) -> float {
							return active ? 1.f : 0.f;
						},
						[&](const std::optional<uint8_t> &currentIndex) -> float {
							return currentIndex.transform([](auto &&val) {
											return static_cast<float>(val);
										})
								.value_or(defaultValue);
						},
						[](const ::Combo::ComboFloatOption &value) -> float {
							return value.value;
						},
					},
					opt->value
				);
			}

			return std::visit(
				Utils::overloaded{
					[&](const Option::Boolean &opt) -> float {
						return opt.active ? 1.f : 0.f;
					},
					[&](const Option::ValueList &opt) -> float {
						return opt.currentIndex.transform([](auto &&val) {
												   return static_cast<float>(val);
											   })
							.value_or(defaultValue);
					},
					[&](const Option::ValueSlider &opt) -> float {
						return opt.getValue();
					},
				},
				definition
			);
		}
	};

	struct impl_GetInt : FormulaBase<int32_t, Type::constant> {
		Utils::HashedString name;
		uint32_t defaultValue = 0;

		void print(Descriptor &descriptor, const Context &context, Step) const {
			descriptor.add(std::string{name.str}, eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			auto &definition = context.source.options->at(name.hash);
			if (auto *opt = matchingOverride(context.overrides, Utils::hashCombine(context.source.instanceKey, name.hash), definition)) {
				return std::visit(
					Utils::overloaded{
						[](const bool &active) -> int32_t {
							return active ? 1 : 0;
						},
						[&](const std::optional<uint8_t> &currentIndex) -> int32_t {
							return currentIndex.transform([](auto &&val) -> int32_t {
											return val;
										})
								.value_or(defaultValue);
						},
						[](const ::Combo::ComboFloatOption &value) -> int32_t {
							return static_cast<int32_t>(value.value);
						},
					},
					opt->value
				);
			}

			return std::visit(
				Utils::overloaded{
					[&](const Option::Boolean &opt) -> int32_t {
						return opt.active ? 1 : 0;
					},
					[&](const Option::ValueList &opt) -> int32_t {
						return opt.currentIndex.value_or(defaultValue);
					},
					[&](const Option::ValueSlider &opt) -> int32_t {
						return opt.getValue();
					},
				},
				definition
			);
		}
	};

	struct impl_GetIndex : FormulaBase<int32_t, Type::constant> {
		Utils::HashedString name;
		uint32_t defaultValue = 0;

		void print(Descriptor &descriptor, const Context &context, Step) const {
			descriptor.add(std::string{name.str}, eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			auto &definition = context.source.options->at(name.hash);
			if (auto *opt = matchingOverride(context.overrides, Utils::hashCombine(context.source.instanceKey, name.hash), definition)) {
				return std::get<std::optional<uint8_t>>(opt->value).value_or(defaultValue);
			}

			return ::Option::getIndex(*context.source.options, name, defaultValue);
		}
	};

	[[nodiscard]] inline uint32_t activeAgentIndex(const Context &context) {
		static constexpr Utils::HashedString key{"activeAgent"};

		uint32_t ret = ::Option::getIndex(*context.team.options, key, context.team.activeAgentIndex);
		if (auto *opt = matchingOverride(context.overrides, Utils::hashCombine(0, key.hash), context.team.options->at(key.hash))) {
			ret = std::get<std::optional<uint8_t>>(opt->value).value_or(ret);
		}

		if (ret >= context.team.agents.size()) return context.team.activeAgentIndex;
		return ret;
	}
}// namespace Formula