/**
 *  @file pico.h
 *  @brief Pico controller support.
 *  @author pianoid
 *  @date 05/2022
 *
 * This unit provides methods to read pico controller state. (WIP)<br>
 */

#ifndef _PICO_H_
#define _PICO_H_

#include "genesis.h"

#define PICO_UP 0x0001
#define PICO_DOWN 0x0002
#define PICO_LEFT 0x0004
#define PICO_RIGHT 0x0008
#define PICO_RED 0x0010
#define PICO_PEN 0x0080
#define PICO_BTN_ALL 0x00FF

/**
 * @brief
 *      Pico event callback.
 *
 * @param changed
 *      Changed state(button for which state changed).<br>
 *      <b>PICO_UP</b>    = UP button<br>
 *      <b>PICO_DOWN</b>  = DOWN button<br>
 *      <b>PICO_LEFT</b>  = LEFT button<br>
 *      <b>PICO_RIGHT</b> = RIGHT button<br>
 *      <b>PICO_RED</b>   = RED(CENTER) button<br>
 *      <b>PICO_PEN</b>   = Pencil button<br>
 * @param state
 *      Current button state.<br>
 */
typedef void PicoEventCallback(u16 changed, u16 state);

/**
 * @brief
 *      Initialize Pico controller.
 */
void PICO_init(void);

/**
 * @brief
 *      Reset Pico controller.
 */
void PICO_reset(void);

/**
 * @brief
 *      Set the callback function for controller state changed.<br>
 * @param CB
 *      Callback to call when controller(s) state changed.<br>
 *      The function prototype should reply to _joyEventCallback type :<br>
 *      void function(u16 changed, u16 state);<br>
 *<br>
 *      <b>Ex 1</b> : if you just pressed UP button you receive :<br>
 *      changed = PICO_UP, state = PICO_UP<br>
 *      <b>Ex 2</b> : if you just released the DOWN button you receive :<br>
 *      changed = PICO_DOWN, state = 0<br>
 */
void PICO_setEventHandler(PicoEventCallback *CB);

/**
 * @brief
 *      Update pico button state.
 */
void PICO_update(void);

#endif