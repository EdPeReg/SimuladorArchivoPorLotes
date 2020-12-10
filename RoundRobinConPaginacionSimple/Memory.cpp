#include "Memory.h"

Memory::Memory() : countProcess(0)
{

}

Memory::~Memory() {
    for(auto& marco : marcos) {
        delete marco;
    }
    marcos.clear();
}

void Memory::setTotalFrames(std::deque<Process> &processes, std::deque<Process>& listos) {
    for(auto& process : processes) {
        process.setEstado("LISTOS");
        ++countProcess;
        short frames = process.getTamano() / NUM_PAGES;
        short remainder = process.getTamano() % NUM_PAGES;

        // Add the complete frame (with 5 pages each frame).
        for(int i = 0; i < frames; ++i) {
            Marco *marco = new Marco;
            marco->setProcess(process);
            marco->setSizePagina(NUM_PAGES);
            marcos.push_back(marco);
        }

        // There is a frame that doesn't complete 5 pages.
        if(remainder != 0) {
            Marco *marco = new Marco;
            marco->setProcess(process);
            marco->setSizePagina(remainder);
            marcos.push_back(marco);
        }

        // Delete the process that can't fit in our frame.
        if(marcos.size() > LIMIT_FRAMES) {
            process.setEstado("");
            --countProcess;
            // Just pop all the frames that were inserted.
            for(int i = 0; i < frames; ++i) {
                marcos.pop_back();
            }

            // Pop the frame that doesn't complete 5 pages.
            if(remainder != 0) {
                marcos.pop_back();
            }
//            break;
        } else {
            listos.push_back(process);
        }
    }
}

void Memory::removeFrames(const Process &process)
{
    size_t i = 0;
    while(i < marcos.size()) {
        if(marcos.at(i)->getProcess().getId() == process.getId()) {
            marcos.erase(marcos.begin() + i);
            i = 0;
        } else {
            ++i;
        }
    }
}

std::deque<Marco *> Memory::getMarcos() const
{
    return marcos;
}

short Memory::getMarcosSize() const
{
    return marcos.size();
}

short Memory::getCountProcess() const
{
    return countProcess;
}

short Memory::getLimitFrames() const
{
    return LIMIT_FRAMES;
}
