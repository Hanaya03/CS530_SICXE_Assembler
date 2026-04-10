#ifndef LABEL_H
#define LABEL_H

class Label{
	private:
		int mAddress;
		char mFlag;
	public:
		Label();
		Label(int, char);
	
		int GetAddress();
};

#endif
