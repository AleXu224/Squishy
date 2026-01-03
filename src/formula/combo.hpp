#pragma once

#include "combo/override.hpp"
#include "compiled/node.hpp"
#include "compiled/operators.hpp"
#include "fmt/core.h"
#include "formula/node.hpp"
#include "formulaContext.hpp"
#include <vector>


namespace Formula {
	struct Combo {
		struct Entry {
			float multiplier;
			std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> reaction;
			FloatNode node;
		};
		std::vector<Entry> nodes;

		[[nodiscard]] inline Formula::Compiled::FloatNode compile(const Context &context) const {
			using namespace Formula::Compiled::Operators;
			Formula::Compiled::FloatNode ret = Formula::Compiled::ConstantFloat();

			for (const auto &node: nodes) {
				ret = ret + (Formula::Compiled::FloatNode(Formula::Compiled::ConstantFloat{.value = node.multiplier}) * node.node.compile(context.withReaction(node.reaction)));
			}

			return ret;
		}

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

		[[nodiscard]] inline auto compile(const Context &context) const {
			return runFor(context, [&node = node](const Context &context) {
				return node.compile(context);
			});
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Combo {}", eval(context));
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return runFor(context, [&node = node](const Formula::Context &context) {
				return node.eval(context);
			});
		}
	};
}// namespace Formula