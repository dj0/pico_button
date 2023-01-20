#include <genesis.h>
#include <string.h>
#include "pico.h"
#include "res_image.h"

const int WIDTH = 320;  //screen width
const int HEIGHT = 224; //screen height
const int TILE_WIDTH = 40;
const int TILE_HEIGHT = 28;
const int textOffset = 0;

vu16 xPos = 0;
vu16 yPos = 0;
char xText[3] = "X:\0";
char yText[3] = "Y:\0";
char xPosText[5] = "0";
char yPosText[5] = "0";
char btnOn[3] = "ON\0";
char btnOff[4] = "OFF\0";
char xValText[5] = "0";
char yValText[5] = "0";
char penText[5] = "PEN:\0";
char leftText[6] = "LEFT:\0";
char upText[4] = "UP:\0";
char rightText[7] = "RIGHT:\0";
char downText[6] = "DOWN:\0";
char redText[5] = "RED:\0";

Sprite *ball;
int ball_pos_x = 100;
int ball_pos_y = 100;
int ball_color = 0;

int penBtn = FALSE;
int redBtn = FALSE;
int leftBtn = FALSE;
int rightBtn = FALSE;
int upBtn = FALSE;
int downBtn = FALSE;

int counter = 0;

void updateTextDisplay()
{
    sprintf(xValText, "%d", (int)ball_pos_x);
    sprintf(yValText, "%d", (int)ball_pos_y);
    
    SYS_disableInts();
    VDP_clearText(3, 1+textOffset, 5);
    VDP_clearText(12, 1+textOffset, 5);
    VDP_clearText(5, 2+textOffset, 3);
    VDP_clearText(14, 2+textOffset, 3);
    VDP_clearText(6, 3+textOffset, 3);
    VDP_clearText(16, 3+textOffset, 3);
    VDP_clearText(4, 4+textOffset, 3);
    VDP_clearText(15, 4+textOffset, 3);

    VDP_drawText(xValText, 3, 1+textOffset);
    VDP_drawText(yValText, 12, 1+textOffset);
    
    if(penBtn){
        VDP_drawText(btnOn, 5, 2+textOffset);
        PAL_setColor(22, RGB24_TO_VDPCOLOR(0xffffff));
    }else{
        VDP_drawText(btnOff, 5, 2+textOffset);
        PAL_setColor(22, ball_color);
    }
    redBtn ? VDP_drawText(btnOn, 14, 2+textOffset) :  VDP_drawText(btnOff, 14, 2+textOffset);
    leftBtn ? VDP_drawText(btnOn, 6, 3+textOffset) :  VDP_drawText(btnOff, 6, 3+textOffset);
    rightBtn ? VDP_drawText(btnOn, 16, 3+textOffset) :  VDP_drawText(btnOff, 16, 3+textOffset);
    upBtn ? VDP_drawText(btnOn, 4, 4+textOffset) :  VDP_drawText(btnOff, 4, 4+textOffset);
    downBtn ? VDP_drawText(btnOn, 15, 4+textOffset) :  VDP_drawText(btnOff, 15, 4+textOffset);
    SYS_enableInts();
}

void myPicoHandler(u16 changed, u16 state)
{
    penBtn = state & PICO_PEN ? TRUE : FALSE;
    redBtn = state & PICO_RED ? TRUE : FALSE;
    leftBtn = state & PICO_LEFT ? TRUE : FALSE;
    rightBtn = state & PICO_RIGHT ? TRUE : FALSE;
    upBtn = state & PICO_UP ? TRUE : FALSE;
    downBtn = state & PICO_DOWN ? TRUE : FALSE;
}

int main()
{
    VDP_init();
    PICO_init();
    PICO_setEventHandler(&myPicoHandler);
    SPR_init();

    ball = SPR_addSprite(&imgball, 100, 100, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    VDP_loadTileSet(bgtile.tileset, 1, DMA);
    PAL_setPalette(PAL1, bgtile.palette->data, DMA);

    VDP_setTextPlane(BG_A);

    VDP_drawText(xText, 1, 1+textOffset);
    VDP_drawText(yText, 10, 1+textOffset);
    VDP_drawText(penText, 1, 2+textOffset);
    VDP_drawText(redText, 10, 2+textOffset);
    VDP_drawText(leftText, 1, 3+textOffset);
    VDP_drawText(rightText, 10, 3+textOffset);
    VDP_drawText(upText, 1, 4+textOffset);
    VDP_drawText(downText, 10, 4+textOffset);

    VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 1), 0, 0, 40, 30);
    ball_color = PAL_getColor(22);

    while (1)
    {
        ball_pos_x = PICO_penCoordX();
        ball_pos_y = PICO_penCoordY();
        ball_pos_x = ball_pos_x + 1;
        ball_pos_y = ball_pos_y + 1;
        if(ball_pos_x > 600) ball_pos_x = 0;
        if(ball_pos_y > 600) ball_pos_y = 0;
        SPR_setPosition(ball, ball_pos_x, ball_pos_y);
        SPR_update();
        PICO_update();
        counter++;
        SYS_doVBlankProcess();
        if(counter % 4)
            updateTextDisplay();
    }
    return (0);
}
