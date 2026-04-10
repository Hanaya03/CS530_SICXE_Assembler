#include "Label.h"

Label::Label(){
	mAddress = 0;
	mFlag = 'N';
}

Label::Label(int a, char f){
	mAddress = a;
	mFlag = f;
}

int Label::GetAddress(){
	return mAddress;
}
