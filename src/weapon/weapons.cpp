#include "weapons.hpp"

#include "weapon/weapons/StaffOfTheScarletSands.hpp"


void Weapon::initWeapons() {
	list = {
		{Weapon::Datas::staffOfTheScarletSands.key, Weapon::Datas::staffOfTheScarletSands},
	};

	for (auto &weapon: list) {
		weapon.second.nodes = weapon.second.getNodes();
	}
}
