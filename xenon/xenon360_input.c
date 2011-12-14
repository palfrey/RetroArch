/*  SSNES - A Super Ninteno Entertainment System (SNES) Emulator frontend for libsnes.
 *  Copyright (C) 2010 - Hans-Kristian Arntzen
 *  Copyright (C) 2011 - Daniel De Matteis
 *
 *  Some code herein may be based on code found in BSNES.
 * 
 *  SSNES is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  SSNES is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with SSNES.
 *  If not, see <http://www.gnu.org/licenses/>.
 */


#include "../driver.h"
#include <stdint.h>
#include "../libsnes.hpp"

#include <input/input.h>
#include <usb/usbmain.h>

#include <stdlib.h>

struct controller_data_s pad[5];

static void xenon360_input_poll(void *data)
{
   (void)data;
   for (unsigned i = 0; i < 5; i++)
   {
      usb_poll();
      get_controller_data(&pad[i], i);
   }
}

static int16_t xenon360_input_state(void *data, const struct snes_keybind **binds,
      bool port, unsigned device,
      unsigned index, unsigned id)
{
   (void)data;
   (void)binds;
   (void)index;

   if (device != SNES_DEVICE_JOYPAD)
      return 0;

   unsigned player = 0;
   if (port == SNES_PORT_2 && device == SNES_DEVICE_MULTITAP)
      player = index + 1;
   else if (port == SNES_PORT_2)
      player = 1;

   uint64_t button = 0;

   // Hardcoded binds.
   switch (id)
   {
      case SNES_DEVICE_ID_JOYPAD_A:
         button = pad[player].b ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_B:
         button = pad[player].a ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_X:
         button = pad[player].y ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_Y:
         button = pad[player].x ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_LEFT:
         button = pad[player].left ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_RIGHT:
         button = pad[player].right ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_UP:
         button = pad[player].up ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_DOWN:
         button = pad[player].down ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_START:
         button = pad[player].start ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_SELECT:
         button = pad[player].select ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_L:
         button = pad[player].lt ? 1 : 0;
         break;
      case SNES_DEVICE_ID_JOYPAD_R:
         button = pad[player].rt ? 1 : 0;
         break;
      default:
         button = 0;
   }

   return button;
}

static void xenon360_free_input(void *data)
{
   (void)data;
}

static void* xenon360_input_init(void)
{
   return (void*)-1;
}

static bool xenon360_key_pressed(void *data, int key)
{
   (void)data;
   #if 0
   switch (key)
   {
      case SSNES_FAST_FORWARD_HOLD_KEY:
         return CTRL_RSTICK_UP(state[0]) && CTRL_R2(~state[0]);
      case SSNES_LOAD_STATE_KEY:
         return (CTRL_RSTICK_UP(state[0]) && CTRL_R2(state[0]));
      case SSNES_SAVE_STATE_KEY:
         return (CTRL_RSTICK_DOWN(state[0]) && CTRL_R2(state[0]));
      case SSNES_STATE_SLOT_PLUS:
         return (CTRL_RSTICK_RIGHT(state[0]) && CTRL_R2(state[0]));
      case SSNES_STATE_SLOT_MINUS:
         return (CTRL_RSTICK_LEFT(state[0]) && CTRL_R2(state[0]));
      case SSNES_REWIND:
         return CTRL_RSTICK_DOWN(state[0]) && CTRL_R2(~state[0]);
      default:
         break;
   }
   #endif

   return false;
}

const input_driver_t input_xenon360 = {
   .init = xenon360_input_init,
   .poll = xenon360_input_poll,
   .input_state = xenon360_input_state,
   .key_pressed = xenon360_key_pressed,
   .free = xenon360_free_input,
   .ident = "xenon360",
};
