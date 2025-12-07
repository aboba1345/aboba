# Документация проекта - Структуры данных CLI

## Обзор

Этот проект реализует команднострочный интерфейс (CLI) для работы с шестью основными типами структур данных:

1. **Array (Массив)** - динамический массив с расширением
2. **ForwardList (Односвязный список)** - список с указателями только на следующий элемент
3. **DoubleList (Двусвязный список)** - список с указателями на предыдущий и следующий элементы
4. **Stack (Стек)** - адаптер над ForwardList, LIFO (Last-In-First-Out)
5. **Queue (Очередь)** - адаптер над ForwardList, FIFO (First-In-First-Out)
6. **BTree (Полное бинарное дерево поиска)** - BST с проверкой полноты

## Архитектура

### Паттерны проектирования

1. **Factory Pattern (Фабрика)**
   - `createStructure(char type)` в `Factory.h/cpp`
   - Создает правильный тип структуры по символу ('M', 'F', 'L', 'S', 'Q', 'T')
   - Используется при загрузке базы данных из файла

2. **Adapter Pattern (Адаптер)**
   - `Stack` и `Queue` реализованы как адаптеры над `ForwardList`
   - Обеспечивают единый интерфейс сериализации/десериализации
   - Переиспользуют проверенную логику `ForwardList`

3. **Database Registry Pattern (Реестр базы данных)**
   - `std::map<std::string, Structure*> database` в `main.cpp`
   - Хранит именованные структуры
   - Позволяет создавать несколько структур каждого типа

4. **Polymorphism (Полиморфизм)**
   - Базовый класс `Structure` с методами `serialize()` и `deserialize()`
   - Все структуры наследуются от `Structure`
   - Единый интерфейс для сохранения/загрузки

### Модель работы: STATELESS

Каждый вызов программы независим и следует циклу:

```
./lab1 --file db.txt --query "COMMAND args"
    ↓
1. LOAD:   Загрузить всю базу данных из файла db.txt
    ↓
2. EXECUTE: Выполнить одну команду
    ↓
3. SAVE:   Переписать файл db.txt с новым состоянием
    ↓
4. EXIT:   Завершиться
```

Преимущества:
- Нет долгоживущих процессов
- Безопасность конкурентного доступа (каждый вызов - новый процесс)
- Простота восстановления после сбоев

## Используемые команды

### Array (M-команды)
- `MCREATE` - создать новый массив (по умолчанию: `default`)
- `MPUSH value` - добавить элемент в конец
- `MPUSHAT value index` - вставить элемент по индексу
- `MGET index` - получить элемент
- `MDEL index` - удалить элемент
- `MSET key index` - установить значение
- `MLEN` - получить длину

### ForwardList (F-команды)
- `FCREATE` - создать новый односвязный список
- `FPUSH value mode` - добавить элемент (mode: 0=front, 1=back, 2=after[0], 3=before[last])
- `FDEL mode` - удалить элемент
- `FSEARCH value` - найти элемент (возвращает TRUE/FALSE)
- `FGET index` - получить элемент
- `FLEN` - получить длину

### DoubleList (L-команды)
- `LCREATE` - создать новый двусвязный список
- `LPUSH value mode` - добавить элемент (mode: 0=head, 1=tail, 2=after[0], 3=before[last])
- `LDEL mode` - удалить элемент (режимы: 0=head, 1=tail, 2=after[0], 3=before[last])
- `LGET index` - получить элемент
- `LLEN` - получить длину

### Stack (S-команды)
- `SCREATE` - создать новый стек
- `SPUSH value` - добавить элемент на вершину
- `SPOP` - удалить элемент с вершины (LIFO)
- `SLEN` - получить размер

### Queue (Q-команды)
- `QCREATE` - создать новую очередь
- `QPUSH value` - добавить элемент в конец
- `QPOP` - удалить элемент с фронта (FIFO)
- `QLEN` - получить размер

### BTree (T-команды)
- `TCREATE` - создать новое дерево
- `TINSERT key` - вставить ключ
- `TSEARCH key` - найти ключ (возвращает TRUE/FALSE)
- `TCHECK` - проверить, является ли дерево полным
- `TDEL key` - удалить ключ
- `TGET mode` - обход дерева (mode: PRE, IN, POST, BFS)
- `TGETNODES key mode` - найти соседа (PREV/NEXT в порядке in-order)

### Общие команды
- `PRINT name` - вывести содержимое структуры

## Формат файла

Файл содержит одну структуру на одну строку:

```
TYPE name count value1 value2 ... valueN
```

Типы:
- `M` - Array
- `F` - ForwardList
- `L` - DoubleList (DFList)
- `S` - Stack
- `Q` - Queue
- `T` - BTree

