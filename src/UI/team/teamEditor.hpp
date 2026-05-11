#pragma once

#include "core/core.hpp"

#include "team/instance.hpp"
#include "widgets/textInput.hpp"

namespace UI {
	using namespace squi;
	struct TeamEditor : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Team::Instance instance;
		std::function<void(const Team::Instance &)> onSubmit{};

		struct State : WidgetState<TeamEditor> {
			VoidObservable closeEvent{};
			Team::Instance team;
			TextInput::Controller nameController{};
			Observer<const std::string &> nameUpdateEvent{};

			void initState() override {
				team = widget->instance;

				nameController.setText(team.name);
				nameUpdateEvent = nameController.getTextObserver([this](const std::string &name) {
					setState([&]() {
						team.name = name;
					});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI