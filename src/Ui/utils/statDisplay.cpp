#include "statDisplay.hpp"
#include "align.hpp"
#include "box.hpp"
#include "stack.hpp"
#include "text.hpp"

using namespace squi;

UI::StatDisplay::operator squi::Child() const {
	return Box{
		.widget{
			.height = 36.f,
			.margin = Margin{0.f},
			.padding = Padding{12.f, 0.f},
		},
		.color = isTransparent ? Color{1.f, 1.f, 1.f, 0.0419f} : Color{0.f, 0.f, 0.f, 0.f},
		.borderRadius{4.f},
		.child{
			Stack{
				.children{
					Align{
						.xAlign = 0.f,
						.child = Text{
							.text = Utils::Stringify(stat.stat),
						},
					},
					Align{
						.xAlign = 1.f,
						.child = Text{
							.widget{
								.onDebugLayout = [](){
									(void)1;
								},
							},
							.text = Utils::Stringify(stat),
						},
					},
				},
			},
		}
	};
}
