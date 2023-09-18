#include "character.hpp"
#include "observer.hpp"
#include "widget.hpp"


namespace Squishy {
	struct CharacterDetails {
		// Args
		squi::Widget::Args widget;
		std::shared_ptr<Character> character;
		std::shared_ptr<squi::VoidObservable> statsChangedEvent;

		operator squi::Child() const;
	};
}// namespace Squishy