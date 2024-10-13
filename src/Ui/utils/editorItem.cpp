#include "editorItem.hpp"
#include "align.hpp"
#include "container.hpp"
#include "stack.hpp"
#include "text.hpp"

using namespace squi;

UI::EditorItem::operator Child() const {
	return Stack{
		.widget = widget.withDefaultHeight(Size::Shrink),
		.children{
			Container{
				.widget{
					.width = Size::Shrink,
					.height = 32.f,
				},
				.child = Align{
					.xAlign = 0.f,
					.child = Text{.text = name},
				},
			},
			Align{
				.xAlign = 1.f,
				.yAlign = 0.f,
				.child = child,
			},
		},
	};
}