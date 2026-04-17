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

    public:
        BlockData();
        BlockData(int n);

        void IncrementCtr(int x);
        void SetCtr(int x);
        int GetCtr();
        
        void SetStartAddr(int a);
        int GetStartAddr();

        void SetLength(int l);
        int GetLength();

        void SetBlockNumber(int n);
        int GetBlockNumber();
};
#endif