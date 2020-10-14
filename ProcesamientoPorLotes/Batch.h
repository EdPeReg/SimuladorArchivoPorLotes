#ifndef BATCH_H
#define BATCH_H

#include <QList>
#include <QDebug>

#include "Process.h"

class Batch {
    private:
        int size;
    public:
        QList<Process*> processes;

        Batch() : size(0) {  };

        ~Batch() { // Not sure about this.
            for(auto it = processes.begin(); it != processes.end(); ++it) {
                delete (*it);
            }
        }

        int getSize() const {
            return size;
        }

        QList<Process*> getProcesses() const {
            return processes;
        }

        void insertProcess(Process* process) {
            processes.push_back(process);
            ++size;
        }

        void deleteProcess(int i) {
            delete processes.at(i);
        }

        void showProccesses() const {
            int i = 1;
            for(auto it = processes.begin(); it != processes.end(); ++it) {
                qDebug() << "\nProceso: " << i;
                qDebug() << "BATCH: " << (*it)->getNumBatch();
                qDebug() << "Name: " << (*it)->getProgrammerName();
                qDebug() << "Operation: " << (*it)->getOperation();
                qDebug() << "TME: " << (*it)->getTiempoMaximoEst();
                qDebug() << "ESTADO: " << (*it)->getEstado();
                qDebug() << "ID: " << (*it)->getId();
                ++i;
            }
        }
};

#endif // BATCH_H
