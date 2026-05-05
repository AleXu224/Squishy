#pragma once

#include "disc/key.hpp"
#include "agent/key.hpp"
#include "combo/key.hpp"
#include "glaze/glaze.hpp"// IWYU pragma: keep
#include "team/key.hpp"
#include "engine/key.hpp"


// Disc
template<>
struct glz::meta<::Disc::InstanceKey> {
	using T = ::Disc::InstanceKey;
	static constexpr auto value = object(
		&T::key
	);
};
template<>
struct glz::meta<::Disc::SetKey> {
	using T = ::Disc::SetKey;
	static constexpr auto value = object(
		&T::key
	);
};
// Agent
template<>
struct glz::meta<::Agent::InstanceKey> {
	using T = ::Agent::InstanceKey;
	static constexpr auto value = object(
		&T::key
	);
};
template<>
struct glz::meta<::Agent::DataKey> {
	using T = ::Agent::DataKey;
	static constexpr auto value = object(
		&T::key
	);
};
// Engine
template<>
struct glz::meta<::Engine::InstanceKey> {
	using T = ::Engine::InstanceKey;
	static constexpr auto value = object(
		&T::key
	);
};
template<>
struct glz::meta<::Engine::DataKey> {
	using T = ::Engine::DataKey;
	static constexpr auto value = object(
		&T::key
	);
};
// Team
template<>
struct glz::meta<::Team::InstanceKey> {
	using T = ::Team::InstanceKey;
	static constexpr auto value = object(
		&T::key
	);
};
// Combo
template<>
struct glz::meta<::Combo::InstanceKey> {
	using T = ::Combo::InstanceKey;
	static constexpr auto value = object(
		&T::key
	);
};