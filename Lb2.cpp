

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;
const int INF = 1e9;



void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
// Звичайна функція для показу будь-якої з двох матриць
void displayMatrix(const vector<vector<int>>& grid, bool isDistance) {
    int sz = grid.size();
    cout << "     ";
    for (int col = 0; col < sz; ++col) cout << setw(4) << col + 1;
    cout << "\n    " << string(sz * 4 + 2, '-') << "\n";

    for (int r = 0; r < sz; ++r) {
        cout << setw(2) << r + 1 << " |";
        for (int c = 0; c < sz; ++c) {
            if (isDistance && grid[r][c] == INF) cout << setw(4) << "INF";
            else if (!isDistance && grid[r][c] == -1) cout << setw(4) << "-";
            else if (!isDistance) cout << setw(4) << grid[r][c] + 1;
            else cout << setw(4) << grid[r][c];
        }
        cout << "\n";
    }
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "ukr");

    while (true) {
        cout << "\n=== Обчислення найкоротших шляхів ===\n";
        int size;
        cout << "Кількість вершин (1-20): ";
        if (!(cin >> size) || size < 1 || size > 20) break;

        vector<vector<int>> W(size, vector<int>(size));
        vector<vector<int>> P(size, vector<int>(size, -1));

        cout << "Введіть матрицю по рядках (значення через пробіл, INF/999 - немає ребра):\n\n";
        for (int row = 0; row < size; ++row) {
            bool rowValid = false;
            cout << "\nРядок " << row + 1 << " ";
            while (!rowValid) {
                

                string line;
                getline(cin, line);

                
                if (line.empty()) {
                    continue;
                }

                stringstream ss(line);
                string token;
                int col = 0;
                bool lineError = false;

                // Зчитуємо значення по одному із рядка
                while (ss >> token) {
                    if (col >= size) {
                        lineError = true; // Елементів виявилося більше, ніж size
                        break;
                    }

                    if (token == "INF" || token == "inf" || token == "∞" || token == "999") {
                        W[row][col] = INF;
                    }
                    else {
                        try {
                            W[row][col] = stoi(token);
                        }
                        catch (...) {
                            lineError = true; // Замість числа введено літери/символи
                            break;
                        }
                    }
                    col++;
                }

                // Перевіряємо, чи введено рівно стільки елементів, скільки потрібно
                if (lineError || col < size) {
                    cout << "Помилка! Неправильна кількість або формат елементів у рядку. Спробуйте ще раз.\n";
                }
                else {
                    // Якщо все успішно, налаштовуємо матрицю шляхів P для цього рядка
                    for (int c = 0; c < size; ++c) {
                        if (W[row][c] != INF && row != c) P[row][c] = c;
                        else if (row == c) P[row][c] = row;
                    }
                    rowValid = true; // Рядок успішно прийнято, виходимо з циклу while
                }
            }
        }

        // Алгоритм Флойда-Уоршелла
        for (int m = 0; m < size; ++m) {
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (W[i][m] < INF && W[m][j] < INF && W[i][m] + W[m][j] < W[i][j]) {
                        W[i][j] = W[i][m] + W[m][j];
                        P[i][j] = P[i][m];
                    }
                }
            }
        }

        // Перевірка на від'ємний цикл
        bool minusCycle = false;
        for (int i = 0; i < size; ++i) {
            if (W[i][i] < 0) minusCycle = true;
        }

        if (minusCycle) {
            cout << "\nПомилка: присутній від'ємний цикл.\n";
            continue;
        }

        while (true) {
            cout << "\n1 - Усі матриці\n2 - Шлях між двома вершинами\n3 - Нова матриця\n4 - Вихід\nВаш вибір: ";
            int action;
            if (!(cin >> action) || action == 4) return 0;
            if (action == 3) break;

            if (action == 1) {
                cout << "\nМатриця мінімальних відстаней (D):\n";
                displayMatrix(W, true);
                cout << "\nМатриця найкоротших шляхів (P):\n";
                displayMatrix(P, false);
            }
            else if (action == 2) {
                int start, finish;
                cout << "Початкова вершина (1-" << size << "): ";
                cin >> start;
                cout << "Кінцева вершина (1-" << size << "): ";
                cin >> finish;
                start--; finish--;

                if (W[start][finish] == INF) {
                    cout << "Шлях відсутній.\n";
                }
                else {
                    cout << "Мінімальна довжина: " << W[start][finish] << "\nМаршрут: " << start + 1;
                    int step = start;
                    while (step != finish) {
                        step = P[step][finish];
                        cout << " -> " << step + 1;
                    }
                    cout << "\n";
                }
            }
        }
    }
    return 0;
}