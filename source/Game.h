#pragma once
#include "Map.h"
#include <curses.h>
#include "windows.h" // sleep
#include "Config.h"
#include "Timer.h"
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;
using std::srand;
using std::rand;


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
		srand(117);

		Timer t;
		t.start();
		Timer tshot;
		tshot.start();
		Timer tMaxSpeedInterval;
		tMaxSpeedInterval.start();
		Timer tMana;
		tMana.start();

		mainPlayer = map->GetMainPlayer();
		
		while (1) {
			if (Clean()) return;

			auto ch = getch();
			if (ch != ERR && tMaxSpeedInterval.elapsedSeconds() > 0.05) {

				auto pos = mainPlayer->getPos();

				if (ch == 'w' || ch == KEY_UP) {
					map->Step(mainPlayer, Vec::UP);
				}
				else if (ch == 'd' || ch == KEY_RIGHT) {
					map->Step(mainPlayer, Vec::RIGHT);
				}
				else if (ch == 's' || ch == KEY_DOWN) {
					map->Step(mainPlayer, Vec::DOWN);
				}
				else if (ch == 'a' || ch == KEY_LEFT) {
					map->Step(mainPlayer, Vec::LEFT);
				}
				else if (ch == ' ') { //Стрелятб
					Fire(static_pointer_cast<Character>(mainPlayer));
				}

				tMaxSpeedInterval.start();
			}

			if (tshot.elapsedSeconds() >= 0.2) {
				moveProjectilies();
				tshot.start();
			}

			if (t.elapsedSeconds() >= 1.0) {
				MoveMobs();
				t.start();
			}
			if (tMana.elapsedSeconds() >= 1.0) {
				RestoreMana(1);
				tMana.start();
			}

			map->Draw();

			Sleep(17); // 60 fps
		}
	};

	bool Clean() {
		if (mainPlayer->isMarkForDelete()) {
			return true;
		}

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
		return false;
	}

	void Fire(shared_ptr<Character> a) {
		auto pl = a->Fire();
		if (pl && map->Add(pl)) {
			moveDirection.insert(pl);
		}
	}

	void RestoreMana(int count) {
		for (auto it = actors->begin(); it != actors->end(); it++) {
			auto en = dynamic_pointer_cast<Character>(*it);
			if (en) {
				en->AddMana(count);
			}
		}
	}

	void MoveMobs() { //AI
		for (auto it = actors->begin(); it != actors->end(); it++) {
			auto en = dynamic_pointer_cast<Enemy>(*it);
			if (en) {
				map->Step(en, Vec::DIRS[rand() % 4]);
				Fire(en);
			}
		}
	}

	void moveProjectilies() {
		for (auto it = moveDirection.begin(); it != moveDirection.end(); it++) {
			if (static_pointer_cast<Projectile>(*it)->IsNext()) {
				map->Step(*it, (*it)->getDirection());
			}
			else {
				(*it)->MarkForDelete();
			}
		}
	};
};
