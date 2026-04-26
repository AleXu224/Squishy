#pragma once

#include "agent/key.hpp"
#include "team/key.hpp"

#include "formula/base.hpp"
#include "formula/constant.hpp"


#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/helpers.hpp"

#include "modifiers/statFactory.hpp"

#include "UI/utils/skillEntry.hpp"
#include "widgets/tooltip.hpp"
#include <map>


namespace UI {
	struct ModsGenerator {
		[[nodiscard]] virtual squi::Children generate(const Formula::Context &ctx, bool *transparent = nullptr) const { return {}; };

		virtual ~ModsGenerator() = default;
	};

	struct DerivedModsGenerator : public ModsGenerator {
		const Stats::Sheet<Formula::FloatNode> &sheet;

		DerivedModsGenerator(const Stats::Sheet<Formula::FloatNode> &sheet) : sheet(sheet) {}

		squi::Children generate(const Formula::Context &ctx, bool *transparentP) const {
			squi::Children ret{};

			bool transparentBackup;
			if (!transparentP) transparentP = &transparentBackup;
			auto &transparent = *transparentP;

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
			for (const auto &attribute: Misc::damageAttributes) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromDamageAttribute(sheet, attribute, skill), Stats::fromDamageAttribute<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(attribute, skill));
				}
			}
			for (const auto &attackSource: Misc::attackSources) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromAttackSource(sheet, attackSource, skill), Stats::fromAttackSource<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(attackSource, skill));
				}
			}
			for (const auto &anomaly: Misc::damageAnomalies) {
				for (const auto &skill: Misc::skillStats) {
					evalFunc(Stats::fromAnomaly(sheet, anomaly, skill), Stats::fromAnomaly<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(anomaly, skill));
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
		std::variant<Agent::InstanceKey, Team::InstanceKey> instanceKey{};
		Formula::Context ctx;
		std::vector<Node::Types> nodes;
		std::optional<std::map<uint32_t, std::reference_wrapper<Option::Types>>> options;
		std::optional<std::reference_wrapper<const Stats::ModsSheet>> modsSheet{};
		std::shared_ptr<UI::ModsGenerator> modsGenerator = std::make_shared<ModsGenerator>();
		Formula::BoolNode displayCondition = Formula::ConstantBool({}, true);

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI
