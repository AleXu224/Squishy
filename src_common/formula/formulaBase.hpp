#pragma once

#include "type.hpp"


namespace Formula {
	struct Context;
	struct FoldArgs;

	template<class T, class Ret>
	concept HasConstantValue = std::is_same_v<std::remove_cvref_t<decltype(std::declval<T>().value)>, Ret>;

	template<class RetType>
	struct NodeType;

	template<class T>
	using FormulaType = decltype(std::declval<T>().eval(std::declval<const Context &>()));

	template<class T>
	concept HasFold = requires(T t) {
		{ t.fold(std::declval<const Context &>(), std::declval<const FoldArgs &>()) } -> std::same_as<NodeType<FormulaType<T>>>;
	};

	template<class T, bool percentage = false>
	struct ConstantBase;

	template<class T, Type type_ = Type::none>
	struct FormulaBase {
		static constexpr Type type = type_;

		[[nodiscard]] consteval Type getType() const {
			return type;
		}

		[[nodiscard]] constexpr T getConstantValue(this auto &&self) {
			if constexpr (HasConstantValue<std::remove_cvref_t<decltype(self)>, T>) {
				return self.value;
			} else {
				return {};
			}
		}

		[[nodiscard]] NodeType<T> fold(this auto &&self, const Context &context, const FoldArgs &args) {
			using Fn = std::remove_cvref_t<decltype(self)>;
			if constexpr (HasConstantValue<Fn, T>) {
				return NodeType(self);
			} else if constexpr (self.type == Formula::Type::constant) {
				return NodeType(ConstantBase<T>{.value = self.eval(context)});
			} else {
				static_assert(false, "Type does not support folding, consider adding a fold method or making the base class a constant type");
			}
		}

		using RetType = T;
	};
}// namespace Formula