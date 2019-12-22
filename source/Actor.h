#pragma once
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
};


class Actor {
	Vec pos;
	char sym;
public:
	Vec GetPos() { return pos; }
	char getSym() { return sym; }
	Actor(char sym, Vec pos) : pos(pos), sym(sym) {}

};



enum direction { TOP, RIGHT, DOWN, LEFT };

class Character : public Actor {
public:
	int hp, damage, maxHp, mana, maxMana;

	int getHp() { return hp; }
	int getDamage() { return damage; }
	int getMaxHp() { return maxHp; }
	int getMana() { return mana; }
	int getMaxMana() { return mana; }

	Character(int hp, int damage, int maxHp, int mana, int maxMana, char sym, Vec pos) : Actor(sym, pos), hp(hp), damage(damage), maxHp(maxHp), mana(mana), maxMana(maxMana) {}

	//virtual void Collide(std::shared_ptr<Actor>) = 0;
	//virtual void Collide(AidKit) = 0;
	//virtual void Collide(Dragon) = 0;
	//virtual void Collide(Knight) = 0;
	//virtual void Collide(Wall) = 0;
	//virtual void Collide(Projectile) = 0;
	//virtual void Collide(Princess) = 0;
	//virtual void Collide(Zombie) = 0;

};


class Knight : public Character {
public: Knight(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character(hp, damage, maxHp, mana, maxMana, sym, pos) {}

	  //void Collide(std::shared_ptr<Actor> obj) override{
		 // obj->Collide(*this);
	  //}
	  //void Collide(AidKit o) override {}
	  //void Collide(Dragon o) override {}
	  //void Collide(Knight o) override {}
	  //void Collide(Wall o) override {}
	  //void Collide(Projectile o) override {}
	  //void Collide(Princess o) override {}
	  //void Collide(Zombie o) override {}

};

class Princess : public Character {
public:	Princess(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character(hp, damage, maxHp, mana, maxMana, sym, pos) {}
	  //void Collide(std::shared_ptr<Actor> obj) override {
		 // obj->Collide(*this);
	  //}
	  //void Collide(AidKit o) override {}
	  //void Collide(Dragon o) override {}
	  //void Collide(Knight o) override {}
	  //void Collide(Wall o) override {}
	  //void Collide(Projectile o) override {}
	  //void Collide(Princess o) override {}
	  //void Collide(Zombie o) override {}

};

class Zombie : public Character {
public:	Zombie(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character(hp, damage, maxHp, mana, maxMana, sym, pos) {}
	  //void Collide(std::shared_ptr<Actor> obj) override {
		 // obj->Collide(*this);
	  //}
	  //void Collide(AidKit o) override {}
	  //void Collide(Dragon o) override {}
	  //void Collide(Knight o) override {}
	  //void Collide(Wall o) override {}
	  //void Collide(Projectile o) override {}
	  //void Collide(Princess o) override {}
	  //void Collide(Zombie o) override {}
};

class Dragon : public Character {
public:	Dragon(Vec pos, int hp, int damage, int maxHp, int mana, int maxMana, char sym) : Character(hp, damage, maxHp, mana, maxMana, sym, pos) {}
	  
	  //void Collide(std::shared_ptr<Actor> obj) override {
		 // obj->Collide(*this);
	  //}
	  //void Collide(AidKit o) override {}
	  //void Collide(Dragon o) override {}
	  //void Collide(Knight o) override {}
	  //void Collide(Wall o) override {}
	  //void Collide(Projectile o) override {}
	  //void Collide(Princess o) override {}
	  //void Collide(Zombie o) override {}
};

class Wall : public Actor {
public:	Wall(Vec pos, char sym) : Actor(sym, pos) {}
	  //void Collide(std::shared_ptr<Actor> obj) override {
		 // obj->Collide(*this);
	  //}
	  //void Collide(AidKit o) override {}
	  //void Collide(Dragon o) override {}
	  //void Collide(Knight o) override {}
	  //void Collide(Wall o) override {}
	  //void Collide(Projectile o) override {}
	  //void Collide(Princess o) override {}
	  //void Collide(Zombie o) override {}
};

class AidKit : public Actor {
public:	AidKit(Vec pos, int restoreHp, char sym) : Actor(sym, pos) {}
	  //void Collide(std::shared_ptr<Actor> obj) override {
		 // obj->Collide(*this);
	  //}
	  //void Collide(AidKit o) override {}
	  //void Collide(Dragon o) override {}
	  //void Collide(Knight o) override {}
	  //void Collide(Wall o) override {}
	  //void Collide(Projectile o) override {}
	  //void Collide(Princess o) override {}
	  //void Collide(Zombie o) override {}
};

class Projectile : public Actor { // пуля


public:	Projectile(Vec pos, direction dir) : Actor(DirConst(dir), pos) {

}

	  static char DirConst(direction dir) {
		  switch (dir) {
		  case TOP: return '^';
		  case RIGHT: return '>';
		  case DOWN: return 'v';
		  case LEFT: return '<';
		  }
	  }

	  //void Collide(std::shared_ptr<Actor> obj) override {
		 // obj->Collide(*this);
	  //}
	  //void Collide(AidKit o) override {}
	  //void Collide(Dragon o) override {}
	  //void Collide(Knight o) override {}
	  //void Collide(Wall o) override {}
	  //void Collide(Projectile o) override {}
	  //void Collide(Princess o) override {}
	  //void Collide(Zombie o) override {}
};