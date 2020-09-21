#ifndef BATCH_H
#define BATCH_H

#include <QVector>
#include <QDebug>

#include "Process.h"

class Batch {
    private:
        bool analized;
        int size;
        QVector<Process*> processes;
    public:
        Batch() : analized(false), size(0) {  };

        ~Batch() {
            for(auto it = processes.begin(); it != processes.end(); ++it) {
                delete (*it);
            }
        }

        bool isAnalized() const {
            return analized;
        }

        int getSize() const {
            return size;
        }

        void setIsAnalized(bool analized) {
            this->analized = analized;
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
};

#endif // BATCH_H
