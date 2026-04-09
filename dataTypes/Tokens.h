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

struct Bits{
	int n;
	int i;
	int x;
	int b;
	int p;
	int e;

	Bits(){
		n = 1;
		i = 1;
		x = 0;
		b = 0;
		p = 0;
		e = 0;	
	}
	
	void SetN(int val){ n = val; }
	void SetI(int val){ i = val; }
	void SetX(int val){ x = val; }
	void SetB(int val){ b = val; }
	void SetP(int val){ p = val; }
	void SetE(int val){ e = val; }
};

struct Operand{
	bool isLabel;
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
 *******************************************************************************************************************/
class Token{
	private:
		int mAddress;
		int mErrId;
		std::string mSrcStmt;
		EFlag mAddrMode;
		Code* mCode;
		Label* mLabel;
		Bits mFlagBits;
	public:
		Token();
		Token(int, int, std::string, EFlag, Code*, Label*);
		Bits GetFlagBits();
		void SetAddress(int);
		int GetAddress();
		void SetErrId(int);
		int GetErrId();
		void SetAddressingMode(EFlag);
		EFlag GetAddressingMode();
		void SetSrcStmt(std::string);
		std::string GetSrcStmt();
		void SetCodePtr(Code*);
		Code* GetCodePtr();
		void SetLabelPtr(Label*);
		Label* GetLabelPtr();
};

#endif
