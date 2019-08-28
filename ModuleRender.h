#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include "Globals.h"


struct SDL_Renderer;
struct SDL_Texture;


class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PostUpdate();
	update_status PreUpdate();
	bool CleanUp();

	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section);
public:
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* tex;
};

#endif //__ModuleRenderer_H__