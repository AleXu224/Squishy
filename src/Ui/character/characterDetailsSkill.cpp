#include "characterDetailsSkill.hpp"

#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/skillEntry.hpp"
#include "Ui/utils/tooltip.hpp"

using namespace squi;
UI::DetailsSkill::operator squi::Child() const {
	using namespace squi;
	if (!displayCondition.eval(ctx)) return nullptr;

	auto entries = [&]() -> Children {
		Children ret{};

		bool transparent = true;

		if (!nodes.empty()) {
			Children ret2{};
			for (const auto &node: nodes) {
				// FIXME: transparent is gonna be off if one of the nodes is missing
				if (node.formula.eval(ctx) == 0.f) continue;
				ret2.emplace_back(UI::Tooltip{
					.message = node.formula.print(ctx),
					.child = UI::SkillEntry{
						.isTransparent = transparent = !transparent,
						.name = node.name,
						.value = node.formula.eval(ctx),
						.color = Node::getColor(node.data, ctx),
						.isPercentage = Node::isPercentage(node.data),
					},
				});
			}
			if (!ret2.empty()) {
				ret.emplace_back(Column{
					.widget{
						.padding = Padding{4.f},
					},
					.children = ret2,
				});
			}
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

		auto optionsChildren = [&]() {
			Children ret{};

			for (const auto &option: options.value()) {
				std::visit(
					Utils::overloaded{
						[&](Option::Boolean &opt) {
							if (opt.displayCondition && !opt.displayCondition.value().eval(ctx)) return;
							ret.emplace_back(UI::ToggleOption{
								.option = opt,
								.characterKey = characterKey,
								.ctx = ctx,
							});
						},
						[&](Option::ValueList &opt) {
							if (opt.displayCondition && !opt.displayCondition.value().eval(ctx)) return;
							ret.emplace_back(UI::ValueListOption{
								.option = opt,
								.characterKey = characterKey,
								.ctx = ctx,
							});
						},
					},
					option.second.get()
				);
			}

			return ret;
		}();

		if (optionsChildren.empty()) return ret;

		ret.emplace_back(Box{
			.widget{
				.height = Size::Shrink,
				.padding = Padding{4.f},
			},
			.color{1.f, 1.f, 1.f, 0.05f},
			.borderRadius{0.f, 0.f, 7.f, 7.f},
			.child = Column{
				.spacing = 4.f,
				.children = optionsChildren,
			},
		});

		return ret;
	}();

	if (entries.empty()) return nullptr;

	return UI::DisplayCard{
		.title = name,
		.subtitle = subtitle,
		.children = entries,
	};
}
