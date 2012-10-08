#include "Player.h"
#pragma once
//Generated using ReClass 2011 by DrUnKeN ChEeTaH

#define KNIFE_OFFSET 0xD2260
#define SUBGUN_OFFSET 0xD2700
#define PISTOL_OFFSET 0xD2388
#define RIFLE_OFFSET 0xD24B0
#define SHOTGUN_OFFSET 0xD25D8
#define SNIPER_OFFSET 0xD2828
#define ASSAULT_OFFSET 0xD2950
#define CPISTOL_OFFSET 0xD2A78
#define GRENADE_OFFSET 0xD2BA0
#define PISTOL2_OFFSET 0xD2CC8

class CGunInfo;

class CGunInfo
{
public:
	char modelname[260]; //0x0000 
	short sound; //0x0104 
	short reload; //0x0106 
	short reloadtime; //0x0108 
	short attackdelay; //0x010A 
	short damage; //0x010C 
	short projspeed; //0x010E 
	short part; //0x0110 
	short spread; //0x0112 
	short recoil; //0x0114 
	short magsize; //0x0116 
	short mdl_kick_rot; //0x0118 
	short mdk_kick_back; //0x011A 
	short recoilincrease; //0x011C 
	short recoilbase; //0x011E 
	short maxrecoil; //0x0120 
	short recoilbackfade; //0x0122 
	short pushfactor; //0x0124 
	short m_bIsAuto; //0x0126 

};//Size=0x0128





