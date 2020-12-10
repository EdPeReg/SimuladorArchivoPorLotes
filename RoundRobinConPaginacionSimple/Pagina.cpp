#include "Pagina.h"

Pagina::Pagina() : sizePagina(0)
{

}

bool Pagina::isFull() const
{
    return sizePagina == 5;
}

void Pagina::setProcess(const Process &process)
{
    this->process = process;
}

void Pagina::setSizePagina(short sizePagina) {
    this->sizePagina = sizePagina;
}
