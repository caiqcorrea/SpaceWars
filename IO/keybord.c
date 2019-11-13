#include "keybord.h"

#include "../base/auxiliar.h"
#include "../graficos/xwc.h"

static Bool isKeyPressed[MAX_KEYS+1];

static KeyNameEnum KeySym_To_Enum(KeySym key);

static KeyNameEnum KeySym_To_Enum(KeySym key)
{
    switch (key)
    {
    case (XK_d):
        return NAVE1_HOR;
    case (XK_a):
        return NAVE1_ANT;
    case (XK_w):
        return NAVE1_ACE;
    case (XK_s):
        return NAVE1_DIS;

    case (XK_Right):
        return NAVE2_HOR;
    case (XK_Left):
        return NAVE2_ANT;
    case (XK_Up):
        return NAVE2_ACE;
    case (XK_Down):
        return NAVE2_DIS;
    }

    throwException("KeySym_To_Enum", "Tecla invalida", index_out_of_range_exception);
    return MAX_KEYS;
}

void initKeybord(WINDOW *win)
{
    int i;
    for (i = 0; i < MAX_KEYS; i++)
        isKeyPressed[i] = FALSE;

    InitKBD(win);
}

void atualizaKeybord(WINDOW *win)
{
    while (WCheckKBDRelease(win))
    {
        WGetKeyRelease(win);
        isKeyPressed[KeySym_To_Enum(WLastKeyReleaseSym())] = FALSE;
    }

    while (WCheckKBDPress(win))
    {
        WGetKeyPress(win);
        isKeyPressed[KeySym_To_Enum(WLastKeyPressSym())] = TRUE;
    }
}

Bool isPressed(KeyNameEnum key)
{
    return isKeyPressed[key];
}
