#pragma once

#include "combo/combo.hpp"
#include "core/core.hpp"
#include "store.hpp"
#include "widgets/textInput.hpp"

namespace UI {
	using namespace squi;
	struct ComboEditor : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Combo::Combo &combo;
		Character::InstanceKey characterKey;
		Formula::Context ctx;
		std::function<void(Combo::Combo)> onSave;

		struct State : WidgetState<ComboEditor> {
			std::vector<Combo::Option> store;
			Combo::Combo combo;
			VoidObserver characterUpdateEvent;
			VoidObservable comboUpdateEvent;
			VoidObserver comboUpdateEventObserver;
			VoidObservable closeEvent;
			TextInput::Controller nameController;
			Observer<const std::string &> nameObserver;

			void initState() override {
				combo = widget->combo;
				characterUpdateEvent = ::Store::characters.at(widget->characterKey).updateEvent.observe([this]() {
					setState([&]() {});
				});

				comboUpdateEventObserver = comboUpdateEvent.observe([this]() {
					setState([&]() {});
				});

				nameController.setText(combo.name);
				nameObserver = nameController.getTextObserver([this](const std::string &newVal) {
					combo.name = newVal;
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI