#pragma once

#include "character/key.hpp"
#include "dropdownButton.hpp"
#include "stats/loadout.hpp"
#include "store.hpp"
#include "widgetArgs.hpp"


namespace UI {
	template<class T>
	struct Selector {
		// Args
		std::string_view titlePrefix;
		std::string_view valuePrefix;
		std::function<T(void)> getter;
		std::function<void(const T &)> setter;
		std::function<std::string(const T &)> printer;
		std::vector<T> values;

		operator squi::Child() const {
			return squi::DropdownButton{
				.widget{
					.width = squi::Size::Expand,
				},
				.style = squi::ButtonStyle::Standard(),
				.text = std::format("{} {}", titlePrefix, printer(getter())),
				.items = [valuePrefix = valuePrefix, getter = getter, setter = setter, printer = printer, values = values]() {
					std::vector<squi::ContextMenu::Item> ret;
					ret.reserve(values.size());
					for (const auto &value: values) {
						ret.emplace_back(squi::ContextMenu::Item{
							.text = std::format("{} {}", valuePrefix, printer(value)),
							.content = [setter, value]() {
								setter(value);
							},
						});
					}
					return ret;
				}(),
			};
		}
	};

	[[nodiscard]] inline uint8_t getAscensionFromLvl(uint8_t lvl) {
		if (lvl <= 20) return 0;
		if (lvl <= 40) return 1;
		if (lvl <= 50) return 2;
		if (lvl <= 60) return 3;
		if (lvl <= 70) return 4;
		if (lvl <= 80) return 5;
		if (lvl <= 90) return 6;
		throw std::runtime_error("Invalid lvl");
	}
	[[nodiscard]] inline uint8_t getLvlFromAscension(uint8_t ascension) {
		if (ascension == 0) return 20;
		if (ascension == 1) return 40;
		if (ascension == 2) return 50;
		if (ascension == 3) return 60;
		if (ascension == 4) return 70;
		if (ascension == 5) return 80;
		if (ascension == 6) return 90;
		throw std::runtime_error("Invalid ascension");
	}

	template<class T>
	struct LevelSelector {
		// Args
		std::string_view titlePrefix;
		std::function<T &()> sheetGetter;
		Character::InstanceKey characterKey{};

		using PairType = std::pair<uint8_t, uint8_t>;

		operator squi::Child() const {
			return Selector<PairType>{
				.titlePrefix = titlePrefix,
				.valuePrefix = "Level",
				.getter = [sheetGetter = sheetGetter]() {
					auto &sheet = sheetGetter();
					return std::make_pair(sheet.level, getLvlFromAscension(sheet.ascension));
				},
				.setter = [characterKey = characterKey, sheetGetter = sheetGetter](const PairType &val) {
					auto &character = Store::characters.at(characterKey);
					auto &sheet = sheetGetter();
					sheet.level = val.first;
					sheet.ascension = getAscensionFromLvl(val.second);
					character.updateEvent.notify();
				},
				.printer = [](const PairType &val) {
					return std::format("{}/{}", val.first, val.second);
				},
				.values = {
					PairType{1, 20},
					{20, 20},
					{20, 40},
					{40, 40},
					{40, 50},
					{50, 50},
					{50, 60},
					{60, 60},
					{60, 70},
					{70, 70},
					{70, 80},
					{80, 80},
					{80, 90},
					{90, 90},
				},
			};
		}
	};

	struct CharacterLevelSelector {
		// Args
		Character::InstanceKey characterKey;

		operator squi::Child() const {
			return LevelSelector<Stats::CharacterSheet>{
				.titlePrefix = "Character Level",
				.sheetGetter = [characterKey = characterKey]() -> Stats::CharacterSheet & {
					return Store::characters.at(characterKey).state.stats.sheet;
				},
				.characterKey = characterKey,
			};
		}
	};

	struct WeaponLevelSelector {
		// Args
		Character::InstanceKey characterKey;

