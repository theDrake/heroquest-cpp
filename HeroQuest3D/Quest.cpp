/******************************************************************************
   Filename: Quest.cpp

     Author: David C. Drake (http://davidcdrake.com)

Description: Definition of a 'Quest' class responsible for managing all aspects
             of a given quest, including its associated location, NPCs, traps,
             items, events, etc.
******************************************************************************/

#include "Quest.h"

//-----------------------------------------------------------------------------
//      Method: Quest
//
// Description: Constructs a Quest object (via the 'initialize' method)
//              according to a given quest number, width, height, and
//              perspective.
//
//      Inputs: questNo     - Integer representing a specific quest.
//              width       - Number of cell columns.
//              height      - Number of cell rows.
//              perspective - Integer representing the desired perspective.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
Quest::Quest(int questNo, int width, int height, int perspective)
{
  initialize(questNo, width, height, perspective);
}

//-----------------------------------------------------------------------------
//      Method: ~Quest
//
// Description: Destructs the Quest object and any member objects it contains.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
Quest::~Quest()
{
  cells_.clear();
  characters_.clear();
}

//-----------------------------------------------------------------------------
//      Method: initialize
//
// Description: Initializes the Quest object -- including all associated cells,
//              NPCs, and items -- according to a given quest number, width,
//              height, and perspective.
//
//      Inputs: questNo     - Integer representing a specific quest.
//              width       - Number of cell columns.
//              height      - Number of cell rows.
//              perspective - Integer representing the desired perspective.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Quest::initialize(int questNo, int width, int height, int perspective)
{
  questNo_     = questNo;
  width_       = width;
  height_      = height;
  perspective_ = perspective;

  initializeCells();
  removeWalls(0, 0);
  setStartAndFinish();

  // Remove ceiling tiles (if desired):
  if (false)
  {
    for (int i = 0; i < DEFAULT_MAZE_WIDTH; ++i)
    {
      for (int j = 0; j < DEFAULT_MAZE_HEIGHT; ++j)
      {
        int cellIndex = i + j * width_;
        if (cellIndex < 0 || cellIndex > cells_.size())
        {
          continue;
        }
        cells_[cellIndex]->removeWall(TOP);
      }
    }
  }

  // Set default textures:
  int t = 0 + TEXTURE_OFFSET_PER_QUEST * (questNo - 1);
  for (int i = 0; i < DEFAULT_MAZE_WIDTH; ++i)
  {
    for (int j = 0; j < DEFAULT_MAZE_HEIGHT; ++j)
    {
      // Set north, south, east, and west wall textures:
      for (int side = 0; side < 4; ++side)
      {
        cells_[i + j * width_]->setTexture(side, getTextureNo(t));
      }

      // Set the floor texture:
      cells_[i + j * width_]->setTexture(BOTTOM, getTextureNo(t + 1));

      // Set the ceiling texture:
      cells_[i + j * width_]->setTexture(TOP, getTextureNo(t + 2));
    }
  }

  // Set the start and finish door textures:
  cells_[startX_]->setTexture(SOUTH, getTextureNo(t + 3));
  cells_[finishX_ + (height_ - 1) * width_]->setTexture(NORTH,
                                                        getTextureNo(t + 3));

  // Finally, initialize NPCs:
  initializeCharacters();
}

//-----------------------------------------------------------------------------
//      Method: initializeCells
//
// Description: Creates and initializes all Cell objects associated with the
//              current quest and adds them to the 'cells_' vector.
//
//      Inputs: None.
//
//     Outputs: The number of cells created.
//-----------------------------------------------------------------------------
int Quest::initializeCells()
{
  int nCells = 0;

  cells_.clear();
  for (int i = 0; i < width_; ++i)
  {
    for (int j = 0; j < height_; ++j)
    {
      cells_.push_back(new Cell());
      ++nCells;
    }
  }

  // Assign each cell a pointer to each of its neighbors:
  for (int i = 0; i < DEFAULT_MAZE_WIDTH; ++i)
  {
    for (int j = 0; j < DEFAULT_MAZE_HEIGHT; ++j)
    {
      assignNeighbors(i, j);
    }
  }

  return nCells;
}

