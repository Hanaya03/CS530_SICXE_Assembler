#ifndef CODE_H

#define CODE_H

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
