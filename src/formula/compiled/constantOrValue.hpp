#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<class T, FormulaLike V>
	struct ConstantOr {
		std::variant<Constant<T>, V> val;

		[[nodiscard]] T eval(const Formula::Context &context) const {
			return std::visit(
				[&](auto &&val) {
					return val.eval(context);
				},
				val
			);
		}

		[[nodiscard]] bool isConstant() const {
			return std::visit(
				[](auto &&val) {
					return val.isConstant();
				},
				val
			);
		}
	};

	template<FormulaLike... T>
	struct ValueOr {
		std::variant<T...> val;

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return std::visit(
				[&](auto &&val) {
					return val.eval(context);
				},
				val
			);
		}

		[[nodiscard]] bool isConstant() const {
			return std::visit(
				[](auto &&val) {
					return val.isConstant();
				},
				val
			);
		}
	};

	template<class T, FormulaLike V>
	struct TeamConstantOr {
		std::array<std::variant<Constant<T>, V>, 4> vals;

		[[nodiscard]] T eval(const Formula::Context &context) const {
			T ret{};
			for (const auto &val: vals) {
				ret += std::visit(
					[&](auto &&val) {
						return val.eval(context);
					},
					val
				);
			}
			return ret;
		}

		[[nodiscard]] bool isConstant() const {
			for (const auto &val: vals) {
				auto constant = std::visit(
					[](auto &&val) {
						return val.isConstant();
					},
					val
				);
				if (!constant) return false;
			}
			return true;
		}
	};
}// namespace Formula::Compiled