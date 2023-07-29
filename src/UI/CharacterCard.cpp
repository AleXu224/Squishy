#include "UI/CharacterCard.hpp"
#include "UI/Card.hpp"
#include "UI/Screens/CharacterScreen.hpp"
#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "container.hpp"
#include "row.hpp"
#include "statSheet.hpp"
#include "text.hpp"
#include <format>

using namespace squi;
using namespace Squishy;

struct StatDisplay {
	// Args
	StatSheet &sheet;
	StatSheet::Stat &stat;
	std::string_view name;
    bool isTransparent = false;
    bool isPercent = false;

	struct Storage {
		// Data
		StatSheet &sheet;
		StatSheet::Stat &stat;
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>(sheet, stat);

		return Box{
			.widget{
				.height = 36.f,
			},
			.color = isTransparent ? Color::HEX(0x00000000) : Color::RGBA(1, 1, 1, 0.04),
			.borderRadius = 4.f,
			.child{
				Row{
					.widget{
						.padding = Padding{0.f, 12.f, 0.f, 12.f},
					},
					.alignment = Row::Alignment::center,
					.children{
						Text{
							.text = std::string{name},
							.fontSize = 14.f,
							.lineWrap = true,
						},
						Container{
							.widget{
								.width = 12.f,
							},
						},
						Text{
							.widget{
								.onUpdate = [storage, isPercent = isPercent](Widget &widget) {
									reinterpret_cast<Text::Impl &>(widget).setText(std::format("{:.2f}{}", storage->stat.get(storage->sheet) * (isPercent ? 100.f : 1.f), isPercent ? "%" : ""));
								},
							},
							.fontSize = 14.f,
						},
					},
				},
			},
		};
	}
};

StatsDisplay::operator Child() const {
	return Column{
			   .widget = widget,
			   .spacing = 4.f,
			   .children{
				   StatDisplay{sheet, sheet.stats.HP, "HP", true},
				   StatDisplay{sheet, sheet.stats.ATK, "ATK"},
				   StatDisplay{sheet, sheet.stats.DEF, "DEF", true},
				   StatDisplay{sheet, sheet.stats.EM, "Elemental Mastery"},
				   StatDisplay{sheet, sheet.stats.ER, "Energy Recharge", true, true},
				   StatDisplay{sheet, sheet.stats.All.CritRate, "Crit Rate", false, true},
				   StatDisplay{sheet, sheet.stats.All.CritDMG, "Crit DMG", true, true},
				   StatDisplay{sheet, sheet.stats.HB, "Healing Bonus", false, true},
				   [&]() -> Child {
					   switch (sheet.element) {
						   case Element::Anemo:
							   return StatDisplay{sheet, sheet.stats.Anemo.DMG, "Anemo DMG Bonus", true, true};
						   case Element::Cryo:
							   return StatDisplay{sheet, sheet.stats.Cryo.DMG, "Cryo DMG Bonus", true, true};
						   case Element::Dendro:
							   return StatDisplay{sheet, sheet.stats.Dendro.DMG, "Dendro DMG Bonus", true, true};
						   case Element::Electro:
							   return StatDisplay{sheet, sheet.stats.Electro.DMG, "Electro DMG Bonus", true, true};
						   case Element::Geo:
							   return StatDisplay{sheet, sheet.stats.Geo.DMG, "Geo DMG Bonus", true, true};
						   case Element::Hydro:
							   return StatDisplay{sheet, sheet.stats.Hydro.DMG, "Hydro DMG Bonus", true, true};
						   case Element::Pyro:
							   return StatDisplay{sheet, sheet.stats.Pyro.DMG, "Pyro DMG Bonus", true, true};
						   default:
							   std::unreachable();
					   }
				   }(),
			   },
		   };
}

CharacterCard::operator Child() const {
	return Card{
		.widget{
			.height = Size::Shrink,
		},
		.child{
			Column{
				.children{
					Box{
						.widget{
							.height = 64.f,
							.padding = Padding{0.f, 16.f, 0.f, 16.f},
						},
						.color = Color::RGBA(1, 1, 1, 0.08),
						.shouldClipContent = false,
						.child = Align{
							.xAlign = 0.f,
							.yAlign = 0.5f,
							.child = Text{
								.text = "Basic Stats",
								.fontSize = 24.f,
							},
						},
					},
					StatsDisplay{
						.widget{
							.height = Size::Shrink,
							.margin = 4.f,
						},
						.sheet = sheet,
					},
				},
			},
		},
	};
}