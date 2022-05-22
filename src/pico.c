#include "pico.h"

static vu16 pBtnState;

static PicoEventCallback *picoEventCB;

void PICO_init(void)
{
    /* Reset controller state change event callback */
    picoEventCB = NULL;
    PICO_reset();
}

void PICO_reset()
{
    pBtnState = 0;
}

static u16 readPicoBtn()
{
    vu8 *pb;
    u16 val;

    pb = (vu8 *)0x800003;

    val = *pb;
    return val;
}

void PICO_setEventHandler(PicoEventCallback *CB)
{
    picoEventCB = CB;
}

void PICO_update(void)
{
    u16 val;
    u16 newstate;
    u16 change;

    /* disable ints */
    SYS_disableInts();
    val = readPicoBtn();

    val = (~(val)) & 0x00FF;
    newstate = val & PICO_BTN_ALL;
    change = pBtnState ^ newstate;
    pBtnState = newstate;
    if ((picoEventCB) && (change))
        picoEventCB(change, newstate);

    /* restore ints */
    SYS_enableInts();
}
