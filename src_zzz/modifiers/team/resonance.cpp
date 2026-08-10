#include "resonance.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/team.hpp"
#include "stats/value.hpp"

namespace Modifiers::Team {
	struct ResonanceFrm : Formula::FormulaBase<float> {
		SheetMember<Stats::Sheet<Stats::Value<float, 1>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(context.team.resonances).fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.add(std::format("Disc {}", member.getName()), {eval(context), member.isPercentage()});
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(context.team.resonances).get(context);
		}
	};
	struct EnemyResonanceFrm : Formula::FormulaBase<float> {
		EnemyMember<Stats::EnemySheet<Stats::Value<float, 1>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(context.team.resonancesEnemy).fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.add(std::format("Disc {}", member.getName()), {eval(context), member.isPercentage()});
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(context.team.resonancesEnemy).get(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &resonances() {
		static auto ret = statFactory<Formula::FloatNode, ResonanceFrm>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &resonancesEnemy() {
		static auto ret = enemyFactory<Formula::FloatNode, EnemyResonanceFrm>(EnemyPointerFactory<Stats::EnemySheet<Stats::Value<float, 1>>>{}, EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers::Team