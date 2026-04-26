#include "combo.hpp"

#include "combo/override.hpp"


float Combo::Combo::eval(const Formula::Context &context) const {
	float ret = 0.f;

	Formula::Context ctx = context;

	Overrides overrides;

	for (const auto &entry: entries) {
		std::visit(
			Utils::overloaded{
				[&](const ::Combo::Entry &entry) {
					Overrides newOverrides = overrides;
					newOverrides.push(entry.options);
					const auto &node = std::visit(
						[&](auto &&source) {
							return source.resolve(newOverrides);
						},
						entry.source
					);
					ret += entry.multiplier * node.formula.eval(ctx);
				},
				[&](const ::Combo::StateChangeEntry &entry) {
					overrides.push(entry.options);
				},
			},
			entry
		);
	}

	return ret;
}
