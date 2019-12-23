#pragma once
#include "Actor.h"
#include "Game.h"
#include <vector>
#include <map>
#include <set>
#include <curses.h>
#include <memory>

using std::vector;
using std::map;
using std::set;
using std::pair;
using std::shared_ptr;
using std::make_shared;

struct Cell {
	Actor* actor;
};

// отрисовка и тд
class Map {
	shared_ptr <set<shared_ptr<Actor>>> actors;
	shared_ptr<Actor> mainPlayer;
	map<pair<int, int>, shared_ptr<Actor>> posBase;
	set<pair<int, int>> fogOfWar;
	bool fogOfWarIsEnable;
	int fogOfWarDistSqr;


public:
	Map(std::pair<shared_ptr<set<shared_ptr<Actor>>>, shared_ptr<Knight>> data, std::pair<int, bool> fog)
		: actors(data.first)
		, mainPlayer(data.second)
		, fogOfWarIsEnable(fog.second)
		, fogOfWarDistSqr(fog.first* fog.first) {

		for (auto it = actors->begin(); it != actors->end(); ++it) {
			auto x = (*it)->GetPos().x;
			auto y = (*it)->GetPos().y;
			posBase[{x, y}] = *it;
		}

	};

	void Draw() { //Рисовать вокруг игрока
		int my, mx;
		getmaxyx(stdscr, my, mx);

		auto hx = mainPlayer->GetPos().x;
		auto hy = mainPlayer->GetPos().y;
		pair<int, int> center = { hx,hy };

		auto zx = hx - mx / 2;
		auto lx = hx + mx / 2;

		auto zy = hy - my / 2;
		auto ly = hy + my / 2;

		for (int y = zy; y < ly; y++) {
			for (int x = zx; x < lx; x++) {

				if (fogOfWarIsEnable && fogOfWar.find({ x, y }) == fogOfWar.end()) {
					if ((y - hy) * (y - hy) + (x - hx) * (x - hx) <= fogOfWarDistSqr) {
						fogOfWar.insert({ x, y });
						// TODO: вставить сюда рандом генерацию карты
					}
					else {
						mvaddch(y - hy + my / 2, x - hx + mx / 2, '#');
						continue;
					}
				}

				auto it = posBase.find({ x, y });
				if (it != posBase.end()) {
					mvaddch(y - hy + my / 2, x - hx + mx / 2, it->second->GetSym());
				}
				else {
					mvaddch(y - hy + my / 2, x - hx + mx / 2, ' ');
				}

			}
		}
		//mvaddch(hy, hx, mainPlayer->getSym());

		refresh();

	};

	shared_ptr<Actor> GetMainPlayer() {
		return mainPlayer;
	}

	void Move(shared_ptr<Actor> from, Vec newPos) {
		// call collide in Actor and in newPos Actor if it exist
		auto it = posBase.find({ newPos.x, newPos.y });
		auto fromPos = from->GetPos();

		if (it == posBase.end()) {
			posBase.erase({ fromPos.x, fromPos.y });
			posBase[{ newPos.x, newPos.y }] = from;
			from->SetPos(newPos);
		}
		else {
			from->Collide(it->second);
			it->second->Collide(from);
		}
	}
	void Step(shared_ptr<Actor> from, Vec dir) {
		from->SetLastDir(dir);
		Move(from, from->GetPos() + dir);
	}
	
	bool Add(shared_ptr<Actor> a) {
		auto pos = a->GetPos();
		auto it = posBase.find({ pos.x, pos.y });

		if (it == posBase.end()) {
			posBase[{ pos.x, pos.y }] = a;
			actors->insert(a);
		}

		return it == posBase.end();
	}

	void Hide(shared_ptr<Actor> from) {
		auto fromPos = from->GetPos();
		posBase.erase({ fromPos.x, fromPos.y });
	}

	~Map() {
	};
};
