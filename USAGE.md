# USAGE EXAMPLES & COMMAND REFERENCE

## Build & Run

```bash
cd "c:\Users\tikho\OneDrive\Рабочий стол\lab_1 (3)\lab_1"
clang++ -g -Wall -std=c++17 main.cpp FileIO.cpp FullBinaryTree.cpp Array.cpp ForwardList.cpp DoubleList.cpp Stack.cpp Queue.cpp -o lab1.exe
```

## Invocation Format

```bash
./lab1.exe --file <path> --query '<COMMAND> <ARGS...>'
```

---

## ARRAY Commands

### Create & Basic Operations

```bash
# Create empty array
./lab1.exe --file db.txt --query "MCREATE"

# Push values (add to end)
./lab1.exe --file db.txt --query "MPUSH 10"
./lab1.exe --file db.txt --query "MPUSH 20"
./lab1.exe --file db.txt --query "MPUSH 30"

# Insert at specific index
./lab1.exe --file db.txt --query "MPUSHAT 15 1"

# Get element by index
./lab1.exe --file db.txt --query "MGET 1"
# Output: 15

# Set/Replace element
./lab1.exe --file db.txt --query "MSET 25 1"

# Delete element by index
./lab1.exe --file db.txt --query "MDEL 2"

# Get length
./lab1.exe --file db.txt --query "MLEN"
# Output: 3
```

### Saved Format
```
M array 3 10 15 20
```

---

## FORWARD LIST Commands

### Modes
- Mode 0: Head
- Mode 1: Tail
- Mode 2: After Head (index 0)
- Mode 3: Before Tail (before last)

### Operations

```bash
# Insert with mode
./lab1.exe --file list.txt --query "FPUSH first 1"   # Tail
./lab1.exe --file list.txt --query "FPUSH second 1"  # Tail
./lab1.exe --file list.txt --query "FPUSH start 0"   # Head
./lab1.exe --file list.txt --query "FPUSH after_head 2"  # After head

# Delete with mode
./lab1.exe --file list.txt --query "FDEL 0"  # Delete head
./lab1.exe --file list.txt --query "FDEL 1"  # Delete tail (O(n) without prev)

# Delete by value
./lab1.exe --file list.txt --query "FDELVAL unwanted"

# Search
./lab1.exe --file list.txt --query "FSEARCH first"
# Output: TRUE

# Get by index
./lab1.exe --file list.txt --query "FGET 0"
# Output: start
```

### Saved Format
```
F list 4 start first second after_head
```

---

## DOUBLE LIST Commands

### Modes (same as Forward List)
- Mode 0: Head (O(1))
- Mode 1: Tail (O(1) with prev)
- Mode 2: After Head
- Mode 3: Before Tail (O(1) with prev)

### Operations

```bash
# Insert with mode
./lab1.exe --file dlist.txt --query "LPUSH first_head 0"   # Head
./lab1.exe --file dlist.txt --query "LPUSH last_tail 1"    # Tail
./lab1.exe --file dlist.txt --query "LPUSH middle 1"       # Tail

# Delete with mode (O(1) for 0,1,3 thanks to prev)
./lab1.exe --file dlist.txt --query "LDEL 0"  # Delete head O(1)
./lab1.exe --file dlist.txt --query "LDEL 1"  # Delete tail O(1)
./lab1.exe --file dlist.txt --query "LDEL 3"  # Delete before tail O(1)

# Get by index
./lab1.exe --file dlist.txt --query "LGET 0"
```

### Saved Format
```
L dlist 2 first_head last_tail
```

---

## STACK Commands

```bash
# Push
./lab1.exe --file stack.txt --query "SPUSH a"
./lab1.exe --file stack.txt --query "SPUSH b"
./lab1.exe --file stack.txt --query "SPUSH c"

# Pop (LIFO - Last In First Out)
./lab1.exe --file stack.txt --query "SPOP"
# Output: c
```

### Saved Format (Top → Bottom)
```
S st 3 c b a
```

---

## QUEUE Commands

```bash
# Push
./lab1.exe --file queue.txt --query "QPUSH first"
./lab1.exe --file queue.txt --query "QPUSH second"
./lab1.exe --file queue.txt --query "QPUSH third"

# Pop (FIFO - First In First Out)
./lab1.exe --file queue.txt --query "QPOP"
# Output: first
```

### Saved Format (Head → Tail)
```
Q qu 3 first second third
```

