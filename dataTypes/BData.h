#ifndef BData_H
#define BData_H

#include <unordered_map>
#include <vector>
#include <string>

/**************************************************************************************
 * Class to track the location counter and address metadata for a single program block
 * 
 * 
 * Member Variables:
 *      locCtr: This Program block's location counter
 *      startAddr: the starting address of this program block
 *      length: The total size of this program block
 *      blockNumber: This program block's assigned value- for ordering/indexing
 *************************************************************************************/
class BlockData{
    private:
        int locCtr;
        int startAddr;
        int length;
        int blockNumber;

    public:
        BlockData();       // defaults: all fields 0
        BlockData(int n);  // initializes blockNumber to n, rest default to 0

        void IncrementCtr(int x);  // adds x to locCtr
        void SetCtr(int x);        // sets locCtr to x
        int GetCtr();
        
        void SetStartAddr(int a);
        int GetStartAddr();

        void SetLength(int l);
        int GetLength();

        void SetBlockNumber(int n);
        int GetBlockNumber();
};
#endif