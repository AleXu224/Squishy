#include "instance.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/skillFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/value.hpp"


namespace Modifiers::Agent {
	template<uint32_t count>
	struct Frm {
		template<Stats::Sheet<Stats::Value<float, count>> Stats::AgentSheet::*location>
		struct InstanceStats : Formula::FormulaBase<float> {
			SheetMember<Stats::Sheet<Stats::Value<float, count>>> stat;
			SheetMemberIdentifier member;
			[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
				return stat.resolve(std::invoke(location, context.source.stats.sheet)).fold(context, args);
			}

			void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step prevStep) const {
				// descriptor.add("Agent Base", {eval(context), member.isPercentage()});
				stat.resolve(std::invoke(location, context.source.stats.sheet)).print(descriptor, context, prevStep);
			}

			[[nodiscard]] float eval(const Formula::Context &context) const {
				return stat.resolve(std::invoke(location, context.source.stats.sheet)).get(context);
			}
		};
	};

	struct InstanceSkills : Formula::FormulaBase<int32_t> {
		SkillMember<Skills<Stats::Value<int32_t, 2>>> stat;
		SheetMemberIdentifier member;

		[[nodiscard]] Formula::IntNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(context.source.stats.sheet.skills).fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.add("Agent Base", eval(context));
		}

		[[nodiscard]] int32_t eval(const Formula::Context &context) const {
			return stat.resolve(context.source.stats.sheet.skills).get(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &instanceBase() {
		static auto ret = statFactory<Formula::FloatNode, Frm<2>::InstanceStats<&Stats::AgentSheet::base>>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 2>>>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &instanceInitial() {
		static auto ret = statFactory<Formula::FloatNode, Frm<1>::InstanceStats<&Stats::AgentSheet::initial>>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &instanceCombat() {
		static auto ret = statFactory<Formula::FloatNode, Frm<2>::InstanceStats<&Stats::AgentSheet::combat>>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 2>>>{}, StatNameFactory{});
		return ret;
	}
	const Skills<Formula::IntNode> &instanceSkills() {
		static auto ret = skillFactory<Formula::IntNode, InstanceSkills>(SkillPointerFactory<Skills<Stats::Value<int32_t, 2>>>{}, SkillNameFactory{});
		return ret;
	}
}// namespace Modifiers::Agent