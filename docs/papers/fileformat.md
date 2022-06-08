# Формат файла магнитной системы

## Определение версии файла
Первая версия не имела номера в своем описании. Четвертая строка файла всегда была следующей:

> x\ty\tz\tMx\tMy\tMz\tr

где \t - символ табуляции. Если эта строка присутствует, значит файл первой версии.
# Версия 1
В начале файла идет размер подложки вдоль осей x,y,z. Каждый размер - число double с новой строки.
Далее идет заголовок (всегда одинаковый):

> x\ty\tz\tMx\tMy\tMz\tr

Затем описание частиц. По одной строке на частицу.
Данные для каждой частицы описываются через \t символ (таб) в следующем порядке:
* x (позиция)
* y (позиция)
* z (позиция)
* Mx (позиция)
* My (позиция)
* Mz (позиция)
* width (высота частицы, для овальной формы)
* height (ширина частицы, для овальной формы)
* radius (радиус частицы, для круглой частицы)
* shape (форма, выбор между CIRCLE, ELLIPSE, SQUARE)
Каждое из полей имеет тип double за исключением последнего. Последнее - строка, один из вариантов.
Если система описана в 2-мерном пространстве, описывается третьи параметры указываются нулями.

Пример:
```
10
10
0
x	y	z	Mx	My	Mz	r
0.5	1	0	-1	-0	-0	0	0	0.5	SQUARE
0.5	0	0	1	0	0	0	0	0.5	SQUARE
0	0.5	0	0	1	0	0	0	0.5	SQUARE
1	0.5	0	-0	-1	-0	0	0	0.5	SQUARE
1.5	1	0	1	0	0	0	0	0.5	SQUARE
1.5	0	0	-1	-0	-0	0	0	0.5	SQUARE
```

# Версия 2
## Структура
Файл условно делится на 3 раздела данных:
1. Заголовок [header]
2. Список частиц [parts]
3. Список дополнительных параметров и разделов

## Общие правила оформления разделов
* Число разделов может изменяться от типа системы.
* Первый раздел обязательно [header]
* Второй раздел обязательно [parts]
* Название раздела оформляется прописными латинскими буквами и символами ._-
* Название заключено в квадратные скобки
* Пустые строки должны отсутствовать в документе (даже в конце)

Например:
```
    [external_data]
    10 20 30
    40 50 60
    [external_data2]
    10 20 30
    40 50 60
```

## Заголовок [header]
Заголовок оформляется в виде списка параметров формата <ключ>=<значение>.
Список ключей следующий:
* version (int) - версия формата файла. Целое число, текущее - двойка.
* dimensions (int) - число размерностей используемого пространства. Может быть 1, 2 либо 3.
* type (string) - тип магнитной системы. Выбор производится между поддерживаемыми. Чистая "болванка" имеет тип standart.
* size (int) - число спинов системы
* emin (double) - минимальная энергия системы. 0, если неизвестна
* emax (double) - максимальная энергия системы. 0, если неизвестна
* state (string) - состояние системы, начиная с первого в списке спина. Длина строки должна быть size символов, все символы либо 0 либо 1. Состояние описывает текущую конфигурацию системы, то есть описанные далее магнитные моменты спинов направлены согласно этому состоянию (уже перевернутые). Состояние дублируется при описании частиц, тут приводится только как контрольная сумма.
* minstate (string) - состояние для минимальной энергии, пустая строка если неизвестно
* maxstate (string) - состояние для максимальной энергии, пустая строка если неизвестно
* sizescale (double) - масштаб величин расстояния и размера (для привода к реальным величинам). Величина, на которую уомножается координатная сетка системы при пересчете масштабов в реальных величин. Если не задан, указывать 1.
* magnetizationscale (double) - масштаб единиц намагниченности. Величина, на которую домножаются координаты вектора магнитного момента. Если не задан, указывать 1.
* interactionrange (double) - радиус взаимодействия, 0 - если все со всеми

Все пункты списка обязательны к заполнению и должны соответствовать указанным выше требованиям.
Пример:
```
[header]
version=2
dimensions=2
type=standart
size=10
emin=-15.2
emax=16.1
state=1001110010
minstate=0111001110
maxstate=1000110001
sizescale=2.2e-5
magnetizationscale=2.7822e-13
```

## Список частиц [parts]
Все частицы хранятся в форме списка чисел и строк, разделенных знаками табуляции и символами новой строки. Для одной частицы одна строка данных. Данные в строке задаются в следующем порядке:
* ID (unsigned int) - уникальный идентификатор частицы. Не является порядковым. Нужен только для связи с последующими разделами файла.
* x (double) - позиция центра частицы вдоль оси X
* y (double) - позиция центра частицы вдоль оси Y
* z (double) - позиция центра частицы вдоль оси Z
* Mx (double) - намагниченность частицы (как макроспина) вдоль оси X
* My (double) - намагниченность частицы (как макроспина) вдоль оси Y
* Mz (double) - намагниченность частицы (как макроспина) вдоль оси Z
* state (bool) - состояние частицы. 0 - исходное, 1 - перевернутое.

Пример:
```
[parts]
0	692.82	0	0	0	1	0	1
1	519.615	300	0	-0.86	0.5	0	0
2	173.205	300	0	-0.86	-0.5	0	0
3	0	0	0	0	-1	0	1
4	173.205	-300	0	0.86	-0.5	0	1
5	519.615	-300	0	0.86	0.5	0	1
```

## Дополнительные разделы
Список доп. разделов зависит от функциональности перегруженных классов.
Следующая функциональность уже доступна в библиотеке:
* [Honeycomb структура](https://github.com/uyras/partsEngine/wiki/%D0%A4%D0%BE%D1%80%D0%BC%D0%B0%D1%82-%D1%84%D0%B0%D0%B9%D0%BB%D0%B0-Honeycomb)
* [Squarespinice структура](https://github.com/uyras/partsEngine/wiki/%D0%A4%D0%BE%D1%80%D0%BC%D0%B0%D1%82-%D1%84%D0%B0%D0%B9%D0%BB%D0%B0-Square-Spin-Ice)