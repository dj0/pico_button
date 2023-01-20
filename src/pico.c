#include "pico.h"

static vu16 pBtnState;
static vu16 penAxisX;
static vu16 penAxisY;

static PicoEventCallback *picoEventCB;

void PICO_init(void)
{
    /* Reset controller state change event callback */
    picoEventCB = NULL;
    PICO_reset();

    vu8 *pb;
    pb = (vu8 *)0x800019;
    *pb = 0x53;
    pb = (vu8 *)0x80001b;
    *pb = 0x45;
    pb = (vu8 *)0x80001d;
    *pb = 0x47;
    pb = (vu8 *)0x80001f;
    *pb = 0x41;
}

void PICO_reset()
{
    pBtnState = 0;
    penAxisX = 0;
    penAxisY = 0;
}

static u16 readPicoBtn()
{
    vu8 *pb;
    u16 val;

    pb = (vu8 *)0x800003;
    asm volatile("nop");
    asm volatile("nop");
    val = *pb;
    return val;
}

void PICO_setEventHandler(PicoEventCallback *CB)
{
    picoEventCB = CB;
}

void readPenCoord()
{
    vu8 *pb;
    vu16 x, y;

    pb = (vu8 *)0x800005;
        asm volatile(
            "movep.w (0,%1),%0\n"
            "andi.w #-0x7c01,%0\n"
            : "=d"(x)
            : "a"(pb)
            :);
    if(x < 0x8000){
        penAxisX = x;
    }

    pb = (vu8 *)0x800009;

        asm volatile(
            "movep.w (0,%1),%0\n"
            "andi.w #-0x7c01,%0\n"
            : "=d"(y)
            : "a"(pb)
            :);
    if(y < 0x8000){
        penAxisY = y;
    }
}

u16 PICO_readPenX()
{
    return (u16)penAxisX;
}

u16 PICO_readPenY()
{
    return (u16)penAxisY;
}

int PICO_penCoordX(void)
{
    return (int)(penAxisX - 0x003c);
}

int PICO_penCoordY(void)
{
    return (int)(penAxisY - 0x01fc);
}

void PICO_update(void)
{
    u16 val;
    u16 newstate;
    u16 change;

    /* disable ints */
    SYS_disableInts();
    readPenCoord();
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
