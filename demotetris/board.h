#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <vector>
#include "block.h"

class Board {
  public:
	Board();
	int column;
	int row;
	vector<Block> blocks;
	int current;

	int getColumn();
	int getRow();
	vector<Block> getBlocks();
	int getCurrent();

	void setColumn(int c);
	void setRow(int r);
	void setBlock(vector<Block> bv);
	void addBlock(Block b);
	void setCurrent(int c);

	void moveBlock(float x,float y,float z);
	void rotateBlock(float angle,char axis);
	bool
};

#endif
