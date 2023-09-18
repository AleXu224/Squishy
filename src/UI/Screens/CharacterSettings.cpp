#include "UI/Screens/CharacterSettings.hpp"
#include "UI/DropdownButton.hpp"
#include "expander.hpp"
#include "scrollableFrame.hpp"


using namespace squi;
using namespace Squishy;


Squishy::CharacterSettings::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(statsChangedEvent, character);

	return ScrollableFrame{
		.scrollableWidget{
			.padding = 8.f,
		},
		.spacing = 8.f,
		.children{
			Expander{
				.heading = "Character Level",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.level,
					.onSelect = [storage](auto item) {
						if (item.value > 80) storage->character->sheet.ascension = 6;
						else if (item.value > 70)
							storage->character->sheet.ascension = 5;
						else if (item.value > 60)
							storage->character->sheet.ascension = 4;
						else if (item.value > 50)
							storage->character->sheet.ascension = 3;
						else if (item.value > 40)
							storage->character->sheet.ascension = 2;
						else if (item.value > 20)
							storage->character->sheet.ascension = 1;
						else
							storage->character->sheet.ascension = 0;
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items{
						{"1", 1},
						{"20", 20},
						{"40", 40},
						{"50", 50},
						{"60", 60},
						{"70", 70},
						{"80", 80},
						{"90", 90},
					},
				},
			},
			Expander{
				.heading = "Character Ascension",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.ascension,
					.onSelect = [storage](auto item) {
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items{
						{"0", 0},
						{"1", 1},
						{"2", 2},
						{"3", 3},
						{"4", 4},
						{"5", 5},
						{"6", 6},
					},
				},
			},
			Expander{
				.heading = "Constellation",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.constellation,
					.onSelect = [storage](auto item) {
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items = []() {
						std::vector<DropdownButton<uint8_t>::Item> ret{};
						for (uint8_t i = 0; i <= 6; ++i) {
							ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("C{}", i), i});
						}
						return ret;
					}(),
				},
			},
			Expander{
				.heading = "Weapon Level",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.weaponLevel,
					.onSelect = [storage](auto item) {
						if (item.value > 80) storage->character->sheet.weaponAscension = 6;
						else if (item.value > 70)
							storage->character->sheet.weaponAscension = 5;
						else if (item.value > 60)
							storage->character->sheet.weaponAscension = 4;
						else if (item.value > 50)
							storage->character->sheet.weaponAscension = 3;
						else if (item.value > 40)
							storage->character->sheet.weaponAscension = 2;
						else if (item.value > 20)
							storage->character->sheet.weaponAscension = 1;
						else
							storage->character->sheet.weaponAscension = 0;
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items{
						{"1", 1},
						{"20", 20},
						{"40", 40},
						{"50", 50},
						{"60", 60},
						{"70", 70},
						{"80", 80},
						{"90", 90},
					},
				},
			},
			Expander{
				.heading = "Weapon Ascension",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.weaponAscension,
					.onSelect = [storage](auto item) {
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items{
						{"0", 0},
						{"1", 1},
						{"2", 2},
						{"3", 3},
						{"4", 4},
						{"5", 5},
						{"6", 6},
					},
				},
			},
			Expander{
				.heading = "Weapon Refinement",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.weaponRefinement,
					.onSelect = [storage](auto item) {
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items = []() {
						std::vector<DropdownButton<uint8_t>::Item> ret{};
						for (uint8_t i = 1; i <= 5; ++i) {
							ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("R{}", i), i});
						}
						return ret;
					}(),
				},
			},
			Expander{
				.heading = "Normal Attack Level",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.talents.normal,
					.onSelect = [storage](auto item) {
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items = []() {
						std::vector<DropdownButton<uint8_t>::Item> ret{};
						for (uint8_t i = 1; i <= 10; ++i) {
							ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("Level {}", i), static_cast<uint8_t>(i - 1)});
						}
						return ret;
					}(),
				},
			},
			Expander{
				.heading = "Elemental Skill Level",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.talents.skill,
					.onSelect = [storage](auto item) {
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items = []() {
						std::vector<DropdownButton<uint8_t>::Item> ret{};
						for (uint8_t i = 1; i <= 10; ++i) {
							ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("Level {}", i), static_cast<uint8_t>(i - 1)});
						}
						return ret;
					}(),
				},
			},
			Expander{
				.heading = "Elemental Burst Level",
				.child = DropdownButton<uint8_t>{
					.value = storage->character->sheet.talents.burst,
					.onSelect = [storage](auto item) {
						storage->character->update();
						storage->statsChangedEvent->notify();
					},
					.items = []() {
						std::vector<DropdownButton<uint8_t>::Item> ret{};
						for (uint8_t i = 1; i <= 10; ++i) {
							ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("Level {}", i), static_cast<uint8_t>(i - 1)});
						}
						return ret;
					}(),
				},
			},
		},
	};
}