//-----------------------------------------------------------------------------
//      Method: initializeCharacters
//
// Description: Creates and initializes all non-player character (NPC) objects
//              associated with the current quest and adds them to the
//              'characters_' vector.
//
//      Inputs: None.
//
//     Outputs: The number of NPCs created.
//-----------------------------------------------------------------------------
int Quest::initializeCharacters()
{
  int nCharacters = 0;

  characters_.clear();
  for (int i = 0; i < DEFAULT_NUM_NPCS; ++i)
  {
    characters_.push_back(new Character(GOBLIN, this));
    ++nCharacters;
  }

  return nCharacters;
}

//-----------------------------------------------------------------------------
//      Method: assignNeighbors
//
// Description: Assigns neighbor pointers to the cell located at a given set of
//              coordinates.
//
//      Inputs: x, y - Coordinates of the cell to be assigned neighbors.
//
//     Outputs: The number of neighbors assigned, or -1 if the arguments are
//              invalid.
//-----------------------------------------------------------------------------
int Quest::assignNeighbors(int x, int y)
{
  int nNeighbors = 0;

  if (x < 0 || y < 0 || x >= DEFAULT_MAZE_WIDTH || y >= DEFAULT_MAZE_HEIGHT)
  {
    return -1;
  }

  for (int i = 0; i < 4; ++i)
  {
    if (x - 1 >= 0)
    {
      cells_[x + y * width_]->setNeighbor(WEST,
                                          cells_[x - 1 + y * width_]);
      ++nNeighbors;
    }
    if (x + 1 < DEFAULT_MAZE_WIDTH)
    {
      cells_[x + y * width_]->setNeighbor(EAST,
                                          cells_[x + 1 + y * width_]);
      ++nNeighbors;
    }
    if (y - 1 >= 0)
    {
      cells_[x + y * width_]->setNeighbor(SOUTH,
                                          cells_[x + (y - 1) * width_]);
      ++nNeighbors;
    }
    if (y + 1 < DEFAULT_MAZE_HEIGHT)
    {
      cells_[x + y * width_]->setNeighbor(NORTH,
                                          cells_[x + (y + 1) * width_]);
      ++nNeighbors;
    }
  }

  return nNeighbors;
}

//-----------------------------------------------------------------------------
//      Method: removeWalls
//
// Description: Recursively removes cell walls so as to create a maze.
//
//      Inputs: i, j - Coordinates of the cell to be checked for a suitable
//                     wall to remove.
//
//     Outputs: 0 if successful, -1 if the arguments are invalid.
//-----------------------------------------------------------------------------
int Quest::removeWalls(int i, int j)
{
  int wall, cellIndex = i + j * width_;

  if (i < 0 || j < 0 || i >= DEFAULT_MAZE_WIDTH || j >= DEFAULT_MAZE_HEIGHT)
  {
    return -1;
  }

  cells_[cellIndex]->setVisited(true);

  while((wall = cells_[cellIndex]->removeRandomWall()) >= 0)
  {
    switch(wall)
    {
    case NORTH:
      removeWalls(i, j + 1);
      break;
    case SOUTH:
      removeWalls(i, j - 1);
      break;
    case EAST:
      removeWalls(i + 1, j);
      break;
    case WEST:
      removeWalls(i - 1, j);
      break;
    default:
      break;
    }
  }

  return 0;
}

//-----------------------------------------------------------------------------
//      Method: setStartAndFinish
//
// Description: Randomly selects x-coordinates for the quest's start and finish
//              locations (their y-coordinates are "0" and "height_ - 1",
//              respectively).
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Quest::setStartAndFinish()
{
  startX_  = rand() % width_;
  finishX_ = rand() % width_;
}

//-----------------------------------------------------------------------------
//      Method: setPlayer
//
// Description: Provides the quest with a pointer to the player character.
//
//      Inputs: player - Pointer to the player character.
//
//     Outputs: The newly assigned player pointer.
//-----------------------------------------------------------------------------
Character* Quest::setPlayer(Character* player)
{
  return player_ = player;
}

//-----------------------------------------------------------------------------
//      Method: setPerspective
//
// Description: Sets the current perspective (i.e., camera location) to a given
//              value.
//
//      Inputs: perspective - Integer representing the desired perspective.
//
//     Outputs: The newly assigned perspective value.
//-----------------------------------------------------------------------------
int Quest::setPerspective(int perspective)
{
  return perspective_ = perspective;
}

