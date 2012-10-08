#include "Player.h"
#pragma once

#define STDOUT 1
#define MAX_INT_LEN 10

typedef void (__cdecl* fcnDrawText)(const char *str, int left, int top, int r, int g,  int caret, int maxwidth,...);
static const fcnDrawText hkDrawText = (fcnDrawText) 0x00418AA0;


typedef void (__cdecl* fcnDrawTextf)(const char *fstr, int left, int top, ...);
static const fcnDrawTextf hkDrawTextf = (fcnDrawTextf) 0x004181F0;

void drawText(int x, int y, const char* text,...)
{
	  char buffer[256];
  va_list args;
  va_start (args, text);
  vsprintf (buffer,text, args);
  va_end (args);
	glPushMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glScalef(0.2f,.2f,.2f);
	hkDrawText(buffer,x*5,y*5,0,0,-1,-1);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void drawTextWithScale(float scale, int x, int y, const char* text,...)
{
	  char buffer[256];
  va_list args;
  va_start (args, text);
  vsprintf (buffer,text, args);
  va_end (args);
	glPushMatrix();
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glScalef(scale,scale,scale);
	hkDrawText(buffer,x*(1/scale),y*(1/scale),0,0,-1,-1);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


struct color
{
    float r, g, b, alpha;
    color(){}
    color(float r, float g, float b) : r(r), g(g), b(b), alpha(1.0f) {}
    color(float r, float g, float b, float a) : r(r), g(g), b(b), alpha(a) {}
};

void lineStyle(float width, int r, int g, int b)
{
    glLineWidth(width);
    glColor3ub(r,g,b);
}

class CGUI
{
public:
	int x, y, height, width;
	float m_fselectedScale;
	int m_nselected;
	int m_nESPID;
	bool m_bVisible, m_bESP,m_bAim,m_bAimTeam, m_bTrigger,m_bWeaponHack, m_bNoScope, m_bNoRecoil, m_bPerfectWeapons,m_bCrosshair;
	color clr;
	CGUI(int x, int y, int h, int w, color clr):x(x),y(y),height(h),width(w),clr(clr)
	{
		m_bVisible = true;
		m_fselectedScale=0.35f;
		m_nselected = 0;
		m_nESPID=0;
		m_bESP=true;
		m_bAim=true;
		m_bAimTeam=false;
		m_bTrigger=true;
		m_bWeaponHack=false;
		m_bNoScope=true;
		m_bNoRecoil=false;
		m_bPerfectWeapons = false;
		m_bCrosshair = true;
	}
	void drawMenuBase();

	void drawOptions(int,int);
};

void CGUI::drawOptions(int x, int y)
{
	drawTextWithScale(0.25f,this->x+5,this->y+10,(this->m_nselected==0)?"\f3>\f5ESP: %s":"\f5ESP: %s",(this->m_bESP)?"\fION":"\f7OFF");

	drawTextWithScale(0.25f,this->x+5,this->y+30,(this->m_nselected==1)?"\f3>\f5AIMBOT: %s":"\f5AIMBOT: %s",(this->m_bAim)?"\fION":"\f7OFF");

	drawTextWithScale(0.25f,this->x+5,this->y+50,(this->m_nselected==2)?"\f3>\f5AIMTEAM: %s":"\f5AIMTEAM: %s",(this->m_bAimTeam)?"\fION":"\f7OFF");

	drawTextWithScale(0.25f,this->x+5,this->y+70,(this->m_nselected==3)?"\f3>\f5TRIGGERBOT: %s":"\f5TRIGGERBOT: %s",(this->m_bTrigger)?"\fION":"\f7OFF");

	drawTextWithScale(0.25f,this->x+5,this->y+90,(this->m_nselected==4)?"\f3>\f5NO SCOPE: %s":"\f5NO SCOPE: %s",(this->m_bNoScope)?"\fION":"\f7OFF");

	drawTextWithScale(0.25f,this->x+5,this->y+110,(this->m_nselected==5)?"\f3>\f5NO RECOIL: %s":"\f5NO RECOIL: %s",(this->m_bNoRecoil)?"\fION":"\f7OFF");

	drawTextWithScale(0.25f,this->x+5,this->y+130,(this->m_nselected==6)?"\f3>\f5WEAPON HACK: %s":"\f5WEAPON HACK: %s",(this->m_bPerfectWeapons)?"\fION":"\f7OFF");

	drawTextWithScale(0.25f,this->x+5,this->y+150,(this->m_nselected==7)?"\f3>\f5CROSSHAIR: %s":"\f5CROSSHAIR: %s",(this->m_bCrosshair)?"\fION":"\f7OFF");


	drawText(this->x+5,this->y+170,"\f9::::::::TELEPORT HACK::::::::");

	drawTextWithScale(0.25f,this->x+5,this->y+190,(this->m_nselected==8)?"\f3>\f5SAVE POS":"\f5SAVE POS");

	drawTextWithScale(0.25f,this->x+5,this->y+210,(this->m_nselected==9)?"\f3>\f5LOAD POS":"\f5LOAD POS");

	//drawTextWithScale(0.25f,this->x+5,this->y+230,(this->m_nselected==10)?"\f3>\f5TELE TO CN: ":"\f5TELE TO CN: ");
}


void CGUI::drawMenuBase()
{
	  glPushMatrix();  //Make sure our transformations don't affect any other transformations in other code
	//glEnable(GL_BLEND); 
	lineStyle(2,0,0,0);
    glTranslatef(x, y,0);  //Translate rectangle to its assigned x and y position
    //Put other transformations here
    glBegin(GL_LINES);   //We want to draw a quad, i.e. shape with four sides
	glColor4f(clr.r,clr.g,clr.b,clr.alpha); //Set the colour to red 
      glVertex2i(0, 0);            //Draw the four corners of the rectangle
      glVertex2i(0, height);
      glVertex2i(width, height);
      glVertex2i(width, 0);  

	  glVertex2i(0, height); 
	   glVertex2i(width, height); 

	    glVertex2i(0, 0);
		 glVertex2i(width, 0); 
    glEnd();
//	glDisable(GL_BLEND);
  glPopMatrix();
  lineStyle(1,0,0,0);
}