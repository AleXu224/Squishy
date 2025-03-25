#pragma once

#include "artifact/key.hpp"
#include "character/key.hpp"
#include "combo/key.hpp"
#include "glaze/glaze.hpp"// IWYU pragma: keep
#include "team/key.hpp"
#include "weapon/key.hpp"


// Artifact
template<>
struct glz::meta<::Artifact::InstanceKey> {
	using T = ::Artifact::InstanceKey;
	static constexpr auto value = object(
		&T::key
	);
};
template<>
struct glz::meta<::Artifact::SetKey> {
	using T = ::Artifact::SetKey;
	static constexpr auto value = object(
		&T::key
	);
};
// Character
template<>
struct glz::meta<::Character::InstanceKey> {
	using T = ::Character::InstanceKey;
	static constexpr auto value = object(
		&T::key
	);
};
template<>
struct glz::meta<::Character::DataKey> {
	using T = ::Character::DataKey;
	static constexpr auto value = object(
		&T::key
	);
};
// Weapon
template<>
struct glz::meta<::Weapon::InstanceKey> {
	using T = ::Weapon::InstanceKey;
	static constexpr auto value = object(
		&T::key
	);
};
template<>
struct glz::meta<::Weapon::DataKey> {
	using T = ::Weapon::DataKey;
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