#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <windows.h> 

using namespace std;
using namespace chrono;

// Функция записи матрицы в файл
void writeMatrixToFile(const float* mat, int size, const string& filename)
{
    ofstream fout(filename);
    if (!fout.is_open())
    {
        cerr << "Не удалось открыть файл " << filename << " для записи" << endl;
        return;
    }
    fout << fixed << setprecision(10); // фиксированный формат, 10 знаков после запятой
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fout << mat[i * size + j];
            if (j < size - 1) fout << " ";
        }
        fout << endl;
    }
    fout.close();
}

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    // Задаём список размеров матриц
    vector<int> sizes = { 200, 400, 800, 1200, 1600, 2000, 2400, 2800, 3200, 3600, 4000 };

    // Открываем файл для записи результатов времени
    ofstream timeFile("timings.txt");
    if (!timeFile.is_open())
    {
        cerr << "Не удалось создать файл timings.txt" << endl;
        return -1;
    }
    timeFile << "N\ttime(s)" << endl; // заголовок

    // Проходим по всем размерам
    for (int N : sizes)
    {
        cout << "Обработка N = " << N << " ..." << endl;

        // Выделение памяти
        float* A = new float[N * N];
        float* B = new float[N * N];
        float* C = new float[N * N];

        // Инициализация матриц A и B 
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
            {
                A[i * N + j] = (i + 1) * (j + 1);
                B[i * N + j] = (i + 1) + 2 * (j + 1);
            }

        // Сохраняем исходные матрицы в файлы с размером в имени
        string fileA = "A_" + to_string(N) + ".txt";
        string fileB = "B_" + to_string(N) + ".txt";
        writeMatrixToFile(A, N, fileA);
        writeMatrixToFile(B, N, fileB);

        // Замер времени умножения
        auto start = high_resolution_clock::now();

        // Последовательное умножение матриц
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
            {
                float sum = 0.0f;
                for (int k = 0; k < N; k++)
                {
                    sum += A[i * N + k] * B[k * N + j];
                }
                C[i * N + j] = sum;
            }

        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;
        double seconds = elapsed.count();



        // Сохраняем результирующую матрицу
        string fileC = "C_" + to_string(N) + ".txt";
        writeMatrixToFile(C, N, fileC);

        // Записываем время и объём в файл
        timeFile << N << "\t" << seconds << "\t" << endl;

        // Освобождаем память
        delete[] A;
        delete[] B;
        delete[] C;

        cout << "Готово N = " << N << ", время = " << seconds << " с" << endl;
    }

    timeFile.close();
    cout << "Все измерения завершены. Результаты сохранены в timings.txt" << endl;
    return 0;
}