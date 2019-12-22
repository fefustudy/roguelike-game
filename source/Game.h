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


		while (1) {
			auto ch = getch();
			if (ch != ERR) {

				if (ch == 'w' || ch == KEY_UP) {

				}
				else if (ch == 'd' || ch == KEY_RIGHT) {

				}
				else if (ch == 's' || ch == KEY_DOWN) {

				}
				else if (ch == 'a' || ch == KEY_LEFT) {

				}
				else if (ch == ' ') { //Стрелятб

				}

				// move user
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
