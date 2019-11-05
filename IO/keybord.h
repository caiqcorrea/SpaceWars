#include "../base/auxiliar.h"
#include "../graficos/xwc.h"

typedef enum
{
    NAVE1_HOR,
    NAVE1_ANT,
    NAVE1_ACE,
    NAVE1_DIS,

    NAVE2_HOR,
    NAVE2_ANT,
    NAVE2_ACE,
    NAVE2_DIS,

    MAX_KEYS
} KeyNameEnum;

void initKeybord(WINDOW *win);

void atualizaKeybord(WINDOW *win);

Bool isPressed(KeyNameEnum key);