/******************************************************************************
   Filename: Item.h

     Author: David C. Drake (http://davidcdrake.com)

Description: Declaration of an 'Item' class.
******************************************************************************/

#ifndef ITEM_H
#define ITEM_H

class Item
{
public:
	Item(int type);
	~Item();
	int getType();
	void draw();
private:
	int type_;
};

#endif
