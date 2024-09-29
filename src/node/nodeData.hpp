#pragma once

#include "color.hpp"
#include "formula/formulaContext.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "utils/optional.hpp"
#include "utils/overloaded.hpp"
#include "variant"


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

	[[nodiscard]] squi::Color getColor(const Data &data, const Formula::Context &ctx);
}// namespace Node