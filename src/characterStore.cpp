#include "characterStore.hpp"
#include "Characters/cyno.hpp"
#include "Weapons/Staff_of_the_Scarlet_Sands.hpp"
#include "artifact.hpp"

using namespace Squishy;

const std::vector<ICharacterData> Store::Data::characters{
	Cyno,
};
const std::vector<IWeaponData> Store::Data::weapons{
	Staff_of_the_Scarlet_Sands,
};
std::vector<std::shared_ptr<Character>> Store::characters{};
std::vector<std::shared_ptr<Artifact>> Store::artifacts{};
