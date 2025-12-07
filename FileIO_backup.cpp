#include "FileIO.h"

#include <iostream>
#include <fstream>

static bool dirExists(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
#if defined(_WIN32)
    return (info.st_mode & _S_IFDIR) != 0;
#else
    return S_ISDIR(info.st_mode);
#endif
}

static void makeDir(const std::string& path) {
#if defined(_WIN32)
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
}

// create directories recursively for a given directory path
static void mkdir_recursive(const std::string& dir) {
    if (dir.empty()) return;
    if (dirExists(dir)) return;
    std::string accum;
    for (size_t i = 0; i < dir.size(); ++i) {
        char c = dir[i];
        accum.push_back(c);
        if (c == '/' || c == '\\') {
            if (!accum.empty() && !dirExists(accum)) makeDir(accum);
        }
    }
    if (!accum.empty() && !dirExists(accum)) makeDir(accum);
}

void ensureDirectoryExists(const std::string& filename) {
    // Extract parent directory from filename
    size_t pos1 = filename.find_last_of('/');
    size_t pos2 = filename.find_last_of('\\');
    size_t pos = std::string::npos;
    if (pos1 != std::string::npos && pos2 != std::string::npos) pos = std::max(pos1, pos2);
    else if (pos1 != std::string::npos) pos = pos1;
    else if (pos2 != std::string::npos) pos = pos2;
    if (pos == std::string::npos) return; // no directory portion
    std::string dir = filename.substr(0, pos);
    mkdir_recursive(dir);
}

void createFile(const std::string& filename) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "#" << std::endl;
        file.close();
    } else {
        throw std::runtime_error("Невозможно создать " + filename);
    }
}

bool fileExists(const std::string& filename) {
    std::ifstream f(filename);
    return f.good();
}

std::string getStructureType(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть " + filename);
    }

    std::string type;
    std::getline(file, type);
    file.close();

    if (type.empty() || type[0] != '#') {
        throw std::runtime_error("Не найден тип структуры");
    }

    return type.substr(1);
}

void saveArrayToFile(const std::string& filename, const Array& array) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть файл для записи");
    }

    file << "#Array" << std::endl;
    for (std::size_t i = 0; i < array.len; ++i) {
        file << getElementArray(const_cast<Array*>(&array), i) << std::endl;
    }
    file.close();
}

Array* loadArrayFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть " + filename);
    }

    std::string line;
    std::getline(file, line);
    if (line != "#Array") {
        throw std::runtime_error("Файл не содержит массива");
    }

    Array* mArray = new Array;
    createArray(mArray, 10);

    std::size_t lineIndex = 0;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (lineIndex >= mArray->size) {
                extendArray(mArray);
            }
            setKeyArray(mArray, line, lineIndex);
            ++lineIndex;
        }
    }
    mArray->len = lineIndex;
    file.close();
    return mArray;
}

void saveForwardListToFile(const std::string& filename, const ForwardList& list) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть файл для записи");
    }

    file << "#ForwardList" << std::endl;
    FNode* current = list.head;
    while (current != nullptr) {
        file << current->key << std::endl;
        current = current->next;
    }
    file.close();
}

ForwardList* loadForwardListFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть " + filename);
    }

    std::string line;
    std::getline(file, line);
    if (line != "#ForwardList") {
        throw std::runtime_error("Файл не содержит односвязанный список");
    }

    ForwardList* list = createFL();
    while (std::getline(file, line)) {
        if (!line.empty()) {
            pushBackFL(list, line);
        }
    }
    file.close();
    return list;
}

void saveDoubleListToFile(const std::string& filename, const DFList& list) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть для записи файл " + filename);
    }

    file << "#DoubleList" << std::endl;
    DFNode* current = list.head;
    while (current != nullptr) {
        file << current->key << std::endl;
        current = current->next;
    }
    file.close();
}

DFList* loadDoubleListFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть " + filename);
    }

    std::string line;
    std::getline(file, line);
    if (line != "#DoubleList") {
        throw std::runtime_error("Файл не содержит двусторонний список");
    }

    DFList* mList = createDFList();
    while (std::getline(file, line)) {
        if (!line.empty()) {
            addNodeTailDFList(mList, line);
        }
    }
    file.close();
    return mList;
}

