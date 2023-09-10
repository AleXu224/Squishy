#pragma once
#include "ICharacterData.hpp"
#include "character.hpp"
#include "node.hpp"
#include "observer.hpp"
#include "talent.hpp"
#include "widget.hpp"

namespace Squishy {
    struct NodeCard {
        // Args
        squi::Widget::Args widget;
        Nodes::NodesVec &nodes;
        std::shared_ptr<Character> character;
        ICharacterData::Conditionals &conditionals;
        Talent talent;
        squi::VoidObservable &observable;
        std::string_view name = "Node Card";

		struct Storage {
            // Data
            Nodes::NodesVec &nodes;
            std::shared_ptr<Character> character;
            bool shouldUpdate = false;
        };
    
        operator squi::Child() const;
    };
}