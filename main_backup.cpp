#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include "Array.h"
#include "ForwardList.h"
#include "DoubleList.h"
#include "Stack.h"
#include "Queue.h"
#include "FullBinaryTree.h"
#include "FileIO.h"
#include "Print.h"

using namespace std;

int safeStoi(const string& str) {
    try {
        return stoi(str);
    } catch (const invalid_argument& e) {
        throw runtime_error("Неверный формат числа: " + str);
    } catch (const out_of_range& e) {
        throw runtime_error("Число вне допустимого диапазона: " + str);
    }
}

class StructureManager {
private:
    string currentFilename;
    string currentStructureType = "none";
    
    // Храним указатели на все структуры
    Array* globalArray = nullptr;
    ForwardList* globalForwardList = nullptr;
    DFList* globalDoubleList = nullptr;
    Stack* globalStack = nullptr;
    Queue* globalQueue = nullptr;
    BTree* globalTree = nullptr;

public:
    ~StructureManager() {
        cleanup();
    }
    void setFilename(const string& filename) {
        currentFilename = filename;
    }

    string getFilename() const {
        return currentFilename;
    }

    void cleanup() {
        // Array: нужно освободить массив перед удалением структуры
        if (globalArray) {
            if (globalArray->head) {
                delete[] globalArray->head;
            }
            delete globalArray;
            globalArray = nullptr;
        }
        
        // ForwardList: deleteFL удаляет все узлы и саму структуру
        if (globalForwardList) {
            deleteFL(globalForwardList);
            globalForwardList = nullptr;
        }
        
        // DoubleList: deleteListDFList удаляет все узлы и саму структуру
        if (globalDoubleList) {
            deleteListDFList(globalDoubleList);
            globalDoubleList = nullptr;
        }
        
        // Stack: нужно очистить узлы перед удалением структуры
        if (globalStack) {
            clearStack(globalStack);
            delete globalStack;
            globalStack = nullptr;
        }
        
        // Queue: deleteQueue удаляет массив данных и саму структуру
        if (globalQueue) {
            deleteQueue(globalQueue);
            globalQueue = nullptr;
        }
        
        // BTree: нужно рекурсивно удалить все узлы
        if (globalTree) {
            // Рекурсивное удаление узлов дерева
            function<void(BNode*)> deleteNode = [&](BNode* node) {
                if (node) {
                    deleteNode(node->left);
                    deleteNode(node->right);
                    delete node;
                }
            };
            deleteNode(globalTree->root);
            delete globalTree;
            globalTree = nullptr;
        }
        
        currentStructureType = "none";
    }

    void saveCurrentStructure() {
        if (currentFilename.empty()) return;
        
            if (currentStructureType == "Array" && globalArray) {
                saveArrayToFile(currentFilename, *globalArray);
            } 
            else if (currentStructureType == "ForwardList" && globalForwardList) {
                saveForwardListToFile(currentFilename, *globalForwardList);
            } 
            else if (currentStructureType == "DoubleList" && globalDoubleList) {
                saveDoubleListToFile(currentFilename, *globalDoubleList);
            } 
            else if (currentStructureType == "Stack" && globalStack) {
                saveStackToFile(currentFilename, *globalStack);
            } 
            else if (currentStructureType == "Queue" && globalQueue) {
                saveQueueToFile(currentFilename, *globalQueue);
            } 
            else if (currentStructureType == "FBTree" && globalTree) {
                saveTreeToFile(currentFilename, *globalTree);
            }
    }

