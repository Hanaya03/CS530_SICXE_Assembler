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
