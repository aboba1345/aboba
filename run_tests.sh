#!/bin/bash

# Comprehensive Test Suite for lab1 project

echo "========================================"
echo "COMPREHENSIVE TEST SUITE FOR LAB1"
echo "========================================"

cd "c:\Users\tikho\OneDrive\Рабочий стол\lab_1 (3)\lab_1"

# Test 1: Array Operations
echo -e "\n=== TEST 1: ARRAY OPERATIONS ==="
rm -f array_test.txt
echo "Create array..."
./lab1.exe --file array_test.txt --query "MCREATE"
echo "MPUSH 10..."
./lab1.exe --file array_test.txt --query "MPUSH 10"
echo "MPUSH 20..."
./lab1.exe --file array_test.txt --query "MPUSH 20"
echo "MPUSH 30..."
./lab1.exe --file array_test.txt --query "MPUSH 30"
echo "MPUSHAT value 1 (insert at index 1)..."
./lab1.exe --file array_test.txt --query "MPUSHAT value 1"
echo "MGET 1..."
./lab1.exe --file array_test.txt --query "MGET 1"
echo "MLEN..."
./lab1.exe --file array_test.txt --query "MLEN"
echo "File content:"
cat array_test.txt

# Test 2: Forward List Operations with modes
echo -e "\n=== TEST 2: FORWARD LIST OPERATIONS ==="
rm -f flist_test.txt
echo "FPUSH a 1 (push to tail)..."
./lab1.exe --file flist_test.txt --query "FPUSH a 1"
echo "FPUSH b 1 (push to tail)..."
./lab1.exe --file flist_test.txt --query "FPUSH b 1"
echo "FPUSH start 0 (push to head)..."
./lab1.exe --file flist_test.txt --query "FPUSH start 0"
echo "FSEARCH a..."
./lab1.exe --file flist_test.txt --query "FSEARCH a"
echo "FSEARCH notfound..."
./lab1.exe --file flist_test.txt --query "FSEARCH notfound" 2>&1 || true
echo "FGET 1..."
./lab1.exe --file flist_test.txt --query "FGET 1"
echo "File content:"
cat flist_test.txt

# Test 3: Double List with modes
echo -e "\n=== TEST 3: DOUBLE LIST OPERATIONS ==="
rm -f dlist_test.txt
echo "LPUSH first 0 (add to head)..."
./lab1.exe --file dlist_test.txt --query "LPUSH first 0"
echo "LPUSH second 1 (add to tail)..."
./lab1.exe --file dlist_test.txt --query "LPUSH second 1"
echo "LPUSH last 1 (add to tail)..."
./lab1.exe --file dlist_test.txt --query "LPUSH last 1"
echo "LDEL 0 (delete head)..."
./lab1.exe --file dlist_test.txt --query "LDEL 0"
echo "LDEL 1 (delete tail - O(1) with prev)..."
./lab1.exe --file dlist_test.txt --query "LDEL 1"
echo "File content:"
cat dlist_test.txt

# Test 4: Stack Operations
echo -e "\n=== TEST 4: STACK OPERATIONS ==="
rm -f stack_test.txt
echo "SPUSH elem1..."
./lab1.exe --file stack_test.txt --query "SPUSH elem1"
echo "SPUSH elem2..."
./lab1.exe --file stack_test.txt --query "SPUSH elem2"
echo "SPUSH elem3..."
./lab1.exe --file stack_test.txt --query "SPUSH elem3"
echo "SPOP (should output elem3)..."
./lab1.exe --file stack_test.txt --query "SPOP"
echo "File content (after SPOP):"
cat stack_test.txt

# Test 5: Queue Operations
echo -e "\n=== TEST 5: QUEUE OPERATIONS ==="
rm -f queue_test.txt
echo "QPUSH first..."
./lab1.exe --file queue_test.txt --query "QPUSH first"
echo "QPUSH second..."
./lab1.exe --file queue_test.txt --query "QPUSH second"
echo "QPUSH third..."
./lab1.exe --file queue_test.txt --query "QPUSH third"
echo "QPOP (should output first)..."
./lab1.exe --file queue_test.txt --query "QPOP"
echo "File content (after QPOP):"
cat queue_test.txt

# Test 6: Tree Operations (BST + Full Tree check)
echo -e "\n=== TEST 6: BINARY SEARCH TREE OPERATIONS ==="
rm -f tree_test.txt
echo "Building tree: 50 -> 30, 70 -> 20, 40, 60, 80..."
./lab1.exe --file tree_test.txt --query "TINSERT 50"
./lab1.exe --file tree_test.txt --query "TINSERT 30"
./lab1.exe --file tree_test.txt --query "TINSERT 70"
./lab1.exe --file tree_test.txt --query "TINSERT 20"
./lab1.exe --file tree_test.txt --query "TINSERT 40"
./lab1.exe --file tree_test.txt --query "TINSERT 60"
./lab1.exe --file tree_test.txt --query "TINSERT 80"

echo "TCHECK (should be TRUE - full tree)..."
./lab1.exe --file tree_test.txt --query "TCHECK"

echo "TGET PRE (Pre-order)..."
./lab1.exe --file tree_test.txt --query "TGET PRE"

echo "TGET IN (In-order - sorted)..."
./lab1.exe --file tree_test.txt --query "TGET IN"

echo "TGET POST (Post-order)..."
./lab1.exe --file tree_test.txt --query "TGET POST"

echo "TGET BFS (Level-order)..."
./lab1.exe --file tree_test.txt --query "TGET BFS"

echo "TSEARCH 40..."
./lab1.exe --file tree_test.txt --query "TSEARCH 40"

echo "TSEARCH 999 (not found)..."
./lab1.exe --file tree_test.txt --query "TSEARCH 999"

echo "File content (Pre-order save):"
cat tree_test.txt

# Test 7: Error Handling
echo -e "\n=== TEST 7: ERROR HANDLING ==="
echo "Unknown command..."
./lab1.exe --file test.txt --query "INVALIDCMD" 2>&1 || true

echo "Invalid index..."
./lab1.exe --file array_test.txt --query "MGET abc" 2>&1 || true

echo -e "\n========================================"
echo "TEST SUITE COMPLETED"
echo "========================================"
