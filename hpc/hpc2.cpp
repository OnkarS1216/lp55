#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

#define SIZE 10000

// -------- PRINT (only first 20 elements) --------
void printArray(vector<int>& arr) {
    for (int i = 0; i < 20; i++) {
        cout << arr[i] << " ";
    }
    cout << "... \n";
}

// -------- BUBBLE SORT (SEQUENTIAL) --------
void bubbleSeq(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
}

// -------- BUBBLE SORT (PARALLEL) --------
void bubblePar(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n; i++) {

        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);

        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
    }
}

// -------- MERGE --------
void merge(vector<int>& a, int l, int m, int r) {
    vector<int> left(a.begin() + l, a.begin() + m + 1);
    vector<int> right(a.begin() + m + 1, a.begin() + r + 1);

    int i = 0, j = 0, k = l;

    while (i < left.size() && j < right.size())
        a[k++] = (left[i] < right[j]) ? left[i++] : right[j++];

    while (i < left.size()) a[k++] = left[i++];
    while (j < right.size()) a[k++] = right[j++];
}

// -------- MERGE SORT (SEQUENTIAL) --------
void mergeSeq(vector<int>& a, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSeq(a, l, m);
        mergeSeq(a, m + 1, r);
        merge(a, l, m, r);
    }
}

// -------- MERGE SORT (PARALLEL) --------
void mergePar(vector<int>& a, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            mergePar(a, l, m);

            #pragma omp section
            mergePar(a, m + 1, r);
        }

        merge(a, l, m, r);
    }
}

// -------- MAIN --------
int main() {
    vector<int> arr(SIZE), temp;

    srand(1); // fixed seed for same output

    for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % 100000;

    double start, end;

    // Bubble Sequential
    temp = arr;
    start = omp_get_wtime();
    bubbleSeq(temp);
    end = omp_get_wtime();
    cout << "Bubble Seq Time: " << end - start << "\n";
    printArray(temp);

    // Bubble Parallel
    temp = arr;
    start = omp_get_wtime();
    bubblePar(temp);
    end = omp_get_wtime();
    cout << "Bubble Par Time: " << end - start << "\n";
    printArray(temp);

    // Merge Sequential
    temp = arr;
    start = omp_get_wtime();
    mergeSeq(temp, 0, SIZE - 1);
    end = omp_get_wtime();
    cout << "Merge Seq Time: " << end - start << "\n";
    printArray(temp);

    // Merge Parallel
    temp = arr;
    start = omp_get_wtime();
    mergePar(temp, 0, SIZE - 1);
    end = omp_get_wtime();
    cout << "Merge Par Time: " << end - start << "\n";
    printArray(temp);

    return 0;
}