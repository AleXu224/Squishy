#pragma once

#include "color.hpp"
#include "formula/formulaContext.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "utils/entryType.hpp"
#include "utils/optional.hpp"
#include "utils/overloaded.hpp"
#include "variant"


namespace Node {
	struct AtkData {
		Utils::JankyOptional<Misc::Element> element{};
		Misc::AttackSource source{};
	};

	struct CustomAtkData {
		Misc::Element element;
	};

	struct InfoData {
		Utils::EntryType type;
		squi::Color color;
		bool optimizable;
	};

	struct HealData {};

	struct ShieldData {
		Utils::JankyOptional<Misc::Element> element{};
	};

	using Data = std::variant<AtkData, CustomAtkData, InfoData, HealData, ShieldData>;

	[[nodiscard]] constexpr bool isPercentage(const Data &data) {
		return std::visit(
			Utils::overloaded{
				[](const AtkData &) {
					return false;
				},
				[](const CustomAtkData &) {
					return false;
				},
				[](const InfoData &info) {
					return info.type == Utils::EntryType::multiplier;
				},
				[](const HealData &) {
					return false;
				},
				[](const ShieldData &) {
					return false;
				},
			},
			data
		);
	}

	[[nodiscard]] squi::Color getColor(const Data &data, const Formula::Context &ctx);
	[[nodiscard]] bool getOptimizable(const Data &data);
}// namespace Node