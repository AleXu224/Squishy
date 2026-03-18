export interface NanokaCharacter {
    name: string;
    desc: string;
    chara_info: CharaInfo;
    weapon: string;
    rarity: string;
    element: string;
    icon: string;
    stamina_recovery: number;
    base_hp: number;
    base_atk: number;
    base_def: number;
    crit_rate: number;
    crit_dmg: number;
    elemental_mastery: number;
    level_exp: number[];
    stats_modifier: StatsModifier;
    skills: Item[];
    passives: Item[];
    constellations: Item[];
    materials: Materials;
}

export interface CharaInfo {
    release_date: Date;
    birth: number[];
    vision: string;
    vision_special_type_before: string;
    vision_special_type_after: string;
    constellation: string;
    region: string;
    title: string;
    native: string;
    detail: string;
    va: Va;
    stories: Story[];
    quotes: Quote[];
    special_food: SpecialFood;
    namecard: Item;
    costume: Item[];
    trace_effect: Item[];
}

export interface Item {
    id: number;
    name: string;
    desc: string;
    icon?: string;
    quality?: number;
    param_list?: number[];
    unlock?: number;
    promote?: { [key: string]: Promote };
}

export interface Promote {
    level: number;
    icon: string;
    desc: string[];
    param: number[];
}

export interface Quote {
    title: string;
    text: string;
    unlocked: string[];
}

export interface SpecialFood {
    id: number;
    recipe: number;
    name: string;
    icon: string;
    rank: number;
}

export interface Story {
    title: string;
    text: string;
    unlock: string[];
}

export interface Va {
    chinese: string;
    japanese: string;
    english: string;
    korean: string;
}

export interface Materials {
    ascensions: TalentElement[];
    talents: Array<TalentElement[]>;
}

export interface TalentElement {
    mats: Mat[];
    cost: number;
}

export interface Mat {
    name: string;
    id: number;
    count: number;
    rank: number;
}

export interface StatsModifier {
    hp: { [key: string]: number };
    atk: { [key: string]: number };
    def: { [key: string]: number };
    ascension: StatsModifierAscension[];
    prop_grow_curves: PropGrowCurve[];
}

export interface StatsModifierAscension {
    fight_prop_base_hp: number;
    fight_prop_base_defense: number;
    fight_prop_base_attack: number;
    [key: string]: number;
}

export interface PropGrowCurve {
    grow_curve: string;
    type: string;
}
