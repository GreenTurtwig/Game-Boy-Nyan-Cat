#include <gb/gb.h>
#include <stdio.h>

#include "cat.c" // Load Nyan Cat sprites
#include "christmas.c"
#include "gameboy.c"
#include "tac.c"

#include "rainbow.c" // Load rainbow sprites
#include "title.c" // Load title screen background
#include "gbt_player.h" //Load GBT Player

extern const unsigned char * song_Data[];

static int fc = 0; // Basically a "counter"
static int titleFin = 0; // There are no booleans
static int selection = 1; // Nyan Cat style selected
static int abPressed = 0; // Keep track of if the dpad is pressed (to prevent Nyan Cat from changing style while held down)

static int x = 70; // X and Y coords of Nyan Cat
static int y = 70;
static int c = 0; // Counter so Nyan Cat knows what frame of animation it is on

void title(); // Make C happy!
void nyanCat();
void initialiseNyanCat();

void main() {
	  SHOW_BKG;
    SHOW_SPRITES;
    set_bkg_data(0, 94, tiledata); // Set title screen background

    while(1) { // Main "game" loop
        wait_vbl_done();
        fc++;

        if (titleFin == 0) { // Check if the title screen has finished
            title();
        } else {
            nyanCat();
        }
    }
}

void title() {
    if (fc == 30) {
        set_bkg_tiles(0, 0, 20, 18, tilemap); // Set background to show "PRESS START"
    } else if (fc == 60) {
        set_bkg_tiles(0, 0, 20, 18, tilemapBlank); // Set background to hide "PRESS START"
        fc = 0;
    }

    if (joypad() & J_START || joypad() & J_A) { // Check if Start or A button is pressed
        fc = 0;
        titleFin = 1;
        HIDE_BKG;
        initialiseNyanCat();
    }
}

void nyanCat() {
    gbt_update(); // Music

    if (joypad() != J_A && joypad() != J_B) { // Check if AB are not pressed anymore
        abPressed = 0;
    }

    if (joypad() & J_A && abPressed == 0) { // If AB are pressed up and AB weren't already pressed
        selection++; // Up selection by one

        if (selection == 5) { // If selection
          set_sprite_data(0, 96, cat);
          selection = 1;
        } else if (selection == 2) {
          set_sprite_data(0, 96, gameboy);
        } else if (selection == 3) {
          set_sprite_data(0, 96, christmas);
        } else if (selection == 4) {
          set_sprite_data(0, 96, tac);
        }
        abPressed = 1; // Set the AB buttons to "pressed"
    }
    if (joypad() & J_B && abPressed == 0) {
        selection--;

        if (selection == 0) {
          set_sprite_data(0, 96, tac);
          selection = 4;
        } else if (selection == 1) {
          set_sprite_data(0, 96, cat);
        } else if (selection == 2) {
          set_sprite_data(0, 96, gameboy);
        } else if (selection == 3) {
          set_sprite_data(0, 96, christmas);
        }
        abPressed = 1;
    }


    // Change coords on dpad press
    if (joypad() & J_UP) { // The "&" is there to check if "J_UP" is one of the things returned by "joypad()"
      y--;                 // You have to use "&" here because "==" will only work when one dpad direction is pressed
      if (y == 9) { // To prevent Nyan Cat from going off-screen
          y = 10;
      }
    }
    if (joypad() & J_DOWN) {
      y++;
      if (y == 135) {
          y = 134;
      }
    }
    if (joypad() & J_LEFT) {
      x--;
      if (x == 7) {
          x = 8;
      }
    }
    if (joypad() & J_RIGHT) {
      x++;
      if (x == 137) {
          x = 136;
      }
    }

    // Update position of sprite
    move_sprite(0, x, y);
    move_sprite(1, x + 8, y);
    move_sprite(2, x, y + 16);
    move_sprite(3, x + 8, y + 16);
    move_sprite(4, x + 16, y);
    move_sprite(5, x + 24, y);
    move_sprite(6, x + 16, y + 16);
    move_sprite(7, x + 24, y + 16);



    if (fc % 5 == 0) { // If there is no remainder

      // Nyan Cat Sprite
      set_sprite_tile(0, 0 + (16 * c));
      set_sprite_tile(1, 3 + (16 * c));
      set_sprite_tile(2, 4 + (16 * c));
      set_sprite_tile(3, 6 + (16 * c));
      set_sprite_tile(4, 8 + (16 * c));
      set_sprite_tile(5, 11 + (16 * c));
      set_sprite_tile(6, 12 + (16 * c));
      set_sprite_tile(7, 14 + (16 * c));

      c++;
      if (c == 6) { // Check if Nyan Cat's frame counter is 6
      	c = 0;
      }
    }

    // Rainbow Sprite
    // if ((fc == 19)) { // When the counter reaches 19 change frame of the rainbow
    //   set_sprite_tile(8, 112);
    //   set_sprite_tile(9, 114);
    //   set_sprite_tile(10, 116);
    //   set_sprite_tile(11, 118);
    //   set_sprite_tile(12, 120);
    //   set_sprite_tile(13, 122);
    //   set_sprite_tile(14, 124);
    //   set_sprite_tile(15, 126);
    // } else if (fc == 39) { // When the counter reaches 39 revert frame of the rainbow
    //   set_sprite_tile(8, 96);
    //   set_sprite_tile(9, 98);
    //   set_sprite_tile(10, 100);
    //   set_sprite_tile(11, 102);
    //   set_sprite_tile(12, 104);
    //   set_sprite_tile(13, 106);
    //   set_sprite_tile(14, 108);
    //   set_sprite_tile(15, 110);
    // }

    if (fc == 40) { // Reset counter when it reaches 40
        fc = 0;
    }
}

