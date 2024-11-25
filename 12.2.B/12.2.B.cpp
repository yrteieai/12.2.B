#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>
using namespace std;

enum Kurs { I = 1, II, III, IV, V, VI };
enum Spec { ME, MF, FI, INF, KN };

struct Student {
    union ThirdMark {
        int mark_prog;
        int mark_num_methods;
        int mark_pedagogy;
    };
    char prizv[64];
    int kurs;
    Spec spec;
    int mark_physics;
    int mark_math;
    ThirdMark third_mark;
};

void input_save(const char* fname);
void print_data(const char* fname);
void calculate_marks(const char* fname, int& count5, int& count4, int& count3);
double calculate_percent(const char* fname);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char fname[256];
    char choice;
    do {
        cout << "\nМеню:\n";
        cout << "1 - Введення даних та збереження у файл\n";
        cout << "2 - Відображення даних з файлу\n";
        cout << "3 - Обчислення кількості оцінок з програмування\n";
        cout << "4 - Обчислення процента студентів (фізика і математика >= 4)\n";
        cout << "0 - Вихід\n";
        cout << "Ваш вибір: ";
        cin >> choice;

        switch (choice) {
        case '1':
            cout << "Введіть ім'я файлу: ";
            cin >> fname;
            input_save(fname);
            break;
        case '2':
            cout << "Введіть ім'я файлу: ";
            cin >> fname;
            print_data(fname);
            break;
        case '3': {
            cout << "Введіть ім'я файлу: ";
            cin >> fname;
            int count5, count4, count3;
            calculate_marks(fname, count5, count4, count3);
            cout << "Кількість оцінок з програмування:\n";
            cout << "5: " << count5 << "\n4: " << count4 << "\n3: " << count3 << endl;
            break;
        }
        case '4': {
            cout << "Введіть ім'я файлу: ";
            cin >> fname;
            double percent = calculate_percent(fname);
            cout << "Відсоток студентів: " << percent << "%\n";
            break;
        }
        case '0':
            cout << "Завершення програми.\n";
            break;
        default:
            cout << "Невірний вибір! Спробуйте ще раз.\n";
        }
    } while (choice != '0');

    return 0;
}

void input_save(const char* fname) {
    ofstream fout(fname, ios::binary);
    if (!fout) {
        cerr << "Помилка відкриття файлу для запису.\n";
        return;
    }

    Student student;
    char choice;
    do {
        cout << "\nВведіть дані студента:\n";
        cout << "Прізвище: ";
        cin >> student.prizv;
        cout << "Курс (1-6): ";
        cin >> student.kurs;
        cout << "Спеціальність (0 - МЕ, 1 - МФ, 2 - ФІ, 3 - ІН, 4 - КН): ";
        int spec;
        cin >> spec;
        student.spec = static_cast<Spec>(spec);
        cout << "Оцінка з фізики: ";
        cin >> student.mark_physics;
        cout << "Оцінка з математики: ";
        cin >> student.mark_math;

        if (student.spec == KN) {
            cout << "Оцінка з програмування: ";
            cin >> student.third_mark.mark_prog;
        }
        else if (student.spec == INF) {
            cout << "Оцінка з чисельних методів: ";
            cin >> student.third_mark.mark_num_methods;
        }
        else {
            cout << "Оцінка з педагогіки: ";
            cin >> student.third_mark.mark_pedagogy;
        }

        fout.write(reinterpret_cast<char*>(&student), sizeof(Student));
        cout << "Додати ще одного студента? (Y/N): ";
        cin >> choice;

    } while (choice == 'Y' || choice == 'y');

    fout.close();
}

void print_data(const char* fname) {
    ifstream fin(fname, ios::binary);
    if (!fin) {
        cerr << "Помилка відкриття файлу для читання.\n";
        return;
    }

    Student student;
    cout << "------------------------------------------------------------------------------------\n";
    cout << "| Прізвище       | Курс | Спец.         | Фізика | Математика | Третя оцінка       |\n";
    cout << "------------------------------------------------------------------------------------\n";

    while (fin.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        cout << "| " << setw(14) << left << student.prizv
            << "| " << setw(4) << right << student.kurs
            << "| " << setw(13);

        switch (student.spec) {
        case ME: cout << "МЕ"; break;
        case MF: cout << "МФ"; break;
        case FI: cout << "ФІ"; break;
        case INF: cout << "ІН"; break;
        case KN: cout << "КН"; break;
        }

        cout << "| " << setw(7) << student.mark_physics
            << "| " << setw(11) << student.mark_math;

        if (student.spec == KN) {
            cout << "| " << setw(21) << student.third_mark.mark_prog;
        }
        else if (student.spec == INF) {
            cout << "| " << setw(21) << student.third_mark.mark_num_methods;
        }
        else {
            cout << "| " << setw(21) << student.third_mark.mark_pedagogy;
        }
        cout << " |\n";
    }

    cout << "------------------------------------------------------------------------------------\n";
    fin.close();
}

void calculate_marks(const char* fname, int& count5, int& count4, int& count3) {
    ifstream fin(fname, ios::binary);
    if (!fin) {
        cerr << "Помилка відкриття файлу для читання.\n";
        return;
    }

    count5 = count4 = count3 = 0;
    Student student;

    while (fin.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.spec == KN) {
            if (student.third_mark.mark_prog == 5) count5++;
            else if (student.third_mark.mark_prog == 4) count4++;
            else if (student.third_mark.mark_prog == 3) count3++;
        }
    }

    fin.close();
}

double calculate_percent(const char* fname) {
    ifstream fin(fname, ios::binary);
    if (!fin) {
        cerr << "Помилка відкриття файлу для читання.\n";
        return 0.0;
    }

    int total = 0, count = 0;
    Student student;

    while (fin.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        total++;
        if (student.mark_physics >= 4 && student.mark_math >= 4) {
            count++;
        }
    }

    fin.close();
    return total > 0 ? static_cast<double>(count) / total * 100 : 0.0;
}
