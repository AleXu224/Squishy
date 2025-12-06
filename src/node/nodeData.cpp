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
			[&](const ModsData &) {
				return Utils::elementToColor(Misc::Element::physical);
			},
			[&](const DirectLunarData &node) -> squi::Color {
				switch (node.damageType) {
					case Misc::LunarDamageType::lunarCharged:
						return squi::Color::css(236, 184, 255);
					case Misc::LunarDamageType::lunarBloom:
						return Utils::elementToColor(Misc::Element::dendro);
					case Misc::LunarDamageType::lunarCrystallize:
						return 0xFCD451FF;
				}
				std::unreachable();
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
			[&](const ShieldData &) {
				return true;
			},
			[&](const ModsData &) {
				return false;
			},
			[&](const DirectLunarData &) {
				return true;
			},
		},
		data
	);
}