---

## TREE (Full Binary Search Tree) Commands

### Build BST

```bash
./lab1.exe --file tree.txt --query "TINSERT 50"
./lab1.exe --file tree.txt --query "TINSERT 30"
./lab1.exe --file tree.txt --query "TINSERT 70"
./lab1.exe --file tree.txt --query "TINSERT 20"
./lab1.exe --file tree.txt --query "TINSERT 40"
./lab1.exe --file tree.txt --query "TINSERT 60"
./lab1.exe --file tree.txt --query "TINSERT 80"
```

### Tree Structure
```
        50
       /  \
      30   70
     / \   / \
    20 40 60 80
```

### Check if Full Binary Tree

```bash
./lab1.exe --file tree.txt --query "TCHECK"
# Output: TRUE (every node has 0 or 2 children)
```

### Traversals

```bash
# Pre-order (Root → Left → Right)
./lab1.exe --file tree.txt --query "TGET PRE"
# Output: 50 30 20 40 70 60 80

# In-order (Left → Root → Right) - Sorted
./lab1.exe --file tree.txt --query "TGET IN"
# Output: 20 30 40 50 60 70 80

# Post-order (Left → Right → Root)
./lab1.exe --file tree.txt --query "TGET POST"
# Output: 20 40 30 60 80 70 50

# BFS (Level-order)
./lab1.exe --file tree.txt --query "TGET BFS"
# Output: 50 30 70 20 40 60 80
```

### Search

```bash
./lab1.exe --file tree.txt --query "TSEARCH 40"
# Output: TRUE

./lab1.exe --file tree.txt --query "TSEARCH 999"
# Output: FALSE
```

### Delete

```bash
./lab1.exe --file tree.txt --query "TDEL 30"
# Removes node 30, replaces with min from right subtree (40)
```

### Saved Format (Pre-Order)
```
T tree 7 50 30 20 40 70 60 80
```

---

## Error Handling

### Unknown Command
```bash
./lab1.exe --file db.txt --query "INVALID"
# stderr: ERROR 10: Unknown command
# exit code: 1
```

### Invalid Index
```bash
./lab1.exe --file db.txt --query "MGET abc"
# stderr: ERROR 30: Invalid index/argument
# exit code: 1
```

### Empty Structure
```bash
./lab1.exe --file empty.txt --query "SPOP"
# stderr: ERROR 40: Empty structure
# exit code: 1
```

### Structure Not Found
```bash
./lab1.exe --file new.txt --query "TINSERT 10"
./lab1.exe --file other.txt --query "TGET PRE"
# stderr: ERROR 20: Structure not found (on load failure)
# exit code: 1
```

---

## Database File Format Reference

### Single-line format with space-separated values

```
M array 3 10 20 30
F list 2 a b
L dlist 2 x y
S st 2 top second
Q qu 3 head middle tail
T tree 3 root left right
```

### Column Breakdown
- Column 1: Type marker (M/F/L/S/Q/T)
- Column 2: Structure name
- Column 3: Count of elements
- Columns 4+: Element values

---

## Complete Example Session

```bash
#!/bin/bash

# Create array database
./lab1.exe --file my_db.txt --query "MPUSH 100"
./lab1.exe --file my_db.txt --query "MPUSH 200"
./lab1.exe --file my_db.txt --query "MPUSH 300"

# Load and query
./lab1.exe --file my_db.txt --query "MGET 1"     # 200
./lab1.exe --file my_db.txt --query "MLEN"       # 3

# Switch to tree
./lab1.exe --file my_tree.txt --query "TINSERT 50"
./lab1.exe --file my_tree.txt --query "TINSERT 30"
./lab1.exe --file my_tree.txt --query "TINSERT 70"
./lab1.exe --file my_tree.txt --query "TGET IN"   # 30 50 70
./lab1.exe --file my_tree.txt --query "TCHECK"    # TRUE
```

---

## Notes

1. **Persistence:** Each operation saves the structure to disk
2. **Type Safety:** Cannot mix operations (cannot use F-commands on array)
3. **Error Recovery:** Errors exit with code 1, messages go to stderr
4. **Performance:** O(1) tail operations on DoubleList thanks to prev pointer
5. **BST Property:** Tree insertion maintains BST ordering (smaller left, larger right)
6. **Full Tree:** TCHECK validates that each node has exactly 0 or 2 children
