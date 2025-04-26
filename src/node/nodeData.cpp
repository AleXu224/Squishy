#include "nodeData.hpp"

#include "Ui/elementToColor.hpp"
#include "formula/element.hpp"

squi::Color Node::getColor(const Data &data, const Formula::Context &ctx) {
	return std::visit(
		Utils::overloaded{
			[&](const AtkData &node) {
				return Utils::elementToColor(Formula::getElement(node.source, node.element, ctx));
			},
			[&](const CustomAtkData &node) {
				return Utils::elementToColor(node.element);
			},
			[&](const InfoData &info) {
				return info.color;
			},
			[&](const HealData &) {
				return Utils::elementToColor(Misc::Element::dendro);
			},
			[&](const ShieldData &node) {
				return Utils::elementToColor(node.element.value_or(Misc::Element::physical));
			},
		},
		data
	);
}
bool Node::getOptimizable(const Data &data) {
	return std::visit(
		Utils::overloaded{
			[&](const AtkData &node) {
				return true;
			},
			[&](const CustomAtkData &node) {
				return true;
			},
			[&](const InfoData &info) {
				return info.optimizable;
			},
			[&](const HealData &) {
				return true;
			},
			[&](const ShieldData &) {
				return true;
			},
		},
		data
	);
}
