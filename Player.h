#include <Windows.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cstdarg>
#include <cstdio>
using namespace std;
#pragma once
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

DWORD dwAppBase = (DWORD)GetModuleHandleA("AC_CLIENT.EXE");


#define OFFSET_PLAYER 0xdf73c	
#define OFFSET_ENEMY 0xE4E08
#define OFFSET_RESERVEDSPACE 0xE4E10
#define OFFSET_PLAYERNUM 0xDFCA8


#define OFFSET_GAMEMODE 0xDFAB8
#define OFFSET_GAMEMODE2 0xE4DB4

bool isTeamGame(int i)
{
	switch(i)
	{
	case 0:
		return true;
	case 1:
		return true;
	case 2:
		return false;
	case 3:
		return false;
	case 4:
		return true;
	case 5:
		return true;
	case 6:
		return false;
	case 7:
		return true;
	case 8:
		return false;
	case 9:
		return false;
	case 10:
		return false;
	case 11:
		return true;
	case 12:
		return false;
	case 13:
		return false;
	case 14:
		return true;
	case 15:
		return true;

	}
}
enum MODE
{
    GMODE_DEMO = -1,
    GMODE_TEAMDEATHMATCH = 0,
    GMODE_COOPEDIT,//1
    GMODE_DEATHMATCH,//2
    GMODE_SURVIVOR,//3
    GMODE_TEAMSURVIVOR,//4
    GMODE_CTF,//5
    GMODE_PISTOLFRENZY,//6
    GMODE_BOTTEAMDEATHMATCH, //7
    GMODE_BOTDEATHMATCH,    //8
    GMODE_LASTSWISSSTANDING,
    GMODE_ONESHOTONEKILL, //10
    GMODE_TEAMONESHOTONEKILL,
    GMODE_BOTONESHOTONEKILL, //12
    GMODE_HUNTTHEFLAG,         // 13
    GMODE_TEAMKEEPTHEFLAG,
    GMODE_KEEPTHEFLAG,
    GMODE_NUM
};

//Generated using ReClass 2011 by DrUnKeN ChEeTaH

//004E4E08
//004B0043 -> END?
//extern Players* pPlayer2;

//TEAM 1 -> RED(CLA), 0 -> BLUE(RSFV)

//RealTeam 1-> BLUE(RVSF), 0->RED(CLA)



class CVector2 {
public:
  CVector2():fX(.0f),fY(.0f){}
  CVector2 (float XX, float YY):fX(XX),fY(YY){}
  float fX;
  float fY;
  CVector2  operator+(const CVector2 &);
  CVector2  operator-(const CVector2 &);
  CVector2  operator*(const CVector2 &);
  CVector2  operator/(const CVector2 &);
};

CVector2 CVector2::operator+(const CVector2 &vect2)
{
	float fVecX = fX+vect2.fX;
	float fVecY = fY+vect2.fY;
	return CVector2(fVecX,fVecY);
}

CVector2 CVector2::operator-(const CVector2 &vect2)
{
	float fVecX = fX-vect2.fX;
	float fVecY = fY-vect2.fY;
	return CVector2(fVecX,fVecY);
}

CVector2 CVector2::operator*(const CVector2 &vect2)
{
	float fVecX = fX*vect2.fX;
	float fVecY = fY*vect2.fY;
	return CVector2(fVecX,fVecY);
}

CVector2 CVector2::operator/(const CVector2 &vect2)
{
	float fVecX = fX/vect2.fX;
	float fVecY = fY/vect2.fY;
	return CVector2(fVecX,fVecY);
}

class vec3{
public:

	vec3(float XX,float YY,float ZZ):x(XX),y(YY),z(ZZ){}
    float x,y,z;
};


class CVector3:public CVector2 {
public:
	CVector3():CVector2(.0f,.0f),fZ(.0f){}
	CVector3(float XX,float YY,float ZZ):CVector2(XX,YY),fZ(ZZ){}
    float fZ;
	CVector3  operator+(const CVector3 &);
	CVector3  operator-(const CVector3 &);
	CVector3  operator*(const CVector3 &);
	CVector3  operator/(const CVector3 &);
	void normalizeVector();
	const float dotVector(const CVector3 &vec);
	const CVector3 multiply(float value);
	float squaredlen() const { return fX*fX + fY*fY + fZ*fZ; }
	float magnitude() const { return sqrtf(squaredlen()); }

