#ifndef PLAYERRESOURCECACHE_H
#define PLAYERRESOURCECACHE_H

class PlayerResourceCache {
private:
	int stone;
	int gold;
	int wood;
	
	int gatherUnit = 5;
public:
	PlayerResourceCache() {}
	void init(int stone, int gold, int wood);
	
	int getStone();
	int getWood();
	int getGold();
	
	void gatherStone();
	void gatherGold();
	void gatherWood();
	
	void spend(int stone, int gold, int wood);
	bool canAfford(int stone, int gold, int wood);
	
	void update();
};

#endif