export interface Character {
    Name: string;
    Desc: string;
    CharaInfo: CharaInfo;
    Weapon: string;
    Rarity: string;
    Icon: string;
    StaminaRecovery: number;
    BaseHP: number;
    BaseATK: number;
    BaseDEF: number;
    CritRate: number;
    CritDMG: number;
    LevelEXP: number[];
    StatsModifier: StatsModifier;
    Skills: Skill[];
    Passives: Namecard[];
    Constellations: Namecard[];
    Materials: Materials;
}

export interface CharaInfo {
    ReleaseDate: Date;
    Birth: number[];
    Vision: string;
    Constellation: string;
    Region: string;
    Title: string;
    Native: string;
    Detail: string;
    VA: Va;
    Stories: Story[];
    Quotes: Quote[];
    SpecialFood: SpecialFood;
    Namecard: Namecard;
    Costume: Namecard[];
    TraceEffect: any[];
}

export interface Namecard {
    Id?: number;
    Name: string;
    Desc: string;
    Icon: string;
    Quality?: number;
    ParamList?: number[];
    Unlock?: number;
}

export interface Quote {
    Title: string;
    Text: string;
    Unlocked: string[];
}

export interface SpecialFood {
    Id: number;
    Recipe: number;
    Name: string;
    Icon: string;
    Rank: number;
}

export interface Story {
    Title: string;
    Text: string;
    Unlock: string[];
}

export interface Va {
    Chinese: string;
    Japanese: string;
    English: string;
    Korean: string;
}

export interface Materials {
    Ascensions: Ascension[];
    Talents: Array<Ascension[]>;
}

export interface Ascension {
    Mats: Mat[];
    Cost: number;
}

export interface Mat {
    Name: string;
    Id: number;
    Count: number;
    Rank: number;
}

export interface Skill {
    Name: string;
    Desc: string;
    Promote: { [key: string]: Promote };
}

export interface Promote {
    Level: number;
    Icon: string;
    Desc: string[];
    Param: number[];
}

export interface StatsModifier {
    HP: { [key: string]: number };
    ATK: { [key: string]: number };
    DEF: { [key: string]: number };
    Ascension: { [key: string]: number }[];
    PropGrowCurves: PropGrowCurve[];
    RecommendedProps: any[];
}

export interface PropGrowCurve {
    type: string;
    growCurve: string;
}
