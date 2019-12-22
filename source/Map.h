#pragma once
#include "Actor.h"
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
	vector<shared_ptr<Actor>> actors;
	shared_ptr<Actor> mainPlayer;
	map<pair<int, int>, shared_ptr<Actor>> posBase;
	map<pair<int, int>, bool> FogOfWar;


public:
	Map(std::pair<vector<shared_ptr<Actor>>, shared_ptr<Knight>> data) : actors(data.first) {

		mainPlayer = data.second;

		for (auto it = actors.begin(); it != actors.end(); ++it) {
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

		//i=0------H=6----------20-----H=26-----------40
		//-------i=6
		//
		// 123456789
		//        h    
		//     h    
		//


		//hx - mx/2 -> hx - mx/2 + mx
		//

		auto zx = hx - mx / 2;
		auto lx = hx + mx / 2;

		auto zy = hy - my / 2;
		auto ly = hy + my / 2;

		for (int y = zy; y < ly; y++) {
			for (int x = zx; x < lx; x++) {
				auto it = posBase.find({ x, y });
				if (it != posBase.end()) {
					mvaddch(y - hy + my / 2, x - hx + mx / 2, it->second->getSym());
				}
				else {
					mvaddch(y - hy + my / 2, x- hx + mx / 2, ' ');
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
			//from->Collide(*(it->second));
			//it->second->Collide(*from);
		}
	}

	~Map() {
	};
};
