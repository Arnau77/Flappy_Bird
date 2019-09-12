#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "Modulebackground.h"
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
	position.x = 35;
	position.y = 80;
	speed = 0;
	angle = 0;
	return ret;
}

//Clean Up
bool ModulePlayer::CleanUp() {
	App->audio->StopChunk();
	App->audio->UnLoadChunk(jumpsound);
	App->audio->UnLoadChunk(hitsound);
	return true;
}

update_status ModulePlayer::PreUpdate() {
	if (App->background->startpipes == false && App->input->keyboardstate[SDL_SCANCODE_SPACE] == KEY_PUSHED) {
		App->background->startpipes = true;
		Animation* current_animation = &fly;
		current_animation->Reset();
	}
	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModulePlayer::Update()
{
	Animation* current_animation = &fly;
	if (App->background->startpipes == true) {
		current_animation = &jump;
		if (App->input->keyboardstate[SDL_SCANCODE_SPACE] == KEY_PUSHED) {
			App->audio->PlayChunk(jumpsound,0,1);
			speed = -2;
		}
		else speed +=0.125;
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
	if (game_over == true)current_animation = &idle;
/*
	////////////////////GODMODE/////////////////////////

	/*if (App->input->keyboardstate[SDL_SCANCODE_F5] == KEY_PUSHED) {
		if (body->to_delete != false) {
			body = App->collision->AddCollider({ position.x,(position.y - 100),35,20 }, COLLIDER_PLAYER, this);
			body2 = App->collision->AddCollider({ position.x,(position.y - 50),25,15 }, COLLIDER_PLAYER, this);
			body3 = App->collision->AddCollider({ position.x,(position.y - 20),45,35 }, COLLIDER_PLAYER, this);
		}
		else {
			body->to_delete = true;
			body2->to_delete = true;
			body3->to_delete = true;
		}

	}*/


	////////////////////LOSE AUTOMATICALLY/////////////////////////
	//if (App->input->keyboardstate[SDL_SCANCODE_F4] == KEY_PUSHED)App->UI->DamageTaken(1, 100, 0);

	// Draw everything --------------------------------------
	SDL_Rect currentframe = current_animation->GetCurrentFrame();
	App->render->Blit(App->background->graphics, position.x, position.y, &currentframe, angle);
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate() {
	return UPDATE_CONTINUE;
}


void ModulePlayer::OnCollision(Collider* player, Collider* other) {
	
}