    void handleMCommand(const vector<string>& tokens) {
        try {
            if (currentStructureType != "none" && currentStructureType != "Array") {
                cerr << "ERROR 30: Invalid index/argument" << endl;
                exit(1);
            }
            currentStructureType = "Array";
            
            if (tokens[0] == "MCREATE") {
                if (!globalArray) {
                    globalArray = new Array;
                    createArray(globalArray, 10);
                }
                return;
            }
            
            // Остальные команды требуют инициализированного массива
            if (!globalArray) {
                globalArray = new Array;
                createArray(globalArray, 10);
            }
            
            if (tokens[0] == "MPUSH" && tokens.size() >= 2) {
                string value = tokens[1];
                addElementEndArray(globalArray, value);
            }
            else if (tokens[0] == "MPUSHAT" && tokens.size() >= 3) {
                string value = tokens[1];
                std::size_t index = static_cast<std::size_t>(safeStoi(tokens[2]));
                addElementIndexArray(globalArray, value, index);
            }
            else if (tokens[0] == "MGET" && tokens.size() >= 2) {
                std::size_t index = static_cast<std::size_t>(safeStoi(tokens[1]));
                cout << getElementArray(globalArray, index) << endl;
            }
            else if (tokens[0] == "MDEL" && tokens.size() >= 2) {
                std::size_t index = static_cast<std::size_t>(safeStoi(tokens[1]));
                deleteElementArray(globalArray, index);
            }
            else if (tokens[0] == "MSET" && tokens.size() >= 3) {
                std::size_t index = static_cast<std::size_t>(safeStoi(tokens[2]));
                string value = tokens[1];
                setKeyArray(globalArray, value, index);
            }
            else if (tokens[0] == "MLEN") {
                cout << getArrayLength(globalArray) << endl;
            }
            else {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        } catch (const exception& e) {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }

    void handleFCommand(const vector<string>& tokens) {
        try {
            if (currentStructureType != "none" && currentStructureType != "ForwardList") {
                cerr << "ERROR 30: Invalid index/argument" << endl;
                exit(1);
            }
            
            currentStructureType = "ForwardList";
            
            if (tokens[0] == "FCREATE") {
                if (!globalForwardList) {
                    globalForwardList = createFL();
                }
                return;
            }
            
            // Остальные команды требуют инициализированного списка
            if (!globalForwardList) {
                globalForwardList = createFL();
            }
            
            if (tokens[0] == "FPUSH" && tokens.size() >= 3) {
                string value = tokens[1];
                int mode = safeStoi(tokens[2]);
                
                if (mode == 0) {
                    // Head: вставка в начало
                    pushFrontFL(globalForwardList, value);
                } else if (mode == 1) {
                    // Tail: вставка в конец
                    pushBackFL(globalForwardList, value);
                } else if (mode == 2) {
                    // After Head: вставка после head
                    if (globalForwardList->head != nullptr) {
                        insertAfterFL(globalForwardList, value, 0);
                    } else {
                        pushFrontFL(globalForwardList, value);
                    }
                } else if (mode == 3) {
                    // Before Tail: вставка перед tail
                    // Вставляем перед последним элементом (индекс len-1)
                    if (globalForwardList->head != nullptr) {
                        int len = 0;
                        FNode* cur = globalForwardList->head;
                        while (cur != nullptr) {
                            len++;
                            cur = cur->next;
                        }
                        if (len > 0) {
                            insertBeforeFL(globalForwardList, value, len - 1);
                        } else {
                            pushFrontFL(globalForwardList, value);
                        }
                    } else {
                        pushFrontFL(globalForwardList, value);
                    }
                } else {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "FDEL" && tokens.size() >= 2) {
                int mode = safeStoi(tokens[1]);
                
                if (mode == 0) {
                    // Head: удалить первый
                    popFrontFL(globalForwardList);
                } else if (mode == 1) {
                    // Tail: удалить последний
                    // Требует обхода списка
                    if (globalForwardList->head != nullptr) {
                        if (globalForwardList->head->next == nullptr) {
                            FNode* temp = globalForwardList->head;
                            globalForwardList->head = nullptr;
                            globalForwardList->tail = nullptr;
                            delete temp;
                        } else {
                            FNode* cur = globalForwardList->head;
                            while (cur->next->next != nullptr) {
                                cur = cur->next;
                            }
                            delete cur->next;
                            cur->next = nullptr;
                            globalForwardList->tail = cur;
                        }
                    }
                } else if (mode == 2) {
                    // After Head: удалить второй элемент
                    if (globalForwardList->head != nullptr && globalForwardList->head->next != nullptr) {
                        FNode* temp = globalForwardList->head->next;
                        globalForwardList->head->next = temp->next;
                        if (temp == globalForwardList->tail) {
                            globalForwardList->tail = globalForwardList->head;
                        }
                        delete temp;
                    }
                } else if (mode == 3) {
                    // Before Tail: удалить предпоследний элемент
                    if (globalForwardList->head != nullptr && globalForwardList->head->next != nullptr) {
                        if (globalForwardList->head->next->next == nullptr) {
                            // Второй элемент это tail
                            delete globalForwardList->head->next;
                            globalForwardList->head->next = nullptr;
                            globalForwardList->tail = globalForwardList->head;
                        } else {
                            FNode* cur = globalForwardList->head;
                            while (cur->next->next->next != nullptr) {
                                cur = cur->next;
                            }
                            delete cur->next->next;
                            cur->next->next = nullptr;
                            globalForwardList->tail = cur->next;
                        }
                    }
                } else {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "FDELVAL" && tokens.size() >= 2) {
                string value = tokens[1];
                if (!removeByValueFL(globalForwardList, value)) {
                    cerr << "ERROR 20: Structure not found" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "FSEARCH" && tokens.size() >= 2) {
                string value = tokens[1];
                FNode* result = findByValueFL(globalForwardList, value);
                bool found = (result != nullptr);
                cout << (found ? "TRUE" : "FALSE") << endl;
            }
            else if (tokens[0] == "FGET" && tokens.size() >= 2) {
                int index = safeStoi(tokens[1]);
                cout << getAtFL(globalForwardList, index) << endl;
            }
            else {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        } catch (const exception& e) {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }

    void handleLCommand(const vector<string>& tokens) {
        if (currentStructureType != "none" && currentStructureType != "DoubleList") {
            cout << "Неправильный тип, текущий: " << currentStructureType << endl;
            return;
        }
        
        currentStructureType = "DoubleList";
        if (tokens.size() <= 1 && (tokens[0] != "LCREATE" && tokens[0] != "LSIZE")){
            cout << "Ошибка ввода. Проверьте вводимые значения!" << endl;
        }
        if (tokens[0] == "LCREATE" && tokens.size() >= 1) {
            globalDoubleList = createDFList();
            saveCurrentStructure();
        }
        else if (tokens[0] == "LPUSHBACK" && tokens.size() >= 2) {
            string value = tokens[1];
            
            if (!globalDoubleList) {
                globalDoubleList = new DFList;
            }
            addNodeTailDFList(globalDoubleList, value);
            saveCurrentStructure();
        }
        else if (tokens[0] == "LPUSHHEAD" && tokens.size() >= 2) {
            string value = tokens[1];
            
            if (!globalDoubleList) {
                globalDoubleList = new DFList;
            }
            addNodeHeadDFList(globalDoubleList, value);
            saveCurrentStructure();
        }
        else if (tokens[0] == "LGET" && tokens.size() >= 2) {
            int index = safeStoi(tokens[1]);
            
            if (globalDoubleList) {
                cout << getElementDFList(globalDoubleList, index) << endl;
            } else {
                cout << "Двусторонний список не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "LDEL" && tokens.size() >= 2) {
            string value = tokens[1];
            
            if (globalDoubleList) {
                try {
                    deleteNodeByValueDFList(globalDoubleList, value);
                    saveCurrentStructure();
                } catch (const exception& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
            } else {
                cout << "Двусторонний список не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "LINSERTAFTER" && tokens.size() >= 3) {
            string value = tokens[1];
            int index = safeStoi(tokens[2]);
            
            if (globalDoubleList) {
                addNodeAfterDFList(globalDoubleList, value, index);
                saveCurrentStructure();
            } else {
                cout << "Двусторонний список не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "LINSERTBEFORE" && tokens.size() >= 3) {
            string value = tokens[1];
            int index = safeStoi(tokens[2]);
            
            if (globalDoubleList) {
                addNodeBeforeDFList(globalDoubleList, value, index);
                saveCurrentStructure();
            } else {
                cout << "Двусторонний список не инициализирован" << endl;
            }
        }
        // В методе handleLCommand добавьте:
    else if (tokens[0] == "LDELBEFORE" && tokens.size() >= 2) {
        int index = safeStoi(tokens[1]);
    
        if (globalDoubleList) {
            try {
                deleteNodesBeforeIndex(globalDoubleList, index);
                saveCurrentStructure();
                cout << "Удалены узлы до индекса " << index << endl;
            } catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
        } else {
            cout << "Двусторонний список не инициализирован" << endl;
        }
    }
    else if (tokens[0] == "LDELAFTER" && tokens.size() >= 2) {
        int index = safeStoi(tokens[1]);
    
        if (globalDoubleList) {
            try {
                deleteNodesAfterIndex(globalDoubleList, index);
                saveCurrentStructure();
                cout << "Удалены узлы после индекса " << index << endl;
            } catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
        } else {
            cout << "Двусторонний список не инициализирован" << endl;
        }
    }
    else if (tokens[0] == "LDELHEAD") {
        if (globalDoubleList) {
            try {
                string deletedValue = deleteNodeHeadDFList(globalDoubleList);
                saveCurrentStructure();
                cout << "Удалена голова списка со значением: " << deletedValue << endl;
            } catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
        } else {
            cout << "Двусторонний список не инициализирован" << endl;
        }
    }
    else if (tokens[0] == "LDELFROMLIST" && tokens.size() >= 3) {
        int start = safeStoi(tokens[1]);
        int end = safeStoi(tokens[2]);
    
        if (globalDoubleList) {
            try {
                deleteNodesFromTo(globalDoubleList, start, end);
                saveCurrentStructure();
                cout << "Удалены узлы с " << start << " по " << end << endl;
            } catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
        } else {
            cout << "Двусторонний список не инициализирован" << endl;
        }
    }
    else if (tokens[0] == "LCLEAR") {
        if (globalDoubleList) {
            clearDFList(globalDoubleList);
            saveCurrentStructure();
            cout << "Список очищен" << endl;
        } else {
            cout << "Двусторонний список не инициализирован" << endl;
        }
    }
        else if (tokens[0] == "LSIZE") {
            if (globalDoubleList) {
                cout << getLengthDFList(globalDoubleList) << endl;
            } else {
                cout << "Двусторонний список не инициализирован" << endl;
            }
        } else {
            cout << "Ошибка ввода. Введите --help для просмотра инструкции." << endl;
        }
    }

    void handleSCommand(const vector<string>& tokens) {
        if (currentStructureType != "none" && currentStructureType != "Stack") {
            cout << "Неправильный тип, текущий: " << currentStructureType << endl;
            return;
        }
        
        currentStructureType = "Stack";
        if (tokens[0] == "SPUSH" && tokens.size() >= 2) { 
            string value = tokens[1];
            
            if (!globalStack) {
                globalStack = new Stack;
                initializeStack(globalStack);
            }
            pushStack(globalStack, value);
            saveCurrentStructure();
        }
        else if (tokens[0] == "SPOP") {
            if (globalStack) {
                try {
                    cout << popStack(globalStack) << endl;
                    saveCurrentStructure();
                } catch (const exception& e) {
                    cout << e.what() << endl;
                }
            } else {
                cout << "Стек не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "SPEEK") {
            if (globalStack) {
                try {
                    cout << peekStack(globalStack) << endl;
                } catch (const exception& e) {
                    cout << e.what() << endl;
                }
            } else {
                cout << "Стек не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "SSIZE") {
            if (globalStack) {
                cout << getStackSize(globalStack) << endl;
            } else {
                cout << "Стек не инициализирован" << endl;
            }
        } else {
            cout << "Ошибка ввода. Введите --help для просмотра инструкции." << endl;
        }
    }

    void handleQCommand(const vector<string>& tokens) {
        if (currentStructureType != "none" && currentStructureType != "Queue") {
            cout << "Неправильный тип, текущий: " << currentStructureType << endl;
            return;
        }
        
        currentStructureType = "Queue";
        if (tokens[0] == "QPUSH" && tokens.size() >= 2) {
            string value = tokens[1];
            
            if (!globalQueue) {
                globalQueue = createQueue();
            }
            enqueue(globalQueue, value);
            saveCurrentStructure();
        }
        else if (tokens[0] == "QPOP") {
            if (globalQueue) {
                try {
                    cout << dequeue(globalQueue) << endl;
                    saveCurrentStructure();
                } catch (const exception& e) {
                    cout << e.what() << endl;
                }
            } else {
                cout << "Очередь не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "QFRONT") {
            if (globalQueue) {
                try {
                    cout << frontQueue(globalQueue) << endl;
                } catch (const exception& e) {
                    cout << e.what() << endl;
                }
            } else {
                cout << "Очередь не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "QSIZE") {
            if (globalQueue) {
                cout << getQueueSize(globalQueue) << endl;
            } else {
                cout << "Очередь не инициализирован" << endl;
            }
        } else {
            cout << "Ошибка ввода. Введите --help для просмотра инструкции." << endl;
        }
    }

    void handleTCommand(const vector<string>& tokens) {
        if (currentStructureType != "none" && currentStructureType != "FBTree") {
            cout << "Неправильный тип, текущий: " << currentStructureType << endl;
            return;
        }
        
        currentStructureType = "FBTree";
        if (tokens[0] == "TINSERT" && tokens.size() >= 2) {
            int key = safeStoi(tokens[1]);
            
            if (!globalTree) {
                globalTree = new BTree;
            }
            addNode(globalTree, key);
            saveCurrentStructure();
        }
        else if (tokens[0] == "TDEL" && tokens.size() >= 2) {
            int key = safeStoi(tokens[1]);
            
            if (globalTree) {
                deleteNode(globalTree, key);
                saveCurrentStructure();
            } else {
                cout << "Дерево не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "TFIND" && tokens.size() >= 2) {
            int key = safeStoi(tokens[1]);
            
            if (globalTree) {
                cout << (tGet(globalTree, key)) << endl;
            } else {
                cout << "Дерево не инициализирован" << endl;
            }
        }
        else if (tokens[0] == "TISFULL" && tokens.size() >= 1) {
            
            if (globalTree) {
                cout << (isFullTree(*globalTree) ? "Дерево является полным" : "Дерево не является полным") << endl;
            } else {
                cout << "Дерево не инициализирован" << endl;
            }
        } else {
            cout << "Ошибка ввода. Введите --help для просмотра инструкции." << endl;
        }
    }
    void printCurrentStructure() {
        if (currentStructureType == "Array" && globalArray) {
            PRINT(*globalArray);
        } else if (currentStructureType == "ForwardList" && globalForwardList) {
            PRINT(*globalForwardList);
        } else if (currentStructureType == "DoubleList" && globalDoubleList) {
            PRINT(*globalDoubleList);
        } else if (currentStructureType == "Stack" && globalStack) {
            PRINT(*globalStack);
        } else if (currentStructureType == "Queue" && globalQueue) {
            PRINT(*globalQueue);
        } else if (currentStructureType == "FBTree" && globalTree) {
            PRINT(*globalTree);
        } else {
            cout << "Неизвестная структура" << endl;
        }
    }
    bool loadStructureFromFile(const string& filename) {
        void* structure = nullptr;
        string type;
        
        try {
            structure = ::loadStructureFromFile(filename, type);
            
            // Очищаем текущие структуры перед загрузкой новой
            cleanup();
            
            // Присваиваем новую структуру
            if (type == "Array") {
                globalArray = static_cast<Array*>(structure);
            } else if (type == "ForwardList") {
                globalForwardList = static_cast<ForwardList*>(structure);
            } else if (type == "DoubleList") {
                globalDoubleList = static_cast<DFList*>(structure);
            } else if (type == "Stack") {
                globalStack = static_cast<Stack*>(structure);
            } else if (type == "Queue") {
                globalQueue = static_cast<Queue*>(structure);
            } else if (type == "FBTree") {
                globalTree = static_cast<BTree*>(structure);
            } else {
                // Если неизвестный тип, освобождаем память
                if (structure) {
                    delete static_cast<Array*>(structure);
                }
                throw runtime_error("Неизвестный тип: " + type);
            }
            
            currentStructureType = type;
            currentFilename = filename;
            return true;
        } 
        catch (const exception& e) {
            // Если произошла ошибка после создания structure, освобождаем память
            if (structure && type != "none") {
                if (type == "Array") {
                    Array* arr = static_cast<Array*>(structure);
                    if (arr->head) delete[] arr->head;
                    delete arr;
                } else if (type == "ForwardList") {
                    deleteFL(static_cast<ForwardList*>(structure));
                } else if (type == "DoubleList") {
                    deleteListDFList(static_cast<DFList*>(structure));
                } else if (type == "Stack") {
                    Stack* st = static_cast<Stack*>(structure);
                    clearStack(st);
                    delete st;
                } else if (type == "Queue") {
                    deleteQueue(static_cast<Queue*>(structure));
                } else if (type == "FBTree") {
                    BTree* tree = static_cast<BTree*>(structure);
                    function<void(BNode*)> deleteNode = [&](BNode* node) {
                        if (node) {
                            deleteNode(node->left);
                            deleteNode(node->right);
                            delete node;
                        }
                    };
                    deleteNode(tree->root);
                    delete tree;
                } else {
                    // Неизвестный тип, пытаемся удалить как Array
                    delete static_cast<Array*>(structure);
                }
            }
            cout << e.what() << endl;
            return false;
        }
    }
};


void processQuery(const string& query, StructureManager& manager) {
    stringstream ss(query);
    vector<string> tokens;
    string token;
    
    while (ss >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.empty()) {
        cerr << "ERROR 10: Unknown command" << endl;
        exit(1);
    }
    
    string command = tokens[0];
    
    try {
        if (command[0] == 'M') manager.handleMCommand(tokens);
        else if (command[0] == 'F') manager.handleFCommand(tokens);
        else if (command[0] == 'L') manager.handleLCommand(tokens);
        else if (command[0] == 'S') manager.handleSCommand(tokens);
        else if (command[0] == 'Q') manager.handleQCommand(tokens);
        else if (command[0] == 'T') manager.handleTCommand(tokens);
        else if (command == "PRINT") {
            manager.printCurrentStructure();
        }
        else {
            cerr << "ERROR 10: Unknown command" << endl;
            exit(1);
        }
    } 
    catch (const exception& e) {
        cerr << "ERROR 30: Invalid index/argument" << endl;
        exit(1);
    }
};

void instruction(){
    cout << "Array:\n"
    << "MADD - добавить элемент по индексу. Пример: MADD Putin 5\n"
    << "MPUSH - добавить элемент в конец массива. Пример: MPUSH Putin\n"
    << "MGET - вывести элемент на экран. Пример: MGET 5\n"
    << "MDEL - удалить элемент из массива. Пример: MDEL 5\n"
    << "MCHANGE - заменить элемент в массиве. Пример: MCHANGE Putin4ever 5\n"
    << "MLEN - вывод длины массива\n"
    << "\n"
    << "Forward List:\n"
    << "FCREATE - создать односторонний список.\n"
    << "FPUSHBACK - добавить элемент в конец списка. Пример: FPUSHBACK Gorbachov\n"
    << "FPUSHFRONT - добавить элемент в начало списка. Пример: FPUSHFRONT Lenin\n"
    << "FINSERTAFTER - добавить элемент после ключа. Пример: FINSERTAFTER Hruschov 2\n"
    << "FINSERTBEFORE - добавить элемент перед ключом. Пример: FINSERTBEFORE Stalin 2\n"
    << "FGET - вывести элемент на экран. Пример: FGET 2\n"
    << "FPOPFRONT - удалить элемент из начала списка.\n"
    << "FDEL - удалить элемент по значению. Пример: FDEL ValueToRemove\n"
    << "FFIND - поиск элемента по значению. Пример: FFIND Andropov\n"
    << "\n"
    << "Double Linked List:\n" 
    << "LCREATE - создать двусторонний список.\n"
    << "LPUSHBACK - добавить элемент в конец списка. Пример: LPUSHBACK Gorbachov\n"
    << "LINSERTAFTER - добавить элемент после ключа. Пример: LINSERTAFTER Hruschov 2\n"
    << "LINSERTBEFORE - добавить элемент перед ключом. Пример: LINSERTBEFORE Stalin 2\n"
    << "LPUSHHEAD - добавить голову списка. Пример: LPUSHHEAD USSRHeads \n"
    << "LGET - вывести элемент на экран. Пример: LGET 2\n"
    << "LDEL - удалить элемент по значению. Пример: LDEL ValueToRemove\n"
    << "LSIZE - вывод количества элементов списка\n"
    << "\n"
    << "Stack:\n"
    << "SPUSH - добавление элемента.\n"
    << "SPOP - удаление элемента.\n"
    << "SPEEK - вывод головы стэка.\n"
    << "SSIZE - вывод количества элементов стэка.\n"
    << "\n"
    << "Queue:\n"
    << "QPUSH - добавление элемента.\n"
    << "QPOP - удаление элемента.\n"
    << "QFRONT - вывод головы очереди.\n"
    << "QSIZE - вывод количества элементов очереди.\n"
    << "\n"
    << "Full Binary Tree:\n"
    << "TINSERT - добавление элемента.\n"
    << "TDEL - удаление элемента.\n"
    << "TFIND - поиск элемента в дереве. Пример: TFIND 2\n"
    << "TISFULL - проверка дерева на полноту.\n";
}

int main(int argc, char* argv[]) {
    string filename;
    string query;
    StructureManager manager;
    bool helpRequested = false;
    
    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
            manager.setFilename(filename);
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        } else if (arg == "--help") {
            helpRequested = true;
        }
    }
    
    // STATELESS режим: Load → Execute → Save → Exit
    try {
        // === ИНИЦИАЛИЗАЦИЯ ===
        if (helpRequested) {
            instruction();
            return 0;
        }
        
        // === ДЕСЕРИАЛИЗАЦИЯ ===
        if (!filename.empty()) {
            if (!fileExists(filename)) {
                // Если файл не существует - создаём пустую базу в памяти
                // Структура будет создана при первой команде
            } else {
                // Загружаем существующую структуру
                if (!manager.loadStructureFromFile(filename)) {
                    cerr << "ERROR 10: Unknown command" << endl;
                    return 1;
                }
            }
        }
        
        // === ИСПОЛНЕНИЕ ===
        if (!query.empty()) {
            processQuery(query, manager);
        } else {
            // Если ни query ни help не указаны - ошибка
            cerr << "ERROR 10: Unknown command" << endl;
            return 1;
        }
        
        // === СЕРИАЛИЗАЦИЯ ===
        if (!filename.empty()) {
            manager.saveCurrentStructure();
        }
        
    } catch (const exception& e) {
        cerr << "ERROR 10: Unknown command" << endl;
        return 1;
    }
    
    // === ЗАВЕРШЕНИЕ ===
    return 0;
}
