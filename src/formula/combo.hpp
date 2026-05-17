#pragma once

#include "combo/override.hpp"
#include "formula/base.hpp"
#include "formula/operators.hpp"
#include <vector>


namespace Formula {
	struct Combo : FormulaBase<float> {
		struct Entry {
			float multiplier;
			std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> reaction;
			FloatNode node;
		};
		std::vector<Entry> nodes;

		FloatNode fold(const Context &ctx, const FoldArgs &args) const {
			using namespace Formula::Operators;
			FloatNode ret = ConstantFlat{.value{}};

			for (const auto &node: nodes) {
				ret = ret + (Constant{.value = node.multiplier} * node.node.fold(ctx.withReaction(node.reaction), args));
			}

			return ret.fold(ctx, args);
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("Combo {}", eval(context));
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
	struct ComboOptionOverride : FormulaBase<float> {
		::Combo::Overrides overrides;
		T node;

		inline auto runFor(const Context &context, auto &&lbd) const {
			const ::Combo::Overrides *lastOverrides = overrides.parent;
			if (context.overrides != nullptr) {
				overrides.parent = context.overrides;
			}

			auto ret = lbd(context.withOverrides(&overrides));

			overrides.parent = lastOverrides;

			return ret;
		}

		FloatNode fold(const Context &ctx, const FoldArgs &args) const {
			return runFor(ctx, [&node = node, &args](const Context &context) {
				// It's alrgith to return the folded node here because option is guaranteed to be folded to a constant
				// meaning that any further folding from this node will just return the same thing
				return node.fold(context, args);
			});
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("Combo {}", eval(context));
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return runFor(context, [&node = node](const Formula::Context &context) {
				return node.eval(context);
			});
		}
	};
}// namespace Formula