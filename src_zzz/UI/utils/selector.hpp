#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"
#include "misc/promotion.hpp"
#include "stats/loadout.hpp"
#include "store.hpp"
#include "widgets/dropdownButton.hpp"


namespace UI {
	using namespace squi;
	template<class T>
	struct Selector : StatelessWidget {
		// Args
		Key key;
		std::string titlePrefix;
		std::string valuePrefix;
		std::function<T(void)> getter;
		std::function<void(const T &)> setter;
		std::function<std::string(const T &)> printer;
		std::vector<T> values;

		[[nodiscard]] Child build(const Element &) const {
			return DropdownButton{
				.widget{
					.width = squi::Size::Expand,
				},
				.text = std::format("{} {}", titlePrefix, printer(getter())),
				.items = [this]() {
					std::vector<squi::ContextMenu::Item> ret;
					ret.reserve(values.size());
					for (const auto &value: values) {
						ret.emplace_back(squi::ContextMenu::Button{
							.text = std::format("{} {}", valuePrefix, printer(value)),
							.callback = [this, &value]() {
								setter(value);
							},
						});
					}
					return ret;
				}(),
			};
		}
	};

	[[nodiscard]] inline uint8_t getPromotionFromLvl(uint8_t lvl) {
		return Misc::promotionsAtLvl(lvl, 5).front().promotion;
	}
	[[nodiscard]] inline uint8_t getLvlFromPromotion(uint8_t promotion) {
		return Misc::promotionsAtLvl(promotion, 5).front().maxLevel;
	}

	template<class T>
	struct LevelSelector : StatelessWidget {
		// Args
		Key key;
		std::string titlePrefix;
		std::function<T &()> sheetGetter;
		Agent::InstanceKey agentKey{};

		using PairType = std::pair<uint8_t, uint8_t>;

		[[nodiscard]] Child build(const Element &) const {
			std::vector<PairType> values{};
			for (const auto &promotion: Misc::promotions) {
				values.emplace_back(std::make_pair(promotion.minLevel, promotion.maxLevel));
			}

			return Selector<PairType>{
				.titlePrefix = titlePrefix,
				.valuePrefix = "Level",
				.getter = [this]() {
					auto &sheet = sheetGetter();
					if constexpr (std::derived_from<T, Stats::EngineSheet>) {
						return std::make_pair(sheet.level, getLvlFromPromotion(sheet.modification));
					} else {
						return std::make_pair(sheet.level, getLvlFromPromotion(sheet.promotion));
					}
				},
				.setter = [this](const PairType &val) {
					auto &agent = Store::agents.at(agentKey);
					auto &sheet = sheetGetter();
					sheet.level = val.first;
					if constexpr (std::derived_from<T, Stats::EngineSheet>) {
						return sheet.modification = getPromotionFromLvl(val.second);
					} else {
						sheet.promotion = getPromotionFromLvl(val.second);
					}
					agent.updateEvent.notify();
				},
				.printer = [](const PairType &val) {
					return std::format("{}/{}", val.first, val.second);
				},
				.values = values,
			};
		}
	};

	struct AgentLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;


