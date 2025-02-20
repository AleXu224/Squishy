#include "nodeData.hpp"

#include "Ui/elementToColor.hpp"
#include "formula/element.hpp"

squi::Color Node::getColor(const Data &data, const Formula::Context &ctx) {
	return std::visit(
		Utils::overloaded{
			[&](const AtkData &node) {
				return Utils::elementToColor(Formula::getElement(node.source, node.element, ctx));
			},
			[&](const InfoData &info) {
				return info.color;
			},
			[&](const HealData &) {
				return Utils::elementToColor(Misc::Element::dendro);
			},
		},
		data
	);
}