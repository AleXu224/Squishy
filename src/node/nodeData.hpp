#pragma once

#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "utils/optional.hpp"
#include "utils/overloaded.hpp"
#include "variant"
#include "color.hpp"
#include "formula/formulaContext.hpp"
#include "Ui/elementToColor.hpp"
#include "formula/stat.hpp"

namespace Node {
	struct AtkData {
		Utils::JankyOptional<Misc::Element> element{};
		Misc::AttackSource source{};
	};

	struct InfoData {
		bool isPercentage = false;
        squi::Color color;
	};

	using Data = std::variant<AtkData, InfoData>;

	[[nodiscard]] constexpr bool isPercentage(const Data &data) {
		return std::visit(
			Utils::overloaded{
				[](const AtkData &) {
					return false;
				},
				[](const InfoData &info) {
					return info.isPercentage;
				},
			},
			data
		);
	}

    [[nodiscard]] constexpr squi::Color getColor(const Data &data, const Formula::Context &ctx) {
		return std::visit(
			Utils::overloaded{
				[&](const AtkData &node) {
					return Utils::elementToColor(Formula::_getElement(node.source, node.element, ctx));
				},
				[&](const InfoData &info) {
					return info.color;
				},
			},
			data
		);
	}
}// namespace Node