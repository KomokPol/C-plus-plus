# C++

Решения домашних заданий по дисциплине C++.

## Структура

- `BDZ/` — длинная арифметика, контейнеры и умные указатели
- `EDZ_3/` — алгоритмы STL (`copy`, `remove`, `reverse`, `unique`, …)
- `EDZ_4/` — собственные `Cstring` и `Rational`
- `EDZ_6/` — геометрия фигур
- `EDZ_7/` — отдельные задачи `A.cpp` … `J.cpp`

## Сборка

```bash
g++ -std=c++17 -Wall -Wextra EDZ_7/A.cpp -o main
./main
```

Задания с тестами на Catch (`BDZ/`):

```bash
g++ -std=c++17 -I BDZ BDZ/big_integer_test.cpp BDZ/big_integer.cpp -o test
./test
```
