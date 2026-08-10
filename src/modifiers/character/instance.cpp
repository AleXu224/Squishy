#include "instance.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/value.hpp"

namespace Modifiers::Character {
	struct InstanceStats : Formula::FormulaBase<float> {
		SheetMember<Stats::Sheet<Stats::Value<float, 2>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(context.source.stats.sheet.stats).fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.add(std::format("Character Base {}", member.getName()), {eval(context), member.isPercentage()});
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(context.source.stats.sheet.stats).get(context);
		}
	};

	struct InstanceTalents : Formula::FormulaBase<int32_t> {
		TalentMember<Talents<Stats::Value<int32_t, 1>>> stat;
		SheetMemberIdentifier member;

		[[nodiscard]] Formula::IntNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(context.source.stats.sheet.talents).fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.add(std::format("Character Base {}", member.getName()), eval(context));
		}

		[[nodiscard]] int32_t eval(const Formula::Context &context) const {
			return stat.resolve(context.source.stats.sheet.talents).get(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &instanceStats() {
		static auto ret = statFactory<Formula::FloatNode, InstanceStats>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 2>>>{}, StatNameFactory{});
		return ret;
	}
	const Talents<Formula::IntNode> &instanceTalents() {
		static auto ret = talentFactory<Formula::IntNode, InstanceTalents>(TalentPointerFactory<Talents<Stats::Value<int32_t, 1>>>{}, TalentNameFactory{});
		return ret;
	}
}// namespace Modifiers::Character