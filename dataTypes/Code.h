/************************************************
* Code.h										*
*												*
* Hector Anaya 826734851        				*
* Emiliano Nolasco 130310255        			*
* Ethan Fudge 827570933         				*
*												*
* CS530, Spring 2026							*
* Assignment #2, SICXE Assembler				*
*************************************************/
#ifndef CODE_H

#define CODE_H

/************************************************************************
 * Class to associate data with an operation code
 *
 * Member variables:
 * 	mHex: The hexadecimal value of the operation stored as an int
 *	mFormat: The instruction format stored as an int
 *	mLength: The size of the instruction - what to add to the locCtr
 *
 ************************************************************************/
class Code{
	private:
		int mHex;
		int mFormat;
		int mLength;
	public:
		Code();					// defaults: hex=0x00, format=0, length=0
        Code(int, int, int);    // hex, format, length
		int GetHex();
		int GetLength();
		int GetFormat();
};

#endif
