#include <iostream>
#include <fstream>
#include <windows.h>
#include <random>

using namespace std;

double PCFreq = 0.0;
long long int counterStart = 0;

void StartCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    counterStart = li.QuadPart;
}

double GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - counterStart) / PCFreq;
}

int* genAR(int size, int sortedType) {
    int* arr = new int[size]; // Dynamic array
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 1000);

    switch (sortedType) {
        case 1: // Generate a fully random array
            for (int i = 0; i < size; ++i) {
                arr[i] = dis(gen); // RNG
            }
            break;
        case 2: // Generate an array with 1/3 sorted
            for (int i = 0; i < size; ++i) {
                if (i < size / 3)
                    arr[i] = i; // Generate strictly sorted elements up to 1/3 of the size
                else
                    arr[i] = dis(gen);
            }
            break;
        case 3: // Generate an array with 2/3 sorted
            for (int i = 0; i < size; ++i) {
                if ( i < size * 2 / 3)
                    arr[i] = i; // Generate strictly sorted elements from 1/3 to 2/3 of the size
                else
                    arr[i] = dis(gen);
            }
            break;
        default:
            cerr << "Invalid sortedType! Choose 1, 2, or 3." << '\n';
            delete[] arr;
            return nullptr;
    }

    return arr;
}

float* genFloatAR(int size, int sortedType) {
    float* arr = new float[size]; // Dynamic array
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1000.0f);

    switch (sortedType) {
        case 1: // Generate a fully random array
            for (int i = 0; i < size; ++i) {
                arr[i] = dis(gen); // RFG
            }
            break;
        case 2: // Generate an array with 1/3 sorted
            for (int i = 0; i < size; ++i) {
                if (i < size / 3)
                    arr[i] = i * 0.1f; // Generate strictly sorted elements up to 1/3 of the size
                else
                    arr[i] = dis(gen);
            }
            break;
        case 3: // Generate an array with 2/3 sorted
            for (int i = 0; i < size; ++i) {
                if (i < size * 2 / 3)
                    arr[i] = i * 0.1f; // Generate strictly sorted elements from 1/3 to 2/3 of the size
                else
                    arr[i] = dis(gen);
            }
            break;
        default:
            cerr << "Invalid sortedType! Choose 1, 2, or 3." << '\n';
            delete[] arr;
            return nullptr;
    }

    return arr;
}

int* readfile(const char* filename, int& size) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << '\n';
        return nullptr;
    }

    // Read the size of the array
    file >> size;
    if (size <= 0) {
        cerr << "Error: Invalid array size" << '\n';
        return nullptr;
    }

    // Dynamically allocate memory for the array
    int* arr = new int[size];

    // Read integers from the file
    for (int i = 0; i < size; ++i) {
        file >> arr[i];
    }

    file.close();

    return arr;
}

void printArray(int* arr, int size) {
    if (arr == nullptr || size <= 0) {
        cout << "Array is empty." << '\n';
        return;
    }

    // Print the elements of the array
    cout << "Array elements:" << '\n';
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << '\n';
}

void printFloatArray(float* arr, int size) {
    if (arr == nullptr || size <= 0) {
        cout << "Array is empty." << '\n';
        return;
    }

    // Print the elements of the array
    cout << "Array elements:" << '\n';
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << '\n';
}

void bubble(int size, int arr[]) {
    // BSORT on og array
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void merge(int array[], int const left, int const mid, int const right) {
    // Merge sort helper function
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;

    // Create temporary arrays
    int* leftArray = new int[subArrayOne];
    int* rightArray = new int[subArrayTwo];

    // Copy data to temporary arrays leftArray[] and rightArray[]
    for (int i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (int j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    int indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    // Merge the temporary arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        } else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    // Copy the remaining elements of left[], if any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    // Copy the remaining elements of right[], if any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }

    delete[] leftArray;
    delete[] rightArray;
}

void mergeSort(int array[], int const begin, int const end) {
    //Merge sort on og array
    // Merge sort implementation
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void insertionSort(int arr[], int n) {
    // Insert on og array
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void floatSort(float arr[], int n) {
    // Insertion on original float array
    for (int i = 1; i < n; i++) {
        float key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int* bubbleSorted(int size, int arr[]) {
    // "COPY SORT"
    int* sortedArr = new int[size];
    for (int i = 0; i < size; ++i) {
        sortedArr[i] = arr[i];
    }

    // Start measuring time
    StartCounter();
    // Bubble sort the array
    bubble(size, sortedArr);
    // Get the elapsed time
    double time = GetCounter();

    // Write time to file
    ofstream outfile("time.txt", ios::app);
    outfile << "Bubble Sort Time: " << time << '\n';
    outfile.close();

    return sortedArr;
}

int* mergeSorted(int array[], int size) {
    //"COPY SORT"
    int* sortedArr = new int[size];
    for (int i = 0; i < size; ++i) {
        sortedArr[i] = array[i];
    }

    // Start measuring time
    StartCounter();
    // Merge sort the array
    mergeSort(sortedArr, 0, size - 1);
    // Get the elapsed time
    double time = GetCounter();

    // Write time to file
    ofstream outfile("time.txt", ios::app);
    outfile << "Merge Sort Time: " << time << '\n';
    outfile.close();

    return sortedArr;
}

int* insertionSorted(int arr[], int size) {
    //"COPY SORT"
    int* sortedArr = new int[size];
    for (int i = 0; i < size; ++i) {
        sortedArr[i] = arr[i];
    }

    // Start measuring time
    StartCounter();
    // Insertion sort the array
    insertionSort(sortedArr, size);
    // Get the elapsed time
    double time = GetCounter();

    // Write time to file
    ofstream outfile("time.txt", ios::app);
    outfile << "Insertion Sort Time: " << time << '\n';
    outfile.close();

    return sortedArr;
}

float* floatSorted(int size, float* arr) {
    // Create a new array and copy elements from the original array
    float* sortedArr = new float[size];
    for (int i = 0; i < size; ++i) {
        sortedArr[i] = arr[i];
    }

    // Start measuring time
    StartCounter();
    // Sort the float array
    floatSort(sortedArr, size);
    double time = GetCounter();

    // Write time to file
    ofstream outfile("time.txt", ios::app);
    outfile << "Float Sort Time: " << time << '\n';
    outfile.close();

    return sortedArr;
}


int main() {
    int* arr = nullptr;
    int* sortedArr = nullptr;
    float* floatArr = nullptr;
    int choice;
    int size = 0;
    while (true) {

        cout << "Choose an option:" << '\n';
        cout << "1. Read from file" << '\n';
        cout << "2. Generate Integer Array" << '\n';
        cout << "3. Show Array" << '\n';
        cout << "4. Sort Array" << '\n';
        cout << "5. Show Sorted Array" << '\n';
        cout << "6. Generate Float Array" << '\n';
        cout << "7. Sort Float Array" << '\n'; // Added option to sort float array
        cout << "8. Show Float Array" << '\n'; // Added option to show float array
        cout << "9. Sort Integer Array (Original)" << '\n'; // Added option to sort the original array
        cout << "10. Reverse order" << '\n';
        cout << "Enter your choice (1-10): ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string filename;
                cout << "Enter the name of the file: ";
                cin >> filename;
                arr = readfile(filename.c_str(), size);
                if (arr != nullptr) {
                    // Print the size of the array
                    cout << "Size of the array: " << size << '\n';
                    printArray(arr, size);
                }
                break;
            }
            case 2: {
                int sortedType;
                cout << "Enter the size of the integer array: ";
                cin >> size;
                cout << "Enter the type of sorting for the array (1-3): ";
                cin >> sortedType;
                arr = genAR(size, sortedType);
                // Print out the generated array (for demonstration purposes)
                cout << "Generated integer array: ";
                // printArray(arr, size);
                break;
            }
            case 3: {
                printArray(arr, size);
                break;
            }
            case 4: {
                if (arr == nullptr) {
                    cout << "Array is empty. Please load or generate an array first." << '\n';
                    break;
                }
                // Now sort the array
                int sortChoice;
                cout << "Choose a sorting algorithm:" << '\n';
                cout << "1. Bubble Sort" << '\n';
                cout << "2. Merge Sort" << '\n';
                cout << "3. Insertion Sort" << '\n';
                cout << "Enter your choice (1-3): ";
                cin >> sortChoice;
                switch (sortChoice) {
                    case 1: {
                        sortedArr = bubbleSorted(size, arr);
                        cout << "Sorted using Bubble Sort." << '\n';
                        // printArray(sortedArr, size);
                        break;
                    }
                    case 2: {
                        sortedArr = mergeSorted(arr, size);
                        cout << "Sorted using Merge Sort." << '\n';
                        //printArray(sortedArr, size);
                        break;
                    }
                    case 3: {
                        sortedArr = insertionSorted(arr, size);
                        cout << "Sorted using Insertion Sort." << '\n';
                        //printArray(sortedArr, size);
                        break;
                    }
                    default: {
                        cout << "Invalid choice! Please choose a number between 1 and 3." << '\n';
                        break;
                    }
                }
                break;
            }
            case 5: {
                cout << "Sorted array:" << '\n';
                printArray(sortedArr, size);
                break;
            }
            case 6: {
                int sortedType;
                cout << "Enter the size of the float array: ";
                cin >> size;
                cout << "Enter the type of sorting for the array (1-3): ";
                cin >> sortedType;
                floatArr = genFloatAR(size, sortedType);
                    //Print out the generated float array (for demonstration purposes)
                cout << "Generated float array: ";
                // printFloatArray(floatArr, size);
                break;
            }
            case 7: { // Added case to sort float array
                if (floatArr == nullptr) {
                    cout << "Float array is empty. Please generate a float array first." << '\n';
                    break;
                }
                // Now sort the float array
                floatSorted(size,floatArr);
                cout << "Float array sorted." << '\n';
                break;
            }
            case 8: { // Added case to show float array
                if (floatArr == nullptr) {
                    cout << "Float array is empty. Please generate or load a float array first." << '\n';
                    break;
                }
                // Show the float array
                cout << "Float array:" << '\n';
                printFloatArray(floatArr, size);
                break;
            }
             case 9: { // Added case to sort the original integer array with insertion sort
                    if (arr == nullptr) {
                        cout << "Integer array is empty. Please generate or load an integer array first." << '\n';
                        break;
                    }
                    // Now sort the original integer array itself using insertion sort
                    insertionSort(arr, size);
                    cout << "Original array sorted using Insertion Sort." << '\n';
                    break;
        }
            case 10: { // Added case to sort the original integer array in reverse order
                    if (arr == nullptr) {
                        cout << "Integer array is empty. Please generate or load an integer array first." << '\n';
                        break;
                    }
                    // Now sort the original integer array itself in reverse order
                    sort(arr, arr + size, greater<int>());
                    cout << "Original array sorted in reverse order." << '\n';
                    break;
        }

            default: {
                cout << "Invalid choice! Please choose a number between 1 and 9." << '\n';
                break;
            }
        }
    }
}
