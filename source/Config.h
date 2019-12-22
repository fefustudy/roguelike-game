#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include "Actor.h"

using nlohmann::json;
using std::string;
using std::vector;
using std::ifstream;
using std::shared_ptr;
using std::make_shared;

class Config {
	json j, m;

public:
	Config() {
		//auto sym = j["Wall"]["sym"].get<string>();
		////auto yy = j["PAPA"]["TUTU"].get<string>();
		//std::cout << sym << std::endl;
		//std::cout << "huh" << std::endl;
		//std::cout << "huh" << std::endl;
	}

	template<class T>
	T GetInfo(string name, string param) {
		return j[name][param].get<T>()
	}


	std::pair<vector<shared_ptr<Actor>>, shared_ptr<Knight>>  FabricMap() {

		ifstream("config.json") >> j;
		ifstream("mapConfig.json") >> m;

		ifstream in("map.txt");
		string str;

		vector<shared_ptr<Actor>> out;

		shared_ptr<Knight> mainPlayer;

		int y = 0;
		while (std::getline(in, str)) {
			int  x = 0;
			for (char& c : str) {
				auto cs = string(1, c);

				auto a = m[cs].get<string>();

				if (a == "Knight") {
					mainPlayer = make_shared<Knight>(Vec(x, y),
						j[a]["hp"].get<int>(),
						j[a]["damage"].get<int>(),
						j[a]["maxHp"].get<int>(),
						j[a]["mana"].get<int>(),
						j[a]["maxMana"].get<int>(),
						j[a]["sym"].get<string>()[0]);

					out.push_back(mainPlayer);
				}
				else if (a == "Princess") {
					out.push_back(make_shared<Princess>(Vec(x, y),
						j[a]["hp"].get<int>(),
						j[a]["damage"].get<int>(),
						j[a]["maxHp"].get<int>(),
						j[a]["mana"].get<int>(),
						j[a]["maxMana"].get<int>(),
						j[a]["sym"].get<string>()[0]));
				}
				else if (a == "Zombie") {
					auto hp = j[a]["hp"].get<int>();

					out.push_back(make_shared < Zombie>(Vec(x, y),
						hp,
						j[a]["damage"].get<int>(),
						j[a]["maxHp"].get<int>(),
						j[a]["mana"].get<int>(),
						j[a]["maxMana"].get<int>(),
						j[a]["sym"].get<string>()[0]));
				}
				else if (a == "Dragon") {
					out.push_back(make_shared < Dragon>(Vec(x, y),
						j[a]["hp"].get<int>(),
						j[a]["damage"].get<int>(),
						j[a]["maxHp"].get<int>(),
						j[a]["mana"].get<int>(),
						j[a]["maxMana"].get<int>(),
						j[a]["sym"].get<string>()[0]));
				}
				else if (a == "Wall") {
					out.push_back(make_shared < Wall>(Vec(x, y), j[a]["sym"].get<string>()[0]));
				}
				else if (a == "AidKit") {
					out.push_back(make_shared < AidKit>(Vec(x,y),
						j[a]["restoreHp"].get<int>(),
						j[a]["sym"].get<string>()[0]));
				}
				x++;
			}
			y++;
		}
		return { out, mainPlayer };
	}
};