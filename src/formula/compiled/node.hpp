#pragma once

#include "formula/formulaContext.hpp"
#include "memory"
#include "misc/element.hpp"
#include "type.hpp"
#include "utils/optional.hpp"


namespace Formula::Compiled {
	template<class T, class Ret>
	concept HasConstantValue = std::is_same_v<std::remove_cvref_t<decltype(std::declval<T>().value)>, Ret>;

	template<class T>
	concept isMonomial = requires(T t) {
		{ t.add(std::declval<typename T::RetType>()) };
		{ t.subtract(std::declval<typename T::RetType>()) };
		{ t.mult(std::declval<typename T::RetType>()) };
		{ t.divide(std::declval<typename T::RetType>()) };
	};

	template<class T, class Ret>
	concept isSumMonomial = std::is_same_v<std::remove_cvref_t<decltype(std::declval<T>().sumParam)>, Ret>;
	template<class T, class Ret>
	concept isProdMonomial = std::is_same_v<std::remove_cvref_t<decltype(std::declval<T>().multParam)>, Ret>;

	template<class T>
	concept wrapable = requires(T t) {
		{ t.wrap() };
	};

	template<class RetType>
	struct NodeType {
		using Type = RetType;

		struct interface {
			interface() = default;
			[[nodiscard]] interface(const interface &) = delete;
			interface(interface &&) = delete;
			interface &operator=(const interface &) = delete;
			interface &operator=(interface &&) = delete;

			[[nodiscard]] constexpr virtual RetType eval(const Context &) const = 0;
			[[nodiscard]] constexpr virtual Formula::Compiled::Type getType() const = 0;
			[[nodiscard]] constexpr virtual std::unique_ptr<interface> clone() const = 0;
			[[nodiscard]] constexpr virtual Type getConstantValue() const = 0;
			[[nodiscard]] constexpr virtual std::string print() const = 0;

			[[nodiscard]] constexpr virtual NodeType add(RetType) const = 0;
			[[nodiscard]] constexpr virtual NodeType subtract(RetType) const = 0;
			[[nodiscard]] constexpr virtual NodeType mult(RetType) const = 0;
			[[nodiscard]] constexpr virtual NodeType divide(RetType) const = 0;
			[[nodiscard]] constexpr virtual RetType getSumParam() const = 0;
			[[nodiscard]] constexpr virtual RetType getMultParam() const = 0;
			[[nodiscard]] constexpr virtual NodeType getMonomialValue() const = 0;
			constexpr virtual ~interface() = default;
		};

		template<class Fn>
		struct implementation final : interface {
			constexpr explicit(true) implementation(Fn fn) : fn{fn} {}

			[[nodiscard]] constexpr RetType eval(const Context &context) const override {
				return fn.eval(context);
			}
			[[nodiscard]] constexpr Formula::Compiled::Type getType() const override {
				return fn.getType();
			}
			[[nodiscard]] constexpr Type getConstantValue() const override {
				if constexpr (HasConstantValue<Fn, RetType>) {
					return fn.value;
				} else {
					return {};
				}
			}
			[[nodiscard]] constexpr std::string print() const override {
				return fn.print();
			}
			[[nodiscard]] constexpr NodeType add(RetType val) const override {
				if constexpr (isMonomial<Fn>) {
					return fn.add(val).wrap();
				} else if constexpr (wrapable<Fn>) {
					return fn.wrap();
				} else {
					return fn;
				}
			}
			[[nodiscard]] constexpr NodeType subtract(RetType val) const override {
				if constexpr (isMonomial<Fn>) {
					return fn.subtract(val).wrap();
				} else if constexpr (wrapable<Fn>) {
					return fn.wrap();
				} else {
					return fn;
				}
			}
			[[nodiscard]] constexpr NodeType mult(RetType val) const override {
				if constexpr (isMonomial<Fn>) {
					return fn.mult(val).wrap();
				} else if constexpr (wrapable<Fn>) {
					return fn.wrap();
				} else {
					return fn;
				}
			}
			[[nodiscard]] constexpr NodeType divide(RetType val) const override {
				if constexpr (isMonomial<Fn>) {
					return fn.divide(val).wrap();
				} else if constexpr (wrapable<Fn>) {
					return fn.wrap();
				} else {
					return fn;
				}
			}
			[[nodiscard]] constexpr RetType getSumParam() const override {
				if constexpr (isSumMonomial<Fn, RetType>) {
					return fn.sumParam;
				} else {
					return {};
				}
			}
			[[nodiscard]] constexpr RetType getMultParam() const override {
				if constexpr (isProdMonomial<Fn, RetType>) {
					return fn.multParam;
				} else {
					return {};
				}
			}
			[[nodiscard]] constexpr NodeType getMonomialValue() const override {
				if constexpr (isMonomial<Fn>) {
					return std::move(fn.value);
				} else if constexpr (wrapable<Fn>) {
					return fn.wrap();
				} else {
					return fn;
				}
			}
			[[nodiscard]] constexpr std::unique_ptr<interface> clone() const override {
				return std::make_unique<implementation<Fn>>(fn);
			}

		private:
			Fn fn{};
		};

		template<class T>
		constexpr NodeType(const T &t)
			: fn(std::make_unique<implementation<T>>(t)) {}

		[[nodiscard]] constexpr RetType eval(const Context &context) const {
			return fn->eval(context);
		}
		[[nodiscard]] constexpr Formula::Compiled::Type getType() const {
			return fn->getType();
		}
		[[nodiscard]] constexpr RetType getConstantValue() const {
			return fn->getConstantValue();
		}
		[[nodiscard]] constexpr std::string print() const {
			return fn->print();
		}
		[[nodiscard]] constexpr NodeType add(RetType value) const {
			return fn->add(value);
		}
		[[nodiscard]] constexpr NodeType subtract(RetType value) const {
			return fn->subtract(value);
		}
		[[nodiscard]] constexpr NodeType mult(RetType value) const {
			return fn->mult(value);
		}
		[[nodiscard]] constexpr NodeType divide(RetType value) const {
			return fn->divide(value);
		}
		[[nodiscard]] constexpr RetType getSumParam() const {
			return fn->getSumParam();
		}
		[[nodiscard]] constexpr RetType getMultParam() const {
			return fn->getMultParam();
		}
		[[nodiscard]] constexpr NodeType getMonomialValue() const {
			return fn->getMonomialValue();
		}

		NodeType() = default;

		~NodeType() = default;

		[[nodiscard]] constexpr bool hasValue() const {
			return fn != nullptr;
		}

	private:
		std::shared_ptr<interface> fn{};
	};

	using FloatNode = NodeType<float>;
	using BoolNode = NodeType<bool>;
	using IntNode = NodeType<int32_t>;
	using ElementNode = NodeType<Utils::JankyOptional<Misc::Element>>;
}// namespace Formula::Compiled