#pragma once
#include "ICharacterData.hpp"
#include "character.hpp"
#include "node.hpp"
#include "observer.hpp"
#include "talent.hpp"
#include "widget.hpp"
#include <memory>

namespace Squishy {
	struct NodeCard {
		// Args
		squi::Widget::Args widget;
		Nodes::NodesVec &nodes;
		std::shared_ptr<Character> character;
		ICharacterData::Conditionals &conditionals;
		Talent talent;
		std::weak_ptr<squi::VoidObservable> statsChangedEvent;
		std::string_view name = "Node Card";

		struct Storage {
			// Data
			Nodes::NodesVec &nodes;
			std::shared_ptr<Character> character;
			std::shared_ptr<squi::VoidObservable::Observer> observer;
			bool shouldUpdate = false;
			bool hasConditionals = true;
			bool hasNodes = true;
		};

		operator squi::Child() const;
	};
}// namespace Squishy