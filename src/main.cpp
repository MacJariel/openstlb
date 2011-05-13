#include "Setup/common.h"
#include "BattleMapSystem/BattleMapGameplay.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#if 0
#include "GfxSystem/Palette.h"
#include "GfxSystem/Sprite.h"
#include "GfxSystem/SpriteAnimation.h"
#include "BattleMapSystem/BattleMap.h"
#include "BattleMapSystem/UnitType.h"
#include "ResourceSystem/Archives/ArchiveMgr.h"
#include "Utils/StringConversions.h"
#include "Utils/LzwCodec.h"

TTF_Font *DEBUG_font;

SDL_Surface* theScreen;

Sprite* s;

void loadPalette(const string& paletteFile, SDL_Surface* surface, int firstcolor)
{
	FILE* f = fopen(paletteFile.c_str(), "rb");
	SDL_Color colors[255];
	int i = 0;
	while (!feof(f))
	{
		fread(&colors[i].r, 1, 1, f);
		fread(&colors[i].g, 1, 1, f);
		fread(&colors[i].b, 1, 1, f);
		i++;
	}
	SDL_SetPalette(surface, SDL_PHYSPAL, colors, firstcolor, i);
}

void blitMap(SDL_Surface* screen, BattleMapSystem::BattleMap& battleMap, vector<Sprite*>& layer1Sprites,
		vector<Sprite*>& layer2Sprites, vector<SpriteAnimation*>& animSprites, int xFrom, int yFrom)
{
	SDL_FillRect(screen, NULL, 0);

	int xTo = xFrom + 15;
	int yTo = yFrom + 50;

	for (int i = 0; i < battleMap.width() * battleMap.heigth(); ++i)
	{
		int x = i % battleMap.width();
		int y = i / battleMap.width();

		if (x < xFrom || x >= xTo || y < yFrom || y >= yTo)
			continue;

		Sprite* sprite = layer1Sprites[battleMap.layer1SpriteIdx(i)];

		PaletteFilter* paletteFilter = 0;

		if (x < 1 || x >= battleMap.width() - 1 || y < 7 || y >= battleMap.heigth() - 7)
		{
			paletteFilter = &palettes[pfDarker];
		}

		sprite->draw(screen, x - xFrom, y - yFrom, battleMap.level(i), paletteFilter);

		//Sprite::drawText(screen, x - xFrom, y - yFrom, battleMap.level(i), toStdString(i));
		//blitTile(screen, posX, posY, x, y, battleMap.level(i), *sprite);
	}

	for (int i = 0; i < battleMap.width() * battleMap.heigth(); ++i)
	{
		int x = i % battleMap.width();
		int y = i / battleMap.width();

		if (x < xFrom || x >= xTo || y < yFrom || y >= yTo)
			continue;

		uint16_t idx = battleMap.layer2SpriteIdx(i);
		uint8_t lower = idx & 0xFF;
		uint8_t upper = (idx >> 8);
		if (lower != 0)
		{
			lower--;
			Sprite* sprite = layer2Sprites[lower];
			sprite->draw(screen, x - xFrom, y - yFrom, battleMap.level(i));
		}
		else
		{
			s->draw(screen, x - xFrom, y - yFrom, battleMap.level(i));

		}
	}

	for (int i = 0; i < battleMap.animCnt(); ++i)
	{
		BattleMapSystem::AnimItem animItem;
		battleMap.animItem(i, animItem);

		if (animItem.x < xFrom || animItem.x >= xTo || animItem.y < yFrom || animItem.y >= yTo)
			continue;

		SpriteAnimation* sprite = animSprites[animItem.idx];
		sprite->draw(screen, animItem.x - xFrom, animItem.y - yFrom, battleMap.level(i));
		sprite->next();
	}

	for (int i = 0; i < battleMap.width() * battleMap.heigth(); ++i)
	{
		int x = i % battleMap.width();
		int y = i / battleMap.width();

		if (x < xFrom || x >= xTo || y < yFrom || y >= yTo)
			continue;

		//uint16_t idx = battleMap.layer2SpriteIdx(i) >> 12;
		uint16_t idx = battleMap.layer2SpriteIdx(i) >> 8;
		//uint16_t idx = battleMap.layer2SpriteIdx(i) & 0xFF;
		//blitTileNumber(screen, posX, posY, x, y, battleMap.level(i), idx);
		//blitTileNumber(screen, posX, posY, x, y, battleMap.level(i), i);

		/*
		 // unk1
		 for (int j = 0; j < battleMap.mUnk1Cnt; ++j)
		 {
		 uint16_t data = *(battleMap.mUnk1Data + j);
		 if (data == i)
		 {
		 char aa[10];
		 sprintf(aa, "U1: %d, %d", i, j);
		 blitTileString(screen, posX, posY, x, y, battleMap.level(i), aa);
		 }
		 }

		 // unk2
		 for (int j = 0; j < battleMap.mUnk2Cnt; ++j)
		 {
		 uint16_t data = *(battleMap.mUnk2Data + j);
		 if (data == i)
		 {
		 char aa[10];
		 sprintf(aa, "U2: %d, %d", i, j);
		 blitTileString(screen, posX, posY, x, y, battleMap.level(i), aa);
		 }
		 }
		 */

	}
	CyclePalette();
	SDL_Flip(screen);
}
#endif

