#pragma once
#include "node.hpp"
#include "statSheet.hpp"
#include "widget.hpp"
#include <variant>
#include <vector>

namespace Squishy {
    struct NodeCard {
        // Args
        squi::Widget::Args widget;
        Nodes::NodesVec &nodes;
        StatSheet &sheet;
        std::string_view name = "Node Card";
    
        struct Storage {
            // Data
        };
    
        operator squi::Child() const;
    };
}