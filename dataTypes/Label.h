/************************************************
* Label.h										*
*												*
* Hector Anaya 826734851        				*
* Emiliano Nolasco 130310255        			*
* Ethan Fudge 827570933         				*
*												*
* CS530, Spring 2026							*
* Assignment #2, SICXE Assembler				*
*************************************************/
#ifndef LABEL_H
#define LABEL_H

#include <string>

/************************************************************************
 * Class to hold necessary data for labels as they're encountered
 *
 * Member variables:
 * 	mAddress: The offset relative to this label's program block.
 *	mFlag: This label's address flag, 'R' for relative and 'A' for absolute.
 *	mBlock: The index of the program block that label belongs to.
 ************************************************************************/
class Label{
	private:
		int mAddress;
		char mFlag;
		int mBlock;
	public:
        Label();                        // defaults: address=0, flag='N'
        Label(int, char);               // address and flag, no block
        Label(int a, char f, int n);    // address, flag, and block index

        int GetAddress() const;  		// returns mAddress
        int GetBlock() const;   		// returns mBlock
        char GetFlag() const;   		// returns mFlag
};

#endif
