#include "Tokens.h"

Token::Token(){
	mAddress = 0;
	mErrId = 0;
	mSrcStmt = "empty";
	mCode = NULL;
	mLabel = NULL;
}

Token::Token(int a, int e, std::string s, EFlag f, Code* c, Label* l){
	mAddress = a;
	mErrId = e;
	mSrcStmt = s;
	mAddrMode = f;
	mCode = c;
	mLabel = l;
}

Bits Token::GetFlagBits(){
	return mFlagBits;
}

void Token::SetAddress(int a){
	mAddress = a;
}

int Token::GetAddress(){
	return mAddress;
}

int Token::GetErrId(){
	return mErrId;
}

EFlag Token::GetAddressingMode(){
	return mAddrMode;
}

void Token::SetSrcStmt(std::string s){
	mSrcStmt = s;
}

std::string Token::GetSrcStmt(){
	return mSrcStmt;
}

Code* Token::GetCodePtr(){
	return mCode;
}

Label* Token::GetLabelPtr(){
	return mLabel;
}
