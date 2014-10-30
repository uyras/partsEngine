#partsEngine
===========

Движка для управления магнитными частицами

##Подключение библиотеки к новому проекту

Создаем пустой репозиторий на GitHub, при создании пишем что надо создать .gitignore для qt и readme.md
Заходим в папку, в которой лежит PartsEngine
Открываем bash, пишем:
```
git clone https://github.com/user/repo_name.git
```
Создаем в папке файл repo_name.pro с текстом:
```
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -L$$PWD/../PartsEngine -lPartsEngine

INCLUDEPATH += ../PartsEngine/

OTHER_FILES += \
    README.md \
    .gitignore
```
Создаем main.cpp с текстом (начальный вариант):
```
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "config.h"
#include "PartArray.h"
#include <ctime>

using namespace std;

int main(){
    config::Instance()->srand(time(NULL));

    //пишем код эксперимента тут

    cout<<"finish";
        return 0;
}

```
файл .gitignore должен содержать следующие данные:
```
# C++ objects and libs

*.slo
*.lo
*.o
*.a
*.la
*.lai
*.so
*.dll
*.dylib

# Qt-es

/.qmake.cache
/.qmake.stash
*.pro.user
*.pro.user.*
*.moc
moc_*.cpp
qrc_*.cpp
ui_*.h
Makefile*
*-build-*

# QtCreator

*.autosave

#QtCtreator Qml
*.qmlproject.user
*.qmlproject.user.*
```
