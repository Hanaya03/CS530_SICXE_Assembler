#ifndef PBLOCKS_H
#define PBLOCKS_H

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <string>
#include "BData.h"

class PBlocks{
	private:
        static std::unordered_map<std::string, BlockData> mBlocks;
        static std::vector<std::string> mBlockOrder;
        static BlockData* pCurrBlock;
		
	public:
        /***********************************************************************
         * Return a pointer to the BlockData object specified by pCurrBlock
         * 
         * input:
         *      none
         * 
         * output:
         *      BlockData* - The pointer to the BData object specified by pCurrBlock
         ************************************************************************/
        static BlockData* GetDataPtr();

        /***********************************************************************
         * Add an entry to the mBlocks member variable
         * 
         * input:
         *      std::string s - The name of the new program block to add.
         * 
         * output:
         *      none
         ************************************************************************/
        static void InsertBlockData(const std::string s);

        static BlockData* GetBlock(int n);
        static BlockData* GetLastBlock();

	static int GetProgramLength();

        static void SetCurrentBlock(const std::string& s);
        static void SetCurrentBlock(const std::string& s, int x);

	static void ClearBlocks();
        static void FinalizeBlocks();
};

#endif
