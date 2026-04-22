/************************************************
* PBlocks.h					*
*						*
* Hector Anaya 826734851       			*
* Emiliano Nolasco 130310255        	        *
* Ethan Fudge 827570933         		*
*						*
* CS530, Spring 2026				*
* Assignment #2, SICXE Assembler		*
*************************************************/
#ifndef PBLOCKS_H
#define PBLOCKS_H

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <string>
#include "BData.h"

/************************************************************************
 * Class to maintain an organized list of the current program blocks found
 * in the .sic file and their BData objects
 * 
 * Member variables:
 * 	mBlocks: A dictionary to provide lookup via names for BData objects
 *	mBlockOrder: A vector to maintain order and indexing of BData names
 *	pCurrBlock: Pointer to keep track of the current program block's
 *                  Bdata object
 ************************************************************************/
class PBlocks{
	private:
        static std::unordered_map<std::string, BlockData> mBlocks;
        static std::vector<std::string> mBlockOrder;
        static BlockData* pCurrBlock;
		
	public:
        
        static BlockData* GetDataPtr();                         // returns pointer to current block
        static void InsertBlockData(const std::string s);       // adds new block, throws on duplicate

        static BlockData* GetBlock(int n);                      // returns block at index n
        static BlockData* GetLastBlock();                       // returns last inserted block

        static int GetProgramLength();                          // total program length in bytes

        static void SetCurrentBlock(const std::string& s);        // sets current block, counter defaults to 0
        static void SetCurrentBlock(const std::string& s, int x); // sets current block, counter initialized to x

        static void ClearBlocks();                                 // resets all block data
        static void FinalizeBlocks();                              // assigns final start addresses and lengths to all blocks
};

#endif
