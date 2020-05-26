#include "draw.h"

#include <raylib.h>

void draw_level(const level *lvl){
    // Draw cells, iterate, including borders outside the level
    for(int y=-1;y<=lvl->size_y;y++){
        for(int x=-1;x<=lvl->size_x;x++){
            char cell = level_get(lvl,x,y);
            // If the cell is a wall, paint it
            if(cell=='#'){
                DrawRectangle(TILE_SIZE*x,TILE_SIZE*y,TILE_SIZE,TILE_SIZE,BROWN);
            }
        }
    }
}

void draw_state(const level *lvl, const state *sta, 
				Texture2D Sprite_Bullet,Texture2D Sprite_Hero, Texture2D Sprite_Brute, Texture2D Sprite_Minion,
  				Rectangle FrameRec_H, Rectangle *Frame_Br, Rectangle *Frame_Mi, Rectangle *FrameRec_B,
   				int *CurrentFrame_Enemy, int *FramesCounter_Enemy, int *CurrentFrame_B, int *FramesCounter_B){

    // Initialize a camera to be used as drawing context
    Camera2D cam;
    // make the camera follow the player
    cam.target.x = sta->pla.ent.x;
    cam.target.y = sta->pla.ent.y;
    // make it so that the player is in the middle of the screen
    cam.offset.x = GetScreenWidth()/2.0;
    cam.offset.y = GetScreenHeight()/2.0;
    // set the camera rotation to 0
    cam.rotation = 0.0;
    // set the camera zoom to 1
    cam.zoom = 1.0;

    // Draw everything relative to the camera from now on
    BeginMode2D(cam);

    // Draw level
    draw_level(lvl);

    // Draw enemies
    for(int i=0;i<sta->n_enemies;i++){
        // Get a copy of the enemy entity
        entity ent = sta->enemies[i].ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        //Modify the Enemy Frame for the Animation 
        if (*FramesCounter_Enemy >= (60/framesSpeed1)){
            (*CurrentFrame_Enemy)= 0;
            (*FramesCounter_Enemy)++;
            if (*CurrentFrame_Enemy > 4) *CurrentFrame_Enemy = 0;
            Frame_Mi->x = (float)*CurrentFrame_Enemy*(float)Sprite_Minion.width/4;
			Frame_Br->x = (float)*CurrentFrame_Enemy*(float)Sprite_Brute.width/4;
        }
        //Draw the Enemy Depending on the Type
		if(sta->enemies[i].kind == MINION){
            DrawTextureRec(Sprite_Minion, *Frame_Mi, vec, WHITE);
        }else{
            DrawTextureRec(Sprite_Brute, *Frame_Br, vec, WHITE);
        }
    }

    // Draw player
    {
        // Get a copy of the player's entity
        entity ent = sta->pla.ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        // Draw the Player Textures According to the Frames Respect to the Speed
        DrawTextureRec(Sprite_Hero, FrameRec_H, vec, WHITE); 
    }

    // Draw bullets
    for(int i=0;i<sta->n_bullets;i++){
        // Get a copy of the bullet entity
        entity ent = sta->bullets[i].ent;
        // Initialize a Vector2 that represents the center of the entity position
        Vector2 vec = {ent.x,ent.y};
        // Draw the animated texture for a bullet 
        if (*FramesCounter_B >= (60/framesSpeed2)){
			(*FramesCounter_B) = 0;
			(*CurrentFrame_B)++;
		if (*CurrentFrame_B > 8) *CurrentFrame_B = 0;
			FrameRec_B->x = (float)*CurrentFrame_B*(float)Sprite_Bullet.width/2;
		}
		
		DrawTextureRec(Sprite_Bullet, *FrameRec_B, vec, WHITE); 
        } 

    // Stop drawing relative to the camera
    EndMode2D();
}
