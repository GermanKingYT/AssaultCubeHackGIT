#include "Player.h"
#include "CGunInfo.h"
#include "CGUI.h"
#include "Tuple2.h"
#include "Tuple3.h"
#include "Tuple4.h"
#include "Matrix4.h"
#include "Geom.h"

#define M_PI		3.14159265358979323846F

int lasttarget = -1;
float lastdist = 100000.0f;
float bestdist=100000.0f;
float tDist = 0.0f;
int lasthealth = 9000;

DWORD *mode = (DWORD*)(dwAppBase+OFFSET_GAMEMODE);
MODE gamemode;
bool g_bLMBDown = false;


GLint m_viewport[4];




int r=255;
int g=255;

color abc(1.0f,1.0f,0.0f,0.2f);
CGUI guiHackMenu(m_viewport[2]/2,m_viewport[3],200,150,abc);


FILE *g_pFile;
Matrix4<float> view,projection;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

void WriteJMP(byte* location, byte* newFunction){
	DWORD dwOldProtection;
	VirtualProtect(location,5,PAGE_EXECUTE_READWRITE,&dwOldProtection);
	location[0] = 0xE9;
	*((dword*)(location+1))=(dword)(newFunction-location)-5;
	VirtualProtect(location,5,dwOldProtection,&dwOldProtection);
}





static bool g_bAimbotTeam=false;
static bool g_bAimbotOn=true;
bool g_bShoot = false;

glmatrixf mvmatrix, projmatrix, mvpmatrix, invmvmatrix, invmvpmatrix;
vec worldpos, camdir, camup, camright;


void readmatrices()
{
    glGetFloatv(GL_MODELVIEW_MATRIX, mvmatrix.v);
    glGetFloatv(GL_PROJECTION_MATRIX, projmatrix.v);
    camright = vec(-mvmatrix[0], -mvmatrix[4], -mvmatrix[8]);
    camup = vec(-mvmatrix[1], -mvmatrix[5], -mvmatrix[9]);
    camdir = vec(mvmatrix[2], mvmatrix[6], mvmatrix[10]);

    mvpmatrix.mul(projmatrix, mvmatrix);
    invmvmatrix.invert(mvmatrix,1.0f);
    invmvpmatrix.invert(mvpmatrix,1.0f);
}


Players* g_pEnemy[31];
CVector3 save1,save2;
Players* pPlayer = *(Players**)(DWORD)(dwAppBase+OFFSET_PLAYER);
DWORD* temp;
DWORD addr1;
DWORD* temp2;
	int *pPlayerNum = (int*)(dwAppBase+OFFSET_PLAYERNUM);
	int *pReservedSpace = (int*)(dwAppBase+OFFSET_RESERVEDSPACE);




