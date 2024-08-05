#pragma once

#include "stats/stat.hpp"
#include "widget.hpp"

namespace UI {
    struct StatDisplay {
        // Args
        bool isTransparent;
        StatValue stat;
    
        operator squi::Child() const;
    };

}