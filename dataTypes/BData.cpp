/************************************************
* BData.cpp										*
*												*
* Hector Anaya 826734851        				*
* Emiliano Nolasco 130310255        			*
* Ethan Fudge 827570933         				*
*												*
* CS530, Spring 2026							*
* Assignment #2, SICXE Assembler				*
*************************************************/
#include "BData.h"

//defualt values
BlockData::BlockData(){
    locCtr = 0;
    startAddr = 0;
    length = 0;
    blockNumber = 0;
}

BlockData::BlockData(int n){
    locCtr = 0;
    startAddr = 0;
    length = 0;
    blockNumber = n;
}

void BlockData::IncrementCtr(int x){ locCtr += x;}
void BlockData::SetCtr(int x){ locCtr = x;}
int BlockData::GetCtr(){return locCtr;}

void BlockData::SetStartAddr(int a){startAddr = a;}
int BlockData::GetStartAddr(){return startAddr;}

void BlockData::SetLength(int l){length = l;}
int BlockData::GetLength(){return length;}

void BlockData::SetBlockNumber(int n){blockNumber = n;}
int BlockData::GetBlockNumber(){return blockNumber;}
