#include "BData.h"

//defualt values
BlockData::BlockData(){
    locCtr = 0;
    startAddr = 0;
    length = 0;
    blockNumber = 0;
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
