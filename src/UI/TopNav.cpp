#include "UI/TopNav.hpp"
#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "gestureDetector.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "text.hpp"
#include <vector>

using namespace squi;
using namespace Squishy;

struct TopNavButton {
	// Args
	std::string_view name;
	ChildRef child;
	std::shared_ptr<TopNav::State> state;

	struct Storage {
		// Data
		ChildRef child;
		std::shared_ptr<TopNav::State> state;
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>(child, state);

		return GestureDetector{
			.onClick = [storage](auto) {
				if (auto selectedTab = storage->state->selectedTab.lock()) {
					selectedTab->setVisible(false);
				}
				if (auto newTab = storage->child.lock()) {
					newTab->setVisible(true);
				}
				storage->state->selectedTab = storage->child;
			},
			.child = Stack{
				.widget{
					.width = Size::Shrink,
				},
				.children{
					Align{
						.child = Text{
							.widget{
								.margin = Margin{12.f, 0.f},
							},
							.text = std::string(name),
							.fontSize = 14.f,
						},
					},
					Align{
						.yAlign = 1.f,
						.child = Box{
							.widget{
								.width = 16.f,
								.height = 3.f,
								.onUpdate = [state = state, storage](Widget &w){
									w.setVisible(state->selectedTab.lock() == storage->child.lock());
								},
							},
							.borderRadius = 1.5f,
						},
					},
				},
			},
		};
	}
};

Squishy::TopNav::operator squi::Child() const {
	auto state = std::make_shared<State>();
	if (!tabs.empty()) state->selectedTab = tabs.front().child;

	return Column{
		.widget{widget},
		.children{
			Row{
				.widget{
					.height = 48.f,
					.margin = 4.f,
				},
				.children = [&]() -> Children {
					std::vector<Child> ret{};
					ret.reserve(tabs.size());
					for (const auto &tab: tabs) {
						ret.push_back(TopNavButton{
							.name = tab.name,
							.child = tab.child,
							.state = state,
						});
					}
					return ret;
				}(),
			},
			Box{
				.widget{
					.height = 1.f,
				},
				.color = Color::RGBA(0, 0, 0, 0.1),
			},
			Box{
				.color = Color::RGBA(1, 1, 1, 0.04),
				.child = Stack{
					.children = [&]() -> Children {
						std::vector<Child> ret{};
						ret.reserve(tabs.size());
						for (const auto &tab: tabs) {
							tab.child->setVisible(false);
							ret.push_back(tab.child);
						}
						if (!ret.empty()) ret.front()->setVisible(true);
						return ret;
					}(),
				},
			},
		},
	};
}
