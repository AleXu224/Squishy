#include "enginePage.hpp"
#include "UI/engine/engineEditor.hpp"
#include "UI/engine/engineSelector.hpp"
#include "engine/data.hpp"
#include "engineCard.hpp"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/grid.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"


using namespace squi;

squi::core::Child UI::EnginePage::State::build(const Element &element) {
	Child addEngineButton = Button{
		.theme = Button::Theme::Accent(element),
		.onClick = [this]() {
			Navigator::of(*this->element).pushOverlay(EngineSelector{
				.onSelect = [this](Engine::DataKey key) {
					Engine::Instance instance{key, {++Store::lastEngineId}};
					auto &engine = Store::engines.insert({instance.instanceKey, instance}).first->second;
					Store::engineListUpdateEvent.notify();

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
	};

	Child buttonBar = Row{
		.widget{.height = Size::Shrink},
		.children{
			addEngineButton,
		},
	};

	return ScrollView{
		.scrollWidget{
			.padding = 8.f,
		},
		.alignment = Flex::Alignment::center,
		.spacing = 8.f,
		.children{
			buttonBar,
			Grid{
				.widget{
					.height = Size::Shrink,
					.sizeConstraints = BoxConstraints{
						.maxWidth = 1520.f,
					},
				},
				.columnCount = Grid::MinSize{.value = 256.f},
				.spacing = 2.f,
				.children = []() {
					Children ret;
					for (auto &[_, engine]: ::Store::engines) {
						ret.emplace_back(UI::EngineCard{
							.engine = &engine,
						});
					}
					return ret;
				}(),
			}
		},
	};
}
