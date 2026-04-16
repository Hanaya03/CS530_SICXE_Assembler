#ifndef BData_H
#define BData_H

#include <unordered_map>
#include <vector>
#include <string>

class BlockData{
    private:
        int locCtr;
        int startAddr;
        int length;
        int blockNumber;

        std::unordered_map<std::string, int> mSymTab;
    public:
        BlockData();

        void IncrementCtr(int x);
        void SetCtr(int x);
        int GetCtr();
        
        void SetStartAddr(int a);
        int GetStartAddr();

        void SetLength(int l);
        int GetLength();

        void SetBlockNumber(int n);
        int GetBlockNumber();

        void InsertLabel(const std::string& s);
        int GetLabelVal(const std::string& s);
};
#endif