    float dist(const CVector3 &e) const { CVector3 t; return dist(e, t); }
    float dist(const CVector3 &e, CVector3 &t) const { t = *this; t.sub(e); return t.magnitude(); }

	CVector3 &mul(float f) { fX *= f; fY *= f; fZ *= f; return *this; }
    CVector3 &div(float f) { fX /= f; fY /= f; fZ /= f; return *this; }
    CVector3 &add(float f) { fX += f; fY += f; fZ += f; return *this; }
    CVector3 &sub(float f) { fX -= f; fY -= f; fZ -= f; return *this; }

    CVector3 &add(const CVector3 &o) { fX += o.fX; fY += o.fY; fZ += o.fZ; return *this; }
    CVector3 &sub(const CVector3 &o) { fX -= o.fX; fY -= o.fY; fZ -= o.fZ; return *this; }


};


const CVector3 CVector3::multiply(float value)
{
fX *= value;
fY *= value;
fZ *= value;
return CVector3(fX,fY,fZ);
}
void CVector3::normalizeVector()
{
float magnitude = sqrtf(fX * fX + fY * fY + fZ * fZ);
fX /= magnitude;
fY /= magnitude;
fZ /= magnitude;
}

const float CVector3::dotVector(const CVector3 &vec) 
{
return (fX * vec.fX )+ (fY * vec.fY) + (fZ * vec.fZ);
}

	//calculate and return cross product
/*	Vector3D crossVector3D(const Vector3D &vec) const
	{
		return Vector3D(y * vec.z - z * vec.y,
				z * vec.x - x * vec.z,
				x * vec.y - y * vec.x);
	}*/



CVector3 CVector3::operator+(const CVector3 &vect3)
{
	float fVecX = fX+vect3.fX;
	float fVecY = fY+vect3.fY;
	float fVecZ = fZ+vect3.fZ;
	return CVector3(fVecX,fVecY,fVecZ);
}

CVector3 CVector3::operator-(const CVector3 &vect3)
{
	float fVecX = fX-vect3.fX;
	float fVecY = fY-vect3.fY;
	float fVecZ = fZ-vect3.fZ;
	return CVector3(fVecX,fVecY,fVecZ);
}

CVector3 CVector3::operator/(const CVector3 &vect3)
{
	float fVecX = fX/vect3.fX;
	float fVecY = fY/vect3.fY;
	float fVecZ = fZ/vect3.fZ;
	return CVector3(fVecX,fVecY,fVecZ);
}

CVector3 CVector3::operator*(const CVector3 &vect3)
{
	float fVecX = fX*vect3.fX;
	float fVecY = fY*vect3.fY;
	float fVecZ = fZ*vect3.fZ;
	return CVector3(fVecX,fVecY,fVecZ);
}



//Generated using ReClass 2011 by DrUnKeN ChEeTaH

class Players;

