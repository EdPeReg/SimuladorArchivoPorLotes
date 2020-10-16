#ifndef BATCH_H
#define BATCH_H

#include <QList>
#include <QDebug>

#include "Process.h"

class Batch {
    private:
        int size;
    public:
        QList<Process> processes;

        Batch() : size(0) {  };

        int getSize() const {
            return size;
        }

        QList<Process> getProcesses() const {
            return processes;
        }

        void insertProcess(const Process& process) {
            processes.push_back(process);
            ++size;
        }

        void deleteProcess() {
            processes.pop_front();
            --size;
        }

        void showProccesses() const {
            int i = 1;
            for(const auto& process : processes) {
                qDebug() << "\nProceso: " << i;
                qDebug() << "BATCH: " << process.getNumBatch();
                qDebug() << "Name: " << process.getProgrammerName();
                qDebug() << "Operation: " << process.getOperation();
                qDebug() << "TME: " << process.getTiempoMaximoEst();
                qDebug() << "ESTADO: " << process.getEstado();
                qDebug() << "ID: " << process.getId();
                ++i;
            }
        }

        void setInitialTR() { // NEW
            for(auto& process : processes) {
                int aux = process.getTiempoMaximoEst();
                process.setTR(aux);
            }
        }
};

#endif // BATCH_H
