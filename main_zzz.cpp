#include "formula/base.hpp"
#include "src_common/formula/clamp.hpp"
#include "src_common/formula/constant.hpp"
#include "src_common/formula/difference.hpp"
#include "src_common/formula/operators.hpp"
#include "src_common/formula/requires.hpp"
#include "src_common/formula/sum.hpp"


int main() {
	using namespace Formula::Operators;
	std::array<std::byte, sizeof(Formula::Context)> falseContext{};
	auto *ctx = std::bit_cast<Formula::Context *>(&falseContext);

	using namespace Formula;

	auto f1 = Constant{.value = 3.f};
	auto f2 = ConstantFlat{.value = 4.f};

	auto print = [&](std::string_view category, auto &&formula) {
		std::println("---- {} ----", category);
		std::println("Plain:");
		std::println("{}", formula.print(*ctx, Step::none));
		auto resEvaluated = formula.eval(*ctx);
		std::println("{}", resEvaluated);

		auto folded = formula.fold(*ctx, {});
		std::println("Folded:");
		std::println("{}", folded.print(*ctx, Step::none));
		auto resFoldedEvaluated = folded.eval(*ctx);
		std::println("{}", resFoldedEvaluated);
		if (resEvaluated != resFoldedEvaluated) {
			std::println("Evaluation mismatch!");
			exit(1);
		}
	};

	auto s = f1 + f2;
	auto d = f1 - f2;
	auto p = f1 * f2;
	auto q = f1 / f2;

	auto c1 = f1 == f2;
	auto c2 = f1 != f2;
	auto c3 = !c1;
	auto c4 = f1 < f2;
	auto c5 = f1 <= f2;
	auto c6 = f1 > f2;
	auto c7 = f1 >= f2;
	auto c8 = c1 && c2;
	auto c9 = c1 || c2;

	print("Sum", s);
	print("Difference", d);
	print("Product", p);
	print("Fraction", q);

	print("Equal", c1);
	print("NotEqual", c2);
	print("Not", c3);
	print("Less", c4);
	print("LessEqual", c5);
	print("Greater", c6);
	print("GreaterEqual", c7);
	print("And", c8);
	print("Or", c9);

	auto f = (f1 + f2) * (f1 - f2) / f2;
	print("Complex formula", f);

	auto trueConst = ConstantBool{.value = true};
	auto falseConst = ConstantBool{.value = false};
	auto if1 = IfElse{
		.requirement = trueConst,
		.trueVal = f1,
		.elseVal = f2,
	};
	auto if2 = IfElse{
		.requirement = falseConst,
		.trueVal = f1,
		.elseVal = f2,
	};
	print("IfTrue", if1);
	print("IfFalse", if2);

	auto clamp = Clamp{
		.val1 = f,
		.min = 0.f,
		.max = 5.f,
	};
	auto min = Min{
		.val1 = f,
		.val2 = ConstantFlat{.value = 2.f},
	};
	auto max = Max{
		.val1 = f,
		.val2 = ConstantFlat{.value = 2.f},
	};

	print("Clamp", clamp);
	print("Min", min);
	print("Max", max);
}