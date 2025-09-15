# Searching Algorithms

This folder contains implementations of fundamental **searching algorithms** in C.  
Searching is the process of finding whether an element exists in a collection (array, list, tree, etc.) and, if so, returning its position.

## 📚 Included Algorithms
- **Linear Search**  
- **Binary Search (Iterative)**  
- **Binary Search (Recursive)**  
- **Jump Search**  
- **Interpolation Search**  
- **Exponential Search**  

## 🔍 Overview
Searching algorithms are essential for efficient data retrieval.  
They are categorized into:
- **Unordered search** (e.g., Linear Search)  
- **Ordered search** (e.g., Binary Search, Jump Search, Interpolation Search)  

Some require sorted input arrays, while others can work on unsorted collections.

## ⏳ Time and Space Complexities

| Algorithm           | Best Case | Average Case | Worst Case | Space Complexity | Requirement     |
|---------------------|-----------|--------------|-------------|------------------|----------------|
| Linear Search       | O(1)      | O(n)         | O(n)        | O(1)             | Unsorted array |
| Binary Search       | O(1)      | O(log n)     | O(log n)    | O(1) / O(log n)* | Sorted array   |
| Jump Search         | O(1)      | O(√n)        | O(√n)       | O(1)             | Sorted array   |
| Interpolation Search| O(1)      | O(log log n) | O(n)        | O(1)             | Sorted, uniform distribution |
| Exponential Search  | O(log i)  | O(log i)     | O(log i)    | O(1)             | Sorted array   |

> *Iterative binary search has O(1) space, recursive version has O(log n).*

## 📂 Folder Structure

searching/
│── linear_search.c
│── binary_search_iterative.c
│── binary_search_recursive.c
│── jump_search.c
│── interpolation_search.c
│── exponential_search.c
│── README.md


## 🚀 How to Run
Compile any file using `gcc`:
```bash
gcc binary_search_iterative.c -o binary_search
./binary_search
