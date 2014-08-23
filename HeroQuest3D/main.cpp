/******************************************************************************
   Filename: main.cpp

     Author: David C. Drake (http://davidcdrake.com), with initial assistance
             from Dr. Barton Stander (http://cit.dixie.edu/faculty/stander.php)

Description: Main file for "HeroQuest 3D," a simple 3D action-adventure game
             based on the board game "HeroQuest" by Milton Bradley and Games
             Workshop.
******************************************************************************/

#include "main.h"

double screenX = 1200;
double screenY = 600;

const int NUM_QUESTS   = 3;
const int NUM_TEXTURES = 12;

GLuint gTextures[NUM_TEXTURES];

double WL = -CELL_SIZE / 2.0;
double WR = DEFAULT_MAZE_WIDTH + CELL_SIZE / 2.0;
double WB = -CELL_SIZE / 2.0;
double WT = DEFAULT_MAZE_HEIGHT + CELL_SIZE / 2.0;

double VL = 0;
double VR = screenX;
double VB = 0;
double VT = screenY;

bool gPerspectiveKeyDown = false;
bool gLeftButtonDown     = false;
bool gMiddleButtonDown   = false;
bool gRightButtonDown    = false;

int gQuestNum = 1;

Quest* gQuest      = NULL;
Character* gPlayer = NULL;

//-----------------------------------------------------------------------------
//	    Method: readTgaImage
//
// Description: Loads an image from a given ".tga" file and returns it as a
//              gliGenericImage object.
//
//	    Inputs: filename - The image's filename.
//
//	   Outputs: A pointer to a newly created gliGenericImage object, or NULL if
//              an error occurs.
//-----------------------------------------------------------------------------
gliGenericImage* readTgaImage(char* filename)
{
  FILE* file;
  gliGenericImage* image;

  file = fopen(filename, "rb");
  
  if (!file)
  {
    cerr << "Error: could not open \"" << filename << "\"." << endl;
	  return NULL;
  }

  image = gliReadTGA(file, filename);
  fclose(file);

  if (!image)
  {
    cerr << "Error: could not decode file format of \"" << filename << "\"." << endl;
    return NULL;
  }

  return image;
}

//-----------------------------------------------------------------------------
//	    Method: readImage
//
// Description: A generic image loader. Loads an image from a given file and
//              returns it as a gliGenericImage object. (For now, this actually
//              only handles ".tga" files.)
//
//	    Inputs: filename - The image's filename.
//
//	   Outputs: A pointer to a newly created gliGenericImage object, or NULL if
//              an error occurs.
//-----------------------------------------------------------------------------
gliGenericImage* readImage(char* filename)
{
	size_t size = strlen(filename);
	if (toupper(filename[size - 3]) == 'T' &&
		  toupper(filename[size - 2]) == 'G' &&
		  toupper(filename[size - 1]) == 'A')
	{
		gliGenericImage* result = readTgaImage(filename);
		if (!result)
		{
			cerr << "Error opening '" << filename << "'." << endl;
			cin.get();
			exit(1);
		}
		return result;
	}
	else
	{
		cerr << "Unknown file type." << endl;
		cin.get();
		exit(1);
	}
}

