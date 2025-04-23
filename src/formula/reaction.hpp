#pragma once

#include "fmt/core.h"
#include "formulaContext.hpp"
#include "reaction/reaction.hpp"
#include "step.hpp"

namespace Formula {
	struct AmplifyingMultiplier {
		[[nodiscard]] Compiled::FloatNode compile(const Context &context) const {
			switch (context.reaction.index()) {
				case 0:
					return Compiled::ConstantFloat{.value = 1.f};
				case 1:
					return std::get<1>(context.reaction)->formula.compile(context);
				case 2:
					return Compiled::ConstantFloat{.value = 1.f};
			}
			std::unreachable();
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Reaction Multiplier {:.1f}%", eval(context) * 100.f);
		}

		[[nodiscard]] static constexpr float eval(const Context &context) {
			switch (context.reaction.index()) {
				case 0:
					return 1.f;
				case 1:
					return std::get<1>(context.reaction)->formula.eval(context);
				case 2:
					return 1.f;
			}
			std::unreachable();
		}
	};
	struct AdditiveMultiplier {
		[[nodiscard]] Compiled::FloatNode compile(const Context &context) const {
			switch (context.reaction.index()) {
				case 0:
					return Compiled::ConstantFloat{.value = 0.f};
				case 1:
					return Compiled::ConstantFloat{.value = 0.f};
				case 2:
					return std::get<2>(context.reaction)->formula.compile(context);
			}
			std::unreachable();
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Reaction Bonus {:.1f}", eval(context));
		}

		[[nodiscard]] static constexpr float eval(const Context &context) {
			switch (context.reaction.index()) {
				case 0:
					return 0.f;
				case 1:
					return 0.f;
				case 2:
					return std::get<2>(context.reaction)->formula.eval(context);
			}
			std::unreachable();
		}
	};
}// namespace Formula