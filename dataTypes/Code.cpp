/************************************************
* Code.cpp										*
*												*
* Hector Anaya 826734851        				*
* Emiliano Nolasco 130310255        			*
* Ethan Fudge 827570933         				*
*												*
* CS530, Spring 2026							*
* Assignment #2, SICXE Assembler				*
*************************************************/
#include "Code.h"

Code::Code(){
	mHex = 0x00;
	mFormat = 0;
	mLength = 0;
}

Code::Code(int h, int f, int l){
	mHex = h;
	mFormat = f;
	mLength = l;
}

int Code::GetHex(){return mHex;}
int Code::GetLength(){return mLength;}
int Code::GetFormat(){return mFormat;}
