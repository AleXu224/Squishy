#include "combo.hpp"

#include "formula/combo.hpp"
#include "formula/custom.hpp"
#include "reaction/list.hpp"

float Combo::Combo::eval(const Formula::Context &context) const {

	float ret = 0.f;

	for (const auto &entry: entries) {
		std::vector<::Combo::Option> store;
		auto newContext = context.withReaction(Reaction::List::fromNodeReaction(entry.reaction));
		newContext.optionStore = &store;
		auto node = Formula::ComboOptionOverride{
			.overrides = entry.options,
			.node = Formula::Custom{
				.func = [&](const Formula::Context &ctx) {
					const auto &node = std::visit(
						[&](auto &&source) {
							return source.resolve(entry.options);
						},
						entry.source
					);
					return entry.multiplier * node.formula.eval(ctx);
				},
			},
		};
		ret += node.eval(newContext);
	}

	return ret;
}
