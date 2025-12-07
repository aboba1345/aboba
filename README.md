# CLI Data Structures

**Автор:** Курсовой проект  
**Вариант:** 4 (Full Binary Tree)  
**Язык:** C++17

## Сборка

```bash
g++ -std=c++17 -Wall -Wextra -O2 *.cpp -o lab1
```

## Запуск

```bash
./lab1 --file <path> --query '<COMMAND> <NAME> [ARGS...]'
```

## Пример входных данных

```
M arr1 3 10 20 30
F list1 2 40 50
T tree1 3 50 30 70
```

## Структуры

- **M** - Array (массив)
- **F** - ForwardList (односвязный список)  
- **L** - DoubleList (двусвязный список)
- **S** - Stack (стек)
- **Q** - Queue (очередь)
- **T** - BTree (полное бинарное дерево)