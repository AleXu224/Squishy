#pragma once

#include "memory"

namespace Utils {
	template<class>
	class function;

	template<class R, class... TArgs>
	class function<R(TArgs...)> {
		struct interface {
			constexpr virtual auto operator()(TArgs...) -> R = 0;
			constexpr virtual ~interface() = default;
		};

		template<class Fn>
		struct implementation final : interface {
			constexpr explicit(true) implementation(Fn fn) : fn{fn} {}
			constexpr auto operator()(TArgs... args) -> R { return fn(args...); }

		private:
			Fn fn{};
		};

	public:
		template<class Fn>
		constexpr function(Fn fn) : fn{std::make_unique<implementation<Fn>>(fn)} {}

		constexpr auto operator()(TArgs... args) const -> R {
			return (*fn)(args...);
		}

	private:
		std::unique_ptr<interface> fn{};
	};

	template<class>
	struct function_traits {};

	template<class R, class B, class... TArgs>
	struct function_traits<R (B:: *)(TArgs...) const> {
		using type = R(TArgs...);
	};

	template<class F>
	function(F) -> function<typename function_traits<decltype(&F::operator())>::type>;
}// namespace Utils