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