//-----------------------------------------------------------------------------
//	    Method: setBorder
//
// Description: Sets the edges of a given image to a given border color.
//
//	    Inputs: image   - Pointer to the image object.
//              r, g, b - Desired RGB values for the border.
//
//	   Outputs: None.
//-----------------------------------------------------------------------------
void setBorder(gliGenericImage* image, int r, int g, int b)
{
	unsigned int border_r = r;
	unsigned int border_g = g;
	unsigned int border_b = b;
	int x, y;
	y = 0;
	for(x = 0; x < image->width; ++x)
	{
		image->pixels[3 * (y * image->width + x) + 0] = border_r;
		image->pixels[3 * (y * image->width + x) + 1] = border_g;
		image->pixels[3 * (y * image->width + x) + 2] = border_b;
	}
	y = 1;
	for(x = 0; x < image->width; ++x)
	{
		image->pixels[3 * (y * image->width + x) + 0] = border_r;
		image->pixels[3 * (y * image->width + x) + 1] = border_g;
		image->pixels[3 * (y * image->width + x) + 2] = border_b;
	}
	y = image->height - 1;
	for(x = 0; x < image->width; ++x)
	{
		image->pixels[3 * (y * image->width + x) + 0] = border_r;
		image->pixels[3 * (y * image->width + x) + 1] = border_g;
		image->pixels[3 * (y * image->width + x) + 2] = border_b;
	}
	y=image->height-2;
	for(x=0; x<image->width; x++)
	{
		image->pixels[3 * (y * image->width + x) + 0] = border_r;
		image->pixels[3 * (y * image->width + x) + 1] = border_g;
		image->pixels[3 * (y * image->width + x) + 2] = border_b;
	}
	x=0;
	for(y=0; y<image->height; y++)
	{
		image->pixels[3 * (y * image->width + x) + 0] = border_r;
		image->pixels[3 * (y * image->width + x) + 1] = border_g;
		image->pixels[3 * (y * image->width + x) + 2] = border_b;
	}
	x=1;
	for(y=0; y<image->height; y++)
	{
		image->pixels[3 * (y * image->width + x) + 0] = border_r;
		image->pixels[3 * (y * image->width + x) + 1] = border_g;
		image->pixels[3 * (y * image->width + x) + 2] = border_b;
	}
	x = image->width - 1;
	for(y = 0; y < image->height; ++y)
	{
		int index = 3 * (y * image->width + x);
		image->pixels[index + 0] = border_r;
		image->pixels[index + 1] = border_g;
		image->pixels[index + 2] = border_b;
	}
	x = image->width - 2;
	for(y = 0; y < image->height; ++y)
	{
		int index = 3 * (y * image->width + x);
		image->pixels[index + 0] = border_r;
		image->pixels[index + 1] = border_g;
		image->pixels[index + 2] = border_b;
	}
}

//-----------------------------------------------------------------------------
//	    Method: isPowerOfTwo
//
// Description: Determines whether a given value is a perfect power of 2 (up to
//              4096). Useful for determining whether a texture image should be
//              loaded in mipmap format.
//
//	    Inputs: i - Integer value to be tested.
//
//	   Outputs: Returns 'true' if the given value is a perfect power of 2 (up
//              to 4096), 'false' otherwise.
//-----------------------------------------------------------------------------
bool isPowerOfTwo(int i)
{
	return i == 2    || i == 4   || i == 8   || i == 16  || i == 32   ||
		     i == 64   || i == 128 || i == 256 || i == 512 || i == 1024 ||
		     i == 2048 || i == 4096;
}

//-----------------------------------------------------------------------------
//	    Method: getTextureNo
//
// Description: Given a texture's index value, returns the integer value that
//              actually represents that texture.
//
//	    Inputs: i - Index of the desired texture.
//
//	   Outputs: An integer representing the desired texture, or -1 if the index
//              is invalid.
//-----------------------------------------------------------------------------
int getTextureNo(int i)
{
  if (i < 0 || i > NUM_TEXTURES)
    return -1;

  return gTextures[i];
}

//--------------------------------------------------------------------------------------------------
// Functions that draw basic primitives.
//--------------------------------------------------------------------------------------------------

void drawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i = 0; i < 32; ++i)
	{
		double theta = (double) i / 32.0 * 2.0 * PI;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void drawRectangle(double x1, double y1,
                   double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1, y1);
	glVertex2d(x2, y1);
	glVertex2d(x2, y2);
	glVertex2d(x1, y2);
	glEnd();
}

void drawTriangle(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();
}

