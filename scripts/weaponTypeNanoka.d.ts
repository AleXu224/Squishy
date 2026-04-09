export interface Weapon {
    name: string;
    desc: string;
    weapon_type: string;
    weapon_prop: WeaponProp[];
    rarity: number;
    icon: string;
    stats_modifier: StatsModifier;
    xp_requirements: { [key: string]: number };
    ascension: { [key: string]: Ascension };
    refinement: { [key: string]: Refinement };
    materials: { [key: string]: Material };
}

export interface Ascension {
    fight_prop_base_attack: number;
}

export interface Material {
    mats: Mat[];
    cost: number;
}

export interface Mat {
    name: string;
    id: number;
    count: number;
    rank: number;
}

export interface Refinement {
    name: string;
    desc: string;
    param_list: number[];
}

export interface StatsModifier {
    atk: Atk;
    [key: string]: Atk | undefined;
}

export interface Atk {
    base: number;
    levels: { [key: string]: number };
}

export interface WeaponProp {
    init_value: number;
    prop_type: string;
    type: string;
}
