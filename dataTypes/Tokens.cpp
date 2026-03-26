#include "Tokens.h"

Token::Token(){
	mAddress = 0;
	mErrId = 0;
	mSrcStmt = "empty";
	mCode = NULL;
	mLabel = NULL;
}

Token::Token(int a, int e, std::string s, Code* c, Label* l){
	mAddress = a;
	mErrId = e;
	mSrcStmt = s;
	mCode = c;
	mLabel = l;
}

int Token::GetAddress(){
	return mAddress;
}

int Token::GetErrId(){
	return mErrId;
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