void drawLine(double x1, double y1,
              double x2, double y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

// Outputs a string of text at the specified location.
void drawText(double x, double y, char* string)
{
	void* font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

  glDisable(GL_BLEND);
}

//-----------------------------------------------------------------------------
// GLUT callback functions.
//-----------------------------------------------------------------------------

void display(void)
{
	// Check for level completion:
	if (isKeyPressed('e') &&
		  gPlayer->getNextX() > gQuest->getFinishX() &&
		  gPlayer->getNextX() < (gQuest->getFinishX() + 1.0) &&
		  gPlayer->getNextY() > (gQuest->getHeight() - 2.0))
	{
		int perspective = gQuest->getPerspective();
		gQuestNum++;
		if (gQuestNum > NUM_QUESTS)
		{
			gQuestNum = 1;
		}
		if (gPlayer)
			delete gPlayer;
		if (gQuest)
			delete gQuest;
		gQuest = new Quest(gQuestNum, DEFAULT_MAZE_WIDTH, DEFAULT_MAZE_HEIGHT);
		gPlayer = new Character(gPlayer->getType(), gQuest);
    gQuest->setPlayer(gPlayer);
		gQuest->setPerspective(perspective);
	}

	// Check for gravity effects:
	if (gPlayer->isOnGround() == false)
	{
		gPlayer->fall();
	}
	
	// Check for user input:
	if (isKeyPressed(KEY_ESCAPE))
	{
		if (gQuest)
			delete gQuest;
		if (gPlayer)
			delete gPlayer;
		exit(0);
	}
	if (isKeyPressed('p') || isKeyPressed('r'))
	{
		gPerspectiveKeyDown = true;
	}
	else if (gPerspectiveKeyDown)
	{
		gPerspectiveKeyDown = false;
		if (gQuest->getPerspective() == FIRST_PERSON)
		{
			gQuest->setPerspective(THIRD_PERSON);
		}
		else
		{
			gQuest->setPerspective(FIRST_PERSON);
		}
		reshape(screenX, screenY);
	}
	if (isKeyPressed(KEY_SPACE) && gPlayer->isOnGround())
	{
		gPlayer->jump();
	}
	if (isKeyPressed(KEY_UP) || isKeyPressed('w'))
	{
		gPlayer->moveForward();
	}
	if (isKeyPressed(KEY_DOWN) || isKeyPressed('s'))
	{
		gPlayer->moveBackward();
	}
	if (isKeyPressed('z'))
	{
		gPlayer->strafeLeft();
	}
	if (isKeyPressed('c'))
	{
		gPlayer->strafeRight();
	}
	if (isKeyPressed(KEY_LEFT) || isKeyPressed('a'))
	{
		gPlayer->rotateLeft();
	}
	if (isKeyPressed(KEY_RIGHT) || isKeyPressed('d'))
	{
		gPlayer->rotateRight();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	if (gQuest->getPerspective() == FIRST_PERSON)
	{
		gluLookAt(gPlayer->getX(),
              gPlayer->getY(),
              gPlayer->getZ() + gPlayer->getHeight() / 1.5,
				      gPlayer->getNextX(),
              gPlayer->getNextY(),
              gPlayer->getNextZ() + gPlayer->getHeight() / 1.5,
				      0.0,
              0.0,
              1.0);
	}
	else if (gQuest->getPerspective() == THIRD_PERSON)
	{
		gluLookAt(DEFAULT_MAZE_WIDTH / 2.0 - 0.25,
              -DEFAULT_MAZE_HEIGHT / 2.0 - 0.35,
              DEFAULT_MAZE_WIDTH + DEFAULT_MAZE_HEIGHT,
				      DEFAULT_MAZE_WIDTH / 2.0 - 0.25,
              DEFAULT_MAZE_HEIGHT / 2.0 - 0.35,
              0.0,
				      0.0,
              0.0,
              1.0);
	}

	// Draw the quest environment:
	gQuest->draw();

	// Draw game characters:
	if (gQuest->getPerspective() != FIRST_PERSON)
		gPlayer->draw();

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	// Reset global variables to the new width and height:
	screenX = w;
	screenY = h;

	/*// Set the pixel resolution of the final picture (screen coordinates):
	double desiredAspectRatio = (double) (DEFAULT_MAZE_HEIGHT + CELL_SIZE) / (DEFAULT_MAZE_WIDTH + CELL_SIZE);
	if (w * desiredAspectRatio > h)
		w = h / desiredAspectRatio;
	h = w * desiredAspectRatio;
	
	double wPad = screenX - w;
	double hPad = screenY - h;
	VL = 0 + wPad * CELL_SIZE / 2.0;
	VR = w + wPad * CELL_SIZE / 2.0;
	VB = 0 + hPad * CELL_SIZE / 2.0;
	VT = h + hPad * CELL_SIZE / 2.0;
	glViewport(VL, VB, VR - VL, VT - VB);*/
	glViewport(0, 0, w, h);

	/*
	// Set the pixel resolution of the final picture (screen coordinates):
	if (w > h)
	{
		w = h;
	}
	glViewport(0, 0, w, 1.0 * DEFAULT_MAZE_HEIGHT/DEFAULT_MAZE_WIDTH * w);
	*/

	// Set the projection mode and the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (gQuest->getPerspective() == FIRST_PERSON)
	{
		gluPerspective(35, (double) w / h, 0.1, (DEFAULT_MAZE_WIDTH + DEFAULT_MAZE_HEIGHT) * 2);
	}
	else if (gQuest->getPerspective() == THIRD_PERSON)
	{
		gluPerspective(35, (double) w / h, 1.0, (DEFAULT_MAZE_WIDTH + DEFAULT_MAZE_HEIGHT) * 2);
	}
	glMatrixMode(GL_MODELVIEW);
}

void SolveRatio(double aLow, double aValue, double aHigh,
				        double bLow, double &bValue, double bHigh)
{
	double ratio = (aValue - aLow) / (aHigh - aLow);
	bValue = bLow + ratio * (bHigh - bLow);
}

void mouse(int mouse_button, int state, int px, int py)
{
	/*double wx, wy; // Same as (px, py), but in World Coordinates.

	py = screenY - py;
	
	SolveRatio(VL, px, VR, WL, wx, WR);
	SolveRatio(VB, py, VT, WB, wy, WT);*/

	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gLeftButtonDown = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		gLeftButtonDown = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
		gMiddleButtonDown = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
		gMiddleButtonDown = false;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		gRightButtonDown = true;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP) 
	{
		gRightButtonDown = false;
	}

	glutPostRedisplay();
}

void initializeMyStuff()
{
	// Initialize textures:
	gliGenericImage* image[NUM_TEXTURES];
	int n = 0;

		// Quest 1:
	image[n++] = readImage("textures/BrickMessy0102_17_S.tga");
	image[n++] = readImage("textures/Gravel0134_19_S.tga");
	image[n++] = readImage("textures/RockSmooth0133_9_S.tga");
	image[n++] = readImage("textures/WoodStudded0042_5_S.tga");

		// Quest 2:
	image[n++] = readImage("textures/Bones0031_5_S.tga");
	image[n++] = readImage("textures/BrickRound0043_14_S.tga");
	image[n++] = readImage("textures/TilesSmall0070_5_S.tga");
	image[n++] = readImage("textures/WoodStudded0029_7_S.tga");

		// Quest 3:
	image[n++] = readImage("textures/MarbleWhite0035_2_S.tga");
	image[n++] = readImage("textures/SandPebbles0054_1_S.tga");
	image[n++] = readImage("textures/MarbleWhite0067_49_S.tga");
	image[n++] = readImage("textures/WoodStudded0035_2_S.tga");

	if (n != NUM_TEXTURES)
	{
		cerr << "Error: expected " << NUM_TEXTURES << " textures, loaded " << n
         << "." << endl;
		cin.get();
		exit(1);
	}

	glGenTextures(NUM_TEXTURES, gTextures);

	for(int i = 0; i < NUM_TEXTURES; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, gTextures[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		bool repeats = false;

    // A border is needed if clamping and not filling the whole polygon:
		bool needsBorder = false;

		if (repeats)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		if (needsBorder)
		{
			setBorder(image[i], 0, 0, 0);
		}

    // Images whose width and height are not powers of 2 must use mipmaps:
    bool mipmaps = true;
		if (!isPowerOfTwo(image[i]->height) || !isPowerOfTwo(image[i]->width))
		{
			mipmaps = true; 
		}

		if (mipmaps)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, image[i]->components,
                        image[i]->width, image[i]->height,
                        image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      //GL_LINEAR_MIPMAP_LINEAR);
                      GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                      //GL_LINEAR);
                      GL_NEAREST);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, image[i]->components,
                   image[i]->width, image[i]->height, 0,
                   image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	// Initialize the quest and the player character:
	gQuest = new Quest(gQuestNum, DEFAULT_MAZE_WIDTH, DEFAULT_MAZE_HEIGHT);
	gPlayer = new Character(PLAYER_BARBARIAN, gQuest);
  gQuest->setPlayer(gPlayer);
}


int main(int argc, char** argv)
{
	srand(time(0));
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenX, screenY);
	glutInitWindowPosition(50, 50);

	bool fullscreen = false;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Hero Quest 3D");
	}

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	initKeyboard();

	glColor3d(0.0, 0.0, 0.0);			    // Foreground color.
	glClearColor(0.0, 0.0, 0.0, 0.0);	// Background color.
	initializeMyStuff();

	glutMainLoop();

  if (gQuest)
  {
    delete gQuest;
    gQuest = NULL;
  }
  if (gPlayer)
  {
    delete gPlayer;
    gPlayer = NULL;
  }

	return 0;
}
