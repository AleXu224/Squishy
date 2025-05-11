#include "combo.hpp"

#include "combo/override.hpp"
#include "reaction/list.hpp"


float Combo::Combo::eval(const Formula::Context &context) const {
	float ret = 0.f;

	Formula::Context ctx = context;

	std::vector<::Combo::Option> store;
	ctx.optionStore = &store;
	uint32_t popCount = 0;
	for (const auto &entry: entries) {
		std::visit(
			Utils::overloaded{
				[&](const ::Combo::Entry &entry) {
					auto newContext = ctx.withReaction(Reaction::List::fromNodeReaction(entry.reaction));
					newContext.optionStore = &store;
					const auto &node = std::visit(
						[&](auto &&source) {
							return source.resolve(entry.options);
						},
						entry.source
					);
					ret += entry.multiplier * node.formula.eval(newContext);
				},
				[&](const ::Combo::StateChangeEntry &entry) {
					popCount += ::Combo::pushOverrides(ctx, entry.options);
				},
			},
			entry
		);
	}
	::Combo::popOverrides(ctx, popCount);

	return ret;
}
