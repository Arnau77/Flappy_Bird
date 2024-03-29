#include "Globals.h"
#include "Application.h"
#include "Modulebackground.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Animation.h"
#include"ModuleCollision.h"
#include "ModulePlayer.h"
#include "ModuleUI.h"
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
	title_height = 75;
	pipe_true[0] = true;
	title_scene = true;
	startpipes = false;
	title_up = true;
	title_avoid_bug_fade = true;
	fade = false;
	pipe_collider = new Collider*[6];
	point_collider = new Collider*[3];
	for (int i = 0; i < 6; i++) {
		if (i < 3) {
			pipe_collider[i] = App->collision->AddCollider({ SCREEN_WIDTH,pipe_height[i],26,160 }, COLLIDER_WALL);
			point_collider[i] = App->collision->AddCollider({ SCREEN_WIDTH,0,1,SCREEN_HEIGHT}, COLLIDER_POINT);
		}
		else pipe_collider[i] = App->collision->AddCollider({ SCREEN_WIDTH,pipe_height[i-3]-420,26,360 }, COLLIDER_WALL);
	}
	return true;
}


update_status ModuleBackground::Update() {
	App->render->Blit(graphics, 0, 0, &(back));  //Background
	//PIPES
	if (App->player->game_over == true) startpipes = false;
	if (App->player->reset == true) {
		for (int i = 0; i < 3; i++) {
			pipe_true[i] = false;
			pipe_movement[i] = SCREEN_WIDTH;
			do {
				pipe_height[i] = rand() % 201;
			} while (pipe_height[i] < 60);
		}
		pipe_true[0] = true;
	}
	if (startpipes == true && App->UI->pause == false) {
		for (int i = 0; i < 3; i++) {
			if (pipe_true[i] == true && pipe_movement[i] <= -26) {
				pipe_movement[i] = SCREEN_WIDTH;
				pipe_true[i] = false;
				do {
					pipe_height[i] = rand() % 201;
				} while (pipe_height[i] < 60);
			}
			else if (pipe_true[i] == true) pipe_movement[i] -= 1;
			else {
				if (i != 0) {
					if (pipe_true[i - 1] == true && pipe_movement[i - 1] <= 66)pipe_true[i] = true;
				}
				else if (pipe_true[2] == true && pipe_movement[2] <= 66)pipe_true[i] = true;
			}
		}
	}
	else if (title_scene==false && startpipes == false && App->player->game_over == false) {
		App->render->Blit(graphics, 25, 45, &(get_ready));
		App->render->Blit(graphics, 45, 90, &(tap_tap));
	}

	for (int i = 0; i < 3; i++) {
		pipe_collider[i]->SetPos(pipe_movement[i], pipe_height[i]);
		point_collider[i]->SetPos(pipe_movement[i]+26, 0);
		pipe_collider[i+3]->SetPos(pipe_movement[i], pipe_height[i] - 420);
	}
	for (int i = 0; i < 3; i++) {
		if (pipe_true[i] == true) {
			App->render->Blit(graphics, pipe_movement[i], pipe_height[i] - 220, &(pipe_down));
			App->render->Blit(graphics, pipe_movement[i], pipe_height[i], &(pipe_up));
		}
	}
	

	//GROUND
	App->render->Blit(graphics, ground_movement, 200, &(ground));
	if (App->player->game_over == false && App->UI->pause == false) {
		if (ground_movement <= -23)ground_movement = 0;
		else ground_movement -= 1;
	}

	////TITLE SCENE
	if (title_scene == true) {
		App->render->Blit(graphics, 15, title_height, &(title));
		if (title_up == true) {
			if (title_height > 65)title_height -= 0.5;
			else title_up = false;
		}
		else {
			if (title_height < 85)title_height += 0.5;
			else title_up = true;
		}
		if (fade == true)App->render->Blit(App->background->graphics, 25, 177, &(start_button));
		else App->render->Blit(App->background->graphics, 25, 175, &(start_button));
		App->render->Blit(App->background->graphics, 80, 175, &(score_button));
		App->render->Blit(App->background->graphics, 25, 215, &(studios));
	}

	
	return UPDATE_CONTINUE;
}
bool ModuleBackground::CleanUp()
{
	delete[] pipe_collider;
	delete[] point_collider;
	App->textures->Unload(graphics);
	return true;
}
void ModuleBackground::OnCollision(Collider* wall, Collider* enemy) {

}