/*
           case '0': color = bvec( 2,  255,  128 ); break;   // green: player talk
            case '1': color = bvec( 96,  160, 255 ); break;   // blue: team chat
            case '2': color = bvec( 230, 230,  20 ); break;   // yellow: gameplay action messages, only actions done by players
            case '3': color = bvec( 255,  64,  64 ); break;   // red: important errors and notes
            case '4': color = bvec( 128, 128, 128 ); break;   // gray
            case '5': color = bvec( 255, 255, 255 ); break;   // white
            case '6': color = bvec(  96,  48,   0 ); break;   // dark brown
            case '7': color = bvec( 153,  51,  51 ); break;   // dark red: dead admin
            case '8': color = bvec( 192,  64, 192 ); break;   // magenta
            case '9': color = bvec( 255, 102,   0 ); break;   // orange
            //extendeded color palette
            //case 'a': case 'A':color = bvec( 0xFF, 0xCC, 0xCC); break;   // some lowercase seem to have special meaning like 'b' (flashing text) so not yet using them
            case 'A':color = bvec( 0xff, 0xb7, 0xb7); break;   // red set
            case 'B':color = bvec( 0xCC, 0x33, 0x33); break;   //
            case 'C':color = bvec( 0x66, 0x33, 0x33); break;   //
            case 'D':color = bvec( 0xF8, 0x98, 0x4E); break;   //

            case 'E':color = bvec( 0xFF, 0xFF, 0xB7); break;   // yellow set
            case 'F':color = bvec( 0xCC, 0xCC, 0x33); break;   //
            case 'G':color = bvec( 0x66, 0x66, 0x33); break;   //
            case 'H':color = bvec( 0xCC, 0xFC, 0x58); break;   //

            case 'I':color = bvec( 0xB7, 0xFF, 0xB7); break;   // green set
            case 'J':color = bvec( 0x33, 0xCC, 0x33); break;   //
            case 'K':color = bvec( 0x33, 0x66, 0x33); break;   //
            case 'L':color = bvec( 0x3F, 0xFF, 0x98); break;   //

            case 'M':color = bvec( 0xB7, 0xFF, 0xFF); break;   // cyan set
            case 'N':color = bvec( 0x33, 0xCC, 0xCC); break;   //
            case 'O':color = bvec( 0x33, 0x66, 0x66); break;   //
            case 'P':color = bvec( 0x4F, 0xCC, 0xF8); break;   //

            case 'Q':color = bvec( 0xB7, 0xB7, 0xFF); break;   // blue set
            case 'R':color = bvec( 0x33, 0x33, 0xCC); break;   //
            case 'S':color = bvec( 0x33, 0x33, 0x66); break;   //
            case 'T':color = bvec( 0xA0, 0x49, 0xFF); break;   //

            case 'U':color = bvec( 0xFF, 0xB7, 0xFF); break;   // magenta set
            case 'V':color = bvec( 0xCC, 0x33, 0xCC); break;   //
            case 'W':color = bvec( 0x66, 0x33, 0x66); break;   //
            case 'X':color = bvec( 0xFF, 0x01, 0xD5); break;   //

            case 'Y':color = bvec( 0xC7, 0xD1, 0xE2); break;   // lt gray
*/



struct block { int x, y, xs, ys; block(int xx, int yy, int xss, int yss):x(xx),y(yy),xs(xss),ys(yss){}};


DWORD renderClientCall = 0x00414240;
DWORD renderClientRetn = 0x00414245;  // 0x00414240 + 0x5
DWORD renderClientFunc = 0x00413AA0;


DWORD renderScopeCall = 0x0045BB19;
DWORD renderScopeFunc = 0x004073A0;
DWORD renderScopeRetn = 0x0045BB1E;


DWORD DrawBigMsgCall = 0x00422BCC;
DWORD DrawBigMsgFunc = 0x00460C20;
DWORD DrawBigMsgRetn = 0x00422BCE;




DWORD drawHUDFunc = 0x00409B60;
DWORD drawHUDRetn = 0x00406C4E;


DWORD beginFunc = 0x004B624C;

//DRAWHUDFUNC 409B60
//CALL 00406C49 
//RETN 00406C4E 



typedef void (__cdecl* fcnGlBegin) (int mode);
static const fcnGlBegin fcBegin = (fcnGlBegin) 0x004B624C;



typedef void (__cdecl* fcnShowMessage) (char* message,...);
static const fcnShowMessage ShowMessage = (fcnShowMessage) 0x00460c20;



typedef void (__cdecl* fcnDrawLine) (int x1, int y1, int x2, int y2, color*);
static const fcnDrawLine hkDrawLine = (fcnDrawLine) 0x00403F20;




/*
\f0-f for colors
*/
typedef void (__cdecl* fcnHudOut) (char* message,...);
static const fcnHudOut HudOut = (fcnHudOut) 0x00408390;









void drawLine(int x1, int y1, int x2, int y2, color *c)
{
   glDisable(GL_BLEND);
   glColor4f(c->r, c->g, c->b, c->alpha);
    glBegin(GL_LINES);
    glVertex2f((float)x1, (float)y1);
    glVertex2f((float)x2, (float)y2);
    glEnd();
    glEnable(GL_BLEND);
}

