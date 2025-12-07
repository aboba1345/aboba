#ifndef FACTORY_H
#define FACTORY_H

#include "Structure.h"

/**
 * @brief Фабрика для полиморфного создания структур данных.
 * 
 * Паттерн Factory Method используется для создания правильного типа структуры
 * на основе символьного кода типа. Это необходимо для:
 *  - Десериализации структур из файла
 *  - Загрузки базы данных с множеством разных типов структур
 *  - Обеспечения единого полиморфного интерфейса создания
 */

/**
 * @brief Создает новую структуру данных указанного типа.
 * 
 * Использует символ типа для определения, какой класс инстанцировать.
 * 
 * Типы:
 *  - 'M': Array (массив) - MCREATE, MPUSH, MGET, ...
 *  - 'F': ForwardList (односвязный список) - FCREATE, FPUSH, FGET, ...
 *  - 'L': DFList (двусвязный список) - LCREATE, LPUSH, LGET, ...
 *  - 'S': Stack (стек, адаптер над ForwardList) - SCREATE, SPUSH, SPOP, ...
 *  - 'Q': Queue (очередь, адаптер над ForwardList) - QCREATE, QPUSH, QPOP, ...
 *  - 'T': BTree (полное бинарное дерево) - TCREATE, TINSERT, TSEARCH, ...
 * 
 * @param type Символ, обозначающий тип структуры ('M', 'F', 'L', 'S', 'Q', 'T')
 * @return Указатель на новую структуру (выделенную в heap, должна быть удалена вызывающей стороной)
 * @throw std::invalid_argument если тип неизвестен
 */
Structure* createStructure(char type);

#endif
