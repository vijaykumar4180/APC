# 🔢 Arbitrary Precision Calculator (APC)

A command-line **Arbitrary Precision Calculator (APC)** developed in **C** that performs arithmetic operations on integers of virtually unlimited size. The calculator overcomes the limitations of built-in integer data types by representing numbers as **doubly linked lists**, enabling operations on numbers containing hundreds or thousands of digits.

---

## 📌 Overview

Standard C data types (`int`, `long`, `long long`) have limited storage capacity and cannot handle extremely large integers. This project implements a Big Integer library using a **doubly linked list**, allowing arithmetic on numbers of arbitrary length.

---

## ✨ Features

- ➕ Addition of large integers
- ➖ Subtraction of large integers
- ✖️ Multiplication of large integers *(if implemented)*
- ➗ Division of large integers *(if implemented)*
- Handles positive and negative numbers
- Supports numbers with hundreds or thousands of digits
- Dynamic memory allocation
- Menu/Command-line based execution

---

## 🛠️ Technologies Used

- C Programming
- Doubly Linked List
- Dynamic Memory Allocation
- Structures
- Modular Programming
- Makefile
- GCC Compiler
- Linux (Ubuntu/WSL)
- Git & GitHub

---

## 📂 Project Structure

```
APC/
├── apc.c              # Big integer operations
├── apc.h              # Function declarations
├── main.c             # Driver program
├── Makefile           # Build automation
├── include/           # Header files (if used)
├── .gitignore
└── README.md
```

---

## 🧠 Data Structure

Each digit of a number is stored in a node of a doubly linked list.

Example:

Number:

```
123456
```

Stored as:

```
NULL <- [1] <-> [2] <-> [3] <-> [4] <-> [5] <-> [6] -> NULL
```

---

## ⚙️ Operations

### Addition

- Traverses both linked lists from the least significant digit.
- Adds corresponding digits with carry propagation.

### Subtraction

- Performs digit-by-digit subtraction.
- Handles borrow operations.

### Multiplication

- Performs repeated digit multiplication with carry handling.

### Division

- Uses long division logic for arbitrary precision numbers.

---

## 🚀 Compilation

Using Makefile:

```bash
make
```

Or using GCC:

```bash
gcc main.c apc.c -o apc
```

---

## ▶️ Execution

Using Makefile:

```bash
./apc
```

or

```bash
make run
```

(if available in your Makefile)

---

## 📋 Sample Output

```
========== Arbitrary Precision Calculator ==========

Enter First Number  : 987654321987654321987654321

Enter Operator (+,-,*,/) : +

Enter Second Number : 123456789123456789123456789

Result

1111111111111111111111111110
```

---

## 📚 Concepts Used

- Doubly Linked List
- Dynamic Memory Allocation
- Structures
- Pointer Manipulation
- String Processing
- Big Integer Arithmetic
- Modular Programming
- Header Files
- Makefile
- Git & GitHub

---

## 🎯 Learning Outcomes

This project helped me understand:

- Implementation of Big Integer arithmetic
- Linked list manipulation
- Efficient memory management
- Handling numbers beyond built-in data types
- Modular software design
- Building multi-file C projects
- Version control using Git and GitHub

---

## 🚀 Future Enhancements

- Modulus (%) operation
- Exponentiation
- Factorial of large numbers
- Square Root
- Expression evaluation
- Decimal number support

---

## 👨‍💻 Author

**Vijay Kumar**

- GitHub: https://github.com/vijaykumar4180

---

⭐ If you found this project useful, consider giving it a star.