int main(int argc, char* args[])
{
#if 0
	BattleMapSystem::UnitType::loadUnitTypes();
	for (int i = 0; i < 87; ++i)
	{
		BattleMapSystem::UnitType unitType(i);
		printf("%2d: %20s %4d %4d\n", i, unitType.name(), (unitType.unk() >> 3) & 1, (unitType.unk() >> 4) & 1);
	}
	return 0;


	//Start SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_Init() Failed: " << TTF_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}

	DEBUG_font = TTF_OpenFont("arial.ttf", 12);
	if (DEBUG_font == NULL)
	{
		std::cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << std::endl;
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}
#endif

	//Start SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_Init() Failed: " << TTF_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}

	////////////////////////
	// CEGUI initialization
	////////////////////////

	// Init screen
	SDL_Surface* screen = SDL_SetVideoMode(1024, 768, 8, SDL_HWPALETTE | SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (!screen)
	{
		printf("Couldn't set video mode: %s\n", SDL_GetError());
		exit(-1);
	}

	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//BattleMapSystem::BattleMapGameplay gameplay("M03_12.DTA");
	BattleMapSystem::BattleMapGameplay gameplay("M01_01.DTA");
	//BattleMapSystem::BattleMapGameplay gameplay("M01_01.DTA");
	//BattleMapSystem::BattleMapGameplay gameplay("M02_02.DTA");
	gameplay.init();
	gameplay.loop();


#if 0
	BattleMapSystem::BattleMap battleMap;
	//battleMap.initFromFile("DATA/common/M03_12.DTA");
	battleMap.initFromFile("DATA/common/M01_01.DTA");
	//battleMap.initFromFile("DATA/common/M02_06.DTA");
	//battleMap.initFromFile("DATA/common/TEMPFILE.MAP");

	loadPalette("MAP.PAL", screen, 0);
	loadPalette("UNITS.PAL", screen, 128);
	loadPalette("SYSTEM.PAL", screen, 224);

	/*
	 for (int i = 0; i < 1024*768; ++i) {
	 *((Uint8*)screen->pixels + i) = i % 256;
	 }
	 SDL_Flip(screen);
	 SDL_Delay(2000);
	 */



	IArchive* envArchive = ArchiveMgr::instance().getArchive(battleMap.archiveName());

	uint8_t* cycleBuffer;
	envArchive->loadFile("CYCLE.PAL", cycleBuffer);
	InitPaletteCycling(cycleBuffer);
	delete cycleBuffer;

	IArchive* unitsArchive = ArchiveMgr::instance().getArchive(ArchiveMgr::T_UNITS);
	unitsArchive->init();
	uint8_t* buffer;
	int32_t size = unitsArchive->loadFile("LPECHF00", buffer);
	vector<unsigned char> input(buffer, buffer + size);
	Utils::LzwCodec codec;
	vector<unsigned char> decoded;
	codec.decode(input, decoded);
	uint8_t* vv = new uint8_t[decoded.size()];
	std::copy(decoded.begin(), decoded.end(), vv);
	s = new Sprite();
	s->daFlag = 1;
	s->init(vv, true);


	LoadPaletteFilter(*envArchive, "DARKER.PAL", palettes[pfDarker]);

	// Layer 1
	const BattleMapSystem::FilenameList& layer1SpriteNames = battleMap.layer1SpriteNames();
	vector<Sprite*> layer1Sprites;
	layer1Sprites.reserve(layer1SpriteNames.size());

	for (int32_t idx = 0; idx < layer1SpriteNames.size(); ++idx)
	{
		Sprite* sprite = new Sprite();
		sprite->init(*envArchive, layer1SpriteNames[idx]);
		layer1Sprites.push_back(sprite);
	}

	// Layer 2
	const BattleMapSystem::FilenameList& layer2SpriteNames = battleMap.layer2SpriteNames();
	vector<Sprite*> layer2Sprites;
	layer2Sprites.reserve(layer2SpriteNames.size());

	for (int32_t idx = 0; idx < layer2SpriteNames.size(); ++idx)
	{
		Sprite* sprite = new Sprite();
		sprite->init(*envArchive, layer2SpriteNames[idx]);
		layer2Sprites.push_back(sprite);
	}

	const BattleMapSystem::FilenameList& animSpriteNames = battleMap.animNames();
	vector<SpriteAnimation*> animSprites;
	animSprites.reserve(animSpriteNames.size());

	for (int32_t idx = 0; idx < animSpriteNames.size(); ++idx)
	{
		SpriteAnimation* spriteAnimation = new SpriteAnimation();
		spriteAnimation->init(*envArchive, animSpriteNames[idx]);
		animSprites.push_back(spriteAnimation);
	}

	// Event loop
	SDL_Event event;
	bool running = true;
	int x = 0;
	int y = 0;

	int mouseX, mouseY;
	SDL_EnableKeyRepeat(300, 10);

	while (running)
	{
		while (SDL_PollEvent(&event))
		//while (SDL_WaitEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					--x;
					break;
				case SDLK_RIGHT:
					++x;
					break;
				case SDLK_UP:
					y -= 2;
					break;
				case SDLK_DOWN:
					y += 2;
					break;

				case SDLK_q:
					running = 0;
					break;
				}

				break;
				//case SDL_KEYUP:
			case SDL_QUIT:
				running = 0;
				break;
			}
		}

		// Scrolling
		/*
		 SDL_GetMouseState(&mouseX, &mouseY);
		 if (mouseX < 8) {
		 --x;
		 } else if (mouseX > screen->w - 8) {
		 ++x;
		 }
		 */

		SDL_Delay(10);
		blitMap(screen, battleMap, layer1Sprites, layer2Sprites, animSprites, x, y);
	}

	/*
	 for (int j = 0; j < 256; ++j) {


	 for (int i = 0; i < 256; ++i) {
	 colors[i].r = i + j;
	 colors[i].g = i + j;
	 colors[i].b = i + j;
	 }
	 SDL_SetPalette(screen, SDL_PHYSPAL, colors, 0, 256);

	 SDL_Rect dest_rect = {200, 200, 100, 100};
	 SDL_BlitSurface(s, NULL, screen, &dest_rect);

	 SDL_Flip(screen);
	 SDL_Delay(20);
	 }
	 */
#endif

	//Quit TTF
	TTF_Quit();

	//Quit SDL
	SDL_Quit();

	return 1;
}
