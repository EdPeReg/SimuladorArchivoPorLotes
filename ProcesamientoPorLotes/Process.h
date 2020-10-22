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
        int TT;
        int TR;
        int TTB;
        int indexTime;
        int TME;
        long result;
    public:
        Process() : programmerName(""), operation(""),
                    estado("SIN ERROR"), tiempoMaximoEst(0), id(0),
                    TT(0), TR(0), TTB(0), indexTime(0), TME(0) { };

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

        void setTT(int& TT) {
            this->TT = TT;
        }

        void setTR(int& TR) {
            this->TR = TR;
        }

        void setTTB(int TTB) {
            this->TTB = TTB;
        }

        void setIndexTime(int& indexTime) {
            this->indexTime = indexTime;
        }

        void setTME(int TME) {
            this->TME = TME;
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

        int getTT() const
        {
            return TT;
        }

        int getTR() const {
            return TR;
        }

        int getTTB() const {
            return TTB;
        }

        int getIndexTime() const {
            return indexTime;
        }

        int getTME() const {
            return TME;
        }

        long getResult() const {
            return result;
        }
};

#endif // PROCESS_H
