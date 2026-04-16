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

int Label::GetAddress(){
	return mAddress;
}
