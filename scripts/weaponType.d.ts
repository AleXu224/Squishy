export interface Weapon {
    Name: string;
    Desc: string;
    WeaponType: string;
    WeaponProp: WeaponProp[];
    Rarity: number;
    Icon: string;
    StatsModifier: StatsModifier;
    XPRequirements: { [key: string]: number };
    Ascension: { [key: string]: Ascension };
    Refinement: { [key: string]: Refinement };
    Materials: { [key: string]: Material };
}

export interface Ascension {
    FIGHT_PROP_BASE_ATTACK: number;
}

export interface Material {
    Mats: Mat[];
    Cost: number;
}

export interface Mat {
    Name: string;
    Id: number;
    Count: number;
    Rank: number;
}

export interface Refinement {
    Name: string;
    Desc: string;
    ParamList: number[];
}

export interface StatsModifier {
    ATK: Atk;
    FIGHT_PROP_CRITICAL?: Atk;
}

export interface Atk {
    Base: number;
    Levels: { [key: string]: number };
}

export interface WeaponProp {
    propType?: string;
    initValue?: number;
    type: string;
}
