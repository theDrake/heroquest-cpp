/*******************************************************************************
   Filename: main.h

     Author: David C. Drake (https://davidcdrake.com)

Description: Main header file for "HeroQuest 3D," a simple 3D action-adventure
             game based on the board game "HeroQuest" by Milton Bradley and
             Games Workshop.
*******************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <iostream>
#include <GL/glut.h>
#include "tga.h"
#include "keys.h"
#include "quest.h"
#include "character.h"

using namespace std;

const double PI = 4.0 * atan(1.0);

void drawCircle(double x1, double y1, double radius);
void drawRectangle(double x1, double y1, double x2, double y2);
void drawTriangle(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3);
void drawLine(double x1, double x2, double y1, double y2);
void drawText(double x, double y, char *string);
void reshape(int w, int h);
int getTextureNo(int i);

#endif  // MAIN_H_
