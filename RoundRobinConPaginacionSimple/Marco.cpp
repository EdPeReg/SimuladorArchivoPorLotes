#include "Marco.h"

#include <QDebug>

Marco::Marco() : sizePage(0)
{

}

void Marco::setProcess(const Process &process)
{
    this->process = process;
}

void Marco::setSizePagina(short sizePage) {
    this->sizePage = sizePage;
}

Process Marco::getProcess() const
{
    return process;
}

short Marco::getSizePage() const {
    return sizePage;
}

//void Marco::insertPagina(const Pagina &pagina)
//{
//    if(!pagina.isFull()) {
//        pagina.setProcess(process);
//        pagina.setSizePagina(5);
//    }
//}

//void Marco::insertProcess(const std::deque<Process> &processes)
//{
//    for(const auto& process : processes) {
//        short numPages = process.getTamano() / TAMANO_PAGINA;
//        short remainder = process.getTamano() % TAMANO_PAGINA;

//        while(numPages) {
//            Pagina pagina;
//            pagina.process = process;
//            if(remainder != 0) {
//                pagina.sizePagina = numPages + 1;
//            }
//            --numPages;
//            qDebug() << "proecess id: " << pagina.process.getId();
//            qDebug() << "size page: " << pagina.sizePagina;
//            insertPagina(pagina);
//        }
//    }

//}
