#include "engineCard.hpp"
#include "UI/engine/engineEditor.hpp"
#include "UI/engine/engineSelector.hpp"
#include "UI/utils/card.hpp"
#include "UI/utils/statDisplay.hpp"
#include "engine/data.hpp"
#include "misc/rarityToColor.hpp"
#include "store.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/image.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"


using namespace squi;

struct EngineHeader : StatelessWidget {
	// Args
	Key key;
	Engine::Instance &engine;

	[[nodiscard]] Child build(const Element &) const {
		return Box{
			.widget{
				.height = 137.f,
			},
			.color = Color::css(0xffffff, 0.0419f),
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Stack{
				.children{
					Image{
						.widget{
							.alignment = Alignment::CenterRight,
						},
						.fit = squi::Image::Fit::contain,
						.image = ImageProvider::fromFile(std::format("assets/Engines/{}/icon.png", engine.stats.data->name)),
					},
					Column{
						.widget{
							.height = Size::Shrink,
							.alignment = Alignment::CenterRight,
							.padding = Padding{12.f, 0.f},
						},
						.spacing = 4.f,
						.children{
							Text{
								.text = engine.stats.data->name,
								.fontSize = 16.f,
								.lineWrap = true,
							},
							Text{
								.text = std::format("Lvl {}", engine.stats.sheet.level),
								.fontSize = 20.f,
							},
							engine.stats.data->baseStats.rarity <= 2//
								? Child()
								: Text{
									  .text = std::format("Overclock {}", engine.stats.sheet.overclock),
									  .fontSize = 20.f,
								  },
						},
					},
				},
			},
		};
	}
};

struct EngineCardContent : StatelessWidget {
	// Args
	Key key;
	Args widget{};
	Engine::Instance &engine;
	UI::EngineCard::Actions actions;

	[[nodiscard]] Child build(const Element &element) const {
		Child header = EngineHeader{
			.engine = engine,
		};
		Child content = Column{
			.widget{
				.padding = Padding{4.f},
			},
			.children{
				UI::StatDisplay{
					.isTransparent = false,
					.stat{
						.stat = Stat::atk,
						.value = engine.stats.data->baseStats.getAtkAt(engine.stats.sheet.level, engine.stats.sheet.modification),
					},
				},
				UI::StatDisplay{
					.isTransparent = true,
					.stat = {
						.stat = engine.stats.data->baseStats.subStat.stat,
						.value = engine.stats.data->baseStats.getSubstatAt(engine.stats.sheet.level, engine.stats.sheet.modification),
					},
				},
			},
		};
		Child footer = Row{
			.widget{
				.height = Size::Shrink,
				.padding = 4.f,
			},
			.spacing = 4.f,
			.children{
				Button{
					.theme = Button::Theme::Standard(),
					.onClick = [&]() {
						Navigator::of(element).pushOverlay(UI::EngineEditor{
							.engine = engine,
							.onSubmit = [](const Engine::Instance &engine) {
								auto &instance = Store::engines.at(engine.instanceKey);
								instance.stats = engine.stats;
								instance.updateEvent.notify();
								for (auto &[_, agent]: Store::agents) {
									if (!engine.isUsedOn(agent.instanceKey)) continue;
									instance.stats.data->getOpts(agent.state.options);
									agent.updateEvent.notify();
								}
							},
						});
					},
					.child = "Edit",
				},
				actions == UI::EngineCard::Actions::list//
					? Button{
						  .theme = Button::Theme::Standard(),
						  .onClick = [this]() {
							  if (!engine.usedOn().empty()) {
								  std::println("Cannot delete a used engine");
								  return;
							  }

							  Store::engines.erase(engine.instanceKey);
							  Store::engineListUpdateEvent.notify();
						  },
						  .child = "Delete",
					  }
					: Child{},
			},
		};
		return Column{
			.children{
				header,
				content,
				actions != UI::EngineCard::Actions::showcase ? footer : Child{},
			},
		};
	}
};

squi::core::Child UI::EngineCard::State::build(const Element &element) {
	if (!widget->engine) {
		return Card{
			.widget = widget->widget,
			.borderColor = Color::css(0xffffff, 0.0419f),
			.child = Button{
				.widget{
					.alignment = Alignment::Center,
					.margin = 8.f,
				},
				.theme = Button::Theme::Accent(element),
				.onClick = [this]() {
					GlobalKey selectorKey{};
					Navigator::of(*this->element).pushOverlay(EngineSelector{
						.key = selectorKey,
						.onSelect = [this, selectorKey](Engine::DataKey key) {
							Engine::Instance instance{key, {++Store::lastEngineId}};
							auto &engine = Store::engines.insert({instance.instanceKey, instance}).first->second;

							if (this->widget->agentKey) {
								Store::agents.at(this->widget->agentKey).state.loadout().swapEngine(engine.instanceKey);
							}

							Store::engineListUpdateEvent.notify();
							Navigator::of(*this->element).popOverlay(selectorKey);

							Navigator::of(*this->element).pushOverlay(EngineEditor{
								.engine = engine,
								.onSubmit = [](const Engine::Instance &engine) {
									auto &instance = Store::engines.at(engine.instanceKey);

									instance.stats = engine.stats;
									instance.updateEvent.notify();
									for (auto &[_, agent]: Store::agents) {
										if (!instance.isUsedOn(agent.instanceKey)) continue;
										instance.stats.data->getOpts(agent.state.options);
										agent.updateEvent.notify();
									}
								},
							});
						},
					});
				},
				.child = "Add engine",
			},
		};
	}

	return Card{
		.widget = widget->widget,
		.borderColor = Misc::rarityToColor.at(widget->engine->stats.data->baseStats.rarity),
		.child = EngineCardContent{
			.engine = *widget->engine,
			.actions = widget->actions,
		},
	};
	;
}
