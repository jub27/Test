#pragma once
#include <iostream>
#include "goto.h"
using namespace std;
class Block
{
protected:
	int pos_x, pos_y;
public:
	Block(int x, int y);
	virtual void print() = 0;
	virtual void push() = 0;
	virtual string getType() = 0;
	virtual void addNearMineCount() = 0;
	virtual bool isZeroBlock() = 0;
	virtual bool isPushed() = 0;
	~Block();
};

class MineBlock : public Block {
public:
	MineBlock(int x, int y);
	void print();
	void push();
	string getType();
	void addNearMineCount();
	bool isZeroBlock();
	bool isPushed();
	~MineBlock();
};

class NormalBlock : public Block {
private:
	int nearMineCount;
	bool open;
public:
	NormalBlock(int x, int y);
	void print();
	void push();
	bool isZeroBlock();
	bool isPushed();
	string getType();
	void addNearMineCount();
	~NormalBlock();
};