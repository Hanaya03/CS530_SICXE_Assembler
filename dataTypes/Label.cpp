/************************************************
* Label.cpp										*
*												*
* Hector Anaya 826734851        				*
* Emiliano Nolasco 130310255        			*
* Ethan Fudge 827570933         				*
*												*
* CS530, Spring 2026							*
* Assignment #2, SICXE Assembler				*
*************************************************/
#include "Label.h"

Label::Label(){
	mAddress = 0;
	mFlag = 'N';
}

Label::Label(int a, char f){
	mAddress = a;
	mFlag = f;
}

Label::Label(int a, char f, int n){
	mAddress = a;
	mFlag = f;
	mBlock = n;
}

int Label::GetAddress() const{
	return mAddress;
}

int Label::GetBlock() const{
	return mBlock;
}

char Label::GetFlag() const{
	return mFlag;
}