void drawESP(int id, int x, int y, int size, color c)
{
	
	//glLoadIdentity(); 
	if(id==0) //SWASTIKA
	{
		glPushMatrix();
	glColor4f(c.r, c.g, c.b, c.alpha);
	glBegin(GL_LINES);
	 glVertex2f((float)(x), (float)y-(size/2));
	 glVertex2f((float)(x), (float)y+(size/2));

	glVertex2f((float)(x), (float)y-(size/2));
	 glVertex2f((float)(x)+(size/2), (float)y-(size/2));


	 glVertex2f((float)(x)-(size/2), (float)y+(size/2));
	 glVertex2f((float)(x), (float)y+(size/2));


	 glVertex2f((float)(x), (float)y);
	 glVertex2f((float)(x)-(size/2), (float)y);


	 glVertex2f((float)(x)-(size/2), (float)y);
	 glVertex2f((float)(x)-(size/2), (float)y-(size/2));


	 glVertex2f((float)(x), (float)y);
	 glVertex2f((float)(x)+(size/2), (float)y);


	 glVertex2f((float)(x)+(size/2), (float)y);
	 glVertex2f((float)(x)+(size/2), (float)y+(size/2));
	glEnd();
	glPopMatrix();
	}
	else if(id==1)
	{
	glPushMatrix();
	glColor4f(c.r, c.g, c.b, c.alpha);
    glBegin(GL_LINES);
    glVertex2f((float)(x)-size, (float)y);
    glVertex2f((float)(x)+size, (float)y);


    glVertex2f((float)(x), (float)(y)-size);
    glVertex2f((float)(x), (float)(y)+size);
    glEnd();
	glPopMatrix();
	}
	


}

void drawCrosshair(int x, int y, int size, color c)
{
	//glEnable(GL_BLEND);
	glColor4f(c.r,c.g,c.b,c.alpha);
    glBegin(GL_LINES);
    glVertex2f((float)(x)-size, (float)y);
    glVertex2f((float)(x)+size, (float)y);
    glEnd();
	glBegin(GL_LINES);
    glVertex2f((float)(x), (float)(y)-size);
    glVertex2f((float)(x), (float)(y)+size);
    glEnd();
 //   glDisable(GL_BLEND);
}





inline void renderhboxpart(Players *d, vec top, vec bottom, vec up)
{
	CVector3 playerCoord = d->m_vec3Coord;
	vec fixed(playerCoord.fX,playerCoord.fY,playerCoord.fZ);
	if(d->m_nIsDead==0 && d->m_vec3Coord.fX >= 0)
    {
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_LINE_LOOP);
        loopi(8)
        {
            vec pos(camright);
            pos.rotate(2*M_PI*i/8.0f, camdir).mul(2).add(fixed);
            glVertex3fv(pos.v);
        }
        glEnd();

        glBegin(GL_LINES);
		glVertex3fv(camup.v);
		glVertex3f(d->m_vec3Coord.fX,d->m_vec3Coord.fX,d->m_vec3Coord.fX);
        glEnd();
    }

    vec spoke;
    spoke.orthogonal(up);
    spoke.normalize().mul(1.1f);

    glBegin(GL_LINE_LOOP);
    loopi(8)
    {
        vec pos(spoke);
        pos.rotate(2*M_PI*i/8.0f, up).add(top);
        glVertex3fv(pos.v);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    loopi(8)
    {
        vec pos(spoke);
        pos.rotate(2*M_PI*i/8.0f, up).add(bottom);
        glVertex3fv(pos.v);
    }
    glEnd();
    glBegin(GL_LINES);
    loopi(8)
    {
        vec pos(spoke);
        pos.rotate(2*M_PI*i/8.0f, up).add(bottom);
        glVertex3fv(pos.v);
        pos.sub(bottom).add(top);
        glVertex3fv(pos.v);
    }
    glEnd();
	glEnable(GL_DEPTH_TEST);
}


void _declspec(naked) begin2(int mode)
{
		glDisable(GL_DEPTH_TEST);

}

