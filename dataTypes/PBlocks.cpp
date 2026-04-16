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

void PBlocks::SetCurrentBlock(const std::string& s){
    auto it = mBlocks.find(s);
    if (it == mBlocks.end()) {
        mBlocks[s] = BlockData();
        mBlockOrder.push_back(s);
        pCurrBlock = &mBlocks[s];
        pCurrBlock->SetCtr(0);
    }else{ pCurrBlock = &it->second;} 
}

void PBlocks::SetCurrentBlock(const std::string& s, int x){
    auto it = mBlocks.find(s);
    if (it == mBlocks.end()) {
        mBlocks[s] = BlockData();
        mBlockOrder.push_back(s);
        pCurrBlock = &mBlocks[s];
        pCurrBlock->SetCtr(x);
    }else{ pCurrBlock = &it->second;} 
}

void PBlocks::FinalizeBlocks(){
    int currentAddr = 0;

    for (const auto& name : mBlockOrder) {
        BlockData& block = mBlocks[name];

        block.SetStartAddr(currentAddr);
        block.SetLength(block.GetCtr());

        currentAddr += block.GetCtr();
    }
}