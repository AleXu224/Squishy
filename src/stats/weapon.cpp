#include "weapon.hpp"

#include "weapon/data.hpp"

Stats::Weapon::Weapon(const ::Weapon::Data &data) : data(&data), sheet(data.baseStats) {}
