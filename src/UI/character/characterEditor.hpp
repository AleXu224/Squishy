#pragma once

#include "core/core.hpp"

#include "character/instance.hpp"

namespace UI {
	using namespace squi;
	struct CharacterEditor : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::Instance character;
		std::function<void(Character::Instance)> onSubmit{};

		struct State : WidgetState<CharacterEditor> {
			VoidObservable closeEvent{};
			std::optional<Character::Instance> character;

			void initState() override {
				character.emplace(widget->character);
			}

			void clampAscension();
			void clampTalents();

			Child build(const Element &element) override;
		};
	};
}// namespace UI