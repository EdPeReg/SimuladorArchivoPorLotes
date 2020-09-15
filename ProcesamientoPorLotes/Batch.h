#ifndef BATCH_H
#define BATCH_H

#include <QVector>
#include <QDebug>

#include "Process.h"

class Batch {
    private:
        QVector<Process*> processes;
    public:
        Batch() {  };

        ~Batch() {
            for(auto it = processes.begin(); it != processes.end(); ++it) {
                delete (*it);
            }
        }

        QVector<Process*> getProcesses() const {
            return processes;
        }

        void insertProcess(Process* process) {
            processes.push_back(process);
        }

        void showProccesses() const {
            int i = 1;
            for(auto it = processes.begin(); it != processes.end(); ++it) {
                qDebug() << "Proceso: " << i;
                qDebug() << "BATCH: " << (*it)->getNumBatch();
                qDebug() << "Name: " << (*it)->getProgrammerName();
                qDebug() << "Operation: " << (*it)->getOperation();
                qDebug() << "TME: " << (*it)->getTiempoMaximoEst();
                qDebug() << "ID: " << (*it)->getId();
                ++i;
            }
        }
};

#endif // BATCH_H