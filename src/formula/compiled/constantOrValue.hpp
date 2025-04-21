#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"
#include "utility"

namespace Formula::Compiled {
	template<class T, FormulaLike V>
	struct ConstantOr {
		std::variant<Constant<T>, V> val;

		[[nodiscard]] T eval(const Formula::Context &context) const {
			switch (val.index()) {
				case 0:
					return std::get<0>(val).eval(context);
				case 1:
					return std::get<1>(val).eval(context);
			}
			std::unreachable();
		}

		[[nodiscard]] bool isConstant() const {
			switch (val.index()) {
				case 0:
					return std::get<0>(val).isConstant();
				case 1:
					return std::get<1>(val).isConstant();
			}
			std::unreachable();
		}
	};

	template<FormulaLike T, FormulaLike V>
	struct ValueOr {
		std::variant<T, V> val;

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			switch (val.index()) {
				case 0:
					return std::get<0>(val).eval(context);
				case 1:
					return std::get<1>(val).eval(context);
			}
			std::unreachable();
		}

		[[nodiscard]] bool isConstant() const {
			switch (val.index()) {
				case 0:
					return std::get<0>(val).isConstant();
				case 1:
					return std::get<1>(val).isConstant();
			}
			std::unreachable();
		}
	};

	template<class T, FormulaLike V>
	struct TeamConstantOr {
		std::array<ConstantOr<T, V>, 4> vals;

		[[nodiscard]] T eval(const Formula::Context &context) const {
			T ret{};
			for (const auto &val: vals) {
				ret += val.eval(context);
			}
			return ret;
		}

		[[nodiscard]] bool isConstant() const {
			for (const auto &val: vals) {
				if (!val.isConstant()) return false;
			}
			return true;
		}
	};
}// namespace Formula::Compiled