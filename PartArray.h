/*
* File:   PartArray.h
* Author: uyras
*
* Created on 20 Ноябрь 2012 г., 17:22
*/

#ifndef PARTARRAY_H
#define PARTARRAY_H

#include <vector>
#include <string>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "Part.h"
#include "StateMachine.h"
#include "StateMachineFree.h"
#include "sysloader.h"

using namespace std;
class StateMachine;
class StateMachineFree;
class Part;
class SysLoader;

class PartArray {
    friend SysLoader;
    friend StateMachine;
public:

    PartArray();
    virtual ~PartArray();

    /**
    * Создает пустой массив частиц с подложкой размером X,Y,Z (в нанометрах)
    * @param x
    * @param y
    * @param z
    */
    PartArray(double x, double y, double z);

    /**
    * Создает подложку размером x,y,z и заполняет её случайным набором частиц до заданной плотности
    */
    PartArray(double x, double y, double z, double density);

    /**
    * Создает подложку размером x,y,z и заполняет её указанным количеством частиц
    */
    PartArray(double x, double y, double z, int count);

	void operator=(const PartArray& a);

    virtual PartArray* copy();
    virtual PartArray* beforeCopy(); //Возвращает экземпляр чистой системы частиц
    virtual void afterCopy(PartArray*);

    //получить частицу по уникальному идентификатору
    Part* getById(unsigned id);

    /**
    * Изменяет размер подложки и чистит массив частиц
    */
    void resize(double x, double y, double z);

    void insert(Part* part); //Добавляет частицу на образец
    void insert(const Part part); //Добавляет копию частицы в образец

    /**
    * Бросает частицы на случайное место в пространстве с заданной частотой
    */
    virtual void dropRandom(double maxDestiny);

    /**
     * @brief dropChain Бросаем частицы в виде цепочки. Это состояние и будет Ground State
     * @param distance Расстояние между центрами частиц системы. По умолчанию частицы стоят плотно друг к другу
     */
    virtual void dropChain(double distance = -1.);

    virtual void dropTetrahedron(int x, int y, int z, double R = 1, Part * tmp = 0);

    void shuffleM(); //хаотично развернуть магнитные моменты случайных частиц

    /*рассчитывает плотность массива
    если simple=true, то считать только исходя из количества частиц и объема одной частицы, иначе
    рассчитывать плтность суммируя объемы всех частиц*/
    double destiny(bool simple=true);

    /**
    * Бросает частицы на случайное место в пространстве с заданной частотой
    */
    virtual void dropRandom(int count);

    /**
     * @brief calcM1 считает общий магнитный момент системы
     * @return вектор намагниченности (сумма всех намагниченностей частиц)
     */
    Vect calcM1();
    Vect M();

	/**
	 * @brief calcM2 Считает сумму модулей магнитных моментов
	 * @return
	 */
    Vect calcM2();

	/**
	 * @brief calcM12 считает относительную сумму векторов (вектор / вектор по модулю)
	 * @return возвращает каждый вектор
	 */
	Vect calcM12();

    /**
    * Рассчитать поле взаимодействия для указанной частицы и записывает её в параметр Interaction
    * @param elem ссылка на элемент, над которым производить взаимодействие
    */
    void calcH(Part* elem);

    /**
    * рассчитывает поле взаимодействия для всех частиц в системе (каждой в параметрах прописывает поле взаимодействия)
    */
    void calcH();

    /**
    * Считать поле взаимодействия только для соседних 8 элементов
    * @param elem
    * @return
    */
    Vect calcInteractionNeighb(Part* elem);

    /**
     * @brief E Считает энергию системы. Метод подбирается автоматически исходя из конфигурации и свойств системы
     * @return
     */
    virtual double E();

    /**
    * рассчитывает внутреннюю энергию во всей системе частиц (метод подсчета через магнитные моменты)
	* Возвращает её и записывает в параметр E1 класса
    * H - вектор внешнего поля
    */
    double EComplete(Vect& H) const;
    double EComplete() const;
    /**
    * рассчитывает внутреннюю энергию для определенной частицы
    * @param elem ссылка на элемент, для которого считать энергию
    * @return энергия
    */
    double EComplete(Part *elem) const;
    double ECompleteFast();


    double calcEnergy1FastIncremental(double initEnergy); //state - новое состояние системы
    double calcEnergy1FastIncrementalFirst(); //считает начальную энергию системы и попутно записывает части энергий в параметры частиц
    double eIncrementalTemp; //энергия, нужна для инкремента. Используется только в 2-х функциях выше



    /**
    * выдает все парамеры частиц на экран
    */
    void cout();

    void save_v1(string file, bool showNotifications = false);
    void save_v2(QString file);
    virtual void save(QString file);

    /**
     * @brief savePVPython Сохраняет систему в скрипте ParaView
     * @param file Имя скрипта
     * @param thteta Количество полигонов по горизонтали
     * @param phi Количество полигонов по вертикали
     */
    void savePVPython(string file="data.py", int thteta=8, int phi=8);

    /**
     * @brief savePVPythonAnimation Сохраняет скрипт в виде анимации
     * @param secondSystem
     * @param file
     * @param thteta
     * @param phi
     * @param frames
     */
    void savePVPythonAnimation(PartArray *secondSystem, string file, int thteta=8, int phi=8, int frames=100);

