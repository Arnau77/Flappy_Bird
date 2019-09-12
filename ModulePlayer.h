#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

struct Mix_Chunk;
struct Collider;

#define MAX_HEIGHT 20

class ModulePlayer : public Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(Collider*, Collider*);

public:
	iPoint position;
	Mix_Chunk *jumpsound;
	Mix_Chunk *hitsound;
	Animation jump;
	Animation fly;
	Animation idle;
	Animation* current_animation;
	float speed;
	bool game_over;
	float angle;
};

#endif