void _declspec(naked) hRenderClient(){
	

			_asm{call renderClientFunc}
			

			//glPolygonMode(GL_FRONT, GL_LINE);

			_asm{jmp renderClientRetn}


		//	glEnable(GL_DEPTH_TEST); 
}

void _declspec(naked) hRenderScope(){
	
	//RenderScope();
	//DrawLine(300,200,10,200,400,10);
	if(!guiHackMenu.m_bNoScope)
	_asm{call renderScopeFunc}
	//hkDrawLine(0,0,1,1,0);
	_asm{
		JMP renderScopeRetn}
}





float RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

	
int RandomInt(int min, int max)
{

	int r = rand() / RAND_MAX;
	return min + r * (max - min);
}





 /*RADI DOBRO  KADA GLEDAM NA ISTOK
CVector2 W2S(vec location)
{
	CVector2 Return;
	CVector3 campostemp = pPlayer->m_vec3Coord;
	CVector3 camangletemp = pPlayer->m_vec2YawPitch;

	vec campos;
	campos.x = campostemp.fX;
	campos.y = campostemp.fY;
	campos.z = campostemp.fZ;

	vec delta, transformed;
	delta = location.sub(campos);
	transformed.x = delta.dot(camup);
	transformed.y = delta.dot(camdir);
	transformed.z = delta.dot(camright);

	if(transformed.z<1.00f)
		transformed.z = 1.00f;
	Return.fX=(1024/2.0f)+transformed.x*((1024/2.0f)/tan(90*M_PI/360.0f))/transformed.z;
	Return.fY=(768/ 2.0f ) + -transformed.y * ( ( 1024/ 2.0f ) / tan( 90* M_PI / 360.0f ) ) / transformed.z;
	return Return;
}*/

/*BETTER BUTH OPOSITE
CVector2 W2S(vec location) 
{
	CVector2 Return;
	CVector3 campostemp = pPlayer->m_vec3Coord;
	CVector3 camangletemp = pPlayer->m_vec2YawPitch;
	vec campos;
	campos.x = campostemp.fX;
	campos.y = campostemp.fY;
	campos.z = campostemp.fZ;

	vec AxisX, AxisY, AxisZ,delta, transformed;
	delta = campos.sub(location);
	transformed.x = delta.dot(camright);
	transformed.y = delta.dot(camup);
	transformed.z = delta.dot(camdir);

	if(transformed.z<1.00f)
		transformed.z = 1.00f;
	Return.fX=(1024/2.0f)+transformed.x*((768/2.0f)/tan(90*M_PI/360.0f))/transformed.z;
	Return.fY=(768/ 2.0f ) + -transformed.y * ( ( 768/ 2.0f ) / tan( 90* M_PI / 360.0f ) ) / transformed.z;
	return Return;
}*/



CVector2 W2S(vec location) 
{
	CVector2 Return;
	CVector3 campostemp = pPlayer->m_vec3Coord;
	CVector3 camangletemp = pPlayer->m_vec2YawPitch;
	vec campos;
	campos.x = campostemp.fX;
	campos.y = campostemp.fY;
	campos.z = campostemp.fZ;

	vec AxisX, AxisY, AxisZ,delta, transformed;
	delta = campos.sub(location);
	transformed.x = delta.dot(camright);
	transformed.y = delta.dot(camup);
	transformed.z = delta.dot(camdir);

	if(transformed.z<1.00f)
		transformed.z = 1.00f;
	Return.fX=(m_viewport[2]/2.0f)+transformed.x*((m_viewport[2]/2.0f)/tan(90*M_PI/360.0f))/transformed.z;
	Return.fY=(m_viewport[3]/ 2.0f ) + -transformed.y * ( (m_viewport[2]/ 2.0f ) / tan( 90* M_PI / 360.0f ) ) / transformed.z;
	return Return;
}

float distanceVect(CVector3 a, CVector3 b)
{
	return sqrt(pow((b.fX-a.fX),2)+pow((b.fY-a.fY),2)+pow((b.fZ-a.fZ),2));
}


