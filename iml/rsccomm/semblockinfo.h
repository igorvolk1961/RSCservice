#ifndef SEMBLOCKINFO_H
#define SEMBLOCKINFO_H

class SemBlockInfo{
public:
    char* semBlock;
    int semBlockSize;

    SemBlockInfo(char* semBlock, int semBlockSize){
        this->semBlock = semBlock;
        this->semBlockSize = semBlockSize;
    }
    ~SemBlockInfo(){
        delete[] semBlock;
    }
    static SemBlockInfo* buildSemBlock(int semType, int semCode, const char* value);
};

#endif // SEMBLOCKINFO_H
