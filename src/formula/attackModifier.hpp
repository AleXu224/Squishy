#pragma once

#include "formula/constant.hpp"
#include "formula/intermediary.hpp"


namespace Formula {
	enum class ModifierLocation {
		DMG,
		additiveDMG,
		multiplicativeDMG,
		critRate,
		critDMG,
	};

	template<IntermediaryLike T>
	[[nodiscard]] consteval auto Modifier(ModifierLocation location, T formula) {
		return std::tuple{
			formula * Constant(location == ModifierLocation::DMG ? 1.f : 0.f),
			formula * Constant(location == ModifierLocation::additiveDMG ? 1.f : 0.f),
			formula * Constant(location == ModifierLocation::multiplicativeDMG ? 1.f : 0.f),
			formula * Constant(location == ModifierLocation::critRate ? 1.f : 0.f),
			formula * Constant(location == ModifierLocation::critDMG ? 1.f : 0.f),
		};
	}

	template<size_t I, class U, size_t... Is>
	[[nodiscard]] consteval auto _iterateModifiers(U tupl, std::index_sequence<Is...>) {
		return (
			... + std::get<I>(std::get<Is>(tupl))
		);
	}

	template<class... T>
	[[nodiscard]] consteval auto Modifiers(T... modifiers) {
		return std::tuple{
			_iterateModifiers<0>(std::tuple<T...>{modifiers...}, std::make_index_sequence<sizeof...(T)>()),
			_iterateModifiers<1>(std::tuple<T...>{modifiers...}, std::make_index_sequence<sizeof...(T)>()),
			_iterateModifiers<2>(std::tuple<T...>{modifiers...}, std::make_index_sequence<sizeof...(T)>()),
			_iterateModifiers<3>(std::tuple<T...>{modifiers...}, std::make_index_sequence<sizeof...(T)>()),
			_iterateModifiers<4>(std::tuple<T...>{modifiers...}, std::make_index_sequence<sizeof...(T)>()),
		};
	}

}// namespace Formula