#include "source.hpp"

#include "Ui/elementToColor.hpp"
#include "formula/combo.hpp"
#include "reaction/list.hpp"
#include "store.hpp"


Node::Instance Combo::Source::Combo::resolve() const {
	auto &combo = ::Store::characters.at(characterKey).combos.at(comboKey);

	std::vector<Formula::Combo::Entry> nodes;
	for (auto &entry: combo.entries) {
		nodes.emplace_back(Formula::Combo::Entry{
			.multiplier = entry.multiplier,
			.reaction = Reaction::List::fromNodeReaction(entry.reaction),
			.node = std::visit([](auto &&val) {
						return val.resolve();
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

	return Node::Instance(NodeInfo{
		.name = combo.name,
		._data{
			.type = Utils::EntryType::points,
			.color = Utils::elementToColor(Misc::Element::physical),
			.optimizable = true,
		},
		._formula = Formula::Combo{.nodes = nodes},
	});
}
