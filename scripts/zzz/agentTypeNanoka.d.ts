export interface NanokaAgent {
    id: number;
    icon: string;
    name: string;
    code_name: string;
    rarity: number;
    weapon_type: { [key: string]: string };
    element_type: { [key: string]: string };
    special_element_type: SpecialElementType;
    hit_type: { [key: string]: string };
    camp: { [key: string]: string };
    gender: number;
    partner_info: PartnerInfo;
    skin: { [key: string]: Skin };
    stats: Stats;
    level: { [key: string]: AgentLevel };
    extra_level: { [key: string]: ExtraLevel };
    level_exp: number[];
    skill: Skill;
    skill_list: { [key: string]: SkillList };
    passive: Passive;
    talent: { [key: string]: Talent };
    fairy_recommend: FairyRecommend;
    strategy: string[];
    potential: number[];
    potential_detail: { [key: string]: PotentialDetail };
    live2_d?: string;
}

export interface ExtraLevel {
    max_level: number;
    extra: { [key: string]: Part4 };
}

export interface Part4 {
    prop: number;
    name: string;
    format: string;
    value?: number;
    icon?: string;
}

export interface FairyRecommend {
    slot4: number;
    slot2: number;
    slot_sub: number;
    part4: Part4;
    part5: Part4;
    part6: Part4;
    part_sub: Part4;
}

export interface AgentLevel {
    hp_max: number;
    attack: number;
    defence: number;
    level_max: number;
    level_min: number;
    materials: { [key: string]: number };
}

export interface PartnerInfo {
    birthday: string;
    full_name: string;
    gender: string;
    icon_path: string;
    impression_f: string;
    impression_m: string;
    profile_desc: string;
    role_icon: string;
    stature: string;
    unlock_condition: string[];
    trust_lv: { [key: string]: string };
}

export interface Passive {
    level: { [key: string]: PassiveLevel };
    materials: { [key: string]: { [key: string]: number } };
}

export interface PassiveLevel {
    level: number;
    id: number;
    name: string[];
    desc: string[];
    extra_property: { [key: string]: ExtraProperty };
    potential: number[];
}

export interface ExtraProperty {
    target: number;
    value: number;
}

export interface PotentialDetail {
    id: number;
    name: string;
    desc: string;
    image: string;
    level_show_name: string;
    level: number;
    ability_list: number[];
    potential_materials: PotentialMaterial[];
}

export interface PotentialMaterial {
    item_id: number;
    number: number;
}

export interface Skill {
    basic: SkillData;
    dodge: SkillData;
    special: SkillData;
    chain: SkillData;
    assist: SkillData;
}

export interface ParamValue {
    main: number;
    growth: number;
    format: string;
    damage_percentage: number;
    damage_percentage_growth: number;
    stun_ratio: number;
    stun_ratio_growth: number;
    sp_recovery: number;
    sp_recovery_growth: number;
    fever_recovery: number;
    fever_recovery_growth: number;
    attribute_infliction: number;
    sp_consume: number;
    attack_data: number[];
    rp_recovery: number;
    rp_recovery_growth: number;
    ether_purify: number;
}

export interface SkillData {
    description: SkillDescription[];
    material: { [key: string]: { [key: string]: number } };
}
export interface SkillDescription {
    name: string;
    desc?: string;
    potential: number[];
    param?: SkillParam[];
}

export interface SkillParam {
    name: string;
    desc: string;
    param?: { [key: string]: ParamValue };
    potential: number[];
}

export interface SkillList {
    name: string;
    desc: string;
    element_type: number;
    hit_type: number;
    potential: number[];
}

export interface Skin {
    name: string;
    desc: string;
    image: string;
}

export interface SpecialElementType {
    name?: string;
    title?: string;
    desc?: string;
    icon?: string;
}

export interface Stats {
    armor: number;
    armor_growth: number;
    attack: number;
    attack_growth: number;
    avatar_piece_id: number;
    break_stun: number;
    crit: number;
    crit_damage: number;
    crit_dmg_res: number;
    crit_res: number;
    defence: number;
    defence_growth: number;
    element_abnormal_power: number;
    element_mystery: number;
    endurance: number;
    hp_growth: number;
    hp_max: number;
    pen_delta: number;
    pen_rate: number;
    rbl: number;
    rbl_correction_factor: number;
    rbl_probability: number;
    shield: number;
    shield_growth: number;
    sp_bar_point: number;
    sp_recover: number;
    stun: number;
    tags: string[];
    rp_max: number;
    rp_recover: number;
}

export interface Talent {
    level: number;
    name: string;
    desc: string;
    desc2: string;
}
