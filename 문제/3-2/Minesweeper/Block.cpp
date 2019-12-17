#include "Block.h"

Block::Block(int x, int y)
{
	pos_x = x;
	pos_y = y;
}

Block::~Block()
{
}

MineBlock::MineBlock(int x, int y) : Block(x, y) {

}

void MineBlock::print() {
	gotoxy(pos_x*2, pos_y);
	cout << "¢Ì";
}

void MineBlock::push() {
	gotoxy(pos_x * 2, pos_y);
	cout << "X";
}

void MineBlock::addNearMineCount() {

}

bool MineBlock::isZeroBlock() {
	return false;
}

bool MineBlock::isPushed() {
	return false;
}

string MineBlock::getType() {
	return "mine";
}
MineBlock::~MineBlock() {

}

NormalBlock::NormalBlock(int x, int y) : Block(x, y) {
	nearMineCount = 0;
	open = false;
}
void NormalBlock::push() {
	open = true;
	print();
}

void NormalBlock::print() {
	gotoxy(pos_x * 2, pos_y);
	if (open) {
		cout << nearMineCount;
	}
	else {
		cout << "¢Ì";
	}
}

bool NormalBlock::isZeroBlock() {
	return nearMineCount == 0;
}

string NormalBlock::getType() {
	return "normal";
}

bool NormalBlock::isPushed() {
	return open;
}

void NormalBlock::addNearMineCount() {
	nearMineCount++;
}
NormalBlock::~NormalBlock() {

}