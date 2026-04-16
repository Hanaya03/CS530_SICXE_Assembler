#ifndef LABEL_H
#define LABEL_H

#include <string>

class Label{
	private:
		int mAddress;
		char mFlag;
		int mBlock;
	public:
		Label();
		Label(int, char);
		Label(int a, char f, int n);

		// void setBlock(int n);
		int GetAddress();
};

#endif
