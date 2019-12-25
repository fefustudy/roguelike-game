#pragma once
#include "Map.h"
#include <curses.h>
#include "windows.h" // sleep resize
#include "Config.h"
#include "Timer.h"
#include <cstdlib>
#include "Factory.h"

using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;
using std::srand;
using std::rand;


enum GAME_STAT { UNDEF, WIN, DEFEAT };



class Game {
	shared_ptr<Config> c;
	shared_ptr<Map> map;
	shared_ptr<Actor> mainPlayer;
	shared_ptr<set<shared_ptr<Actor>>> actors;
	set<shared_ptr<Actor>> moveDirection;
	GAME_STAT s = GAME_STAT::UNDEF;
public:
	KnightFactory knightFactory;
	PrincessFactory princessFactory;
	ZombieFactory zombieFactory;
	DragonFactory dragonFactory;
	WallFactory wallFactory;
	AidKitFactory aidKitFactory;


	Game() {
		c = make_shared<Config>();

		auto fMap = FabricMap();
		actors = fMap.first;
		map = make_shared<Map>(fMap, c->GetFog());
	};

	~Game() {};

	GAME_STAT Start() {

		srand(117);

		Timer t;
		t.start();
		Timer tshot;
		tshot.start();
		Timer tMaxSpeedInterval;
		tMaxSpeedInterval.start();
		Timer tMana;
		tMana.start();

		Timer shotInterval;
		shotInterval.start();

		mainPlayer = map->GetMainPlayer();
		while (1) {


			if (Clean()) return s;

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
				else if (ch == ' ' && shotInterval.elapsedSeconds() > 0.4 ) {
					Fire(static_pointer_cast<Character>(mainPlayer));
					shotInterval.start();
				}

				if (is_termresized()) {
					resize_term(0, 0);
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
			s = GAME_STAT::DEFEAT;
			return true;
		}

		vector<shared_ptr<Actor>> forDel;
		for (auto it = actors->begin(); it != actors->end(); it++) {
			if ((*it)->isMarkForDelete()) {
				forDel.push_back(*it);
			}
		}

		for (auto it = forDel.begin(); it != forDel.end(); it++) {
			if (dynamic_pointer_cast<Princess>(*it)) {
				s = GAME_STAT::WIN;
				return true;
			}

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

	std::pair<shared_ptr<set<shared_ptr<Actor>>>, shared_ptr<Knight>>  FabricMap() {
		ifstream in("map.txt");
		string str;

		auto m = c->GetMapConf();

		auto out = make_shared<set<shared_ptr<Actor>>>();
		shared_ptr<Knight> mainPlayer;

		int y = 0;
		while (std::getline(in, str)) {
			int  x = 0;
			for (char& c : str) {
				auto cs = string(1, c);

				auto a = m[cs].get<string>();

				if (a == "Knight") {
					mainPlayer = static_pointer_cast<Knight> (knightFactory.createActor(Vec(x, y)));
					out->insert(mainPlayer);
				}
				else if (a == "Princess") {
					out->insert(princessFactory.createActor(Vec(x, y)));
				}
				else if (a == "Zombie") {
					out->insert(zombieFactory.createActor(Vec(x, y)));
				}
				else if (a == "Dragon") {
					out->insert(dragonFactory.createActor(Vec(x, y)));
				}
				else if (a == "Wall") {
					out->insert(wallFactory.createActor(Vec(x, y)));
				}
				else if (a == "AidKit") {
					out->insert(aidKitFactory.createActor(Vec(x, y)));
				}
				x++;
			}
			y++;
		}
		return { out, mainPlayer };
	}
};
