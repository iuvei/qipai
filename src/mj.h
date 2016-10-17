/*
 * Majang game algorithm library
 * This file is distributed under the BSD License.
 * Copyright (C) 2015 Huang Tao(huangtao117@gmail.com)
 */
#ifndef _MAJANG_H
#define _MAJANG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <time.h>

/*
 * 麻将常用术语
 * 完整的麻将牌共152张
 * 字牌(28张)+花牌(8张)+序数牌(108张)+百搭牌(8张)
 * 常用的不含百搭144张，若没有花牌为136张。
 * 顺子：由同花色三个连续的牌组成。
 * 刻子：又称“坎”，由三张或四张（又称杠子）相同的牌组成。
 * 面子：顺子和刻子的统称。
 * 将头：又称“眼”、雀頭（吊头），胡牌需要的对子。
 * 胡牌：一副牌必须凑满四（五）副面子及一组将，才可以胡牌。
 * 庄家：每局掷骰子决定开牌位置的人。
 * 连庄：庄家胡牌或者流局即可连庄。
 * 番：也称台头，日语里面为役。
 * 自摸: Winning from the wall，摸牌起和（胡）。
 * 流局: drawn。
 */

/* 游戏状态 */
typedef enum gamestate_e {
    GAME_END = 0,   /* game end */
    GAME_PLAY       /* playing */
}GAME_STATE;

/* 服务器/客户端模式 */
typedef enum game_mode_e {
    GAME_MODE_SERVER = 0,
    GAME_MODE_CLIENT
}GAME_MODE;

/* majiang tile suit */
typedef enum majiang_suit {
    mjSuitNone,
    mjSuitWan,                      /* 万子 */
    mjSuitCharacter = mjSuitWan,
    mjSuitSuo,                      /* 索子 */
    mjSuitBamboo = mjSuitSuo,
    mjSuitTong,                     /* 筒子 */
    mjSuitCircle = mjSuitTong,
    mjSuitFeng,                     /* 风牌 */
    mjSuitWind = mjSuitFeng,
    mjSuitSanY,                     /* 三元牌(中发白) */
    mjSuitZFB = mjSuitSanY,
    mjSuitDragon = mjSuitSanY,
    mjSuitHua,                      /* 花牌 */
    mjSuitFlower = mjSuitHua,
    mjSuitSeason,
    mjSuitUnknow
}mjSuit;

/* majiang ordinal */
typedef enum majiang_ordinal {
    mj1 = 1,
    mj2,
    mj3,
    mj4,
    mj5,
    mj6,
    mj7,
    mj8,
    mj9
}mjOrdinal;

typedef enum majiang_wind {
    mjEast = 1,
    mjSouth,
    mjWest,
    mjNorth
}mjWind;

typedef enum majiang_dragon {
    mjZhong = 1,
    mjRed = mjZhong,
    mjFa,
    mjGreen = mjFa,
    mjBai,
    mjWhite = mjBai
}mjDragon;

typedef enum majiang_flower {
    mjMei = 1,
    mjPlum = mjMei,
    mjLan,
    mjOrchid = mjLan,
    mjZhu,
    mjBamboo = mjZhu,
    mjJu,
    mjChrysan = mjJu
}mjFlower;

typedef enum majiang_season {
    mjCun = 1,
    mjSpring = mjCun,
    mjXia,
    mjSummer = mjXia,
    mjQiu,
    mjAutumn = mjQiu,
    mjDong,
    mjWinter = mjDong
}mjSeason;

typedef enum majiang_id {
    MJ_ID_EMPTY,
    /* 万子 */
    MJ_ID_1W, MJ_ID_2W, MJ_ID_3W, MJ_ID_4W, MJ_ID_5W, MJ_ID_6W, MJ_ID_7W, MJ_ID_8W, MJ_ID_9W,
    /* 索子 */
    MJ_ID_1S, MJ_ID_2S, MJ_ID_3S, MJ_ID_4S, MJ_ID_5S, MJ_ID_6S, MJ_ID_7S, MJ_ID_8S, MJ_ID_9S,
    /* 筒子 */
    MJ_ID_1T, MJ_ID_2T, MJ_ID_3T, MJ_ID_4T, MJ_ID_5T, MJ_ID_6T, MJ_ID_7T, MJ_ID_8T, MJ_ID_9T,
    /* 字牌 */
    MJ_ID_DONG, MJ_ID_NAN, MJ_ID_XI, MJ_ID_BEI,
    MJ_ID_ZHONG, MJ_ID_FA, MJ_ID_BAI,
    /* 花牌 */
    MJ_ID_MEI, MJ_ID_LAN, MJ_ID_ZHU, MJ_ID_JU,
    MJ_ID_CUN, MJ_ID_XIA, MJ_ID_QIU, MJ_ID_SDONG,
    /* 特殊 */
    MJ_ID_UNKNOW
}mjID;

/* 面子类型 */
typedef enum mj_meld_type {
    mjMeldNone,
    mjMeldKe,       /* 刻子 */
    mjMeldShun,     /* 顺子 */
    mjMeldJiang,    /* 将牌(对子) */
    mjMeldChi,      /* 吃获取的顺子 */
    mjMeldPeng,     /* 碰获取的刻子 */
    mjMeldGang      /* 杠 */
}mjMeldType;

typedef enum mj_chi_type {
    mjChiNone,
    mjChiLeft = 0x01,      /* 吃的牌在左边 */
    mjChiMiddle = 0x02,    /* 吃的牌在中间 */
    mjChiRight = 0x04      /* 吃的牌在右边 */
}mjChiType;

typedef enum mj_gang_type {
    mjGangNone,
    mjGangMing,     /* 明杠 */
    mjGangAn,       /* 暗杠 */
    mjGangJia       /* 加杠 */
}mjGangType;

/* 游戏开始后的逻辑状态 */
typedef enum mj_logic_state {
    lsTurn,     /* 轮流摸牌打牌 */
    lsCall      /* 其他玩家吃碰杠 */
}mjLogicState;

/* called (claimed by other players) */
typedef enum mj_call_type {
    callChi,
    callPeng,
    callGang,
    callHu
}mjCallType;

/* a mj pai */
typedef struct mjpai_s {
    int id;     /* start with 1 */
    int suit;   /* suit */
    int sign;   /* sign */
}mjpai_t;

/* 面子 */
typedef struct mj_meld_s {
    int type;       /* 面子类型 */
    int pai_id;     /* 特征牌 */
    int player_no;  /* 吃碰目标玩家 */
    int extra_info; /* 吃,杠附加信息 */
}mj_meld_t;

void mjpai_init_id(mjpai_t* pai, int id);
void mjpai_init_ss(mjpai_t* pai, int suit, int sign);
void mjpai_zero(mjpai_t* pai);

void mjpai_copy(mjpai_t* dest, mjpai_t* src);

/* 洗牌 */
void mj_shuffle(int* pais, int len);

/* 删除一张牌 */
void mj_delete(int* pais, int len, int id);

/* 整理，返回有效数量 */
int mj_trim(int* pais, int len);

/* 返回有效数量 */
int mj_length(int* pais, int len);

/**
 * print cards to a readable string
 */
const char* mj_string(int* pais, int len, int line_number);
const char* mjpai_string(int id);

#ifdef __cplusplus
}
#endif
#endif
