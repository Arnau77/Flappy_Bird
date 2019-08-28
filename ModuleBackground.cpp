#include "Globals.h"
#include "Application.h"
#include "Modulebackground.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Animation.h"
#include"ModuleCollision.h"
#include "SDL/include/SDL.h"
#include <stdlib.h>
#include <time.h>

ModuleBackground::ModuleBackground() :Module() {
	srand(time(NULL));
}

ModuleBackground::~ModuleBackground() {}

bool ModuleBackground::Start() {
	graphics = App->textures->Load("Sprites/spritesheet.png");
	//for (int i = 0; i < 2; i++)ground_movement[i] = 0;
	ground_movement = 0;
	for (int i = 0; i < 3; i++) {
		pipe_true[i] = false;
		pipe_movement[i] = SCREEN_WIDTH;
		do {
			pipe_height[i] = rand() % 201;
		} while (pipe_height[i] < 40);
	}
	pipe_true[0] = true;
	return true;
}
update_status ModuleBackground::Update() {
	App->render->Blit(graphics, 0, 0, &(back));  //Background
	//PIPES
	for (int i = 0; i < 3; i++) {
		if (pipe_true[i] == true) {
			App->render->Blit(graphics, pipe_movement[i], pipe_height[i] - 200, &(pipe_down));
			App->render->Blit(graphics, pipe_movement[i], pipe_height[i], &(pipe_up));
		}
	}
	for (int i = 0; i < 3; i++) {
		if (pipe_true[i] == true && pipe_movement[i] <= -26) {
			pipe_movement[i] = SCREEN_WIDTH;
			pipe_true[i] = false;
			do {
				pipe_height[i] = rand() % 201;
			} while (pipe_height[i] < 40);
		}
		else if (pipe_true[i] == true)pipe_movement[i]-=0.5;
		else {
			if (i != 0) {
				if (pipe_true[i - 1] == true && pipe_movement[i - 1] <= 66)pipe_true[i] = true;
			}
			else if (pipe_true[2] == true && pipe_movement[2] <= 66)pipe_true[i] = true;
		}
	}
	

	//GROUND
	App->render->Blit(graphics, ground_movement, 200, &(ground));
	if (ground_movement <= -23.5)ground_movement = 0;
	else ground_movement-= 0.5;
	
	return UPDATE_CONTINUE;
}
bool ModuleBackground::CleanUp()
{
	App->textures->Unload(graphics);
	return true;
}
void ModuleBackground::OnCollision(Collider* wall, Collider* enemy) {

}