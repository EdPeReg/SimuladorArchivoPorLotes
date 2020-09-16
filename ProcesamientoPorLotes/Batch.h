#ifndef BATCH_H
#define BATCH_H

#include <QVector>
#include <QDebug>

#include "Process.h"

class Batch {
    private:
        int size = 0;
        QVector<Process*> processes;
    public:
        Batch() {  };

        ~Batch() {
            for(auto it = processes.begin(); it != processes.end(); ++it) {
                delete (*it);
            }
        }

        int getSize() const {
            return size;
        }

        void setSize(int size) {
            this->size = size;
        }

        QVector<Process*> getProcesses() const {
            return processes;
        }

        void insertProcess(Process* process) {
            processes.push_back(process);
            ++size;
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

        bool isFull() const {
            return size == 4;
        }
};

#endif // BATCH_H
