#ifndef PARTARRAYMPI_H
#define PARTARRAYMPI_H

#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "PartArray.h"
#include "config.h"
#include "Vect.h"


class PartArrayMPI :
    public PartArray
{
public:


    PartArrayMPI();

    /**
    * Создает пустой массив частиц с подложкой размером X,Y,Z (в нанометрах)
    * @param x
    * @param y
    * @param z
    */
    PartArrayMPI(double x, double y, double z);

    /**
    * Создает подложку размером x,y,z и заполняет её случайным набором частиц до заданной плотности
    */
    PartArrayMPI(double x, double y, double z, double density);

    /**
    * Создает подложку размером x,y,z и заполняет её указанным количеством частиц
    */
    PartArrayMPI(double x, double y, double z, int count);

    PartArrayMPI(char* file);

    void load(char* file); //многопоточная загрузка
    void save(string file, bool showNotifications = false);//многопоточное сохранение
    void save(char* file); //многопоточное сохранение

    // Бросает частицы на плоскость в многопоточном режиме, заполняет плоскость до определенного насыщения
    // Может дополнительно добрасывать частицы на уже набросанную плоскость
    void dropRandomMPI(double maxDestiny, int x=10., int y=10.);

    // удаляет пересекающиеся элементы из швов после dropRandomMPI
    void filterInterMPI();

    void send(int fromThread); //отправлет частицы в другой поток
    void sendBcast(int fromThread); //отправляет все частицы всем потокам, thread - номер потока из которого отправлять
    void recieve(int toThread); //получает частицы из другого потока

    //проверяет, принадлежит ли сектор потоку. startFrom указывает сколько потоков отведено под root нужды.
    //Обычно это один нулевой поток, который не участвует в распределении секторовы
    bool isMySector(int sector, int startFrom=1);

    bool setToGroundState(int thread=0); //переводим систему в GS методом полного перебора. Пространство состояний разбиваем динамически


    /*
    //проверяем сколько частиц стоят по полю, а сколько против поля
    void checkFM2(char* file, double c, double realC);
    void calcEnergyMPI1(); //считает энергию в многопоточном режиме
    */

    Vect sector; //размер одного сектора при разделении плоскости на подзадачи (в MPI)

private:
    //преобразует массив ссылок на частицы в массив частиц
    vector<Part> &transformToParts();
    void transformFromParts(vector<Part> &temp);
};

#endif // PARTARRAY_H
