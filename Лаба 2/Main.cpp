#include <iostream>
#include <windows.h>

int* arr = nullptr;
int arr_size = 0;
int min_index = 0;
int max_index = 0;
double average_value = 0;


DWORD WINAPI min_max_thread(LPVOID lpParam) {
    std::cout << "\n[min_max] Поток запущен." << std::endl;

    int min_val = arr[0];
    int max_val = arr[0];
    min_index = 0;
    max_index = 0;

    for (int i = 0; i < arr_size; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
            min_index = i;
        }
        Sleep(7);

        if (arr[i] > max_val) {
            max_val = arr[i];
            max_index = i;
        }
        Sleep(7);
    }

    std::cout << "[min_max] Минимальный элемент: " << min_val << " (индекс " << min_index << ")" << std::endl;
    std::cout << "[min_max] Максимальный элемент: " << max_val << " (индекс " << max_index << ")" << std::endl;

    std::cout << "[min_max] Поток завершил работу.\n" << std::endl;
    return 0;
}

DWORD WINAPI average_thread(LPVOID lpParam) {
    std::cout << "[average] Поток запущен." << std::endl;

    double sum = 0;

    for (int i = 0; i < arr_size; i++) {
        sum += arr[i];

        Sleep(12);
    }

    average_value = sum / arr_size;

    std::cout << "[average] Среднее арифметическое значение: " << average_value << std::endl;

    std::cout << "[average] Поток завершил работу.\n" << std::endl;
    return 0;
}

int main() {

    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");

    std::cout << "Введите размерность массива: ";
    std::cin >> arr_size;

    if (arr_size <= 0) {
        std::cerr << "Размер массива должен быть больше нуля!" << std::endl;
        return 1;
    }

    arr = new int[arr_size];
    std::cout << "Введите элементы массива (через пробел или Enter): " << std::endl;
    for (int i = 0; i < arr_size; i++) {
        std::cout << "arr[" << i << "] = ";
        std::cin >> arr[i];
    }

    HANDLE hMin_Max;
    HANDLE hAverage;

    hMin_Max = CreateThread(NULL, 0, min_max_thread, NULL, 0, NULL);
    if (hMin_Max == NULL) {
        std::cerr << "Ошибка при создании потока min_max! Код: " << GetLastError() << std::endl;
        delete[] arr;
        return 1;
    }

    hAverage = CreateThread(NULL, 0, average_thread, NULL, 0, NULL);
    if (hAverage == NULL) {
        std::cerr << "Ошибка при создании потока average! Код: " << GetLastError() << std::endl;
        CloseHandle(hMin_Max);
        delete[] arr;
        return 1;
    }

    WaitForSingleObject(hMin_Max, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMin_Max);
    CloseHandle(hAverage);


    int avg_int = static_cast<int>(average_value);

    std::cout << "\n[main] Заменяем минимальный элемент (индекс " << min_index
        << ") и максимальный элемент (индекс " << max_index
        << ") на среднее значение (" << avg_int << ")." << std::endl;

    arr[min_index] = avg_int;
    arr[max_index] = avg_int;

    std::cout << "[main] Полученный массив после замены элементов: " << std::endl;
    for (int i = 0; i < arr_size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    delete[] arr;

    std::cout << "[main] Работа программы успешно завершена." << std::endl;
    return 0;
}