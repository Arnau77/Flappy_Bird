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
	float ground_movement;// [2];
	float pipe_movement[3];
	int pipe_height[3];
	bool pipe_true[3];
};
#endif //