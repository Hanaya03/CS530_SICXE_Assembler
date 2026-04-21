#include "PBlocks.h"

BlockData* PBlocks::pCurrBlock = nullptr;
std::unordered_map<std::string, BlockData> PBlocks::mBlocks;
std::vector<std::string> PBlocks::mBlockOrder;

BlockData* PBlocks::GetDataPtr(){return pCurrBlock;}

void PBlocks::InsertBlockData(const std::string s){
    if (mBlocks.find(s) != mBlocks.end()) {
        throw std::runtime_error("Duplicate block: " + s);
    }
    mBlocks[s] = BlockData();
    mBlockOrder.push_back(s);
}

BlockData* PBlocks::GetBlock(int n){
	return &mBlocks[mBlockOrder[n]];
}

BlockData* PBlocks::GetLastBlock(){
	return &mBlocks[mBlockOrder.back()];
}

int PBlocks::GetProgramLength(){
	return mBlocks[mBlockOrder.back()].GetStartAddr() +  mBlocks[mBlockOrder.back()].GetLength();
}

void PBlocks::SetCurrentBlock(const std::string& s){
    auto it = mBlocks.find(s);
    // New block: register and initialize counter to 0
    if (it == mBlocks.end()) {
        mBlocks[s] = BlockData(mBlockOrder.size());
        mBlockOrder.push_back(s);
        pCurrBlock = &mBlocks[s];
        pCurrBlock->SetCtr(0);
    }else{ pCurrBlock = &it->second;} 
}

void PBlocks::SetCurrentBlock(const std::string& s, int x){
    auto it = mBlocks.find(s);
    if (it == mBlocks.end()) {
        // New block: register and initialize counter to x
        mBlocks[s] = BlockData(mBlockOrder.size());
        mBlockOrder.push_back(s);
        pCurrBlock = &mBlocks[s];
        pCurrBlock->SetCtr(x);
    }else{ pCurrBlock = &it->second;} 
}

void PBlocks::ClearBlocks(){
	mBlocks.clear();
	mBlockOrder.clear();
}

void PBlocks::FinalizeBlocks(){
    int currentAddr = 0;

    // Walk blocks in insertion order, assigning sequential start addresses
    for (const auto& name : mBlockOrder) {
        BlockData& block = mBlocks[name];

        block.SetStartAddr(currentAddr);
        block.SetLength(block.GetCtr());

        currentAddr += block.GetCtr();
    }
}
