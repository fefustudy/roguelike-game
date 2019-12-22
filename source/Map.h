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
	Map(std::pair<vector<shared_ptr<Actor>>, shared_ptr<Knight>> data) : actors(data.first){
		
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
		
		for (size_t y = 0; y < my; y++) {
			for (size_t x = 0; x < mx; x++) {
				auto it = posBase.find({ x, y });
				if (it != posBase.end()) { 
					mvaddch(y, x, it->second->getSym());
				}
				else {
					mvaddch(y, x, ' ');
				}
				
			}
		}
		mvaddch(hy, hx, mainPlayer->getSym());

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
