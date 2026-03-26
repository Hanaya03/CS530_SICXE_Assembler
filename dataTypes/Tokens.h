#ifndef TOKENS_H
#define TOKENS_H

#include "Code.h"
#include "Error.h"
#include "Label.h"

enum EFlag{
	Simple,
	Indirect,
	Immediate
};

/*******************************************************************************************************************
 * Class used to tokenize the lines of the input .sic file
 * 
 * Member variables:
 * 	int mAddress - The address of this token's associated line as calculated by the assembler
 *	int mErrId - The error id associated with this token's line, if any errors were found. 0 means no error
 *	std::string mSrcStmt - The actual text that this token is associated with
 *	EFlag mAddrMode - This line's detected addressing mode
 *	Code* mCode - A pointer to the Code object for this line's operation mnemonic
 *	Label* mLabel - A pointer to the symbol's Label object from the symtab, if it exists
 *
 *********************************************************************************************************************/
class Token{
	private:
		int mAddress;
		int mErrId;
		std::string mSrcStmt;
		EFlag mAddrMode;
		Code* mCode;
		Label* mLabel;
	public:
		Token();
		Token(int, int, std::string, EFlag, Code*, Label*);
		int GetAddress();
		int GetErrId();
		EFlag GetAddressingMode();
		std::string GetSrcStmt();
		Code* GetCodePtr();
		Label* GetLabelPtr();
};

#endif
