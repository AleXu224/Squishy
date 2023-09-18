#include "character.hpp"
#include "observer.hpp"
#include "widget.hpp"
#include <memory>

namespace Squishy {
	struct CharacterSettings {
		// Args
		squi::Widget::Args widget;
		std::shared_ptr<squi::VoidObservable> statsChangedEvent;
		std::shared_ptr<Character> character;

		struct Storage {
			// Data
			std::shared_ptr<squi::VoidObservable> statsChangedEvent;
			std::shared_ptr<Character> character;
		};

		operator squi::Child() const;
	};
}// namespace Squishy