class Players
{
public:
void SetAllCoordsTo(CVector3);
char _0x0000[4];
	CVector3 m_vec3Coord; //0x0004 
	CVector3 m_vec3Accel; //0x0010 
char _0x001C[12];
	CVector3 m_vec3Accel2; //0x0028 
	CVector3 m_vec3Coord2; //0x0034 
	CVector3 m_vec2YawPitch; //0x0040 
	__int32 m_nRecoil;
char _0x0050[164];
	__int32 m_nHealth; //0x00F4 
	__int32 m_nArmor; //0x00F8 
char _0x00FC[8];
	__int32 m_nCurrentWeaponSlot; //0x0104 Knife-0, Submachine 4, Pistol -1
char _0x0108[8];
	__int32 m_nPistolAmmoLeft; //0x0110 
	__int32 m_nCarbineAmmoLeft; //0x0114 
	__int32 m_nShotgunAmmoLeft; //0x0118 
	__int32 m_nSubmachineAmmoLeft; //0x011C 
	__int32 m_nSniperAmmoLeft; //0x0120 
	__int32 AssaultRifleAmmoLeft; //0x0124 
char _0x0128[8];
	__int32 m_nAkimboAmmoLeft; //0x0130 
char _0x0134[4];
	__int32 m_nPistolAmmo; //0x0138 
	__int32 m_nCarbineAmmo; //0x013C 
	__int32 m_nShotgunAmmo; //0x0140 
	__int32 m_nSubmachineAmmo; //0x0144 
	__int32 m_nSniperAmmo; //0x0148 
	__int32 m_nAssaultRifleAmmo; //0x014C 
char _0x0150[4];
	__int32 m_nGrenadesAmmo; //0x0154 
	__int32 m_nAkimboAmmo; //0x0158 
	__int32 m_nKnifeAttacking; //500 Attack
	__int32 m_nPistolAttacking;// 160 attack
	__int32 m_nCarbineAttacking; //720 attack, 250 no bullets
	__int32 m_nShotgunAttacking; //1000 attack, 2581 reload, 250 no bullets
	__int32 m_nSubmachineAttacking; //80 Attack, 1650 Reload, 1900 Reload All bullets, 250 attack no bullets
	__int32 m_nSniperAttacking;  //1500 Attack, 1950 Reload All Bullets,  2269 Reload, 250 no ubllets
	__int32 m_nAssaultRifleAttacking; //120 Attack, 2000 Reload, 2250 Reload All bullets, 250 Attak no bullets
char _0x0178[12];
	__int32 m_nKnifeShots; //0x0184 
	__int32 m_nPistolShots; //0x0188 
	__int32 m_nCarbineShots; //0x018C 
	__int32 m_nShotgunShots; //0x0190 
	__int32 m_nSubmachineShots; //0x0194 
	__int32 m_nSniperShots; //0x0198 
	__int32 m_nAssaultRifleShots; //0x019C 
char _0x01A0[4];
	__int32 m_nBombDropsAdd2; //0x01A4 
	__int32 m_nAkimboShots; //0x01A8 
char _0x01AC[20];
	__int32 m_nDamageDone; //0x01C0 
char _0x01C4[16];
	__int32 m_nCurrentSkinID; //0x01D4 
	__int32 m_nCLATeamSkinID; //0x01D8 
	__int32 m_nRVSFSkinID; //0x01DC 
	__int32 m_nClientNum; //0x01E0 
char _0x01E4[4];
	__int32 m_nPing; //0x01E8 
char _0x01EC[8];
	__int32 m_nFrags; //0x01F4 
char _0x01F8[4];
	__int32 m_nDeaths; //0x01FC 
	__int32 m_nScore; //0x0200 
char _0x0204[20];
	char Name[264]; //0x0218 ->EDITING FROM HERE
//char _0x021C[260];
	__int32 m_nTeam; //1-> BLUE(RVSF), 0->RED(CLA) 4-> SPECTATOR
char _0x0324[8];
	__int32 m_nIsDead; //1 = dead, 0 = alive,5 ->DEAD SPECT
char _0x0330[88];
	CVector3 m_vec3Coord3; //0x0388 
	CVector3 m_vec3Coord4; //0x0394 
	CVector3 m_vec3Coord5; //0x03A0 
	CVector3 m_vec3Coord6; //0x03AC 
	CVector3 m_vec3Coord7; //0x03B8 
	CVector3 m_vec3Coord8; //0x03C4 
	CVector3 m_vec3Coord9; //0x03D0 
char _0x03DC[12];
	CVector2 m_vec2YawPitchAccel; //0x03E8 
	CVector2 m_vec2YawPitch2; //0x03F0 
char _0x03F8[4];
	CVector3 m_vec3CoordsVisibleOnly; //0x03FC 
char _0x0408[20];
};//Size=0x041C


char* getString(char *Name)
{
	char *name;
	for(int i=1;i<strlen(Name);)
	{
		if(Name[i]!='\0')
		{
		i++;
		name[i-1]=Name[i];
		
		}
		else break;
	}
	return name;
}
void Players::SetAllCoordsTo(CVector3 vect3Vec)
{
	this->m_vec3Coord = vect3Vec;
	this->m_vec3Coord2 = vect3Vec;
	this->m_vec3Coord3 = vect3Vec;
	this->m_vec3Coord4 = vect3Vec;
	this->m_vec3Coord5 = vect3Vec;
	this->m_vec3Coord6 = vect3Vec;
	this->m_vec3Coord7 = vect3Vec;
	this->m_vec3Coord8 = vect3Vec;
	this->m_vec3Coord9 = vect3Vec;

}



