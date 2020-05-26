
#include <raylib.h>
#include <math.h>

#include "level.h"
#include "draw.h"
#include "state.h"

int main(int argc, char const *argv[]){

    // Initialization
    const int screen_width = 800;
    const int screen_height = 600;

    InitWindow(screen_width,screen_height,"Presente - the game");
    SetTargetFPS(60);
	
	//-------------------------------------------------------------------------------------------------------------------------
	//load images
	Image HeroRight = LoadImage("./resources/HeroRight.png");
	Image HeroLeft = LoadImage("./resources/HeroLeft.png");
	Image Minion = LoadImage("./resources/Skull.png");
	Image Brute = LoadImage("./resources/Vamp.png");
	Image Bullet = LoadImage("./resources/spell_kun.png");
	
	//Resize images
	ImageResize(&HeroRight, (HeroRight.width*PLAYER_RAD)/80, (HeroRight.height*PLAYER_RAD)/80 );
	ImageResize(&HeroLeft, (HeroLeft.width*PLAYER_RAD)/80 , (HeroLeft.height*PLAYER_RAD)/80 );
	ImageResize(&Minion, (Minion.width*MINION_RAD)/80 , (Minion.height*MINION_RAD)/80 );
	ImageResize(&Brute, (Brute.width*BRUTE_RAD)/80 , (Brute.height*BRUTE_RAD)/80 );
	ImageResize(&Bullet, (Bullet.width*BRUTE_RAD)/80 , (Bullet.height*BRUTE_RAD)/80 );
	
	//Load Texture from an image
	Texture2D Sprite_HeroRight = LoadTextureFromImage(HeroRight);
	Texture2D Sprite_HeroLeft = LoadTextureFromImage(HeroLeft);
	Texture2D Sprite_Minion = LoadTextureFromImage(Minion);
	Texture2D Sprite_Brute = LoadTextureFromImage(Brute);
			//Characters Sprite from https://pixel-poem.itch.io/dungeon-assetpuck
	Texture2D Sprite_Bullet = LoadTextureFromImage(Bullet);
			//Bullet Sprite by Master484 -- Download Sprite From: https://opengameart.org/content/bullet-collection-1-m484
	
	//Variables for Hero Animation
	Texture2D Sprite_Hero = Sprite_HeroRight;
    Rectangle FrameRec_H = {0.0f, 0.0f, (float)Sprite_Hero.width/4, (float)Sprite_Hero.height};
    int CurrentFrame_H = 0; 
    int FramesCounter_H = 0;   
    
    //Variables for Enemy Animation
    Rectangle Frame_Mi = {0.0f, 0.0f, (float)Sprite_Minion.width/4, (float)Sprite_Minion.height};
    Rectangle Frame_Br = {0.0f, 0.0f, (float)Sprite_Brute.width/4, (float)Sprite_Brute.height};
    int CurrentFrame_Enemy = 0; 
    int FramesCounter_Enemy = 0;   
    
    //Variables for Bullet/Spell Animation
    Rectangle FrameRec_B = {0.0f, 0.0f, (float)Sprite_Bullet.width/2, (float)Sprite_Bullet.height};
    int CurrentFrame_B = 0; 
    int FramesCounter_B = 0;
    //-------------------------------------------------------------------------------------------------------------------------
	
    // Initialize level and fill randomly
    level *lvl = level_new(50,40);
    level_fill_random(lvl,6);

    // Initialize state (and add enemies)
    state *sta = state_new();
    state_populate_random(lvl,sta,40);
	
	//------------------------------------------------------------------------------------
	//Music Variables
	
	InitAudioDevice();
	Music Dungeon = LoadMusicStream("./resources/Shippai.mp3");
    PlayMusicStream(Dungeon);
    bool pause = false;
    //Extract from Kaguya-sama Love Is War Original Soundtrack vol.2, 19-Shippai
    
    //------------------------------------------------------------------------------------
	
    // Main loop
    while(!WindowShouldClose()){

		//Music Control
		UpdateMusicStream(Dungeon);
        	// Restart music playing (stop and play)
    		if (IsKeyPressed(KEY_SPACE)){
            	StopMusicStream(Dungeon);
            	PlayMusicStream(Dungeon);
        	}
        	// Pause/Resume music playing
        	if (IsKeyPressed(KEY_P)){
            	pause = !pause;
            	if (pause) PauseMusicStream(Dungeon);
            	else ResumeMusicStream(Dungeon);
    		}

		
        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D);
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        
        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY()-GetScreenHeight()/2;
        float mouse_x = GetMouseX()-GetScreenWidth()/2;
        sta->aim_angle = atan2(-mouse_y,mouse_x);

        // Update the state
        state_update(lvl,sta);
        
		//Direction of the Player
		if(IsKeyDown(KEY_A))Sprite_Hero = Sprite_HeroLeft;    
        if(IsKeyDown(KEY_D))Sprite_Hero = Sprite_HeroRight;
        
        //Speed Respect to Player's Movement
		float FramesSpeed = sqrt(sta->pla.ent.vx*sta->pla.ent.vx+sta->pla.ent.vy*sta->pla.ent.vy);
		//Modify the Player Animation 
		FramesCounter_H++;
        if (FramesCounter_H >= (60/FramesSpeed)){
            FramesCounter_H = 0;
            CurrentFrame_H++;
            if (CurrentFrame_H > 5) CurrentFrame_H = 0;
            FrameRec_H.x = (float)CurrentFrame_H*(float)Sprite_Hero.width/4;
        }
        
        
        // Drawing
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            draw_state(lvl, sta,
				Sprite_Bullet, Sprite_Hero, Sprite_Brute, Sprite_Minion,
  				FrameRec_H, &Frame_Br, &Frame_Mi, &FrameRec_B,
   				&CurrentFrame_Enemy, &FramesCounter_Enemy, &CurrentFrame_B, &FramesCounter_B);
   				
            DrawText("Presente profe!",190,200,20,LIGHTGRAY);
            DrawText("Music {P = Resume|Pause / Space = Play|Stop}",190,100,20,LIGHTGRAY);
        EndDrawing();

    }

    // Closer window
    CloseWindow();

	//Unload images
	UnloadImage(HeroRight);
	UnloadImage(HeroLeft);
	UnloadImage(Minion);
	UnloadImage(Brute);
	UnloadImage(Bullet);
	
	//Unload Texture
	UnloadTexture(Sprite_HeroRight);
	UnloadTexture(Sprite_HeroLeft);
	UnloadTexture(Sprite_Minion);
	UnloadTexture(Sprite_Brute);
	UnloadTexture(Sprite_Bullet);
	
	//Unload Music
	UnloadMusicStream(Dungeon);
    CloseAudioDevice();
	
    // Free memory
    state_free(sta);
    level_free(lvl);

    return 0;
}
