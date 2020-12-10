#ifndef PAGINA_H
#define PAGINA_H

#include "Process.h"

class Pagina
{
    public:
        Pagina();

        bool isFull() const;

        void setProcess(const Process& process);
        void setSizePagina(short sizePagina);

    private:
        Process process;
        short sizePagina;
};

#endif // PAGINA_H