void saveStackToFile(const std::string& filename, const Stack& mStack) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть файл для записи");
    }

    file << "#Stack" << std::endl;

    // Итерируемся по стеку без его изменения
    std::vector<std::string> elements;
    SNode* current = mStack.head;
    while (current != nullptr) {
        elements.push_back(current->key);
        current = current->next;
    }

    // Записываем в обратном порядке (чтобы при загрузке порядок сохранился)
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        file << *it << std::endl;
    }
    file.close();
}

Stack* loadStackFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть " + filename);
    }

    std::string line;
    std::getline(file, line);
    if (line != "#Stack") {
        throw std::runtime_error("Файл не содержит стек");
    }

    Stack* mStack = new Stack;
    initializeStack(mStack);

    std::vector<std::string> elements;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            elements.push_back(line);
        }
    }

    for (auto it = elements.begin(); it != elements.end(); ++it) {
        pushStack(mStack, *it);
    }
    file.close();
    return mStack;
}

void saveQueueToFile(const std::string& filename, const Queue& mQueue) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть для записи " + filename);
    }

    file << "#Queue" << std::endl;

    // Итерируемся по очереди без её изменения
    if (mQueue.size > 0) {
        int current = mQueue.head;
        for (std::size_t i = 0; i < mQueue.size; i++) {
            file << mQueue.data[current] << std::endl;
            current = (current + 1) % Queue::MAX_SIZE;
        }
    }
    file.close();
}

Queue* loadQueueFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть " + filename);
    }

    std::string line;
    std::getline(file, line);
    if (line != "#Queue") {
        throw std::runtime_error("Файл не содержит очередь");
    }

    Queue* mQueue = createQueue();
    while (std::getline(file, line)) {
        if (!line.empty()) {
            enqueue(mQueue, line);
        }
    }
    file.close();
    return mQueue;
}

void saveTreeToFile(const std::string& filename, const BTree& tree) {
    ensureDirectoryExists(filename);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть для записи " + filename);
    }

    file << "#FBTree" << std::endl;
    std::vector<BNode*> nodes;
    if (tree.root != nullptr) {
        nodes.push_back(tree.root);
    }

    while (!nodes.empty()) {
        BNode* current = nodes.front();
        nodes.erase(nodes.begin());

        file << current->key << std::endl;

        if (current->left != nullptr) {
            nodes.push_back(current->left);
        }
        if (current->right != nullptr) {
            nodes.push_back(current->right);
        }
    }
    file.close();
}

BTree* loadTreeFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть " + filename);
    }

    std::string line;
    std::getline(file, line);
    if (line != "#FBTree") {
        throw std::runtime_error("Файл не содержит бинарное дерево");
    }

    BTree* tree = new BTree;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            // Парсим ключ (может быть просто число или число с пробелом)
            std::size_t spacePos = line.find(' ');
            int key;
            try {
                if (spacePos != std::string::npos) {
                    key = std::stoi(line.substr(0, spacePos));
                } else {
                    key = std::stoi(line);
                }
            } catch (const std::exception& e) {
                throw std::runtime_error("Ошибка парсинга ключа в файле: " + line);
            }

            addNode(tree, key);
        }
    }
    file.close();
    return tree;
}

void* loadStructureFromFile(const std::string& filename, std::string& type) {
    type = getStructureType(filename);

    if (type == "Array") {
        return loadArrayFromFile(filename);
    } else if (type == "ForwardList") {
        return loadForwardListFromFile(filename);
    } else if (type == "DoubleList") {
        return loadDoubleListFromFile(filename);
    } else if (type == "Stack") {
        return loadStackFromFile(filename);
    } else if (type == "Queue") {
        return loadQueueFromFile(filename);
    } else if (type == "FBTree") {
        return loadTreeFromFile(filename);
    } else {
        throw std::runtime_error("Неизвестная структура: " + type);
    }
}

void saveStructureToFile(const std::string& filename, const std::string& type, void* structure) {
    if (type == "Array") {
        saveArrayToFile(filename, *static_cast<Array*>(structure));
    } else if (type == "ForwardList") {
        saveForwardListToFile(filename, *static_cast<ForwardList*>(structure));
    } else if (type == "DoubleList") {
        saveDoubleListToFile(filename, *static_cast<DFList*>(structure));
    } else if (type == "Stack") {
        saveStackToFile(filename, *static_cast<Stack*>(structure));
    } else if (type == "Queue") {
        saveQueueToFile(filename, *static_cast<Queue*>(structure));
    } else if (type == "FBTree") {
        saveTreeToFile(filename, *static_cast<BTree*>(structure));
    } else {
        throw std::runtime_error("Неизвестная структура: " + type);
    }
}
