#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "Globals.h"
#include "Animation.h"

struct SDL_Texture;
struct Mix_Chunk;


class ModuleUI :public Module {
public:
	ModuleUI();
	~ModuleUI();
	bool Init();
	update_status Update();
	bool CleanUp();
	void GetRandomPoint(int* rand1, int* rand2);

public:
	int score;
	SDL_Rect play_button{334,142,13,14};
	SDL_Rect pause_button{ 121,306,13,14 };
	SDL_Rect game_over{ 396,61,94,19 };
	SDL_Rect point_table{ 3,259,113,57 };
	SDL_Rect ok_button{ 462,42,40,14 };
	SDL_Rect share_button{ 470,110,40,14 };
	SDL_Rect new_record{ 112,501,16,7 };
	SDL_Rect bronze_medal{ 112,477,22,22 };
	SDL_Rect silver_medal{ 112,453,22,22 };
	SDL_Rect gold_medal{ 121,282,22,22 };
	SDL_Rect platinum_medal{ 121,258,22,22 };
	Animation glitter;
	Mix_Chunk *UI_sound;
	uint timer;
	bool fade_to_black;
	int current_frame_number;
	int rand1;
	int rand2;
	int alpha;
	int counter;
	bool alpha_increase;
	bool start_fade;
	bool increase_max;
	int game_over_height;
	int point_table_height;
	int pause_button_height;
	bool pause;
	bool play_pause;
	int max_score;
	int score_counting;
	char score_text[10];
	char max_score_text[10];
private:
	int font_score;
};
#endif