//-----------------------------------------------------------------------------
//      Method: getPerspective
//
// Description: Returns an integer representing the current perspective (i.e.,
//              camera location).
//
//      Inputs: None.
//
//     Outputs: An integer representing the current perspective.
//-----------------------------------------------------------------------------
int Quest::getPerspective() const
{
  return perspective_;
}

//-----------------------------------------------------------------------------
//      Method: getWidth
//
// Description: Returns the quest location's width, measured in cells.
//
//      Inputs: None.
//
//     Outputs: The quest location's width (i.e., no. of cell columns).
//-----------------------------------------------------------------------------
int Quest::getWidth() const
{
  return width_;
}

//-----------------------------------------------------------------------------
//      Method: getHeight
//
// Description: Returns the quest location's height, measured in cells.
//
//      Inputs: None.
//
//     Outputs: The quest location's height (i.e., no. of cell rows).
//-----------------------------------------------------------------------------
int Quest::getHeight() const
{
  return height_;
}

//-----------------------------------------------------------------------------
//      Method: getStartX
//
// Description: Returns the x-coordinate of the quest's start location.
//
//      Inputs: None.
//
//     Outputs: The x-coordinate of the quest's start location.
//-----------------------------------------------------------------------------
int Quest::getStartX() const
{
  return startX_;
}

//-----------------------------------------------------------------------------
//      Method: getFinishX
//
// Description: Returns the x-coordinate of the quest's finish location.
//
//      Inputs: None.
//
//     Outputs: The x-coordinate of the quest's finish location.
//-----------------------------------------------------------------------------
int Quest::getFinishX() const
{
  return finishX_;
}

//-----------------------------------------------------------------------------
//      Method: isLegalPosition
//
// Description: Determines whether a given (x, y) position is a legal location
//              for a Character with a given collision radius to
//              exist.
//
//      Inputs: x, y   - Coordinates of the position to be tested, measured in
//                       pixels.
//              radius - Collision radius of the Character whose
//                       position is being tested.
//
//     Outputs: Returns 'true' if the position is legal, 'false' otherwise.
//-----------------------------------------------------------------------------
bool Quest::isLegalPosition(double x, double y, double radius) const
{
  int cellIndex  = (int) x + ((int) y) * width_;
  double offsetX = x - (int) x;
  double offsetY = y - (int) y;

  if (y < 0         ||
      y > height_   ||
      x < 0         ||
      x > width_    ||
      cellIndex < 0 ||
      cellIndex > cells_.size())
  {
    return false;
  }

  // Test north wall:
  if (cells_[cellIndex]->hasWallAt(NORTH) && offsetY + radius > 1.0)
  {
    return false;
  }

  // Test south wall:
  if (cells_[cellIndex]->hasWallAt(SOUTH) && offsetY - radius < 0.0)
  {
    return false;
  }

  // Test east wall:
  if (cells_[cellIndex]->hasWallAt(EAST) && offsetX + radius > 1.0)
  {
    return false;
  }

  // Test west wall:
  if (cells_[cellIndex]->hasWallAt(WEST) && offsetX - radius < 0.0)
  {
    return false;
  }

  // Test corners:
  /*if (offsetX + radius > 1.0 && offsetY - radius < 0.0)
  {
    return false;
  }*/

  return true;
}

//-----------------------------------------------------------------------------
//      Method: draw
//
// Description: Draws the quest environment and associated objects according to
//              the current perspective.
//
//      Inputs: None.
//
//     Outputs: None.
//-----------------------------------------------------------------------------
void Quest::draw()
{
  for (int i = 0; i < width_; ++i)
  {
    for (int j = 0; j < height_; ++j)
    {
      int cellIndex = i + j * width_;
      if (cellIndex < 0 || cellIndex > cells_.size())
      {
        continue;
      }
      cells_[cellIndex]->draw(i, j, perspective_);
    }
  }

  vector<Character*>::iterator iter;
  for (iter = characters_.begin(); iter < characters_.end(); ++iter)
  {
    (*iter)->act(player_);
    (*iter)->draw();
  }
}
