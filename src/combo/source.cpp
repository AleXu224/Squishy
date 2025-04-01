#include "source.hpp"

#include "Ui/elementToColor.hpp"
#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "formula/combo.hpp"
#include "reaction/list.hpp"
#include "store.hpp"
#include "weapon/weapons.hpp"


Node::Instance Combo::Source::Combo::resolve(const std::vector<::Combo::Option> &options) const {
	auto &combo = ::Store::characters.at(characterKey).combos.at(comboKey);

	std::vector<Formula::Combo::Entry> nodes;
	for (auto &entry: combo.entries) {
		nodes.emplace_back(Formula::Combo::Entry{
			.multiplier = entry.multiplier,
			.reaction = Reaction::List::fromNodeReaction(entry.reaction),
			.node = std::visit([&](auto &&val) {
						return val.resolve(entry.options);
					},
							   entry.source)
						.formula,
		});
	}

	struct NodeInfo {
		std::string_view name;
		Node::InfoData _data;
		Formula::FloatNode _formula;
	};

	auto formula = Formula::Combo{.nodes = nodes};
	auto formulaInstance = [&]() -> Formula::FloatNode {
		if (options.empty()) return formula;
		return Formula::ComboOptionOverride{
			.overrides = options,
			.node = formula,
		};
	}();

	return Node::Instance(NodeInfo{
		.name = combo.name,
		._data{
			.type = Utils::EntryType::points,
			.color = Utils::elementToColor(Misc::Element::physical),
			.optimizable = true,
		},
		._formula = formulaInstance,
	});
}

Node::Instance Combo::Source::Character::resolve(const std::vector<::Combo::Option> &options) const {
	auto &ret = ::Character::list.at(key).data.nodes.fromEntry(slot).at(index);

	if (!options.empty()) {
		auto nodeCopy = ret;
		nodeCopy.formula = Formula::ComboOptionOverride{
			.overrides = options,
			.node = ret.formula,
		};
		return nodeCopy;
	}

	return ret;
}

Node::Instance Combo::Source::Weapon::resolve(const std::vector<::Combo::Option> &options) const {
	auto &ret = ::Weapon::list.at(key).data.nodes.at(index);

	if (!options.empty()) {
		auto nodeCopy = ret;
		nodeCopy.formula = Formula::ComboOptionOverride{
			.overrides = options,
			.node = ret.formula,
		};
		return nodeCopy;
	}

	return ret;
}

Node::Instance Combo::Source::Artifact::resolve(const std::vector<::Combo::Option> &options) const {
	auto &ret = ::Artifact::sets.at(key).data.fromSetSlot(slot).nodes.at(index);

	if (!options.empty()) {
		auto nodeCopy = ret;
		nodeCopy.formula = Formula::ComboOptionOverride{
			.overrides = options,
			.node = ret.formula,
		};
		return nodeCopy;
	}

	return ret;
}
