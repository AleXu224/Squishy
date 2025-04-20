#pragma once

#include "fmt/core.h"
#include "formulaContext.hpp"
#include "reaction/reaction.hpp"
#include "step.hpp"
#include "utils/overloaded.hpp"


namespace Formula {
	struct AmplifyingMultiplier {
		using RetType = Compiled::ConstantOr<float, decltype(std::declval<Reaction::Amplifying>().formula.compile(std::declval<const Context &>()))>;

		[[nodiscard]] RetType compile(const Context &context) const {
			return std::visit(
				Utils::overloaded{
					[](const Reaction::None *) -> RetType {
						return {Compiled::ConstantFloat(1.f)};
					},
					[&](const Reaction::Amplifying *reaction) -> RetType {
						return {reaction->formula.compile(context)};
					},
					[](const Reaction::Additive *) -> RetType {
						return {Compiled::ConstantFloat(1.f)};
					},
				},
				context.reaction
			);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Reaction Multiplier {:.1f}%", eval(context) * 100.f);
		}

		[[nodiscard]] static constexpr float eval(const Context &context) {
			return std::visit(
				Utils::overloaded{
					[](const Reaction::None *) {
						return 1.f;
					},
					[&](const Reaction::Amplifying *reaction) {
						return reaction->formula.eval(context);
					},
					[](const Reaction::Additive *) {
						return 1.f;
					},
				},
				context.reaction
			);
		}
	};
	struct AdditiveMultiplier {
		using RetType = Compiled::ConstantOr<float, decltype(std::declval<Reaction::Additive>().formula.compile(std::declval<const Context &>()))>;

		[[nodiscard]] RetType compile(const Context &context) const {
			return std::visit(
				Utils::overloaded{
					[](const Reaction::None *) -> RetType {
						return {Compiled::ConstantFloat(0.f)};
					},
					[](const Reaction::Amplifying *) -> RetType {
						return {Compiled::ConstantFloat(0.f)};
					},
					[&](const Reaction::Additive *reaction) -> RetType {
						return {reaction->formula.compile(context)};
					},
				},
				context.reaction
			);
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Reaction Bonus {:.1f}", eval(context));
		}

		[[nodiscard]] static constexpr float eval(const Context &context) {
			return std::visit(
				Utils::overloaded{
					[](const Reaction::None *) {
						return 0.f;
					},
					[](const Reaction::Amplifying *) {
						return 0.f;
					},
					[&](const Reaction::Additive *reaction) {
						return reaction->formula.eval(context);
					},
				},
				context.reaction
			);
		}
	};
}// namespace Formula