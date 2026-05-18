#pragma once

#include "core/core.hpp"
#include "cstdint"
#include "disc/instance.hpp"
#include "store.hpp"


namespace UI {
	using namespace squi;
	struct DiscCard : StatefulWidget {
		enum class Actions : uint8_t {
			list,
			agent,
			showcase,
		};

		// Args
		Key key;
		Args widget;
		Disc::InstanceKey disc;
		Actions actions = Actions::list;

		struct State : WidgetState<DiscCard> {
			VoidObserver discUpdateObserver;

			void observeDisc() {
				if (auto it = ::Store::discs.find(widget->disc); it != ::Store::discs.end()) {
					discUpdateObserver = it->second.updateEvent.observe([this]() {
						this->element->markNeedsRebuild();
					});
				}
			}

			void initState() override {
				observeDisc();
			}

			void widgetUpdated() override {
				observeDisc();
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI