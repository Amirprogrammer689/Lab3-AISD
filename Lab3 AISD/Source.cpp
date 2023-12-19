#include <iostream>
#include <vector>
#include <chrono>
#include <random>

struct stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;
};

stats insertionSort(std::vector<int>& arr) {
    stats statistics;
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
            statistics.comparison_count++;
            statistics.copy_count++;
        }
        arr[j + 1] = key;
        statistics.copy_count++;
    }
    return statistics;
}

stats shellSort(std::vector<int>& arr) {
    stats statistics;
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
                statistics.comparison_count++;
                statistics.copy_count++;
            }
            arr[j] = temp;
            statistics.copy_count++;
        }
    }
    return statistics;
}

void heapify(std::vector<int>& arr, int n, int i, stats& statistics) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest]) {
        largest = l;
    }

    if (r < n && arr[r] > arr[largest]) {
        largest = r;
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        statistics.copy_count += 3;
        statistics.comparison_count += 3;
        heapify(arr, n, largest, statistics);
    }
}

stats heapSort(std::vector<int>& arr) {
    stats statistics;
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, statistics);
    }
    for (int i = n - 1; i >= 0; i--) {
        std::swap(arr[0], arr[i]);
        statistics.copy_count += 3;
        statistics.comparison_count += 3;
        heapify(arr, i, 0, statistics);
    }
    return statistics;
}

//случайный массив
std::vector<int> generateRandomArray(size_t size) {
    std::vector<int> arr(size);
    srand(static_cast<unsigned int>(time(0)));
    for (size_t i = 0; i < size; ++i) {
        arr[i] = rand();
    }
    return arr;
}

//отсортированный массив
std::vector<int> generateSortedArray(size_t size) {
    std::vector<int> arr(size);
    for (size_t i = 0; i < size; ++i) {
        arr[i] = i;
    }
    return arr;
}

//массив отсортирован в обратном порядке
std::vector<int> generateReverseSortedArray(size_t size) {
    std::vector<int> arr(size);
    for (size_t i = 0; i < size; ++i) {
        arr[i] = size - i;
    }
    return arr;
}

//в среднем (сгенерировать 100 случайных массивов и посчитать средние значения);
stats calculateAverageStats(size_t arraySize, size_t numExperiments, std::vector<int>(*generateArray)(size_t), stats(*sortFunction)(std::vector<int>&)) {
    stats totalStats;

    for (size_t i = 0; i < numExperiments; ++i) {
        std::vector<int> arr = generateArray(arraySize);
        stats result = sortFunction(arr);
        totalStats.comparison_count += result.comparison_count;
        totalStats.copy_count += result.copy_count;
    }

    totalStats.comparison_count /= numExperiments;
    totalStats.copy_count /= numExperiments;

    return totalStats;
}

int main() {
    const size_t numExperiments = 100;

    for (size_t arraySize : {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 25000, 50000, 100000}) {

        stats averageStatsRandom = calculateAverageStats(arraySize, numExperiments, generateRandomArray, insertionSort);
        stats averageStatsShaker = calculateAverageStats(arraySize, numExperiments, generateRandomArray, shellSort);
        stats averageStatsComb = calculateAverageStats(arraySize, numExperiments, generateRandomArray, heapSort);

        stats averageStatsSortedBubble = calculateAverageStats(arraySize, numExperiments, generateSortedArray, insertionSort);
        stats averageStatsSortedShaker = calculateAverageStats(arraySize, numExperiments, generateSortedArray, shellSort);
        stats averageStatsSortedComb = calculateAverageStats(arraySize, numExperiments, generateSortedArray, heapSort);

        stats averageStatsReverse = calculateAverageStats(arraySize, numExperiments, generateReverseSortedArray, insertionSort);
        stats averageStatsReverseShaker = calculateAverageStats(arraySize, numExperiments, generateReverseSortedArray, shellSort);
        stats averageStatsReverseComb = calculateAverageStats(arraySize, numExperiments, generateReverseSortedArray, heapSort);

        std::cout << "Array Size: " << arraySize << "\n";
        std::cout << "Random (insertionSort): Comparisons: " << averageStatsRandom.comparison_count << ", Copies: " << averageStatsRandom.copy_count << "\n";
        std::cout << "Random (shellSort): Comparisons: " << averageStatsShaker.comparison_count << ", Copies: " << averageStatsShaker.copy_count << "\n";
        std::cout << "Random (heapSort): Comparisons: " << averageStatsComb.comparison_count << ", Copies: " << averageStatsComb.copy_count << "\n\n";
        std::cout << "Sorted (insertionSort): Comparisons: " << averageStatsSortedBubble.comparison_count << ", Copies: " << averageStatsSortedBubble.copy_count << "\n";
        std::cout << "Sorted (shellSort): Comparisons: " << averageStatsSortedShaker.comparison_count << ", Copies: " << averageStatsSortedShaker.copy_count << "\n";
        std::cout << "Sorted (heapSort): Comparisons: " << averageStatsSortedComb.comparison_count << ", Copies: " << averageStatsSortedComb.copy_count << "\n\n";
        std::cout << "Reverse (insertionSort): Comparisons: " << averageStatsReverse.comparison_count << ", Copies: " << averageStatsReverse.copy_count << "\n";
        std::cout << "Reverse (shellSort): Comparisons: " << averageStatsReverseShaker.comparison_count << ", Copies: " << averageStatsReverseShaker.copy_count << "\n";
        std::cout << "Reverse (heapSort): Comparisons: " << averageStatsReverseComb.comparison_count << ", Copies: " << averageStatsReverseComb.copy_count << "\n";
    }

    return 0;
}


