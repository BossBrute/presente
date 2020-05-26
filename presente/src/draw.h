#ifndef DRAW_H
#define DRAW_H

#include "level.h"
#include "state.h"
#include <raylib.h>
#include <stdio.h>

#define framesSpeed1 4
#define framesSpeed2 80

// Draws the cells of the level in the screen
void draw_level(const level *lvl);

// Draws the whole state and the level in the screen
void draw_state(const level *lvl, const state *sta, 
				Texture2D Sprite_Bullet,Texture2D Sprite_Hero, Texture2D Sprite_Brute, Texture2D Sprite_Minion,
  				Rectangle FrameRec_H, Rectangle *Frame_Br, Rectangle *Frame_Mi, Rectangle *FrameRec_B,
   				int *CurrentFrame_Enemy, int *FramesCounter_Enemy, int *CurrentFrame_B, int *FramesCounter_B);  
  
#endif
