#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "Modulebackground.h"
#include "ModuleUI.h"
//#include "SDL/include/SDL_timer.h"



ModulePlayer::ModulePlayer()
{
	position.x = 100;
	position.y = 100;

	//Idle animation
	idle.PushBack({ 31, 491, 17, 12 });

	// Fly animation
	fly.PushBack({ 3, 491, 17, 12 });
	fly.PushBack({ 31, 491, 17, 12 });
	fly.PushBack({ 59, 491, 17, 12 });
	fly.speed = 0.15f;

	// Jump animation
	jump.PushBack({ 3, 491, 17, 12 });
	jump.PushBack({ 31, 491, 17, 12 });
	jump.PushBack({ 59, 491, 17, 12 });
	jump.speed = 0.3f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	game_over = false;
	jumpsound = App->audio->LoadChunk("Audio_FX/sfx_wing.wav");
	hitsound = App->audio->LoadChunk("Audio_FX/sfx_hit.wav");
	diesound = App->audio->LoadChunk("Audio_FX/sfx_die.wav");
	pointsound= App->audio->LoadChunk("Audio_FX/sfx_point.wav");
	position.x = 35;
	position.y = 80;
	speed = 0;
	angle = 0;
	stop = false;
	point_sound_counter = 0;
	push_reset = reset = false;
	die_sound_bool = false;
	player_collider = App->collision->AddCollider({position.x,position.y,17,12}, COLLIDER_PLAYER, this);
	return ret;
}

//Clean Up
bool ModulePlayer::CleanUp() {
	App->audio->StopChunk();
	App->audio->UnLoadChunk(pointsound);
	App->audio->UnLoadChunk(diesound);
	App->audio->UnLoadChunk(jumpsound);
	App->audio->UnLoadChunk(hitsound);
	return true;
}

update_status ModulePlayer::PreUpdate() {
	if (App->background->fade == true && App->input->keyboardstate[SDL_SCANCODE_SPACE] != KEY_REPEAT) {
		App->audio->PlayChunk(App->UI->UI_sound);
		App->UI->start_fade = true;
		App->background->fade = false;
	}
	if (App->input->keyboardstate[SDL_SCANCODE_SPACE] == KEY_PUSHED && App->UI->pause == false) {
		if (App->background->startpipes == false && App->background->title_scene == false) {
			App->background->startpipes = true;
			Animation* current_animation = &fly;
			current_animation->Reset();
		}
		if (App->background->title_scene == true && App->UI->fade_to_black == false) App->background->fade = App->background->title_avoid_bug_fade = true;
	}
	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModulePlayer::Update()
{
	Animation* current_animation = &fly;
	if (reset == true) {
		reset = false;
		player_collider = App->collision->AddCollider({ position.x,position.y,15,11 }, COLLIDER_PLAYER, this);
	}
	if (App->background->startpipes == true) {
		current_animation = &jump;
		if (App->UI->pause == false) {
			if (stop == true) {
				stop = false;
				player_collider = App->collision->AddCollider({ position.x,position.y,15,11 }, COLLIDER_PLAYER, this);
			}
			if (point_sound_counter == 1)point_sound_counter++;
			else point_sound_counter = 0;
			if (App->input->keyboardstate[SDL_SCANCODE_SPACE] == KEY_PUSHED && game_over == false) {
				App->audio->PlayChunk(jumpsound, 0, 1);
				speed = -2;
			}
			else speed += 0.125;
			position.y += speed;
			angle = speed;
			angle *= 30;
			if (angle >= 90) {
				angle = 90;
				current_animation->Reset();
				current_animation = &idle;
			}
			if (position.y >= 185) {
				App->audio->PlayChunk(hitsound, 0, 1);
				speed = 0;
				position.y = 185;
				angle = 90;
				game_over = true;
				current_animation->Reset();
				current_animation = &idle;
			}
		}
		else {
			stop = true;
			player_collider->to_delete = true;
		}
	}
	if (game_over == true) {
		if (position.y < 185) {
			if (die_sound_bool == false && App->audio->Check_Playing() == 0) {
				App->audio->PlayChunk(diesound, 0, 1);
				die_sound_bool = true;
			}
			current_animation->Reset();
			current_animation = &idle;
			speed += 0.125;
			position.y += speed;
			angle = 90;
		}
		else position.y = 185;
		current_animation = &idle;
		if (push_reset == true && App->input->keyboardstate[SDL_SCANCODE_SPACE] != KEY_REPEAT) {
			if (App->UI->fade_to_black == false) {
				App->UI->start_fade = true;
				App->audio->PlayChunk(App->UI->UI_sound);
			}
			else if(App->UI->alpha==255)reset = true;
		}
	}
	if (reset == true) {
		push_reset = false;
		game_over = false;
		player_collider->to_delete = true;
		position.x = 35;
		position.y = 80;
		speed = 0;
		angle = 0;
		die_sound_bool = false;
		App->background->title_scene = true;
		App->background->title_height = 75;
		App->background->title_up = true;
		App->UI->score = 0;
	}

	////////////////////GODMODE/////////////////////////

	if (App->input->keyboardstate[SDL_SCANCODE_F5] == KEY_PUSHED) {
		if (player_collider->to_delete != false) {
			player_collider = App->collision->AddCollider({ position.x,position.y,15,11 }, COLLIDER_PLAYER, this);
		}
		else {
			player_collider->to_delete = true;
		}

	}


	////////////////////LOSE AUTOMATICALLY/////////////////////////
	if (App->input->keyboardstate[SDL_SCANCODE_F4] == KEY_PUSHED) {
		game_over = true;
		App->UI->pause = false;
		stop = false;
		App->audio->PlayChunk(hitsound, 0, 1);
	}

	/////////////////////TITLE SCENE/////////////////////////////////
	if (App->background->title_scene == true) {
		position.x = 115;
		position.y = App->background->title_height + 5;
	}
	else position.x = 35;


	// Draw everything --------------------------------------
	SDL_Rect currentframe;
	if(App->UI->pause==false) currentframe = current_animation->GetCurrentFrame();
	else currentframe = current_animation->GetCurrentFrameWithoutAnim();
	player_collider->SetPos(position.x, position.y);
	if (angle > 50) {
		player_collider->rect.w = 12;
		player_collider->rect.x += 2;
	}
	else player_collider->rect.w = 15;
	App->render->Blit(App->background->graphics, position.x, position.y, &currentframe, angle);
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate() {
	return UPDATE_CONTINUE;
}


void ModulePlayer::OnCollision(Collider* player, Collider* other) {
	if (other->type == COLLIDER_POINT) {
		if (point_sound_counter == 0) {
			App->audio->PlayChunk(pointsound, 0, -1);
			App->UI->score++;
			point_sound_counter++;
		}
		else if (point_sound_counter == 2)point_sound_counter--;
	}
	else if(game_over==false){
		game_over = true;
		App->audio->PlayChunk(hitsound, 0, 1);
	}
}