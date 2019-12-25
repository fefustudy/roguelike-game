#pragma once
#include <memory>
using std::shared_ptr;
using std::make_shared;

class Enemy;
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
	static const Vec UP, RIGHT, DOWN, LEFT;
	static const Vec DIRS[];
	int x, y;
	Vec(int x, int y) : x(x), y(y) {}

	Vec operator+(const Vec& rhs) {
		return Vec(x + rhs.x, y + rhs.y);
	}

	Vec operator-(const Vec& rhs) {
		return Vec(x - rhs.x, y - rhs.y);
	}
	~Vec() {}
};
const struct Vec Vec::UP = { 0 , -1 };
const struct Vec Vec::RIGHT = { 1 , 0 };
const struct Vec Vec::DOWN = { 0 , 1 };
const struct Vec Vec::LEFT = { -1 , 0 };
const struct Vec Vec::DIRS[] = { Vec::UP, Vec::RIGHT, Vec::DOWN, Vec::LEFT };

class Actor {
	Vec pos;
	char sym;
	Vec lastDir;
	bool isDel = false;
public:

	Vec getPos() { return pos; }
	void SetPos(Vec p) { pos = p; }
	void SetLastDir(Vec p) { lastDir = p; }
	char GetSym() { return sym; }
	Vec getDirection() { return lastDir; }
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
	int getMaxMana() { return maxMana; }

	Character(int hp, int damage, int maxHp, int mana, int maxMana, char sym, Vec pos) : Actor(sym, pos), hp(hp), damage(damage), maxHp(maxHp), mana(mana), maxMana(maxMana) {};

	void AddHp(int p) {
		hp += p;
		if (hp <= 0) MarkForDelete();
		if (hp > maxHp) hp = maxHp;
	}
	
	void AddMana(int p) {
		mana += p;
		if (mana > maxMana) mana = maxMana;
	}

	virtual shared_ptr<Projectile> Fire() { return nullptr; };
};

class Enemy : public Character {
public:
	Enemy(int hp, int damage, int maxHp, int mana, int maxMana, char sym, Vec pos) : Character(hp, damage, maxHp, mana, maxMana, sym, pos) {};

	shared_ptr<Projectile> Fire() override{
		if (mana == 0) return nullptr;
		mana--;
		return make_shared<Projectile>(getPos() + getDirection(), getDirection(), getDamage(), 10, true);
	}
};

class AidKit : public Actor {
	int restoreHp;

public:
	int GetRestoreHp() { return restoreHp; }

	AidKit(Vec pos, int restoreHp, char sym) : Actor(sym, pos), restoreHp(restoreHp) {}
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
	Princess(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character(hp, damage, maxHp, mana, maxMana, sym, pos) {}
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

class Zombie : public Enemy {
public:
	Zombie(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Enemy(hp, damage, maxHp, mana, maxMana, sym, pos) {}
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

class Dragon : public Enemy {
public:
	Dragon(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Enemy(hp, damage, maxHp, mana, maxMana, sym, pos) {}

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

class Knight : public Character {
public:
	Knight(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character(hp, damage, maxHp, mana, maxMana, sym, pos) {}

	void Collide(shared_ptr<Actor> obj) override {
		obj->Collide(this);
	}

	void Collide(AidKit* o) override {
		AddHp(o->GetRestoreHp());
		o->MarkForDelete();
	}

	void Collide(Dragon* o) override {
		AddHp(-o->getDamage());
		o->AddHp(-getDamage());
	}

	void Collide(Knight* o) override {}
	void Collide(Wall* o) override {}
	void Collide(Projectile* o) override {}

	void Collide(Princess* o) override {
		o->MarkForDelete();
	}

	void Collide(Zombie* o) override {
		AddHp(-o->getDamage());
		o->AddHp(-getDamage());
	}

	shared_ptr<Projectile> Fire() override {
		if (mana == 0) return nullptr;
		mana--;
		return make_shared<Projectile>(getPos() + getDirection(), getDirection(), getDamage(), 10, false);
	}

};

class Projectile : public Character { // пуля
	int maxDist;
	bool isEnemy;
	int path = 0;

public:

	Projectile(Vec pos, Vec dir, int damage, int maxDist, bool isEnemy) : Character(1, damage, 1, 0, 0, DirSym(dir), pos), maxDist(maxDist), isEnemy(isEnemy){
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

	void Collide(AidKit* o) override {
		MarkForDelete();
	}
	void Collide(Zombie* o) override {
		if (!isEnemy) o->AddHp(-damage);
		MarkForDelete();
	}
	void Collide(Dragon* o) override {
		if(!isEnemy) o->AddHp(-damage);
		MarkForDelete();
	}
	void Collide(Knight* o) override {
		o->AddHp(-damage);
		MarkForDelete();
	}
	void Collide(Wall* o) override {
		MarkForDelete();
	}
	void Collide(Projectile* o) override {
		MarkForDelete();
		o->MarkForDelete();
	}
	void Collide(Princess* o) override {
		MarkForDelete();
	}
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
