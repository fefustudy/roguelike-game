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
		//auto yy = j["PAPA"]["TUTU"].get<string>();
		//std::cout << yy << std::endl;
	}

	template<class T>
	T GetInfo(string name, string param) {
		return j[name][param].get<T>()
	}
	std::pair<int, bool> GetFog() {
		ifstream("config.json") >> j;

		return{ j["FOG"]["distance"].get<int>() ,j["FOG"]["enable"].get<bool>() };
	}

	std::pair<shared_ptr<set<shared_ptr<Actor>>>, shared_ptr<Knight>>  FabricMap() {

		ifstream("config.json") >> j;
		ifstream("mapConfig.json") >> m;

		ifstream in("map.txt");
		string str;

		auto out = make_shared <set<shared_ptr<Actor>>>();

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

					out->insert(mainPlayer);
				}
				else if (a == "Princess") {
					out->insert(make_shared<Princess>(Vec(x, y),
						j[a]["hp"].get<int>(),
						j[a]["damage"].get<int>(),
						j[a]["maxHp"].get<int>(),
						j[a]["mana"].get<int>(),
						j[a]["maxMana"].get<int>(),
						j[a]["sym"].get<string>()[0]));
				}
				else if (a == "Zombie") {
					auto hp = j[a]["hp"].get<int>();

					out->insert(make_shared < Zombie>(Vec(x, y),
						hp,
						j[a]["damage"].get<int>(),
						j[a]["maxHp"].get<int>(),
						j[a]["mana"].get<int>(),
						j[a]["maxMana"].get<int>(),
						j[a]["sym"].get<string>()[0]));
				}
				else if (a == "Dragon") {
					out->insert(make_shared < Dragon>(Vec(x, y),
						j[a]["hp"].get<int>(),
						j[a]["damage"].get<int>(),
						j[a]["maxHp"].get<int>(),
						j[a]["mana"].get<int>(),
						j[a]["maxMana"].get<int>(),
						j[a]["sym"].get<string>()[0]));
				}
				else if (a == "Wall") {
					out->insert(make_shared < Wall>(Vec(x, y), j[a]["sym"].get<string>()[0]));
				}
				else if (a == "AidKit") {
					out->insert(make_shared < AidKit>(Vec(x, y),
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