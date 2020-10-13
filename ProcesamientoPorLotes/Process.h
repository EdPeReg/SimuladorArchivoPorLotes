#ifndef PROCESS_H
#define PROCESS_H

#include <QString>

class Process {
    private:
        QString programmerName;
        QString operation;
        QString estado;
        int tiempoMaximoEst;
        int id;
        int numBatch;
        int TT;
        int TR;
        long result;
    public:
        Process() : programmerName(""), operation(""),
                    estado("SIN ERROR"), tiempoMaximoEst(0), id(0), numBatch(0),
                    TT(0), TR(0) { };

        void setProgrammerName(const QString& programmerName) {
            this->programmerName = programmerName;
        }

        void setOperation(const QString& operation) {
            this->operation = operation;
        }

        void setEstado(const QString& estado) {
            this->estado = estado;
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

        void setTT(int TT) {
            this->TT = TT;
        }

        void setTR(int TR) {
            this->TR = TR;
        }

        void setResult(long result) {
            this->result = result;
        }

        QString getProgrammerName() const {
            return programmerName;
        }

        QString getOperation() const {
            return operation;
        }

        QString getEstado() const {
            return estado;
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

        int getTT() const
        {
            return TT;
        }

        int getTR() const {
            return TR;
        }
        long getResult() const {
            return result;
        }
};

#endif // PROCESS_H
