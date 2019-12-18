#pragma once
#include<Windows.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<time.h>

enum IMAGE
{
	IMAGE_START,
	IMAGE_DOG = 0,
	IMAGE_TIGER,
	IMAGE_DUCK,
	IMAGE_ELEPHANT,
	IMAGE_COW,
	IMAGE_HORSE,
	IMAGE_CAT,
	IMAGE_MONKEY,
	IMAGE_FROG,
	IMAGE_CHICKEN,
	IMAGE_BLACK,
	IMAGE_CARDEND = 11,
	IMAGE_END,
};

enum CARD
{
	CARD_OPEN,
	CARD_CLOSE,
	CARD_END,
	CARD_MAX = 20,
	CARD_NONE
};

enum EQUALCHECK
{
	EQUALCHECK_TRUE,
	EQUALCHECK_FALSE,
	EQUALCHECK_NEEDLESS
};