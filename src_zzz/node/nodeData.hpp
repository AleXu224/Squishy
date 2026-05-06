#pragma once

#include "color.hpp"
#include "misc/attackSource.hpp"
#include "misc/attribute.hpp"
#include "stats/sheet.hpp"
#include "utils/entryType.hpp"
#include "utils/optional.hpp"
#include "utils/overloaded.hpp"

#include "utils.hpp"

#include "variant"


namespace Node {
	struct AtkData {
		Utils::JankyOptional<Misc::Attribute> attribute{};
		Utils::JankyOptional<Misc::AttackSource> source{};
	};

	struct DazeData {
		Utils::JankyOptional<Misc::Attribute> attribute{};
		Utils::JankyOptional<Misc::AttackSource> source{};
	};

	struct CustomAtkData {
		Misc::Attribute attribute;
	};

	struct InfoData {
		Utils::EntryType type;
		squi::Color color;
		bool optimizable;
	};

	struct HealData {};

	struct ModsData {
		squi::utils::Container<Stats::ModsSheet> mods;
	};

	using Data = std::variant<AtkData, DazeData, CustomAtkData, InfoData, HealData, ModsData>;

	[[nodiscard]] constexpr bool isPercentage(const Data &data) {
		return std::visit(
			Utils::overloaded{
				[](const AtkData &) {
					return false;
				},
				[](const DazeData &) {
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
				[](const ModsData &) {
					return false;
				},
			},
			data
		);
	}

	[[nodiscard]] squi::Color getColor(const Data &data, const Formula::Context &ctx);
	[[nodiscard]] bool getOptimizable(const Data &data);
}// namespace Node