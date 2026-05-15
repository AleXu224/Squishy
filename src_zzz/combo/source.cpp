#include "source.hpp"

#include "UI/attributeToColor.hpp"
#include "agent/agents.hpp"
#include "anomaly/list.hpp"// IWYU pragma: keep
#include "disc/sets.hpp"
#include "engine/engines.hpp"
#include "formula/combo.hpp"
#include "store.hpp"


namespace {
	struct NodeInfo {
		std::string_view name;
		Node::InfoData _data;
		Formula::FloatNode _formula;
	};
}// namespace

Node::Instance Combo::Source::Combo::resolve(const Overrides &overrides) const {
	auto &combo = ::Store::agents.at(agentKey).combos.at(comboKey);

	std::vector<Formula::Combo::Entry> nodes;
	Overrides overridesStack{};
	for (auto &entry: combo.entries) {
		std::visit(
			Utils::overloaded{
				[&](const ::Combo::Entry &entry) {
					Overrides retOverrides = overridesStack;
					retOverrides.push(entry.options);
					nodes.emplace_back(Formula::Combo::Entry{
						.multiplier = entry.multiplier,
						.node = std::visit(//
									[&](auto &&val) {
										return val.resolve(std::move(retOverrides));
									},
									entry.source
						)
									.formula,
					});
				},
				[&](const ::Combo::StateChangeEntry &entry) {
					overridesStack.push(entry.options);
				},
			},
			entry
		);
	}


	auto formula = Formula::Combo{.nodes = std::move(nodes)};
	auto formulaInstance = [&]() -> Formula::FloatNode {
		if (overrides.empty()) return std::move(formula);
		return Formula::ComboOptionOverride{
			.overrides = std::move(overrides),
			.node = std::move(formula),
		};
	}();

	return Node::Instance(NodeInfo{
		.name = combo.name,
		._data{
			.name = combo.name,
			.type = Utils::EntryType::points,
			.color = squi::Color::white,
			.optimizable = true,
		},
		._formula = std::move(formulaInstance),
	});
}

Node::Instance Combo::Source::Agent::resolve(const Overrides &overrides) const {
	const auto &ret = ::Agent::list.at(key).data->nodes.fromEntry(slot).at(index);

	if (!overrides.empty()) {
		auto nodeCopy = ret;
		nodeCopy.formula = Formula::ComboOptionOverride{
			.overrides = overrides,
			.node = ret.formula,
		};
		return nodeCopy;
	}

	return ret;
}

Node::Instance Combo::Source::Engine::resolve(const Overrides &overrides) const {
	const auto &ret = ::Engine::list.at(key).data.nodes.at(index);

	if (!overrides.empty()) {
		auto nodeCopy = ret;
		nodeCopy.formula = Formula::ComboOptionOverride{
			.overrides = overrides,
			.node = ret.formula,
		};
		return nodeCopy;
	}

	return ret;
}

Node::Instance Combo::Source::Disc::resolve(const Overrides &overrides) const {
	const auto &ret = ::Disc::sets.at(key).data.fromSetSlot(slot).nodes.at(index);

	if (!overrides.empty()) {
		auto nodeCopy = ret;
		nodeCopy.formula = Formula::ComboOptionOverride{
			.overrides = overrides,
			.node = ret.formula,
		};
		return nodeCopy;
	}

	return ret;
}

Node::Instance Combo::Source::Anomaly::resolve(const Overrides &overrides) const {
	auto anomaly = [&]() -> const ::Anomaly::Anomaly & {
		switch (reaction) {
			case Misc::Anomaly::burn:
				return ::Anomaly::List::burn;
			case Misc::Anomaly::shock:
				return ::Anomaly::List::shock;
			case Misc::Anomaly::shatter:
				return ::Anomaly::List::shatter;
			case Misc::Anomaly::assault:
				return ::Anomaly::List::assault;
			case Misc::Anomaly::corruption:
				return ::Anomaly::List::corruption;
			case Misc::Anomaly::burnDisorder:
				return ::Anomaly::List::burnDisorder;
			case Misc::Anomaly::shockDisorder:
				return ::Anomaly::List::shockDisorder;
			case Misc::Anomaly::frozenDisorder:
				return ::Anomaly::List::frozenDisorder;
			case Misc::Anomaly::frozenFrostDisorder:
				return ::Anomaly::List::frozenFrostDisorder;
			case Misc::Anomaly::assaultDisorder:
				return ::Anomaly::List::assaultDisorder;
			case Misc::Anomaly::corruptionDisorder:
				return ::Anomaly::List::corruptionDisorder;
		}
		std::unreachable();
	}();

	auto ret = Node::Instance(NodeInfo{
		.name = anomaly.name,
		._data{
			.name = std::string(anomaly.name),
			.type = Utils::EntryType::points,
			.color = Utils::attributeToColor(anomaly.attribute),
			.optimizable = true,
		},
		._formula = anomaly.formula,
	});

	if (!overrides.empty()) {
		auto nodeCopy = ret;
		nodeCopy.formula = Formula::ComboOptionOverride{
			.overrides = overrides,
			.node = ret.formula,
		};
		return nodeCopy;
	}

	return ret;
}
