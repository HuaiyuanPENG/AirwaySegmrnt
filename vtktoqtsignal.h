#ifndef VTKTOQTSIGNAL_H
#define VTKTOQTSIGNAL_H
/*
 * the friend object of vtk object to sent vtk data to qt use qt signal slots methods
 */
#include <QObject>

class  VtkToQtSignal : public QObject
{
    //QT MACRO
    Q_OBJECT
signals:
    //use to sent pixel property
    void SentPiexlPro(int x, int y, int z, float* intensity);
public:
    void Execurte(int x, int y, int z, float *intensity){
        emit SentPiexlPro(x, y, z, intensity);
    }
};


#endif // VTKTOQTSIGNAL_H