		[[nodiscard]] Child build(const Element &) const {
			return LevelSelector<Stats::AgentSheet>{
				.titlePrefix = "Agent Level",
				.sheetGetter = [agentKey = agentKey]() -> Stats::AgentSheet & {
					return Store::agents.at(agentKey).state.stats.sheet;
				},
				.agentKey = agentKey,
			};
		}
	};

	struct EngineLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;


		[[nodiscard]] Child build(const Element &) const {
			return LevelSelector<Stats::EngineSheet>{
				.titlePrefix = "Engine Level",
				.sheetGetter = [agentKey = agentKey]() -> Stats::EngineSheet & {
					return Store::agents.at(agentKey).state.loadout().engine->sheet;
				},
				.agentKey = agentKey,
			};
		}
	};

	struct MindscapeSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey{};

		using Type = uint8_t;

		[[nodiscard]] Child build(const Element &) const {
			return Selector<Type>{
				.titlePrefix = "Mindscape",
				.valuePrefix = "Mindscape",
				.getter = [agentKey = agentKey]() {
					auto &sheet = Store::agents.at(agentKey).state.stats.sheet;
					return sheet.mindscape;
				},
				.setter = [agentKey = agentKey](const Type &val) {
					auto &agent = Store::agents.at(agentKey);
					auto &sheet = agent.state.stats.sheet;
					sheet.mindscape = val;
					agent.updateEvent.notify();
				},
				.printer = [](const Type &val) {
					return std::format("{}", val);
				},
				.values = {0, 1, 2, 3, 4, 5, 6},
			};
		}
	};

	struct OverclockSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey{};

		using Type = uint8_t;

		[[nodiscard]] Child build(const Element &) const {
			return Selector<Type>{
				.titlePrefix = "Overclock",
				.valuePrefix = "Overclock",
				.getter = [agentKey = agentKey]() {
					auto &sheet = Store::agents.at(agentKey).state.loadout().engine->sheet;
					return sheet.overclock;
				},
				.setter = [agentKey = agentKey](const Type &val) {
					auto &agent = Store::agents.at(agentKey);
					auto &sheet = agent.state.loadout().engine->sheet;
					sheet.overclock = val;
					agent.updateEvent.notify();
				},
				.printer = [](const Type &val) {
					return std::format("{}", val);
				},
				.values = {1, 2, 3, 4, 5},
			};
		}
	};

	struct SkillLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey{};
		using _Skills = decltype(Stats::AgentSheet::skills);
		_Skills::Type _Skills::*skill{};
		std::string_view prefix;
		bool isCore = false;

		using Type = uint32_t;

		[[nodiscard]] Child build(const Element &) const {
			return Selector<Type>{
				.titlePrefix = std::format("{} Level", prefix),
				.valuePrefix = "Level",
				.getter = [agentKey = agentKey, skill = skill]() {
					auto &agent = Store::agents.at(agentKey);
					auto &sheet = agent.state.stats.sheet;
					return std::invoke(skill, sheet.skills).constant;
				},
				.setter = [agentKey = agentKey, skill = skill](const Type &val) {
					auto &agent = Store::agents.at(agentKey);
					auto &sheet = agent.state.stats.sheet;
					std::invoke(skill, sheet.skills).constant = val;
					agent.updateEvent.notify();
				},
				.printer = [isCore = isCore](const Type &val) {
					return std::format("{}", isCore ? val : val + 1);
				},
				.values = std::ranges::views::iota(0u, isCore ? Misc::promotions.back().maxCore + 1u : Misc::promotions.back().maxSkill) | std::ranges::to<std::vector>(),
			};
		}
	};

	struct BasicSkillLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const {
			return SkillLevelSelector{
				.agentKey = agentKey,
				.skill = &decltype(Stats::AgentSheet::skills)::basic,
				.prefix = "Basic",
			};
		}
	};
	struct DodgeSkillLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const {
			return SkillLevelSelector{
				.agentKey = agentKey,
				.skill = &decltype(Stats::AgentSheet::skills)::dodge,
				.prefix = "Dodge",
			};
		}
	};
	struct AssistSkillLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const {
			return SkillLevelSelector{
				.agentKey = agentKey,
				.skill = &decltype(Stats::AgentSheet::skills)::assist,
				.prefix = "Assist",
			};
		}
	};
	struct SpecialSkillLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const {
			return SkillLevelSelector{
				.agentKey = agentKey,
				.skill = &decltype(Stats::AgentSheet::skills)::special,
				.prefix = "Special",
			};
		}
	};
	struct ChainSkillLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const {
			return SkillLevelSelector{
				.agentKey = agentKey,
				.skill = &decltype(Stats::AgentSheet::skills)::chain,
				.prefix = "Chain",
			};
		}
	};
	struct CoreSkillLevelSelector : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const {
			return SkillLevelSelector{
				.agentKey = agentKey,
				.skill = &decltype(Stats::AgentSheet::skills)::core,
				.prefix = "Core",
				.isCore = true,
			};
		}
	};
}// namespace UI