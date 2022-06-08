# Формат файла Honeycomb

Заголовок файла описан в статье [формат файла магнитной системы](https://github.com/uyras/partsEngine/wiki/%D0%A4%D0%BE%D1%80%D0%BC%D0%B0%D1%82-%D1%84%D0%B0%D0%B9%D0%BB%D0%B0-%D0%BC%D0%B0%D0%B3%D0%BD%D0%B8%D1%82%D0%BD%D0%BE%D0%B9-%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D1%8B).
Здесь описана только часть спецификации, касающаяся honeycomb структуры.

## Условия работы
* Honeycomb решетка работает только в двумерном пространстве!
* Тип системы должен быть type=honeycomb

## Описание ячеек [cells]
К основному формату дополняется только раздел ячеек [cells].
Описание ячеек через табы (\t), каждая ячейка с новой строки.
В ячейке доступны следующие поля данных:
* x (double) позиция центра ячейки вдоль оси x
* y (double) позиция центра ячейки вдоль оси y
* part 1 (unsigned int)
* ... (ИДы всех 6 частиц ячейки, по часовой стрелке).
* part 6 (unsigned int)

Пример:
```
[cells]
1.1   1.1   0   1   2   3   4   5   6
2.1   2.1   7   8   9   10   2   11   12
```

Следует помнить, что одна частица может принадлежать сразу нескольким ячейкам.    