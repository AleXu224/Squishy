#pragma once

#include "disc/instance.hpp"
#include "core/core.hpp"
#include "cstdint"


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
		Disc::Instance &disc;
		Actions actions = Actions::list;

		struct State : WidgetState<DiscCard> {
			VoidObserver discUpdateObserver;

			void initState() override {
				discUpdateObserver = widget->disc.updateEvent.observe([this]() {
					this->element->markNeedsRebuild();
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI