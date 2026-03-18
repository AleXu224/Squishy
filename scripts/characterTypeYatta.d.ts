export interface YattaCharacter {
    response: number;
    data: Data;
}

export interface Data {
    id: number;
    rank: number;
    name: string;
    element: string;
    weaponType: string;
    region: string;
    specialProp: string;
    bodyType: string;
    icon: string;
    birthday: number[];
    release: number;
    route: string;
    fetter: Fetter;
    upgrade: Upgrade;
    other: Other;
    ascension: { [key: string]: number };
    items: { [key: string]: Item };
    talent: { [key: string]: Talent };
    constellation: { [key: string]: Constellation };
    dictionary: Dictionary | null;
}

export interface Constellation {
    id: number;
    name: string;
    description: string;
    descriptionBuff: null;
    extraData: ExtraData | null;
    icon: string;
    effects?: string[];
}

export interface ExtraData {
    addTalentExtraLevel: AddTalentExtraLevel;
}

export interface AddTalentExtraLevel {
    talentType: string;
    talentIndex: number;
    extraLevel: number;
}

export interface Dictionary {
    talent: string[];
    constellation: string[];
    effectsInfo: { [key: string]: EffectsInfo };
}

export interface EffectsInfo {
    id: number;
    name: string;
    description: string;
    params: Param[] | null;
    props: string[];
}

export interface Param {
    skillFull: string;
    skillId: number;
    skillParam: SkillParam;
}

export interface SkillParam {
    paramNum: number;
    paramValue: number;
    paramMultiplier: number;
}

export interface Fetter {
    title: string;
    detail: string;
    constellation: string;
    native: string;
    cv: Cv;
}

export interface Cv {
    EN: string;
    CHS: string;
    JP: string;
    KR: string;
}

export interface Item {
    name: string;
    rank: number;
    icon: string;
}

export interface Other {
    costume: Costume[];
    furnitureId: number;
    nameCard: NameCard;
    specialFood: SpecialFood;
}

export interface Costume {
    name: string;
    description: string;
    isDefault: boolean;
}

export interface NameCard {
    id: number;
    name: string;
    description: string;
    icon: string;
}

export interface SpecialFood {
    id: number;
    name: string;
    rank: number;
    effectIcon: string;
    icon: string;
}

export interface Talent {
    type: number;
    name: string;
    description: string;
    descriptionBuff: null | string;
    icon: string;
    promote?: { [key: string]: PromoteValue };
    cooldown?: number;
    cost?: number;
    linkedConstellations: LinkedConstellations | null;
    advancedProps?: AdvancedProp[];
    effects?: string[];
}

export interface AdvancedProp {
    name: string;
    elementalGaugeTheory: string;
    internalCooldown: InternalCooldownClass | null | string;
    interruptionResistance: string;
    interruptionResistanceStagger: InterruptionResistanceStagger;
    bluntAttack: boolean;
}

export interface InternalCooldownClass {
    tag: Tag;
    type: Tag;
}

export interface Tag {
    span: null | string;
    text: string;
}

export interface InterruptionResistanceStagger {
    staggerLevel: string;
    horizontalForce: string;
    verticalForce: string;
}

export interface LinkedConstellations {
    isLevelIncluded: boolean;
    prop: LinkedConstellationsProp[];
}

export interface LinkedConstellationsProp {
    id: number;
    type: string;
}

export interface PromoteValue {
    level: number;
    costItems: { [key: string]: number } | null;
    coinCost: number | null;
    description: string[];
    params: number[];
}

export interface Upgrade {
    prop: UpgradeProp[];
    propOther: null;
    promote: PromoteElement[];
}

export interface PromoteElement {
    promoteAudio: string;
    unlockMaxLevel: number;
    promoteLevel: number;
    costItems?: { [key: string]: number };
    requiredPlayerLevel?: number;
    addProps?: AddProps & { [key: string]: number };
    coinCost?: number;
}

export interface AddProps {
    FIGHT_PROP_BASE_HP: number;
    FIGHT_PROP_BASE_DEFENSE: number;
    FIGHT_PROP_BASE_ATTACK: number;
}

export interface UpgradeProp {
    propType: string;
    initValue: number;
    type: string;
}
