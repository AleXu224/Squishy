export const propToStat = new Map<string, string>([
    ["FIGHT_PROP_BASE_HP", "baseHp"],
    ["FIGHT_PROP_BASE_ATTACK", "baseAtk"],
    ["FIGHT_PROP_BASE_DEFENSE", "baseDef"],
    ["FIGHT_PROP_HP", "hp"],
    ["FIGHT_PROP_ATTACK", "atk"],
    ["FIGHT_PROP_DEFENSE", "def"],
    ["FIGHT_PROP_HP_PERCENT", "hp_"],
    ["FIGHT_PROP_ATTACK_PERCENT", "atk_"],
    ["FIGHT_PROP_DEFENSE_PERCENT", "def_"],
    ["FIGHT_PROP_ELEMENT_MASTERY", "em"],
    ["FIGHT_PROP_CHARGE_EFFICIENCY", "er"],
    ["FIGHT_PROP_HEAL_ADD", "hb"],
    ["FIGHT_PROP_CRITICAL_HURT", "cd"],
    ["FIGHT_PROP_CRITICAL", "cr"],
    ["FIGHT_PROP_PHYSICAL_ADD_HURT", "physicalDmg"],
    ["FIGHT_PROP_GRASS_ADD_HURT", "dendroDmg"],
    ["FIGHT_PROP_ROCK_ADD_HURT", "geoDmg"],
    ["FIGHT_PROP_WIND_ADD_HURT", "anemoDmg"],
    ["FIGHT_PROP_WATER_ADD_HURT", "hydroDmg"],
    ["FIGHT_PROP_ICE_ADD_HURT", "cryoDmg"],
    ["FIGHT_PROP_ELEC_ADD_HURT", "electroDmg"],
    ["FIGHT_PROP_FIRE_ADD_HURT", "pyroDmg"],
]);

export const weaponType = new Map<string, string>([
    ["WEAPON_POLE", "Misc::WeaponType::polearm"],
    ["WEAPON_BOW", "Misc::WeaponType::bow"],
    ["WEAPON_CATALYST", "Misc::WeaponType::catalyst"],
    ["WEAPON_CLAYMORE", "Misc::WeaponType::claymore"],
    ["WEAPON_SWORD_ONE_HAND", "Misc::WeaponType::sword"],
]);

export const rarity = new Map<string, string>([
    ["QUALITY_ORANGE", "5"],
    ["QUALITY_PURPLE", "4"],
]);

export const growCurve = new Map<string, string>([
    ["GROW_CURVE_ATTACK_S4", "Curves::CharacterGrow::ATTACK_S4"],
    ["GROW_CURVE_ATTACK_S5", "Curves::CharacterGrow::ATTACK_S5"],
    ["GROW_CURVE_HP_S4", "Curves::CharacterGrow::HP_S4"],
    ["GROW_CURVE_HP_S5", "Curves::CharacterGrow::HP_S5"],
]);