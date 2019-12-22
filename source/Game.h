#pragma once
#include "Map.h"
#include <curses.h>
#include "windows.h" // sleep
#include "Config.h"
#include "Timer.h"

using std::shared_ptr;
using std::make_shared;

enum action { MOVE_TOP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, ATTACK };

class Game {
	shared_ptr<Config> c;
	shared_ptr<Map> map;
	shared_ptr<Actor> mainPlayer;

public:
	Game(){
		c = make_shared<Config>();
		map = make_shared<Map>(c->FabricMap());
	};

	~Game() {};

	void Start() {

		Timer t;
		t.start();
		Timer tshot;
		tshot.start();

		Timer tMaxSpeedInterval;
		tMaxSpeedInterval.start();

		mainPlayer = map->GetMainPlayer();


		while (1) {
			auto ch = getch();
			if (ch != ERR && tMaxSpeedInterval.elapsedSeconds() > 0.05) {
				auto pos = mainPlayer->GetPos();

				if (ch == 'w' || ch == KEY_UP) {
					map->Move(mainPlayer, Vec(pos.x, pos.y - 1));
				}
				else if (ch == 'd' || ch == KEY_RIGHT) {
					map->Move(mainPlayer, Vec(pos.x+1, pos.y));
				}
				else if (ch == 's' || ch == KEY_DOWN) {
					map->Move(mainPlayer, Vec(pos.x, pos.y + 1));
				}
				else if (ch == 'a' || ch == KEY_LEFT) {
					map->Move(mainPlayer, Vec(pos.x - 1, pos.y));
				}
				else if (ch == ' ') { //Стрелятб
					//map->Move(mainPlayer, Vec(pos.x, pos.y - 1));
				}

				// move user
				tMaxSpeedInterval.start();
			}

			if (tshot.elapsedSeconds() >= 0.2) {
				// move Projectilies direction

				tshot.start();
			}



			if (t.elapsedSeconds() >= 1.0) {
				// move mobs randomly

				t.start();
			}

			map->Draw();

			Sleep(17);// 60 fps
		}

	};


};
