#include "UI/Screens/CharacterDetails.hpp"
#include "UI/CharacterCard.hpp"
#include "UI/Masonry.hpp"
#include "UI/NodeCard.hpp"
#include "container.hpp"
#include "scrollableFrame.hpp"
#include "widget.hpp"

using namespace squi;
using namespace Squishy;


CharacterDetails::operator Child() const {
	return ScrollableFrame {
		.children{
			Container{
				.widget{
					.height = Size::Shrink,
				},
				.child = Masonry{
					.widget{
						.padding = 12.f,
					},
					.spacing = 12.f,
					.minChildWidth = 300.f,
					.children{
						CharacterCard{
							.sheet = character->sheet,
						},
						NodeCard{
							.widget{
								.height = Size::Shrink,
							},
							.nodes = character->nodes.normal,
							.character = character,
							.conditionals = character->conditionals,
							.talent = Talent::Normal,
							.statsChangedEvent = statsChangedEvent,
							.name = "Normal Attack",
						},
						NodeCard{
							.widget{
								.height = Size::Shrink,
							},
							.nodes = character->nodes.charged,
							.character = character,
							.conditionals = character->conditionals,
							.talent = Talent::Charged,
							.statsChangedEvent = statsChangedEvent,
							.name = "Charged Attack",
						},
						NodeCard{
							.widget{
								.height = Size::Shrink,
							},
							.nodes = character->nodes.plunge,
							.character = character,
							.conditionals = character->conditionals,
							.talent = Talent::Plunge,
							.statsChangedEvent = statsChangedEvent,
							.name = "Plunge Attack",
						},
						NodeCard{
							.widget{
								.height = Size::Shrink,
							},
							.nodes = character->nodes.skill,
							.character = character,
							.conditionals = character->conditionals,
							.talent = Talent::Skill,
							.statsChangedEvent = statsChangedEvent,
							.name = "Skill",
						},
						NodeCard{
							.widget{
								.height = Size::Shrink,
							},
							.nodes = character->nodes.burst,
							.character = character,
							.conditionals = character->conditionals,
							.talent = Talent::Burst,
							.statsChangedEvent = statsChangedEvent,
							.name = "Burst",
						},
						NodeCard{
							.widget{
								.height = Size::Shrink,
							},
							.nodes = character->nodes.passive1,
							.character = character,
							.conditionals = character->conditionals,
							.talent = Talent::Passive1,
							.statsChangedEvent = statsChangedEvent,
							.name = "Passive 1",
						},
						NodeCard{
							.widget{
								.height = Size::Shrink,
							},
							.nodes = character->nodes.passive2,
							.character = character,
							.conditionals = character->conditionals,
							.talent = Talent::Passive2,
							.statsChangedEvent = statsChangedEvent,
							.name = "Passive 2",
						},
					},
				},
			},
		},
	};
}
