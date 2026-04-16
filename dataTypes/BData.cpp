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

void BlockData::InsertLabel(const std::string& s){
    if (mSymTab.find(s) != mSymTab.end()) {
        throw std::runtime_error("Duplicate label: " + s);
    }
    mSymTab[s] = locCtr;
}

int BlockData::GetLabelVal(const std::string& s){
    auto it = mSymTab.find(s);
    if (it == mSymTab.end()) {
        throw std::runtime_error("Undefined label: " + s);
    }
    return it->second;
}