#include "characterDetailsSkill.hpp"

#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/skillEntry.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/utils/trueFalse.hpp"

using namespace squi;
UI::DetailsSkill::operator squi::Child() const {
	using namespace squi;
	if (!displayCondition.eval(ctx)) return nullptr;

	return UI::DisplayCard{
		.title = name,
		.subtitle = subtitle,
		.children = [&]() -> Children {
			Children ret{};

			if (!nodes.empty()) {
				Children ret2{};
				for (const auto &[node, transparent]: std::views::zip(nodes, Utils::trueFalse)) {
					ret2.emplace_back(UI::Tooltip{
						.message = node.formula.print(ctx),
						.child = UI::SkillEntry{
							.isTransparent = transparent,
							.name = node.name,
							.value = node.formula.eval(ctx),
							.color = Node::getColor(node.data, ctx),
							.isPercentage = Node::isPercentage(node.data),
						},
					});
				}
				ret.emplace_back(Column{
					.widget{
						.padding = Padding{4.f},
					},
					.children = ret2,
				});
			}

			auto ret3 = modsGenerator->generate(ctx);
			if (!ret3.empty()) {
				ret.emplace_back(Column{
					.widget{.padding = 4.f},
					.children = ret3,
				});
			}

			if (!options.has_value() || options->empty())
				return ret;

			ret.emplace_back(Box{
				.widget{
					.height = Size::Shrink,
					.padding = Padding{4.f},
				},
				.color{1.f, 1.f, 1.f, 0.05f},
				.borderRadius{0.f, 0.f, 7.f, 7.f},
				.child = Column{
					.spacing = 4.f,
					.children = [&]() {
						Children ret{};

						for (const auto &option: options.value()) {
							std::visit(
								Utils::overloaded{
									[&](Option::Boolean &opt) {
										ret.emplace_back(UI::ToggleOption{
											.option = opt,
											.characterKey = characterKey,
										});
									},
									[&](Option::ValueList &opt) {
										ret.emplace_back(UI::ValueListOption{
											.option = opt,
											.characterKey = characterKey,
										});
									},
								},
								option.second.get()
							);
						}

						return ret;
					}(),
				},
			});

			return ret;
		}(),
	};
}
