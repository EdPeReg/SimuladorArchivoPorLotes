#ifndef PROCESS_H
#define PROCESS_H

#include <QString>

class Process {
    private:
        QString programmerName;
        QString operation;
        int tiempoMaximoEst;
        int id;
        int numBatch;
        int result;

    public:
        Process() : programmerName(""), operation(""), tiempoMaximoEst(0), id(0), numBatch(0) { };

        void setProgrammerName(const QString& programmerName) {
            this->programmerName = programmerName;
        }

        void setOperation(const QString& operation) {
            this->operation = operation;
        }

        void setTiempoMaximoEst(int tiempoMaximoEst) {
            this->tiempoMaximoEst = tiempoMaximoEst;
        }

        void setId(int id) {
            this->id = id;
        }

        void setNumBatch(int numBatch) {
            this->numBatch = numBatch;
        }

        void setResult(int result) {
            this->result = result;
        }

        QString getProgrammerName() const {
            return programmerName;
        }

        QString getOperation() const {
            return operation;
        }

        int getTiempoMaximoEst() const {
            return tiempoMaximoEst;
        }

        int getId() const {
            return id;
        }

        int getNumBatch() const {
            return numBatch;
        }

        int getResult() const {
            return result;
        }
};

#endif // PROCESS_H
