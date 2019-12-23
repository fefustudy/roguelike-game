#pragma once
#include <memory>
using std::shared_ptr;

class Actor;
class Character;

class Knight;
class Princess;
class Zombie;
class Dragon;
class Wall;
class AidKit;
class Projectile;



struct Vec {
	int x, y;
	Vec(int x, int y) : x(x), y(y) {}

	Vec operator+(const Vec& rhs) {
		return Vec(x + rhs.x, y + rhs.y);
	}

	Vec operator-(const Vec& rhs) {
		return Vec(x - rhs.x, y - rhs.y);
	}
};


class Actor {
	Vec pos;
	char sym;
	Vec lastDir;
	bool isDel = false;
public:

	Vec GetPos() { return pos; }
	void SetPos(Vec p) { pos = p; }
	void SetLastDir(Vec p) { lastDir = p; }
	char GetSym() { return sym; }
	Vec GetDirection() { return lastDir; }
	void MarkForDelete() { isDel = true; }
	bool isMarkForDelete() { return isDel; }
	Actor(char sym, Vec pos) :pos(pos), sym(sym), lastDir(Vec(1, 0)) {}


	virtual void Collide(std::shared_ptr<Actor>) = 0;

	virtual void Collide(AidKit*) = 0;
	virtual void Collide(Dragon*) = 0;
	virtual void Collide(Knight*) = 0;
	virtual void Collide(Wall*) = 0;
	virtual void Collide(Projectile*) = 0;
	virtual void Collide(Princess*) = 0;
	virtual void Collide(Zombie*) = 0;
};



class Character : public Actor {
public:

	int hp, damage, maxHp, mana, maxMana;

	int getHp() { return hp; }
	int getDamage() { return damage; }
	int getMaxHp() { return maxHp; }
	int getMana() { return mana; }
	int getMaxMana() { return mana; }

	Character(int hp, int damage, int maxHp, int mana, int maxMana, char sym, Vec pos) : Actor(sym, pos), hp(hp), damage(damage), maxHp(maxHp), mana(mana), maxMana(maxMana) {};

};


class Knight : public Character {
public:
	Knight(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character(hp, damage, maxHp, mana, maxMana, sym, pos) {}

	void Collide(shared_ptr<Actor> obj) override {
		obj->Collide(this);
	}

	void Collide(AidKit* o) override {}
	void Collide(Dragon* o) override {}
	void Collide(Knight* o) override {}
	void Collide(Wall* o) override {}
	void Collide(Projectile* o) override {}
	void Collide(Princess* o) override {}
	void Collide(Zombie* o) override {}

};

class Princess : public Character {
public:
	Princess(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character( hp, damage, maxHp, mana, maxMana, sym, pos) {}
	void Collide(shared_ptr<Actor> obj) override {
		obj->Collide(this);
	}
	void Collide(AidKit* o) override {}
	void Collide(Dragon* o) override {}
	void Collide(Knight* o) override {}
	void Collide(Wall* o) override {}
	void Collide(Projectile* o) override {}
	void Collide(Princess* o) override {}
	void Collide(Zombie* o) override {}

};

class Zombie : public Character {
public:
	Zombie(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character( hp, damage, maxHp, mana, maxMana, sym, pos) {}
	void Collide(shared_ptr<Actor> obj) override {
		obj->Collide(this);
	}
	void Collide(AidKit* o) override {}
	void Collide(Dragon* o) override {}
	void Collide(Knight* o) override {}
	void Collide(Wall* o) override {}
	void Collide(Projectile* o) override {}
	void Collide(Princess* o) override {}
	void Collide(Zombie* o) override {}
};

class Dragon : public Character {
public:
	Dragon(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character( hp, damage, maxHp, mana, maxMana, sym, pos) {}

	void Collide(shared_ptr<Actor> obj) override {
		obj->Collide(this);
	}
	void Collide(AidKit* o) override {}
	void Collide(Dragon* o) override {}
	void Collide(Knight* o) override {}
	void Collide(Wall* o) override {}
	void Collide(Projectile* o) override {}
	void Collide(Princess* o) override {}
	void Collide(Zombie* o) override {}
};


class Projectile : public Character { // пуля

	int maxDist;
	int path = 0;

public:

	Projectile(Vec pos, Vec dir, int damage, int maxDist) : Character(1, damage, 1, 0, 0, DirSym(dir), pos), maxDist(maxDist) {
		SetLastDir(dir);
	}

	static char DirSym(Vec dir) {
		if (dir.y < 0) return '^';
		else if (dir.y > 0) return 'v';
		else if (dir.x < 0)  return '<';
		else if (dir.x > 0)  return '>';
	}

	bool IsNext() {
		return maxDist > path++;
	}

	void Collide(shared_ptr<Actor> obj) override {
		obj->Collide(this);
	}

	void Collide(AidKit* o) override {}
	void Collide(Dragon* o) override {}
	void Collide(Knight* o) override {}
	void Collide(Wall* o) override {}
	void Collide(Projectile* o) override {}
	void Collide(Princess* o) override {}
	void Collide(Zombie* o) override {}
};



class Wall : public Actor {
public:
	Wall(Vec pos, char sym) : Actor(sym, pos) {}
	void Collide(shared_ptr<Actor> obj) override {
		obj->Collide(this);
	}

	void Collide(Knight* o) override {}
	void Collide(AidKit* o) override {}
	void Collide(Dragon* o) override {}
	void Collide(Wall* o) override {}
	void Collide(Projectile* o) override {}
	void Collide(Princess* o) override {}
	void Collide(Zombie* o) override {}
};

class AidKit : public Actor {
public:
	AidKit(Vec pos, int restoreHp, char sym) : Actor( sym, pos) {}
	void Collide(shared_ptr<Actor> obj) override {
		obj->Collide(this);
	}
	void Collide(AidKit* o) override {}
	void Collide(Dragon* o) override {}
	void Collide(Knight* o) override {}
	void Collide(Wall* o) override {}
	void Collide(Projectile* o) override {}
	void Collide(Princess* o) override {}
	void Collide(Zombie* o) override {}
};