Пример:
```
M default 4 10 value 20 30
F mylist 3 a b c
S stack1 2 elem1 elem2
```

## Коды ошибок

- `ERROR 10: Unknown command` - неизвестная команда или неправильный синтаксис
- `ERROR 20: Structure not found` - структура с указанным именем не существует
- `ERROR 21: Structure already exists` - структура с таким именем уже создана
- `ERROR 30: Invalid index/argument` - неправильный индекс или параметр
- `ERROR 40: Empty structure` - операция на пустой структуре (POP/DEQUEUE)

## Примеры использования

### Создание и работа с массивом
```bash
./lab1 --file db.txt --query "MCREATE"
./lab1 --file db.txt --query "MPUSH 10"
./lab1 --file db.txt --query "MPUSH 20"
./lab1 --file db.txt --query "MLEN"
./lab1 --file db.txt --query "PRINT default"
```

### Работа со стеком (LIFO)
```bash
./lab1 --file db.txt --query "SCREATE"
./lab1 --file db.txt --query "SPUSH elem1"
./lab1 --file db.txt --query "SPUSH elem2"
./lab1 --file db.txt --query "SPUSH elem3"
./lab1 --file db.txt --query "SPOP"  # Вернет elem3
```

### Работа с очередью (FIFO)
```bash
./lab1 --file db.txt --query "QCREATE"
./lab1 --file db.txt --query "QPUSH first"
./lab1 --file db.txt --query "QPUSH second"
./lab1 --file db.txt --query "QPUSH third"
./lab1 --file db.txt --query "QPOP"  # Вернет first
```

### Работа с деревом
```bash
./lab1 --file db.txt --query "TCREATE"
./lab1 --file db.txt --query "TINSERT 50"
./lab1 --file db.txt --query "TINSERT 30"
./lab1 --file db.txt --query "TINSERT 70"
./lab1 --file db.txt --query "TCHECK"  # Проверить, полное ли дерево
./lab1 --file db.txt --query "TGET IN"  # Обход in-order (отсортированный)
```

## Ключевые особенности реализации

### Автоматическое создание структур
Если команда вызывается без предварительного CREATE, структура создается автоматически:
```bash
./lab1 --file db.txt --query "MPUSH 10"  # Автоматически создаст массив "default"
```

### Поддержка именованных структур
```bash
./lab1 --file db.txt --query "MPUSH myarray 10"  # Использует структуру "myarray"
./lab1 --file db.txt --query "MPUSH 10"          # Использует "default"
```

### Правильность LIFO/FIFO
- **Stack**: pushFrontFL + popFrontFL = LIFO ✓
- **Queue**: pushBackFL + popFrontFL = FIFO ✓

### Полное бинарное дерево
Проверка: `countInnerNodes + 1 == countLeavesNodes`

Пример полного дерева:
```
       50
      /  \
    30    70      ← 3 внутренних узла, 4 листовых узла
   / \   / \      ← 3 + 1 = 4 ✓
  20 40 60 80
```

## Структура файлов

```
├── Structure.h              # Базовый полиморфный класс
├── Array.h/cpp              # Динамический массив
├── ForwardList.h/cpp        # Односвязный список
├── DoubleList.h/cpp         # Двусвязный список
├── Stack.h/cpp              # Стек (адаптер)
├── Queue.h/cpp              # Очередь (адаптер)
├── FullBinaryTree.h/cpp     # Бинарное дерево поиска
├── Factory.h/cpp            # Фабрика для создания структур
├── FileIO.h/cpp             # Сохранение/загрузка в/из файла
├── Print.h                  # Вывод структур (специализированные функции PRINT)
├── main.cpp                 # CLI интерфейс, StructureManager, обработчики команд
└── run_tests.sh             # Тестовый скрипт
```

## Документирование

Весь код документирован в стиле Doxygen (`/** ... */`):

1. **Заголовочные файлы (.h)**: 
   - Описание каждого класса и структуры
   - Документация всех публичных методов
   - `@brief`, `@param`, `@return` теги

2. **Файлы реализации (.cpp)**:
   - Подробные комментарии сложных алгоритмов
   - Объяснение форматов сериализации
   - Комментарии к нетривиальной логике

## Компиляция и запуск

```bash
# Компиляция
clang++ -g -Wall -std=c++17 main.cpp FileIO.cpp Factory.cpp FullBinaryTree.cpp \
    Array.cpp ForwardList.cpp DoubleList.cpp Stack.cpp Queue.cpp -o lab1.exe

# Запуск
./lab1.exe --file db.txt --query "MCREATE"

# Тестирование
bash run_tests.sh
```

---

**Язык документации**: Русский  
**Стандарт C++**: C++17  
**Компилятор**: Clang/GCC
