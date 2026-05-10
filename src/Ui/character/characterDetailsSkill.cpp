#include "characterDetailsSkill.hpp"

#include "UI/option/toggleOption.hpp"
#include "UI/option/valueListOption.hpp"
#include "UI/option/valueSliderOption.hpp"
#include "UI/utils/decodeModsSheet.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/skillEntry.hpp"
#include "widgets/container.hpp"
#include "widgets/gestureDetector.hpp"

#include "print"

using namespace squi;
[[nodiscard]] squi::core::Child UI::DetailsSkill::build(const Element &) const {
	if (!displayCondition.eval(ctx)) return Container{};

	auto entries = [&]() -> Children {
		Children ret{};
		Children skillEntries{};

		bool transparent = true;

		for (const auto &node: nodes) {
			if (std::holds_alternative<Node::ModsData>(node.data)) {
				const auto &data = std::get<Node::ModsData>(node.data);
				auto modsChildren = decodeModsSheet(data.mods, ctx, &transparent);
				skillEntries.insert(skillEntries.end(), modsChildren.begin(), modsChildren.end());
				continue;
			}

			if (node.formula.eval(ctx) == 0.f) continue;
			skillEntries.emplace_back(Gesture{
				.onClick = [out = node.formula.fold(ctx, {}).print(ctx)](const Gesture::State &state) {
					std::println("{}", out);
				},
				.child = UI::Tooltip{
					.text = node.formula.print(ctx),
					.child = UI::SkillEntry{
						.isTransparent = transparent = !transparent,
						.name = std::string(node.name),
						.value = node.formula.eval(ctx),
						.color = Node::getColor(node.data, ctx),
						.isPercentage = Node::isPercentage(node.data),
					},
				},
			});
		}

		auto ret3 = modsGenerator->generate(ctx, &transparent);
		skillEntries.insert(skillEntries.end(), ret3.begin(), ret3.end());

		if (modsSheet.has_value()) {
			auto ret2 = decodeModsSheet(modsSheet->get(), ctx, &transparent);
			skillEntries.insert(skillEntries.end(), ret2.begin(), ret2.end());
		}

		if (!skillEntries.empty()) {
			ret.emplace_back(Column{
				.widget{
					.padding = Padding{4.f},
				},
				.children = skillEntries,
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
							if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(ctx)) return;
							ret.emplace_back(UI::ToggleOption{
								.option = opt,
								.instanceKey = instanceKey,
								.ctx = ctx,
							});
						},
						[&](Option::ValueList &opt) {
							if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(ctx)) return;
							ret.emplace_back(UI::ValueListOption{
								.option = opt,
								.instanceKey = instanceKey,
								.ctx = ctx,
							});
						},
						[&](Option::ValueSlider &opt) {
							if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(ctx)) return;
							ret.emplace_back(UI::ValueSliderOption{
								.option = opt,
								.instanceKey = instanceKey,
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

	if (entries.empty()) return Container{};

	return UI::DisplayCard{
		.title = name,
		.subtitle = subtitle,
		.children = entries,
	};
}