bool altVis(CVector3 from,CVector3 to)
{
    static DWORD p = 0x47E780; //v1.1.0.4
	float tox,toy,toz,fromx,fromy,fromz;
	tox = to.fX;
	toy=to.fY;
	toz=to.fZ;

	fromy=from.fY;
	fromx=from.fX;
	fromz=from.fZ;
    __asm 
    {
        push toz
        push toy
        push tox
        push fromz
        push fromy
        push fromx
        xor eax, eax
        call p
        add esp, 24
    }
} 



void _declspec(naked) hDrawHUD()
{
	glGetIntegerv( GL_VIEWPORT, m_viewport );
	readmatrices();
	_asm {call drawHUDFunc}
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, m_viewport[2], m_viewport[3], 0.0f, -1.0f, 1.0f);

	if(guiHackMenu.m_bCrosshair)
	drawCrosshair(m_viewport[2]/2,m_viewport[3]/2,2,color(1.0f,0.0f,0.0f,1.0f));

	if(guiHackMenu.m_bESP==true)
	for(int i=0;i<*pReservedSpace;i++)
	{
		if(g_pEnemy[i]!=NULL && g_pEnemy[i]->m_nIsDead==0)
		{
			
			CVector3 temp = g_pEnemy[i]->m_vec3Coord;
			vec enemyvector;
			enemyvector.x = temp.fX;
			enemyvector.y = temp.fY;
			enemyvector.z = temp.fZ;
			CVector2 cord = W2S(enemyvector);

			//is EnemyTeam
			if(g_pEnemy[i]->m_nTeam!=pPlayer->m_nTeam && g_pEnemy[i]->m_nTeam!=4 && isTeamGame((int)*mode))
			{
			if(altVis(pPlayer->m_vec3Coord,g_pEnemy[i]->m_vec3Coord))
			drawESP(1,cord.fX,cord.fY,5,color(0.0f,1.0f,0.0f,1.0f));
			else
			drawESP(1,cord.fX,cord.fY,5,color(1.0f,.0f,0.0f,1.0f));
			drawText(cord.fX-50,cord.fY-80,"\fEDIST:%1.1f\n\fFHP:%d\n\fGARMOR:%d\n\fHNAME:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",distanceVect(g_pEnemy[i]->m_vec3Coord,pPlayer->m_vec3Coord),g_pEnemy[i]->m_nHealth,g_pEnemy[i]->m_nArmor,g_pEnemy[i]->Name[1],g_pEnemy[i]->Name[2],g_pEnemy[i]->Name[3],g_pEnemy[i]->Name[4],g_pEnemy[i]->Name[5],g_pEnemy[i]->Name[6],g_pEnemy[i]->Name[7],g_pEnemy[i]->Name[8],g_pEnemy[i]->Name[9],g_pEnemy[i]->Name[10],g_pEnemy[i]->Name[11],g_pEnemy[i]->Name[12],g_pEnemy[i]->Name[13],g_pEnemy[i]->Name[14],g_pEnemy[i]->Name[15]);//,g_pEnemy[i]->Name[16],g_pEnemy[i]->Name[17]);
			}
			else if(isTeamGame((int)*mode)==false)
			{
			if(altVis(pPlayer->m_vec3Coord,g_pEnemy[i]->m_vec3Coord))
			drawESP(1,cord.fX,cord.fY,5,color(0.0f,1.0f,0.0f,1.0f));
			else
			drawESP(1,cord.fX,cord.fY,5,color(1.0f,.0f,0.0f,1.0f));
			drawText(cord.fX-50,cord.fY-80,"\fEDIST:%1.1f\n\fFHP:%d\n\fGARMOR:%d\n\fHNAME:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",distanceVect(g_pEnemy[i]->m_vec3Coord,pPlayer->m_vec3Coord),g_pEnemy[i]->m_nHealth,g_pEnemy[i]->m_nArmor,g_pEnemy[i]->Name[1],g_pEnemy[i]->Name[2],g_pEnemy[i]->Name[3],g_pEnemy[i]->Name[4],g_pEnemy[i]->Name[5],g_pEnemy[i]->Name[6],g_pEnemy[i]->Name[7],g_pEnemy[i]->Name[8],g_pEnemy[i]->Name[9],g_pEnemy[i]->Name[10],g_pEnemy[i]->Name[11],g_pEnemy[i]->Name[12],g_pEnemy[i]->Name[13],g_pEnemy[i]->Name[14],g_pEnemy[i]->Name[15]);
			}
		}
	}

	if(guiHackMenu.m_bVisible)
	{
	guiHackMenu.drawMenuBase();
	guiHackMenu.drawOptions(0,0);
	drawText(guiHackMenu.x,guiHackMenu.y-13,"\fP::Menu::");
	}

	drawText(0,m_viewport[3]-13,"\f2M\f1R\f3.\f4L\f5U\f6D\f7I\f8J\f9A\f0K\fA'\fBS\fC M\fUU\fEL\fGT\fJI\fKH\fLA\fMC\fNK");
	glLoadIdentity();
	//drawText("TESTING",1024,768,255,255,-1,-1);
	//
	_asm{JMP drawHUDRetn}

}





 

