#ifndef MEMORY_H
#define MEMORY_H

#include<QDebug>

#include <deque>

#include "Marco.h"

class Memory
{
public:
    Memory();

    void setTotalFrames(std::deque<Process>& processes, std::deque<Process>& listos);
    void removeFrames(const Process& process);
    std::deque<Marco*> getMarcos() const;
    short getMarcosSize() const;
    short getCountProcess() const;
    short getLimitFrames() const;
//    void insertTable(std::deque<Process> listos, std::deque<Process> bloqueados);
    ~Memory();

private:
    std::deque<Marco*> marcos;
    short countProcess;
    const short LIMIT_FRAMES = 38;
    const short NUM_PAGES = 5;
};

#endif // MEMORY_H
