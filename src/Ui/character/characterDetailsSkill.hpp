#pragma once

#include "character/key.hpp"
#include "team/key.hpp"

#include "formula/constant.hpp"
#include "formula/formulaContext.hpp"

#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/helpers.hpp"

#include "modifiers/statFactory.hpp"

#include "Ui/utils/skillEntry.hpp"
#include "widgets/tooltip.hpp"
#include <map>


namespace UI {
	struct ModsGenerator {
		[[nodiscard]] virtual squi::Children generate(const Formula::Context &ctx) const { return {}; };

		virtual ~ModsGenerator() = default;
	};

	struct DerivedModsGenerator : public ModsGenerator {
		const Stats::Sheet<Formula::FloatNode> &sheet;

		DerivedModsGenerator(const Stats::Sheet<Formula::FloatNode> &sheet) : sheet(sheet) {}

		squi::Children generate(const Formula::Context &ctx) const {
			squi::Children ret{};

			bool transparent = true;
			auto evalFunc = [&](auto &&stat, Modifiers::SheetMemberIdentifier identifier) {
				auto message = stat.print(ctx);
				auto value = stat.eval(ctx);

				if (value == 0.f) return;
				ret.emplace_back(Tooltip{
					.text = std::move(message),
					.child = UI::SkillEntry{
						.isTransparent = transparent = !transparent,
						.name = identifier.getName(),
						.value = value,
						.color = squi::Color(0xFFFFFFFF),
						.isPercentage = identifier.isPercentage(),
					},
				});
			};

			for (const auto &stat: Stats::simple) {
				evalFunc(Stats::fromStat(sheet, stat), Stats::fromStat<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(stat));
			}
			for (const auto &element: Misc::damageElements) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromDamageElement(sheet, element, skill), Stats::fromDamageElement<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(element, skill));
				}
			}
			for (const auto &attackSource: Misc::attackSources) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromAttackSource(sheet, attackSource, skill), Stats::fromAttackSource<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(attackSource, skill));
				}
			}
			for (const auto &reaction: Misc::reactions) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromReaction(sheet, reaction, skill), Stats::fromReaction<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(reaction, skill));
				}
			}

			return ret;
		}
	};

	using namespace squi;
	struct DetailsSkill : StatelessWidget {
		// Args
		Key key;
		std::string name;
		std::string subtitle{};
		std::variant<Character::InstanceKey, Team::InstanceKey> instanceKey{};
		Formula::Context ctx;
		std::vector<Node::Types> nodes;
		std::optional<std::map<uint32_t, std::reference_wrapper<Option::Types>>> options;
		std::shared_ptr<UI::ModsGenerator> modsGenerator = std::make_shared<ModsGenerator>();
		Formula::BoolNode displayCondition = Formula::ConstantBool(true);

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI
