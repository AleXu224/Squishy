export const statMap = new Map<string, string>([
    ["Base ATK", "atk"],
    ["Base HP", "hp"],
    ["Base DEF", "def"],
    ["ATK", "atk_"],
    ["HP", "hp_"],
    ["DEF", "def_"],
    ["Anomaly Mastery", "am"],
    ["Anomaly Proficiency", "ap"],
    ["CRIT Rate", "cr"],
    ["CRIT DMG", "cd"],
    ["Energy Regen", "er"],
    ["Physical DMG Bonus", "physicalDmg"],
    ["Fire DMG Bonus", "fireDmg"],
    ["Electric DMG Bonus", "electricDmg"],
    ["Ice DMG Bonus", "iceDmg"],
    ["Ether DMG Bonus", "etherDmg"],

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