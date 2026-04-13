#include "displayStats.hpp"
#include "formula/operators.hpp"
#include "modifiers/statFactory.hpp"
#include "set.hpp"


namespace Modifiers::Artifact {
	using namespace Formula::Operators;
	struct DisplayStatsFrm : Formula::FormulaBase<float> {
		Formula::FloatNode sheetPre;
		Formula::FloatNode sheetPost;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = sheetPre
					 + sheetPost;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (sheetPre + sheetPost).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return sheetPre.eval(context)
				 + sheetPost.eval(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &display1() {
		static auto ret = statFactory<Formula::FloatNode, DisplayStatsFrm>(Set::sheet1PreMods(), Set::sheet1PostMods());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &display2() {
		static auto ret = statFactory<Formula::FloatNode, DisplayStatsFrm>(Set::sheet2PreMods(), Set::sheet2PostMods());
		return ret;
	}
}// namespace Modifiers::Artifact