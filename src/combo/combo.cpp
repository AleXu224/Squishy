#include "combo.hpp"

#include "reaction/list.hpp"

float Combo::Combo::eval(const Formula::Context &context) const {
	float ret = 0.f;

	for (const auto &entry: entries) {
		auto newContext = context.withReaction(Reaction::List::fromNodeReaction(entry.reaction));
		const auto &node = std::visit([](auto &&source) {
			return source.resolve();
		},
									  entry.source);
		ret += entry.multiplier * node.formula.eval(newContext);
	}

	return ret;
}
