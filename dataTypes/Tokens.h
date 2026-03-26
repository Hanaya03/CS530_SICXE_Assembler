#ifndef TOKENS_H
#define TOKENS_H

#include "Code.h"
#include "Error.h"
#include "Label.h"

class Token{
	private:
		int mAddress;
		int mErrId;
		std::string mSrcStmt;
		Code* mCode;
		Label* mLabel;
	public:
		Token();
		Token(int, int, std::string, Code*, Label*);
		int GetAddress();
		int GetErrId();
		std::string GetSrcStmt();
		Code* GetCodePtr();
		Label* GetLabelPtr();
};

#endif
