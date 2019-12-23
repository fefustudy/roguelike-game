#pragma once
#include "Map.h"
#include <curses.h>
#include "windows.h" // sleep
#include "Config.h"
#include "Timer.h"
#include <algorithm> // std::remove_if

using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;


class Game {
	shared_ptr<Config> c;
	shared_ptr<Map> map;
	shared_ptr<Actor> mainPlayer;
	shared_ptr<set<shared_ptr<Actor>>> actors;

	set<shared_ptr<Actor>> moveDirection;

public:
	Game() {
		c = make_shared<Config>();

		auto fMap = c->FabricMap();
		actors = fMap.first;

		map = make_shared<Map>(fMap, c->GetFog());
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
			Clean();

			auto ch = getch();
			if (ch != ERR && tMaxSpeedInterval.elapsedSeconds() > 0.05) {

				auto pos = mainPlayer->GetPos();

				if (ch == 'w' || ch == KEY_UP) {
					map->Step(mainPlayer, Vec(0, -1));
				}
				else if (ch == 'd' || ch == KEY_RIGHT) {
					map->Step(mainPlayer, Vec(1, 0));
				}
				else if (ch == 's' || ch == KEY_DOWN) {
					map->Step(mainPlayer, Vec(0, 1));
				}
				else if (ch == 'a' || ch == KEY_LEFT) {
					map->Step(mainPlayer, Vec(-1, 0));
				}
				else if (ch == ' ') { //Стрелятб
					Fire(static_pointer_cast<Character>(mainPlayer));
				}

				// move user
				tMaxSpeedInterval.start();
			}

			if (tshot.elapsedSeconds() >= 0.2) {
				// move Projectilies direction
				moveProjectilies();
				tshot.start();
			}

			if (t.elapsedSeconds() >= 1.0) {
				// move mobs randomly

				t.start();
			}

			map->Draw();

			Sleep(17); // 60 fps
		}
	};

	void Clean() {
		vector<shared_ptr<Actor>> forDel;

		for (auto it = actors->begin(); it != actors->end(); it++) {
			if ((*it)->isMarkForDelete()) {
				forDel.push_back(*it);
			}
		}

		for (auto it = forDel.begin(); it != forDel.end(); it++) {
			map->Hide(*it);
			actors->erase(*it);
			moveDirection.erase(*it);
		}
	}


	void Fire(shared_ptr<Character> a) {
		auto pl = make_shared<Projectile>(
			a->GetPos() + a->GetDirection(), a->GetDirection(),
			a->getDamage(), 10);

		if (map->Add(pl))
			moveDirection.insert(pl);
	}


	void moveProjectilies() {
		for (auto it = moveDirection.begin(); it != moveDirection.end(); it++) {
			if (static_pointer_cast<Projectile>(*it)->IsNext()) {
				map->Step(*it, (*it)->GetDirection());
			}
			else {
				(*it)->MarkForDelete();
			}
		}
	};
};
