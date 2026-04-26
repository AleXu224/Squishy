#include "nodeData.hpp"

#include "UI/attributeToColor.hpp"
#include "formula/attribute.hpp"

squi::Color Node::getColor(const Data &data, const Formula::Context &ctx) {
	return std::visit(
		Utils::overloaded{
			[&](const AtkData &node) {
				return Utils::attributeToColor(Formula::getAttribute(node.source, node.attribute, ctx));
			},
			[&](const CustomAtkData &node) {
				return Utils::attributeToColor(node.attribute);
			},
			[&](const InfoData &info) {
				return info.color;
			},
			[&](const HealData &) {
				return squi::Color::rgb(42, 173, 0);
			},
			[&](const ModsData &) {
				return squi::Color::white;
			},
		},
		data
	);
}
bool Node::getOptimizable(const Data &data) {
	return std::visit(
		Utils::overloaded{
			[&](const AtkData &) {
				return true;
			},
			[&](const CustomAtkData &) {
				return true;
			},
			[&](const InfoData &info) {
				return info.optimizable;
			},
			[&](const HealData &) {
				return true;
			},
			[&](const ModsData &) {
				return false;
			},
		},
		data
	);
}