void initialiseNyanCat() { // Setup Nyan Cat (and rainbow trail)
    // Play music
    disable_interrupts();
    gbt_play(song_Data, 2, 7);
    gbt_loop(1);
    set_interrupts(VBL_IFLAG);
    enable_interrupts();

    // Nyan Cat Sprite
    SPRITES_8x16;
    set_sprite_data(0, 96, cat);

    set_sprite_tile(0, 0);
    move_sprite(0, x, y);
    set_sprite_tile(1, 3);
    move_sprite(1, x + 8, y);
    set_sprite_tile(2, 4);
    move_sprite(2, x, y + 16);
    set_sprite_tile(3, 6);
    move_sprite(3, x + 8, y + 16);
    set_sprite_tile(4, 8);
    move_sprite(4, x + 16, y);
    set_sprite_tile(5, 11);
    move_sprite(5, x + 24, y);
    set_sprite_tile(6, 12);
    move_sprite(6, x + 16, y + 16);
    set_sprite_tile(7, 14);
    move_sprite(7, x + 24, y + 16);


    // Rainbow Sprite
    // SPRITES_8x16;
    // set_sprite_data(96, 32, rainbow);

    // set_sprite_tile(8, 96);
    // move_sprite(8, 3, y);
    // set_sprite_tile(9, 98);
    // move_sprite(9, 3 + 8, y);
    // set_sprite_tile(10, 100);
    // move_sprite(10, 3, y + 16);
    // set_sprite_tile(11, 102);
    // move_sprite(11, 3 + 8 , y + 16);
    // set_sprite_tile(12, 104);
    // move_sprite(12, 3 + 16, y);
    // set_sprite_tile(13, 106);
    // move_sprite(13, 3 + 24, y);
    // set_sprite_tile(14, 108);
    // move_sprite(14, 3 + 16, y + 16);
    // set_sprite_tile(15, 110);
    // move_sprite(15, 3 + 24, y + 16);

    // set_sprite_tile(16, 96);
    // move_sprite(16, 3 + 26, y);
    // set_sprite_tile(17, 98);
    // move_sprite(17, 3 + 26 + 8, y);
    // set_sprite_tile(18, 100);
    // move_sprite(18, 3 + 26, y + 16);
    // set_sprite_tile(19, 102);
    // move_sprite(19, 3 + 26 + 8 , y + 16);
    // set_sprite_tile(20, 104);
    // move_sprite(20, 3 + 26 + 16, y);
    // set_sprite_tile(21, 106);
    // move_sprite(21, 3 + 26 + 24, y);
    // set_sprite_tile(22, 108);
    // move_sprite(22, 3 + 26 + 16, y + 16);
    // set_sprite_tile(23, 110);
    // move_sprite(23, 3 + 26 + 24, y + 16);
}
