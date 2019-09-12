#include "Globals.h"
#include "Application.h"
#include "Modulebackground.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Animation.h"
#include"ModuleCollision.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL.h"
#include <stdlib.h>
#include <time.h>

ModuleBackground::ModuleBackground() :Module() {
	srand(time(NULL));
}

ModuleBackground::~ModuleBackground() {}

bool ModuleBackground::Start() {
	graphics = App->textures->Load("Sprites/spritesheet.png");
	ground_movement = 0;
	for (int i = 0; i < 3; i++) {
		pipe_true[i] = false;
		pipe_movement[i] = SCREEN_WIDTH;
		do {
			pipe_height[i] = rand() % 201;
		} while (pipe_height[i] < 60);
	}
	pipe_true[0] = true;
	startpipes = false;
	return true;
}
update_status ModuleBackground::Update() {
	App->render->Blit(graphics, 0, 0, &(back));  //Background
	//PIPES
	if (App->player->game_over == true)startpipes = false;
	for (int i = 0; i < 3; i++) {
		if (pipe_true[i] == true) {
			App->render->Blit(graphics, pipe_movement[i], pipe_height[i] - 220, &(pipe_down));
			App->render->Blit(graphics, pipe_movement[i], pipe_height[i], &(pipe_up));
		}
	}
	if (startpipes == true) {
		for (int i = 0; i < 3; i++) {
			if (pipe_true[i] == true && pipe_movement[i] <= -26) {
				pipe_movement[i] = SCREEN_WIDTH;
				pipe_true[i] = false;
				do {
					pipe_height[i] = rand() % 201;
				} while (pipe_height[i] < 60);
			}
			else if (pipe_true[i] == true)pipe_movement[i] -= 1;
			else {
				if (i != 0) {
					if (pipe_true[i - 1] == true && pipe_movement[i - 1] <= 66)pipe_true[i] = true;
				}
				else if (pipe_true[2] == true && pipe_movement[2] <= 66)pipe_true[i] = true;
			}
		}
	}
	

	//GROUND
	App->render->Blit(graphics, ground_movement, 200, &(ground));
	if (App->player->game_over == false) {
		if (ground_movement <= -23)ground_movement = 0;
		else ground_movement -= 1;
	}
	
	return UPDATE_CONTINUE;
}
bool ModuleBackground::CleanUp()
{
	App->textures->Unload(graphics);
	return true;
}
void ModuleBackground::OnCollision(Collider* wall, Collider* enemy) {

}