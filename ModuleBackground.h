#ifndef _MODULEBACKGROUNG_H
#define _MODULEBACKGROUND_H
#include"Module.h"
#include "Animation.h"
#include "ModuleTextures.h"
#include "p2Point.h"

struct Mix_Chunk;
struct SDL_Texture;
struct Collider;

class ModuleBackground :public Module {
public:
	ModuleBackground();
	~ModuleBackground();
	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider*, Collider*);

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect back{0,0,144,256};
	SDL_Rect ground{ 292,0,168,56};
	SDL_Rect pipe_down{ 56,323,26,160 };
	SDL_Rect pipe_up{ 84,323,26,160 };
	SDL_Rect tap_tap{ 292,91, 55,49 };
	SDL_Rect get_ready{ 295,59,92,25 };
	SDL_Rect title{ 347,88,96,22 };
	SDL_Rect score_button{ 470,145,40,14 };
	SDL_Rect start_button{ 470,126,40,14 };
	SDL_Rect studios{ 357,154,95,7 };
	Collider* *pipe_collider;
	Collider* *point_collider;
	bool title_scene;
	bool title_avoid_bug_fade;
	bool fade;
	bool startpipes;
	float ground_movement;
	float pipe_movement[3];
	float title_height;
	bool title_up;
	int pipe_height[3];
	bool pipe_true[3];
};
#endif //