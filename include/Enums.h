#ifndef ENUMS_H
#define ENUMS_H

enum RESOURCE_TYPE { GOLD, STONE, WOOD };
enum RTS_VILLAGER_STATE { VILLAGER_IDLE=0, VILLAGER_WALKING, VILLAGER_GATHER, VILLAGER_DEAD, VILLAGER_MOVE_TO_RESOURCE };
enum RTS_UNIT_STATE { IDLE=0, WALKING, MOVE_TO_ATTACK, ATTACKING, AIMING, TAKE_AIM, RELEASE_AIM, RELOADING, DEAD, MOVE_TO_TOWER, CLIMB_UP, CLIMB_DOWN, GARRISSONED }; 

#endif