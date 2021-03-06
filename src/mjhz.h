/*
*
* code:huangtao117@gmail.com
* 杭州麻将
*
*/
#ifndef _MJHZ_H
#define _MJHZ_H
#ifdef __cplusplus
extern "C" {
#endif

#include "mj.h"

#define MJHZ_MAX_PLAYERS	4       /* 游戏最多选手 */
#define MJHZ_MAX_HAND		14      /* 手上牌最大数量 */
#define MJHZ_MAX_MELD       6       /* 面子最大数量 */
#define MJHZ_DECK_PAIS      136     /* 杭州麻将麻将牌最大数量 */
#define MJHZ_MAX_PAITYPE    34      /* 杭州麻将使用34种牌(27张序数+7张字牌) */
#define MJHZ_WALL_REMAIN    20      /* 长城保留的牌数 */
#define MJHZ_MAX_DISCARDED  20      /* 记录打出的牌 */
#define MJHZ_LEN_JS         35      /* 用于计数 */

/* mjhz hu info */
typedef struct mjhz_hu_s {
    int fan;		/* 番数 */
    int is_tianhu;  /* 天胡 */
    int is_baotou;	/* 爆头 */
    int is_qg;      /* 抢杠 */
    int is_pair7;	/* 7对子 */
    int pair7_h4;	/* 7对子包含4个的数量 */
    int gang;       /* 胡之前连杠数量 */
    int cai_piao;	/* 财飘次数 */
    int pao_no;     /* 点炮选手,-1表示自摸 */
    int joker_eyes; /* 将牌中有财神 */
}mjhz_hu_t;

typedef struct mjhz_player_s {
    int level;
    int state;
    int position;
    int win_lose;
    int hand[MJHZ_MAX_HAND];            /* 手牌 */
    int hand_js[MJHZ_LEN_JS];           /* 手牌计数 */
    int discard[MJHZ_MAX_DISCARDED];    /* 舍牌(打出的牌) */
    int discard_index;
    mj_meld_t meld[MJHZ_MAX_MELD];      /* 已吃碰杠 */
    int meld_index;
    int pai_peng[MJHZ_LEN_JS];			/* 不能弃先碰后,!=1不能碰 */
	int pai_gang[4];	/* 杠牌信息 */
    int keep_gang;      /* 连续杠 */
    int pass_hu;        /* 漏胡标记 */
    mjhz_hu_t hu;       /* 胡牌信息 */
    int wait_hu;        /* 能胡,等待胡牌 */
    int wait_chi;       /* 等待吃(碰杠胡优先) */
    int wait_peng;
    int wait_gang;
    int req_pass;       /* 请求过 */
    int req_chi;        /* 请求吃的牌1 */
    int req_chi2;
    int req_peng;
    int req_gang;
    int req_hu;
    int need_cb;        /* 是否需要承包 */
    int need_dcb;       /* 是否需要倒承包 */
}mjhz_player_t;

typedef struct mjhz_s {
    int mode;               /* client or server mode */
    int game_state;         /* game state */
    int logic_state;        /* mj logic state */
    int round;
    int inning;             /* 局数 */
    int player_num;         /* 2 ~ 4 player */
    int dealer_no;          /* 庄家 dealer no. */
    int first_player_no;    /* first player no. */
    int curr_player_no;     /* current turn player no. */
    int hu_player_no;
    int discard_pai;        /* 当前打出的麻将牌 */
    int discarded_no;       /* 弃牌选手 */
    int gang_pai;           /* 当前杠牌 */
    int joker;              /* 财神(百搭) */
    int flag_liu;           /* 流局标记 */
    int debug;              /* output debug info */
    int step_by_step;       /* for debug */

    int dice[2];
    int deck_all_num;
    int deck_deal_end;      /* where deal end position */
    int deck_deal_index;    /* current deal card index */
    int deck_deal_gang;     /* deal when gang */
    int deck_valid_num;     /* valid number card */

    int enable_chi;         /* 能否吃 */
    int enable_dian_hu;     /* 能否点和(点炮、捉冲) */
    int enable_lou_hu;      /* 漏胡(弃先胡后) */
    int enable_qg;          /* 能否抢杠 */
    int enable_3tan;        /* 吃碰杠3摊 */
    int enable_dl;          /* 笃老:两个骰子一样(>9)算三老庄 */
    int enable_cp_gang;     /* 财飘情况下其他人能否杠 */
    int cai_piao_no;        /* 记录财飘选手编号 */
    int lao_z;              /* 老庄 */

    time_t time_start;      /* 游戏开始时间 */
    time_t time_turn;       /* 选手回合开始时间 */
    int sec_wait;           /* 当前操作总共等待时间(秒) */

    int deck[MJHZ_DECK_PAIS];                   /* deck mj pais */
    mjhz_player_t players[MJHZ_MAX_PLAYERS];    /* players */

    fp_relative_seat pf_relative_seat;
    fp_seat_no pf_seat_no;
    fp_mj_event pf_event;
}mjhz_t;

/* init a mjhz game object */
void mjhz_init(mjhz_t* mj, int mode);

/* start a new game */
void mjhz_start(mjhz_t* mj, int player_num);

/* sort a hand */
void mjhz_sort(int* pais, int len);

/* 出牌(弃牌) */
int mjhz_discard(mjhz_t* mj, int player_no, int pai_id);

/* 判决,执行吃碰杠胡或者摸牌 */
void mjhz_referee(mjhz_t* mj);

/* 抓牌 */
int mjhz_pickup(mjhz_t* mj, int is_gang);

int mjhz_can_chi(mjhz_t* mj, int player_no);
int mjhz_can_peng(mjhz_t* mj, int player_no);

/* 杠判定。返回可以杠的数量。 */
int mjhz_can_gang(mjhz_t* mj, int player_no);

/* 是否全顺子和刻子 */
int mjhz_all_melded(int array[MJHZ_LEN_JS]);

/* 带财神数量 */
int mjhz_all_melded_joker(int array[MJHZ_LEN_JS], int num_joker);

/* 和(胡)判定 */
int mjhz_can_hu(mjhz_t* mj, int player_no);

/* 请求吃碰杠胡 */
int mjhz_req_chi(mjhz_t* mj, int player_no, int pai1, int pai2);

int mjhz_req_peng(mjhz_t* mj, int player_no);

int mjhz_req_gang(mjhz_t* mj, int player_no, int pai);

int mjhz_req_hu(mjhz_t* mj, int player_no);

/* 吃 */
int mjhz_chi(mjhz_t* mj, int player_no, int pai1, int pai2);

/* 碰 */
int mjhz_peng(mjhz_t* mj, int player_no);

/* 杠 */
int mjhz_gang(mjhz_t* mj, int player_no, int pai);

/* 胡(和) */
int mjhz_hu(mjhz_t* mj, int player_no);

/* 过 */
void mjhz_pass(mjhz_t* mj, int player_no);
void mjhz_next_player(mjhz_t* mj);
int mjhz_get_next(mjhz_t* mj, int player_no);

/* 帮助函数 */
void mjhz_dump(mjhz_t* mj);
const char* mjhz_hu_name(mjhz_hu_t* hu);

#ifdef __cplusplus
}
#endif
#endif
