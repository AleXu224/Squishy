#include "ICharacter.hpp"
#include "glaze/glaze.hpp"// IWYU pragma: keep


template<>
struct glz::meta<Serialization::Good::ICharacter::Talent> {
	using T = Serialization::Good::ICharacter::Talent;
	static constexpr auto value = object("auto", &T::auto_, &T::skill, &T::burst);
};