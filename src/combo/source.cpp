#include "source.hpp"

#include "Ui/elementToColor.hpp"
#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "formula/combo.hpp"
#include "reaction/list.hpp"
#include "store.hpp"
#include "weapon/weapons.hpp"

namespace {
	struct NodeInfo {
		std::string_view name;
		Node::InfoData _data;
		Formula::FloatNode _formula;
	};
}// namespace

Node::Instance Combo::Source::Combo::resolve(const Overrides &overrides) const {
	auto &combo = ::Store::characters.at(characterKey).combos.at(comboKey);

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
						.reaction = Reaction::List::fromNodeReaction(entry.reaction),
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
			.type = Utils::EntryType::points,
			.color = Utils::elementToColor(Misc::Element::physical),
			.optimizable = true,
		},
		._formula = std::move(formulaInstance),
	});
}

Node::Instance Combo::Source::Character::resolve(const Overrides &overrides) const {
	const auto &ret = ::Character::list.at(key).data.nodes.fromEntry(slot).at(index);

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

Node::Instance Combo::Source::Weapon::resolve(const Overrides &overrides) const {
	const auto &ret = ::Weapon::list.at(key).data.nodes.at(index);

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

Node::Instance Combo::Source::Artifact::resolve(const Overrides &overrides) const {
	const auto &ret = ::Artifact::sets.at(key).data.fromSetSlot(slot).nodes.at(index);

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

Node::Instance Combo::Source::TransformativeReaction::resolve(const Overrides &overrides) const {
	auto [formula, name, element] = [&]() -> std::tuple<Formula::FloatNode, std::string_view, Misc::Element> {
		switch (reaction) {
			case Misc::TransformativeReaction::burning:
				return {Reaction::List::Transformative::burning.formula, "Burning", Reaction::List::Transformative::burning.damageElement};
			case Misc::TransformativeReaction::superconduct:
				return {Reaction::List::Transformative::superconduct.formula, "Superconduct", Reaction::List::Transformative::superconduct.damageElement};
			case Misc::TransformativeReaction::pyroSwirl:
				return {Reaction::List::Transformative::pyroSwirl.formula, "Pyro Swirl", Reaction::List::Transformative::pyroSwirl.damageElement};
			case Misc::TransformativeReaction::hydroSwirl:
				return {Reaction::List::Transformative::hydroSwirl.formula, "Hydro Swirl", Reaction::List::Transformative::hydroSwirl.damageElement};
			case Misc::TransformativeReaction::electroSwirl:
				return {Reaction::List::Transformative::electroSwirl.formula, "Electro Swirl", Reaction::List::Transformative::electroSwirl.damageElement};
			case Misc::TransformativeReaction::cryoSwirl:
				return {Reaction::List::Transformative::cryoSwirl.formula, "Cryo Swirl", Reaction::List::Transformative::cryoSwirl.damageElement};
			case Misc::TransformativeReaction::electroCharged:
				return {Reaction::List::Transformative::electroCharged.formula, "Electro-Charged", Reaction::List::Transformative::electroCharged.damageElement};
			case Misc::TransformativeReaction::shattered:
				return {Reaction::List::Transformative::shattered.formula, "Shattered", Reaction::List::Transformative::shattered.damageElement};
			case Misc::TransformativeReaction::overloaded:
				return {Reaction::List::Transformative::overloaded.formula, "Overloaded", Reaction::List::Transformative::overloaded.damageElement};
			case Misc::TransformativeReaction::bloom:
				return {Reaction::List::Transformative::bloom.formula, "Bloom", Reaction::List::Transformative::bloom.damageElement};
			case Misc::TransformativeReaction::burgeon:
				return {Reaction::List::Transformative::burgeon.formula, "Burgeon", Reaction::List::Transformative::burgeon.damageElement};
			case Misc::TransformativeReaction::hyperbloom:
				return {Reaction::List::Transformative::hyperbloom.formula, "Hyperbloom", Reaction::List::Transformative::hyperbloom.damageElement};
			case Misc::TransformativeReaction::lunarCharged:
				return {Reaction::List::Transformative::lunarCharged.formula, "Lunar-Charged", Reaction::List::Transformative::lunarCharged.damageElement};
			case Misc::TransformativeReaction::lunarCrystalize:
				return {Reaction::List::Transformative::lunarCrystalize.formula, "Lunar-Crystalize", Reaction::List::Transformative::lunarCrystalize.damageElement};
		}
		std::unreachable();
	}();

	auto ret = Node::Instance(NodeInfo{
		.name = name,
		._data{
			.type = Utils::EntryType::points,
			.color = Utils::elementToColor(element),
			.optimizable = true,
		},
		._formula = formula,
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
