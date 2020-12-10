#ifndef MARCO_H
#define MARCO_H

#include <deque>

#include "Process.h"

class Marco
{
//    struct Pagina {
//        Process process;
//        short sizePagina;
//    };

public:
    Marco();

    void setProcess(const Process& process);
    void setSizePagina(short sizePagina);

    Process getProcess() const;
    short getSizePage() const;
//    void insertPagina(const Pagina& pagina);
//    void insertProcess(const std::deque<Process>& processes);

private:
    Process process;
    short sizePage;
//    Pagina pagina;
//    const short TAMANO_PAGINA = 5;
};

#endif // MARCO_H
