#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <cstdlib>
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
    } catch (...) {
        cerr << "ERROR 30: Invalid index/argument" << endl;
        exit(1);
    }
}

class StructureManager {
private:
    string currentFilename;
    string currentStructureType = "none";
    
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
        if (globalArray) {
            if (globalArray->head) delete[] globalArray->head;
            delete globalArray;
            globalArray = nullptr;
        }
        
        if (globalForwardList) {
            deleteFL(globalForwardList);
            globalForwardList = nullptr;
        }
        
        if (globalDoubleList) {
            deleteListDFList(globalDoubleList);
            globalDoubleList = nullptr;
        }
        
        if (globalStack) {
            clearStack(globalStack);
            delete globalStack;
            globalStack = nullptr;
        }
        
        if (globalQueue) {
            deleteQueue(globalQueue);
            globalQueue = nullptr;
        }
        
        if (globalTree) {
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
        
        try {
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
        } catch (...) {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
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
        } catch (...) {
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
            
            if (!globalForwardList) {
                globalForwardList = createFL();
            }
            
            if (tokens[0] == "FPUSH" && tokens.size() >= 3) {
                string value = tokens[1];
                int mode = safeStoi(tokens[2]);
                
                if (mode == 0) {
                    pushFrontFL(globalForwardList, value);
                } else if (mode == 1) {
                    pushBackFL(globalForwardList, value);
                } else if (mode == 2) {
                    if (globalForwardList->head != nullptr) {
                        insertAfterFL(globalForwardList, value, 0);
                    } else {
                        pushFrontFL(globalForwardList, value);
                    }
                } else if (mode == 3) {
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
                    popFrontFL(globalForwardList);
                } else if (mode == 1) {
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
                    if (globalForwardList->head != nullptr && globalForwardList->head->next != nullptr) {
                        FNode* temp = globalForwardList->head->next;
                        globalForwardList->head->next = temp->next;
                        if (temp == globalForwardList->tail) {
                            globalForwardList->tail = globalForwardList->head;
                        }
                        delete temp;
                    }
                } else if (mode == 3) {
                    if (globalForwardList->head != nullptr && globalForwardList->head->next != nullptr) {
                        if (globalForwardList->head->next->next == nullptr) {
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
        } catch (...) {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }

    void handleLCommand(const vector<string>& tokens) {
        try {
            if (currentStructureType != "none" && currentStructureType != "DoubleList") {
                cerr << "ERROR 30: Invalid index/argument" << endl;
                exit(1);
            }
            
            currentStructureType = "DoubleList";
            
            if (tokens[0] == "LCREATE") {
                if (!globalDoubleList) {
                    globalDoubleList = createDFList();
                }
                return;
            }
            
            if (!globalDoubleList) {
                globalDoubleList = createDFList();
            }
            
            if (tokens[0] == "LPUSH" && tokens.size() >= 3) {
                string value = tokens[1];
                int mode = safeStoi(tokens[2]);
                
                if (mode == 0) {
                    addNodeHeadDFList(globalDoubleList, value);
                } else if (mode == 1) {
                    addNodeTailDFList(globalDoubleList, value);
                } else if (mode == 2) {
                    if (globalDoubleList->head != nullptr) {
                        addNodeAfterDFList(globalDoubleList, value, 0);
                    } else {
                        addNodeHeadDFList(globalDoubleList, value);
                    }
                } else if (mode == 3) {
                    if (globalDoubleList->head != nullptr) {
                        int len = 0;
                        DFNode* cur = globalDoubleList->head;
                        while (cur != nullptr) {
                            len++;
                            cur = cur->next;
                        }
                        if (len > 0) {
                            addNodeBeforeDFList(globalDoubleList, value, len - 1);
                        } else {
                            addNodeHeadDFList(globalDoubleList, value);
                        }
                    } else {
                        addNodeHeadDFList(globalDoubleList, value);
                    }
                } else {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "LDEL" && tokens.size() >= 2) {
                int mode = safeStoi(tokens[1]);
                
                if (mode == 0) {
                    if (globalDoubleList->head != nullptr) {
                        DFNode* temp = globalDoubleList->head;
                        globalDoubleList->head = temp->next;
                        if (globalDoubleList->head != nullptr) {
                            globalDoubleList->head->prev = nullptr;
                        } else {
                            globalDoubleList->tail = nullptr;
                        }
                        delete temp;
                    }
                } else if (mode == 1) {
                    if (globalDoubleList->tail != nullptr) {
                        DFNode* temp = globalDoubleList->tail;
                        globalDoubleList->tail = temp->prev;
                        if (globalDoubleList->tail != nullptr) {
                            globalDoubleList->tail->next = nullptr;
                        } else {
                            globalDoubleList->head = nullptr;
                        }
                        delete temp;
                    }
                } else if (mode == 2) {
                    if (globalDoubleList->head != nullptr && globalDoubleList->head->next != nullptr) {
                        DFNode* temp = globalDoubleList->head->next;
                        globalDoubleList->head->next = temp->next;
                        if (temp->next != nullptr) {
                            temp->next->prev = globalDoubleList->head;
                        } else {
                            globalDoubleList->tail = globalDoubleList->head;
                        }
                        delete temp;
                    }
                } else if (mode == 3) {
                    if (globalDoubleList->tail != nullptr && globalDoubleList->tail->prev != nullptr) {
                        DFNode* temp = globalDoubleList->tail->prev;
                        globalDoubleList->tail->prev = temp->prev;
                        if (temp->prev != nullptr) {
                            temp->prev->next = globalDoubleList->tail;
                        } else {
                            globalDoubleList->head = globalDoubleList->tail;
                        }
                        delete temp;
                    }
                } else {
                    cerr << "ERROR 30: Invalid index/argument" << endl;
                    exit(1);
                }
            }
            else if (tokens[0] == "LGET" && tokens.size() >= 2) {
                int index = safeStoi(tokens[1]);
                cout << getElementDFList(globalDoubleList, index) << endl;
            }
            else {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        } catch (...) {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }

    void handleSCommand(const vector<string>& tokens) {
        try {
            if (currentStructureType != "none" && currentStructureType != "Stack") {
                cerr << "ERROR 30: Invalid index/argument" << endl;
                exit(1);
            }
            
            currentStructureType = "Stack";
            
            if (tokens[0] == "SPUSH" && tokens.size() >= 2) { 
                string value = tokens[1];
                
                if (!globalStack) {
                    globalStack = new Stack;
                    initializeStack(globalStack);
                }
                pushStack(globalStack, value);
            }
            else if (tokens[0] == "SPOP") {
                if (!globalStack) {
                    cerr << "ERROR 40: Empty structure" << endl;
                    exit(1);
                }
                cout << popStack(globalStack) << endl;
            }
            else {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        } catch (...) {
            cerr << "ERROR 40: Empty structure" << endl;
            exit(1);
        }
    }

    void handleQCommand(const vector<string>& tokens) {
        try {
            if (currentStructureType != "none" && currentStructureType != "Queue") {
                cerr << "ERROR 30: Invalid index/argument" << endl;
                exit(1);
            }
            
            currentStructureType = "Queue";
            
            if (tokens[0] == "QPUSH" && tokens.size() >= 2) {
                string value = tokens[1];
                
                if (!globalQueue) {
                    globalQueue = createQueue();
                }
                enqueue(globalQueue, value);
            }
            else if (tokens[0] == "QPOP") {
                if (!globalQueue) {
                    cerr << "ERROR 40: Empty structure" << endl;
                    exit(1);
                }
                cout << dequeue(globalQueue) << endl;
            }
            else {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        } catch (...) {
            cerr << "ERROR 40: Empty structure" << endl;
            exit(1);
        }
    }

    void handleTCommand(const vector<string>& tokens) {
        try {
            if (currentStructureType != "none" && currentStructureType != "FBTree") {
                cerr << "ERROR 30: Invalid index/argument" << endl;
                exit(1);
            }
            
            currentStructureType = "FBTree";
            
            if (tokens[0] == "TINSERT" && tokens.size() >= 2) {
                int key = safeStoi(tokens[1]);
                
                if (!globalTree) {
                    globalTree = new BTree;
                }
                addNode(globalTree, key);
            }
            else if (tokens[0] == "TSEARCH" && tokens.size() >= 2) {
                int key = safeStoi(tokens[1]);
                
                if (!globalTree) {
                    cout << "FALSE" << endl;
                    return;
                }
                
                try {
                    findNode(*globalTree, key);
                    cout << "TRUE" << endl;
                } catch (...) {
                    cout << "FALSE" << endl;
                }
            }
            else if (tokens[0] == "TCHECK") {
                if (!globalTree || globalTree->root == nullptr) {
                    cout << "TRUE" << endl;
                } else {
                    cout << (isFullTree(*globalTree) ? "TRUE" : "FALSE") << endl;
                }
            }
            else if (tokens[0] == "TDEL" && tokens.size() >= 2) {
                int key = safeStoi(tokens[1]);
                
                if (!globalTree) {
                    cerr << "ERROR 20: Structure not found" << endl;
                    exit(1);
                }
                deleteNode(globalTree, key);
            }
            else if (tokens[0] == "TGET" && tokens.size() >= 2) {
                string mode = tokens[1];
                
                if (!globalTree || globalTree->root == nullptr) {
                    cerr << "ERROR 40: Empty structure" << endl;
                    exit(1);
                }
                
                if (mode == "PRE") {
                    // Pre-order обход
                    function<void(BNode*)> preOrder = [&](BNode* node) {
                        if (node) {
                            cout << node->key << " ";
                            preOrder(node->left);
                            preOrder(node->right);
                        }
                    };
                    preOrder(globalTree->root);
                    cout << endl;
                } else if (mode == "IN") {
                    // In-order обход
                    function<void(BNode*)> inOrder = [&](BNode* node) {
                        if (node) {
                            inOrder(node->left);
                            cout << node->key << " ";
                            inOrder(node->right);
                        }
                    };
                    inOrder(globalTree->root);
                    cout << endl;
                } else if (mode == "POST") {
                    // Post-order обход
                    function<void(BNode*)> postOrder = [&](BNode* node) {
                        if (node) {
                            postOrder(node->left);
                            postOrder(node->right);
                            cout << node->key << " ";
                        }
                    };
                    postOrder(globalTree->root);
                    cout << endl;
                } else if (mode == "BFS") {
                    // BFS обход
                    vector<BNode*> queue;
                    queue.push_back(globalTree->root);
                    size_t i = 0;
                    while (i < queue.size()) {
                        BNode* node = queue[i++];
                        cout << node->key << " ";
                        if (node->left) queue.push_back(node->left);
                        if (node->right) queue.push_back(node->right);
                    }
                    cout << endl;
                } else {
                    cerr << "ERROR 10: Unknown command" << endl;
                    exit(1);
                }
            }
            else {
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
        } catch (...) {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }
    
    void printCurrentStructure() {
        try {
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
                cerr << "ERROR 20: Structure not found" << endl;
                exit(1);
            }
        } catch (...) {
            cerr << "ERROR 30: Invalid index/argument" << endl;
            exit(1);
        }
    }
    
    bool loadStructureFromFile(const string& filename) {
        try {
            void* structure = nullptr;
            string type;
            
            structure = ::loadStructureFromFile(filename, type);
            cleanup();
            
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
                if (structure) delete static_cast<Array*>(structure);
                cerr << "ERROR 10: Unknown command" << endl;
                exit(1);
            }
            
            currentStructureType = type;
            currentFilename = filename;
            return true;
        } 
        catch (...) {
            cerr << "ERROR 10: Unknown command" << endl;
            exit(1);
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
    catch (...) {
        cerr << "ERROR 30: Invalid index/argument" << endl;
        exit(1);
    }
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
            cout << "Usage: ./lab1 --file <path> --query '<COMMAND> <ARGS...>'" << endl;
            return 0;
        }
        
        // === ДЕСЕРИАЛИЗАЦИЯ ===
        if (!filename.empty()) {
            if (fileExists(filename)) {
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
        
    } catch (...) {
        cerr << "ERROR 10: Unknown command" << endl;
        return 1;
    }
    
    // === ЗАВЕРШЕНИЕ ===
    return 0;
}
