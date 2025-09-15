# Sorting Algorithms

This folder contains implementations of classic **sorting algorithms** in C.  
Sorting is one of the most fundamental problems in computer science, often used as a basis for learning algorithmic analysis and optimization.

## 📚 Included Algorithms
- **Bubble Sort**  
- **Selection Sort**  
- **Insertion Sort**  
- **Merge Sort**  
- **Quick Sort**  
<!-- - **Heap Sort**  
- **Counting Sort** (non-comparison based)  
- **Radix Sort** (non-comparison based) -->

## 🔍 Overview
Sorting algorithms are typically divided into:
- **Comparison-based algorithms** (e.g., Quick Sort, Merge Sort, Heap Sort)  
- **Non-comparison algorithms** (e.g., Counting Sort, Radix Sort)  

They can also be analyzed by:
- **Time complexity**  
- **Space complexity**  
- **Stability** (whether equal elements keep their relative order)

## ⏳ Time and Space Complexities

| Algorithm       | Best Case | Average Case | Worst Case | Space Complexity | Stable |
|-----------------|-----------|--------------|-------------|------------------|--------|
| Bubble Sort     | O(n)      | O(n²)        | O(n²)       | O(1)             | ✅     |
| Selection Sort  | O(n²)     | O(n²)        | O(n²)       | O(1)             | ❌     |
| Insertion Sort  | O(n)      | O(n²)        | O(n²)       | O(1)             | ✅     |
| Merge Sort      | O(n log n)| O(n log n)   | O(n log n)  | O(n)             | ✅     |
| Quick Sort      | O(n log n)| O(n log n)   | O(n²)       | O(log n)         | ❌     |
<!--| Heap Sort       | O(n log n)| O(n log n)   | O(n log n)  | O(1)             | ❌     |
| Counting Sort   | O(n + k)  | O(n + k)     | O(n + k)    | O(k)             | ✅     |
| Radix Sort      | O(nk)     | O(nk)        | O(nk)       | O(n + k)         | ✅     |-->

> *n = number of elements, k = range of input values or digit length*

## 📂 Folder Structure

sorting/
│── bubble_sort.c
│── selection_sort.c
│── insertion_sort.c
│── merge_sort.c
│── quick_sort.c
<!--│── heap_sort.c
│── counting_sort.c
│── radix_sort.c -->
│── README.md

## 🚀 How to Run
Compile any file using `gcc`:
```bash
gcc quick_sort.c -o quick_sort
./quick_sort