void initHooks()
{
//	WriteJMP((byte*)beginFunc,(byte*)begin2);
	WriteJMP((byte*)0x00406C49,(byte*)hDrawHUD);
	WriteJMP((byte*)renderClientCall,(byte*)hRenderClient);
	WriteJMP((byte*)renderScopeCall,(byte*)hRenderScope);
}








float Get2DDistance(CVector3 v1, CVector3 v2)
{
     if ((v1.fX == 0) && (v2.fX == 0) && (v1.fY == 0) && (v2.fY == 0))
          return 0.0f;
     
     v1.fZ = v2.fZ = 0.0f;
     return v1.dist(v2);
}


int GetBestTarget()
{
	if(guiHackMenu.m_bAim && ((guiHackMenu.m_bTrigger && g_bLMBDown) || !guiHackMenu.m_bTrigger))
	{
	int ttarget=-1;
	if(!isTeamGame((int)*mode))
		guiHackMenu.m_bAimTeam=true;
	for(int i=0;i<*pReservedSpace;i++)
	{
		if(g_pEnemy[i] != NULL && !guiHackMenu.m_bAimTeam && g_pEnemy[i]->m_nTeam == pPlayer->m_nTeam)
			continue;
		else if(g_pEnemy[i] != NULL && (g_pEnemy[i]->m_nIsDead==0 && pPlayer->m_nIsDead==0) && altVis(pPlayer->m_vec3Coord,g_pEnemy[i]->m_vec3Coord))
		{
			tDist = Get2DDistance(pPlayer->m_vec3Coord,g_pEnemy[i]->m_vec3Coord);
			if((tDist <= bestdist && g_pEnemy[i]->m_nHealth <= lasthealth))
			{
				lasthealth = g_pEnemy[i]->m_nHealth;
				ttarget=i;
				bestdist=tDist;
			}
			//else
			//bestdist=tDist;
		}
	}
		if(ttarget==-1)
		{
			lasttarget=-1;
			bestdist=tDist;
			lasthealth=9000;
			return -1;
		}
		lasttarget=ttarget;
		//bestdist=9999999.0f;
		return ttarget;
	}
	lasttarget=-1;
	bestdist=tDist;
	return -1;
	
}



CVector3 save;

