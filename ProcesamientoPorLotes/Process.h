#ifndef PROCESS_H
#define PROCESS_H

#include <QString>

class Process {
    private:
        QString programmerName;
        QString operation;
        QString result;
        int tiempoMaximoEst;
        int id;
        int TT;
        int TR;
        int TTB;
        int indexTime;
        int TME;
        int tiempoLlegada;
        int tiempoFinalizacion;
        int tiempoDeRespuesta;
        int tiempoServicio;
    public:
        Process() :
            programmerName("")
          , operation("")
          , result("")
          , tiempoMaximoEst(0)
          , id(0)
          , TT(0)
          , TR(0)
          , TTB(0)
          , indexTime(0)
          , TME(0)
          , tiempoLlegada(0)
          , tiempoFinalizacion(0)
          , tiempoDeRespuesta(0)
          , tiempoServicio(0) { }

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

        void setTiempoLlegada(int tiempoLlegada) {
            this->tiempoLlegada = tiempoLlegada;
        }

        void setResult(const QString& result) {
            this->result = result;
        }

        void setTiempoFinalizacion(int tiempoFinalizacion) {
            this->tiempoFinalizacion = tiempoFinalizacion;
        }

        void setTiempoDeRespuesta(int tiempoDeRespuest) {
            this->tiempoDeRespuesta = tiempoDeRespuest;
        }

        void setTiempoServicio(int tiempoServicio) {
            this->tiempoServicio = tiempoServicio;
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

        int getTiempoLlegada() const {
            return tiempoLlegada;
        }

        QString getResult() const {
            return result;
        }

        int getTiempoFinalizacion() const {
            return tiempoFinalizacion;
        }

        int getTiempoDeRespuesta() const {
            return tiempoDeRespuesta;
        }

        int getTiempoServicio() const {
            return tiempoServicio;
        }
};

#endif // PROCESS_H
