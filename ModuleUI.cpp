#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleFonts.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL_timer.h"
#include <stdlib.h>
#include <time.h>
#include<stdio.h>


ModuleUI::ModuleUI() {
	max_score = 0;
	srand(time(NULL));

	//Glitter animation
	glitter.PushBack({ 140,343,1,1 });
	glitter.PushBack({ 139,368,3,3 });
	glitter.PushBack({ 138,393,5,5 });
	glitter.speed = 0.05;
}
ModuleUI::~ModuleUI() {}
bool ModuleUI::Init() {
	font_score = App->fonts->Load("Sprites/number_font.png", "0123456789", 1);
	score = score_counting = 0;
	UI_sound = App->audio->LoadChunk("Audio_FX/sfx_swooshing.wav");
	play_pause = pause = increase_max = false;
	timer = 0;
	counter = 0;
	pause_button_height = 10;
	point_table_height = 258;
	game_over_height = 65;
	alpha = 5;
	rand1 = rand2 = 0;
	current_frame_number = 0;
	fade_to_black = false;
	alpha_increase = true;
	return true;
}
bool ModuleUI::CleanUp() {
	App->audio->StopChunk();
	App->audio->UnLoadChunk(UI_sound);
	App->fonts->UnLoad(font_score);
	return true;
}
update_status ModuleUI::Update() {
	if (start_fade == true) {
		fade_to_black = true;
		alpha = 5;
		timer = SDL_GetTicks();
		start_fade = false;
		alpha_increase = true;
	}
	if (fade_to_black == true) {
		App->render->DrawQuad({ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }, 0, 0, 0, alpha);
		if (alpha < 255 && alpha_increase == true) alpha += 25;
		else if(alpha==255){
			alpha_increase = false;
			if (App->background->title_avoid_bug_fade == true) {
				App->background->title_scene = false;
				App->background->title_avoid_bug_fade = false;
				App->player->position.y = 80;
			}
		}
		if (alpha_increase == false)alpha - 25;
		if (timer + 300 <= SDL_GetTicks()) {
			fade_to_black = false;
			if (alpha_increase == true) {
				if (App->background->title_scene == true) {
					App->background->title_scene = false;
					App->player->position.y = 80;
				}
				else App->background->title_scene = true;
			}
		}
	}
	if (App->player->game_over == false) {
		if (timer > 0 && fade_to_black == false) {
			timer = counter = score_counting = 0;
			increase_max = false;
			point_table_height = 258;
		}
		if (App->input->keyboardstate[SDL_SCANCODE_RETURN] == KEY_PUSHED) {
			play_pause = !play_pause;
			pause_button_height += 2;
		}
		else if (App->input->keyboardstate[SDL_SCANCODE_RETURN] != KEY_REPEAT || App->input->keyboardstate[SDL_SCANCODE_SPACE] == KEY_PUSHED) {
			if(pause!=play_pause)App->audio->PlayChunk(UI_sound, 0, -1);
			pause_button_height = 10;
			pause = play_pause;
		}
		if (fade_to_black == false && App->background->title_scene == false) {
			sprintf_s(score_text, 10, "%7d", score);
			App->fonts->BlitText(28, 15, font_score, score_text);
			if (pause == true)App->render->Blit(App->background->graphics, 10, pause_button_height, &(play_button));
			else App->render->Blit(App->background->graphics, 10, pause_button_height, &(pause_button));
		}
	}
	else if (fade_to_black == false) {
		Animation* current_animation;
		current_animation = &glitter;
		if (timer == 0) {
			if (counter == 0)App->render->DrawQuad({ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }, 255, 255, 255, 255);
			timer = SDL_GetTicks();
		}
		switch (counter) {
		case 9:
			if (App->player->push_reset == true)App->render->Blit(App->background->graphics, 25, 177, &(ok_button));
			else App->render->Blit(App->background->graphics, 25, 175, &(ok_button));
			App->render->Blit(App->background->graphics, 80, 175, &(share_button));
			play_pause = pause = false;
			if (App->input->keyboardstate[SDL_SCANCODE_SPACE] == KEY_PUSHED && App->player->position.y == 185) App->player->push_reset = true;
		case 8:
		case 7:
			if (timer + 400 <= SDL_GetTicks() && counter < 9 ) counter = 9;
			else if (timer + 200 <= SDL_GetTicks() && counter < 9) counter = 8;
		case 6:
			if ((timer + 50 <= SDL_GetTicks() || score_counting == 0) && score > score_counting) {
				score_counting++;
				if (max_score < score_counting) {
					max_score++;
					increase_max = true;
				}
				timer = 0;
			}
			else if(counter==6) {
				counter = 7;
				timer = 0;
			}
		case 5:
			if (timer + 700 <= SDL_GetTicks() && counter == 5) counter = 6;
		case 4:
			App->render->Blit(App->background->graphics, 15, point_table_height, &(point_table));
			if (counter > 7 && increase_max == true)App->render->Blit(App->background->graphics, 82, 119, &(new_record));
			if (counter > 7 && score >= 10){
				if (score >= 40)App->render->Blit(App->background->graphics, 28, 112, &(platinum_medal));
				else if (score >= 30)App->render->Blit(App->background->graphics, 28, 112, &(gold_medal));
				else if (score >= 20)App->render->Blit(App->background->graphics, 28, 112, &(silver_medal));
				else App->render->Blit(App->background->graphics, 28, 112, &(bronze_medal));
				SDL_Rect current_frame = current_animation->GetCurrentFrame();
				if (current_animation->finished == 1) {
					GetRandomPoint(&rand1, &rand2);
					current_frame_number = 0;
				}
				else if (current_frame_number < (int)current_animation->current_frame) {
					current_frame_number++;
					rand1 --;
					rand2 --;
				}
				App->render->Blit(App->background->graphics, rand1+28, rand2+112, &(current_frame));
			}
			sprintf_s(score_text, 10, "%7d", score_counting);
			App->fonts->BlitText(68, point_table_height + 17, font_score, score_text);
			sprintf_s(max_score_text, 10, "%7d", max_score);
			App->fonts->BlitText(68, point_table_height + 38, font_score, max_score_text);
			if (point_table_height > 90)point_table_height-=8;
			else if(counter==4){
				counter = 5;
				timer = 0;
			}
		case 3:
		if (timer + 700 <= SDL_GetTicks()&&counter==3) counter = 4;
		case 2:
			if (game_over_height == 65&&counter==2) {
				timer = 0;
				counter = 3;
			}
		case 1:
			App->render->Blit(App->background->graphics, 25, game_over_height, &(game_over));
			if (game_over_height == 65 && counter == 1) {
				game_over_height--;
				App->audio->PlayChunk(UI_sound);
			}
			else if (game_over_height > 45 && counter == 1)game_over_height --;
			else if (game_over_height < 65)game_over_height ++;
			if (game_over_height == 55)counter = 2;
			break;
		case 0:
			if (timer + 700 <= SDL_GetTicks()) counter = 1;
			current_animation->Reset();
			GetRandomPoint(&rand1, &rand2);
		}
	}
	return UPDATE_CONTINUE;
}



void ModuleUI::GetRandomPoint(int* rand1, int* rand2) {
	int min=0;
	int max=21;
	int n_support=0;
	bool complete = false;
	*rand1 = rand() % 21;
	*rand1 ++;
	for (int i = 9; i <= 14; i++) {
		if (*rand1 == i || *rand2 == i) complete = true;
	}
	if (complete == true) {
		*rand2 = rand() % 21;
		*rand1 --;
	}
	else {
		if (*rand1 > 9) {
			for (int i = 22; i >= *rand1; i--) n_support++;
		}
		else n_support = *rand1;
		*rand1--;
		if (n_support > 2) {
			for (int i = 8; i >= n_support; i--) {
				if (i % 2 == 0&&i>=4)min++;
				else if (i < 3)min+=2;
				else min++;
			}
		}
		max = 21 - (min*2);
		*rand2 = rand() % max;
		*rand2 += min;
	}
}