DWORD WINAPI Thread(LPVOID lpArgs)
{
	
	CGunInfo* weapons[10];
	/*
	Maybe detected by anti-cheat?
	I got banned for WeaponHack...
	*/
	

	

	//fstream LogFile;
	//LogFile.open("C:\\Users\\JA\\Documents\\Visual Studio 2010\\Projects\\AssaultCubeHack\\AssaultCubeHack\\Release\\data.txt", ios::out);



	if(pPlayer && g_pEnemy)
	{
	HudOut("\f9AIMBOT IS: %s",(guiHackMenu.m_bAim)?"\fION":"\f7OFF");
	HudOut("\f9AIMBOT TEAM IS: %s",(guiHackMenu.m_bAimTeam)?"\fION":"\f7OFF");
	int tick=0;
	guiHackMenu = CGUI(m_viewport[2]/1.4f,m_viewport[3]/3,240,200,abc);
	while(1)
	{
	for(int i=0;i<*pReservedSpace;i++)
	{
	temp = *(DWORD**)(dwAppBase+OFFSET_ENEMY);
	addr1=*(temp+i); //Iterate through playes
	if(addr1!=0)
	{
	temp2 =*(DWORD**)&addr1;//addr1;
	g_pEnemy[i] = (Players*)(temp2);
	}
	else
	{
	g_pEnemy[i]=NULL;
	}
	}

	


	if(guiHackMenu.m_bNoRecoil)
	pPlayer->m_nRecoil=0;
		
	int tmptarget = GetBestTarget();
	if(tmptarget!=-1 && g_pEnemy[tmptarget]!=NULL)
	{

	pPlayer->m_vec2YawPitch.fX = -(float)atan2(g_pEnemy[tmptarget]->m_vec3Coord.fX+(g_pEnemy[tmptarget]->m_vec3Accel.fX*distanceVect(pPlayer->m_vec3Coord,g_pEnemy[tmptarget]->m_vec3Coord)/1000) - pPlayer->m_vec3Coord.fX, g_pEnemy[tmptarget]->m_vec3Coord.fY+(g_pEnemy[tmptarget]->m_vec3Accel.fY*distanceVect(pPlayer->m_vec3Coord,g_pEnemy[tmptarget]->m_vec3Coord)/1000)-pPlayer->m_vec3Coord.fY)/M_PI*180+180;
	pPlayer->m_vec2YawPitch.fY = (float)atan2(g_pEnemy[tmptarget]->m_vec3Coord.fZ+(g_pEnemy[tmptarget]->m_vec3Accel.fZ*distanceVect(pPlayer->m_vec3Coord,g_pEnemy[tmptarget]->m_vec3Coord)/1000) - pPlayer->m_vec3Coord.fZ, distanceVect(g_pEnemy[tmptarget]->m_vec3Coord, pPlayer->m_vec3Coord))* 180 / M_PI;
//	pPlayer->m_vec2YawPitch.fX = -(float)atan2(g_pEnemy[tmptarget]->m_vec3Coord.fX-pPlayer->m_vec3Coord.fX,g_pEnemy[tmptarget]->m_vec3Coord.fY-pPlayer->m_vec3Coord.fY)/M_PI*180+180;
//	pPlayer->m_vec2YawPitch.fY = (float)atan2(g_pEnemy[tmptarget]->m_vec3Coord.fZ - pPlayer->m_vec3Coord.fZ, distanceVect(g_pEnemy[tmptarget]->m_vec3Coord,pPlayer->m_vec3Coord))* 180 / M_PI;
	}

		
		g_bLMBDown=false;
		if(GetAsyncKeyState(VK_LBUTTON))
		{
			g_bLMBDown=true;
		}
		else if(GetAsyncKeyState(VK_CONTROL))
		{
		g_bLMBDown=true;
		}
		else if(GetAsyncKeyState(VK_INSERT)&1)
		{
		//	guiHackMenu.m_nselected=-1;
			if(guiHackMenu.m_bVisible)
			guiHackMenu.m_bVisible=false;
			else if(!guiHackMenu.m_bVisible)
			guiHackMenu.m_bVisible=true;
		}
		else if(GetAsyncKeyState(VK_UP)&1  && guiHackMenu.m_bVisible == true)
		{
			if(guiHackMenu.m_nselected==0)
			guiHackMenu.m_nselected=9;
			else
			guiHackMenu.m_nselected--;
		}
		else if(GetAsyncKeyState(VK_DOWN)&1  && guiHackMenu.m_bVisible == true)
		{
			if(guiHackMenu.m_nselected==9)
			guiHackMenu.m_nselected=0;
			else
			guiHackMenu.m_nselected++;
		}
		else if(GetAsyncKeyState(VK_LEFT)&1 || GetAsyncKeyState(VK_RIGHT)&1 && guiHackMenu.m_bVisible == true)
		{
			if(guiHackMenu.m_nselected==0)
			{
				if(guiHackMenu.m_bESP)
					guiHackMenu.m_bESP=false;
				else if(!guiHackMenu.m_bESP)
					guiHackMenu.m_bESP=true;
			}
			else if(guiHackMenu.m_nselected==1)
			{
					if(guiHackMenu.m_bAim)
					guiHackMenu.m_bAim=false;
					else if(!guiHackMenu.m_bAim)
					guiHackMenu.m_bAim=true;
			}
			else if(guiHackMenu.m_nselected==2)
			{
					if(guiHackMenu.m_bAimTeam)
					guiHackMenu.m_bAimTeam=false;
					else if(!guiHackMenu.m_bAimTeam)
					guiHackMenu.m_bAimTeam=true;
			}
			else if(guiHackMenu.m_nselected==3)
			{
					if(guiHackMenu.m_bTrigger)
					guiHackMenu.m_bTrigger=false;
					else if(!guiHackMenu.m_bTrigger)
					guiHackMenu.m_bTrigger=true;
			}
			else if(guiHackMenu.m_nselected==4)
			{
					if(guiHackMenu.m_bNoScope)
					guiHackMenu.m_bNoScope=false;
					else if(!guiHackMenu.m_bNoScope)
					guiHackMenu.m_bNoScope=true;
			}
			else if(guiHackMenu.m_nselected==5)
			{
					if(guiHackMenu.m_bNoRecoil)
					guiHackMenu.m_bNoRecoil=false;
					else if(!guiHackMenu.m_bNoRecoil)
					guiHackMenu.m_bNoRecoil=true;
			}
			else if(guiHackMenu.m_nselected==6)
			{
					if(guiHackMenu.m_bPerfectWeapons)
					{
					HudOut("\f3You must restart game to disable this option");
					}
					else if(!guiHackMenu.m_bPerfectWeapons)
					{
					for(int i=0;i<10;i++)
					{
						weapons[i] = (CGunInfo*)(DWORD)((dwAppBase+KNIFE_OFFSET)+(0x128*i));
						cout << weapons[i]->modelname[0] << weapons[i]->modelname[1] << weapons[i]->modelname[2]<<endl;
					//	weapons[i]->attackdelay=0;
						//weapons[i]->reloadtime=0;
						weapons[i]->maxrecoil=0;
						weapons[i]->m_bIsAuto=1;
						weapons[i]->recoil=0;
						weapons[i]->spread=0;
					}
					guiHackMenu.m_bPerfectWeapons=true;
					}
			}

			else if(guiHackMenu.m_nselected==7)
			{
					if(guiHackMenu.m_bCrosshair)
					guiHackMenu.m_bCrosshair=false;
					else if(!guiHackMenu.m_bCrosshair)
					guiHackMenu.m_bCrosshair=true;
			}
			else if(guiHackMenu.m_nselected==8)
			{
				save = pPlayer->m_vec3Coord;
				HudOut("\fJPosition <%f> <%f> <%f> saved!", pPlayer->m_vec3Coord.fX,  pPlayer->m_vec3Coord.fY,  pPlayer->m_vec3Coord.fZ);
			}
			else if(guiHackMenu.m_nselected==9)
			{
				pPlayer->m_vec3Coord2 = save;
				HudOut("\fJPosition <%f> <%f> <%f> loaded!", save.fX, save.fY, save.fZ);
			}
		}
		}
	}
	return 0;
}


bool WINAPI DllMain(HMODULE hModule, DWORD Reason, LPVOID lpReserved)
{
	switch(Reason)
	{
	case DLL_PROCESS_ATTACH:
			initHooks();
			CreateThread(NULL,NULL,Thread,NULL,NULL,NULL);
			break;
	case DLL_PROCESS_DETACH:
			break;
	}
	return true;
}