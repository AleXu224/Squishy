#include "UI/Card.hpp"
#include "box.hpp"

using namespace squi;
using namespace Squishy;

Card::operator Child() const {
    return Box{
        .widget = widget,
        .color = Color::RGBA(1, 1, 1, 0.04),
        .borderRadius = 4.f,
        .child = child,
    };
}