    void load_v1(string file, bool showNotifications = false);
    void load_v2(QString file);
    virtual void load(QString file);

    //чистим массив частиц
    virtual void clear();
    //события для удобной кастомизации процесса очистки
    virtual void beforeClear();
    virtual void afterClear();

    //возвращает количество частиц на объекте
    int count();

    // изменить масштаб образца, не затрагивая радиусы и магнитные моменты
    void scaleSystem(double coff);

    // возвращает вектор(массив) энергий каждой частицы
    std::vector<double> getEVector();

    // возвращает вектор(массив) полей взаимодействия каждой частицы
    std::vector<double> getHVector();

    // возвращает вектор(массив) проекций полей взаимодействия на ось z каждой частицы
    std::vector<double> getHZVector();

    //настройка начальных конфигураций
    //void setAntiferr(); //антиферромсагнетизм (шахматный порядок)
    //void setLines(); //одна линия вверх, одна вниз
    void setMAllUp(); //все вверх
    void setMBruteLines(double segmentSize);
    void setMRandom(); //случайно распределяет ось магнитного момента

    void rotateAllUp(); //поворачивает все векторы вверх
    void rotateAllLines(double segmentSize); //делит весь образец на сегменты и поворачивает один сегмент вверх - второй вниз

    void turnUp(); //повернуть M частиц так, чтобы их угол с осью X был меньше 0.
    void turnRight(); //повернуть M частиц так, чтобы их угол с осью Y был меньше 0.
    void turnToDirection(Vect*); //повернуть все частицы вдоль определенного вектора (сделать так чтобы угол был не более 90гр.)
    void movePosRandomly(double d); //Сдвинуть каждую частицу в случайном направлении на расстояние d;
    void moveMRandomly(double fi); //Сдвинуть вектор магнитного момента каждой частицы на угол fi в случайном направлении



    //просчет массива, возвращает набор энергий на каждом шаге
    //	std::vector<double> processStepXYZ(); //перебор в порядке объявления (сначала Z потом Y потом X)
    std::vector<double> processStep(); //перебор решетки и переворот всех по очереди
    std::vector<double> processMaxH(); //выбирается максимально неустойчивая частица и переворачивается
    std::vector<double> processGroupMaxH(); //выбирается группа максимально неустойчивых частиц и переворачиваются разом
    std::vector<double> processGroupStep(); //все неустойчивые частицы вращаются разом
    std::vector<double> processFromCenter(int x, int y, int z); //обработка из центра к краям, в параметрах координаты центральной частицы
    std::vector<double> processHEffective(); //переворот только тех частиц, для которых энергия выше среднего



    /**************************************************************
    ******временные функции, in process, deprecated and trash******
    **************************************************************/

	//находит состояние минимума энергии системы и переводит её в это состояние.
    //возвращает энергию этого состояния
    double setToGroundState();

    //находит состояние максимума энергии системы и переводит её в это состояние.
    //возвращает энергию этого состояния
    double setToMaximalState();

    virtual StateMachineFree maximalState();
    virtual StateMachineFree groundState();

	//находит состояние минимума энергии системы и переводит её в это состояние методом монте-карло
    //steps - количество попыток
    bool setToMonteCarloGroundState(const double t = 0, int steps=1000);

	//выполняет 1 шаг монте-карло при t температуре (относительная единица)
	//возвращает true если система изменилась и false если нет
	bool processMonteCarloStep(const double t=0);

	//находит минимум, но только переворачивает срау несколько частиц
	bool setToMonteCarloGroundState2();

	//ищем GS методом PT
    //tmin - минимальная температура системы
    //tmax - максимальная температура системы
	bool setToPTGroundState(int replicas, int totalSteps=1000, double tMin=0, double tMax=3);

    //проверяем сколько частиц стоят по полю, а сколько против поля
    void checkFM(string file, double c);

    //сохраняет намагниченность каждой частицы
    void saveEachMagnetization(string file);

	//рассчитываем обменный интеграл системы
    double calcJ();

	//сумма модулей обменных интегралов
	double calcJ2();

    //рассчитываем относительный обменный интеграл (отношение |J| к J)
	double calcJ12();

	void dropAdaptive(int cout);//тестовый метод упорядоивания во время наброса

    /************************
     * Глобальные переменные *
     ***********************/

     //номер состояния, нужен для алгоритма полного обхода состояний
    StateMachine* state; //Указан как ссылка потому что в классах используется перекрестная инициализация

    //сам массив частиц, над которым проводятся исследования
    std::vector < Part* > parts;
    Vect size; //размер плоскости в относительных единицах
    Vect absSize;  //размер плоскости в абсолютных единицах

    void _construct(); //стандартный конструктор.

    virtual QString type();

protected:
    double eMin,eMax,eInit,eTemp;
    StateMachineFree *minstate,*maxstate;
    unsigned int lastId;
    bool _double_equals(double a, double b); //сравнение double
    virtual void subTetrahedron(Part * tmp, double x, double y, double z, double vect=1, double rot=0, double r=1);
    void changeState(); //вызывается когда конфигурация системы изменилась (но не поменялась геометрия)
    void changeSystem(); //вызывается когда изменилась вся система целиком

    QString _type;
};
#endif // PARTARRAY_H
