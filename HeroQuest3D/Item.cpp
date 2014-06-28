//*****************************************************************************
//	  Filename: Item.cpp
//
//	    Author: David C. Drake (www.davidcdrake.com)
//
// Description: Definition of an 'Item' class.
//*****************************************************************************

#include "Item.h"

Item::Item(int type)
{
  type_ = type;
}

Item::~Item()
{}

int Item::getType()
{
  return type_;
}

void Item::draw()
{

}
