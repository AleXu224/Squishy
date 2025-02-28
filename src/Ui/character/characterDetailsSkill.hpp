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
#include "Ui/utils/tooltip.hpp"


namespace UI {
	struct ModsGenerator {
		[[nodiscard]] virtual squi::Children generate(const Formula::Context &ctx) const { return {}; };

		virtual ~ModsGenerator() = default;
	};

	template<auto sheet>
	struct DerivedModsGenerator : public ModsGenerator {
		squi::Children generate(const Formula::Context &ctx) const override {
			squi::Children ret{};

			bool transparent = true;
			auto evalFunc = [&](auto &&stat, Modifiers::SheetMemberIdentifier identifier) {
				auto message = stat.print(ctx);
				auto value = stat.eval(ctx);

				if (value == 0.f) return;
				ret.emplace_back(UI::Tooltip{
					.message = message,
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
				evalFunc(Stats::fromStat<sheet>(stat), Stats::fromStat<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(stat));
			}
			for (const auto &element: Misc::damageElements) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromDamageElement<sheet>(element, skill), Stats::fromDamageElement<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(element, skill));
				}
			}
			for (const auto &attackSource: Misc::attackSources) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromAttackSource<sheet>(attackSource, skill), Stats::fromAttackSource<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(attackSource, skill));
				}
			}
			for (const auto &reaction: Misc::reactions) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromReaction<sheet>(reaction, skill), Stats::fromReaction<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(reaction, skill));
				}
			}

			return ret;
		}
	};

	struct DetailsSkill {
		// Args
		std::string_view name;
		std::string_view subtitle{};
		std::variant<Character::InstanceKey, Team::InstanceKey> instanceKey{};
		const Formula::Context &ctx;
		const std::vector<Node::Types> &nodes;
		std::optional<std::unordered_map<uint32_t, std::reference_wrapper<Option::Types>>> options;
		std::shared_ptr<UI::ModsGenerator> modsGenerator = std::make_shared<ModsGenerator>();
		Formula::BoolNode displayCondition = Formula::ConstantBool(true);

		operator squi::Child() const;
	};
}// namespace UI