		operator squi::Child() const {
			return LevelSelector<Stats::WeaponSheet>{
				.titlePrefix = "Weapon Level",
				.sheetGetter = [characterKey = characterKey]() -> Stats::WeaponSheet & {
					return Store::characters.at(characterKey).state.loadout().weapon->sheet;
				},
				.characterKey = characterKey,
			};
		}
	};

	struct ConstellationSelector {
		// Args
		Character::InstanceKey characterKey{};

		using Type = uint8_t;

		operator squi::Child() const {
			return Selector<Type>{
				.titlePrefix = "Constellation",
				.valuePrefix = "Constellation",
				.getter = [characterKey = characterKey]() {
					auto &sheet = Store::characters.at(characterKey).state.stats.sheet;
					return sheet.constellation;
				},
				.setter = [characterKey = characterKey](const Type &val) {
					auto &character = Store::characters.at(characterKey);
					auto &sheet = character.state.stats.sheet;
					sheet.constellation = val;
					character.updateEvent.notify();
				},
				.printer = [](const Type &val) {
					return std::format("{}", val);
				},
				.values = {0, 1, 2, 3, 4, 5, 6},
			};
		}
	};
	struct RefinementSelector {
		// Args
		Character::InstanceKey characterKey{};

		using Type = uint8_t;

		operator squi::Child() const {
			return Selector<Type>{
				.titlePrefix = "Refinement",
				.valuePrefix = "Refinement",
				.getter = [characterKey = characterKey]() {
					auto &sheet = Store::characters.at(characterKey).state.loadout().weapon->sheet;
					return sheet.refinement;
				},
				.setter = [characterKey = characterKey](const Type &val) {
					auto &character = Store::characters.at(characterKey);
					auto &sheet = character.state.loadout().weapon->sheet;
					sheet.refinement = val;
					character.updateEvent.notify();
				},
				.printer = [](const Type &val) {
					return std::format("{}", val);
				},
				.values = {1, 2, 3, 4, 5},
			};
		}
	};
	struct TalentLevelSelector {
		// Args
		Character::InstanceKey characterKey{};
		using _Talents = decltype(Stats::CharacterSheet::talents);
		_Talents::Type _Talents::*talent{};
		std::string_view prefix;

		using Type = uint32_t;

		operator squi::Child() const {
			return Selector<Type>{
				.titlePrefix = std::format("{} Level", prefix),
				.valuePrefix = "Level",
				.getter = [characterKey = characterKey, talent = talent]() {
					auto &character = Store::characters.at(characterKey);
					auto &sheet = character.state.stats.sheet;
					return std::invoke(talent, sheet.talents).constant;
				},
				.setter = [characterKey = characterKey, talent = talent](const Type &val) {
					auto &character = Store::characters.at(characterKey);
					auto &sheet = character.state.stats.sheet;
					std::invoke(talent, sheet.talents).constant = val;
					character.updateEvent.notify();
				},
				.printer = [](const Type &val) {
					return std::format("{}", val + 1);
				},
				.values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
			};
		}
	};

	struct NormalTalentLevelSelector {
		// Args
		Character::InstanceKey characterKey;

		operator squi::Child() const {
			return TalentLevelSelector{
				.characterKey = characterKey,
				.talent = &decltype(Stats::CharacterSheet::talents)::normal,
				.prefix = "Normal",
			};
		}
	};
	struct SkillTalentLevelSelector {
		// Args
		Character::InstanceKey characterKey;

		operator squi::Child() const {
			return TalentLevelSelector{
				.characterKey = characterKey,
				.talent = &decltype(Stats::CharacterSheet::talents)::skill,
				.prefix = "Skill",
			};
		}
	};
	struct BurstTalentLevelSelector {
		// Args
		Character::InstanceKey characterKey;

		operator squi::Child() const {
			return TalentLevelSelector{
				.characterKey = characterKey,
				.talent = &decltype(Stats::CharacterSheet::talents)::burst,
				.prefix = "Burst",
			};
		}
	};
}// namespace UI