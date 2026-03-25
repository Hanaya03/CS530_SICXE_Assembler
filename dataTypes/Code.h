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
		Code();
		Code(int, int, int);
		int GetHex();
		int GetLength();
		int GetFormat();
};

#endif
