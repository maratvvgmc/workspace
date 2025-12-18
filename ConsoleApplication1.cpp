#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <windows.h> // Для настройки кодировки в Windows

using namespace std;

// Функция для настройки русской кодировки в Windows
void setRussianEncoding() {
    // Устанавливаем кодовую страницу для поддержки кириллицы
    SetConsoleCP(1251);  // Установка кодовой страницы ввода
    SetConsoleOutputCP(1251); // Установка кодовой страницы вывода
    setlocale(LC_ALL, "Russian");
}

// Класс Абитуриент
class Abiturient {
private:
    string lastName;
    string firstName;
    string middleName;
    string faculty;
    int mathScore;
    int rusScore;
    int physicsScore;
    int attestatScore;

public:
    // Конструктор
    Abiturient(string ln, string fn, string mn, string fac)
        : lastName(ln), firstName(fn), middleName(mn), faculty(fac),
        mathScore(0), rusScore(0), physicsScore(0), attestatScore(0) {
    }

    // Геттеры и сеттеры
    string getLastName() const { return lastName; }
    string getFirstName() const { return firstName; }
    string getMiddleName() const { return middleName; }
    string getFaculty() const { return faculty; }

    int getMathScore() const { return mathScore; }
    int getRusScore() const { return rusScore; }
    int getPhysicsScore() const { return physicsScore; }
    int getAttestatScore() const { return attestatScore; }

    int getTotalScore() const { return mathScore + rusScore + physicsScore + attestatScore; }

    void setMathScore(int score) { mathScore = score; }
    void setRusScore(int score) { rusScore = score; }
    void setPhysicsScore(int score) { physicsScore = score; }
    void setAttestatScore(int score) { attestatScore = score; }

    // Метод для отображения информации
    string getInfo() const {
        return lastName + " " + firstName + " " + middleName +
            " | Факультет: " + faculty +
            " | Сумма баллов: " + to_string(getTotalScore());
    }
};

// Класс Ведомость
class Vedomost {
private:
    string faculty;
    int planMest;
    vector<Abiturient> abiturients;

public:
    // Конструктор
    Vedomost(string fac, int plan) : faculty(fac), planMest(plan) {}

    // Метод добавления абитуриента
    void addAbiturient(const Abiturient& abiturient) {
        if (abiturient.getFaculty() != faculty) {
            cout << "Абитуриент не соответствует факультету ведомости." << endl;
            return;
        }
        abiturients.push_back(abiturient);
        cout << "Абитуриент добавлен." << endl;
    }

    // Метод отображения ведомости
    void showVedomost() const {
        cout << "\nВедомость факультета: " << faculty << endl;
        cout << "План набора: " << planMest << endl;
        cout << "Список абитуриентов:" << endl;

        if (abiturients.empty()) {
            cout << "  Нет абитуриентов." << endl;
            return;
        }

        for (size_t i = 0; i < abiturients.size(); i++) {
            cout << "  " << i + 1 << ". " << abiturients[i].getInfo() << endl;
        }
    }

    // Метод получения списка зачисленных
    vector<Abiturient> getZachislennye() const {
        vector<Abiturient> sorted = abiturients;

        // Сортировка по убыванию суммы баллов
        sort(sorted.begin(), sorted.end(),
            [](const Abiturient& a, const Abiturient& b) {
                return a.getTotalScore() > b.getTotalScore();
            });

        // Возвращаем первых planMest абитуриентов
        int count = (planMest < (int)sorted.size()) ? planMest : (int)sorted.size();
        return vector<Abiturient>(sorted.begin(), sorted.begin() + count);
    }

    // Геттеры
    string getFaculty() const { return faculty; }
    int getPlanMest() const { return planMest; }
    const vector<Abiturient>& getAbiturients() const { return abiturients; }

    // Безопасный доступ к абитуриенту
    Abiturient& getAbiturient(int index) {
        if (index >= 0 && index < (int)abiturients.size()) {
            return abiturients[index];
        }
        static Abiturient empty("", "", "", "");
        return empty;
    }

    size_t getAbiturientCount() const { return abiturients.size(); }

    // Очистка ведомости
    void clear() {
        abiturients.clear();
        cout << "Ведомость очищена." << endl;
    }
};

// Функция для безопасного ввода целого числа
int getIntInput(const string& prompt) {
    int value;
    cout << prompt;

    while (true) {
        string input;
        getline(cin, input);

        // Проверяем, пустая ли строка
        if (input.empty()) {
            cout << "Ввод не может быть пустым. Попробуйте снова: ";
            continue;
        }

        // Пробуем преобразовать строку в число
        bool valid = true;
        value = 0;

        for (char c : input) {
            if (c < '0' || c > '9') {
                valid = false;
                break;
            }
            value = value * 10 + (c - '0');
        }

        if (valid) {
            return value;
        }
        else {
            cout << "Ошибка ввода. Пожалуйста, введите целое число: ";
        }
    }
}

// Функция для безопасного ввода строки
string getStringInput(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);

    // Удаляем лишние пробелы в начале и конце
    size_t start = value.find_first_not_of(" \t\n\r");
    if (start != string::npos) {
        value = value.substr(start);
    }

    size_t end = value.find_last_not_of(" \t\n\r");
    if (end != string::npos) {
        value = value.substr(0, end + 1);
    }

    return value;
}

// Основные функции меню
void createVedomost(Vedomost*& vedomost) {
    if (vedomost != nullptr) {
        delete vedomost;
        vedomost = nullptr;
    }

    string faculty = getStringInput("Введите название факультета: ");
    if (faculty.empty()) {
        cout << "Название факультета не может быть пустым!" << endl;
        return;
    }

    int plan = getIntInput("Введите план набора: ");
    if (plan <= 0) {
        cout << "План набора должен быть положительным числом!" << endl;
        return;
    }

    vedomost = new Vedomost(faculty, plan);
    cout << "Ведомость для факультета '" << faculty << "' создана." << endl;
}

void addAbiturient(Vedomost* vedomost) {
    if (vedomost == nullptr) {
        cout << "Сначала создайте ведомость." << endl;
        return;
    }

    string lastName = getStringInput("Фамилия: ");
    if (lastName.empty()) {
        cout << "Фамилия не может быть пустой!" << endl;
        return;
    }

    string firstName = getStringInput("Имя: ");
    if (firstName.empty()) {
        cout << "Имя не может быть пустым!" << endl;
        return;
    }

    string middleName = getStringInput("Отчество: ");
    if (middleName.empty()) {
        cout << "Отчество не может быть пустым!" << endl;
        return;
    }

    Abiturient abiturient(lastName, firstName, middleName, vedomost->getFaculty());
    vedomost->addAbiturient(abiturient);
}

void enterScores(Vedomost* vedomost) {
    if (vedomost == nullptr) {
        cout << "Ведомость не создана." << endl;
        return;
    }

    if (vedomost->getAbiturientCount() == 0) {
        cout << "Нет абитуриентов для ввода баллов." << endl;
        return;
    }

    cout << "Выберите абитуриента:" << endl;
    for (size_t i = 0; i < vedomost->getAbiturientCount(); i++) {
        cout << i + 1 << ". " << vedomost->getAbiturient(i).getInfo() << endl;
    }

    int index = getIntInput("Номер абитуриента: ");
    if (index < 1 || index >(int)vedomost->getAbiturientCount()) {
        cout << "Неверный номер." << endl;
        return;
    }

    Abiturient& a = vedomost->getAbiturient(index - 1);

    cout << "\nВвод баллов для " << a.getLastName() << " "
        << a.getFirstName() << " " << a.getMiddleName() << ":" << endl;

    a.setMathScore(getIntInput("Балл по математике (0-100): "));
    a.setRusScore(getIntInput("Балл по русскому языку (0-100): "));
    a.setPhysicsScore(getIntInput("Балл по физике (0-100): "));
    a.setAttestatScore(getIntInput("Балл аттестата (0-100): "));

    cout << "Баллы введены. Итоговая сумма: " << a.getTotalScore() << endl;
}

void showZachislennye(Vedomost* vedomost) {
    if (vedomost == nullptr) {
        cout << "Ведомость не создана." << endl;
        return;
    }

    vector<Abiturient> zach = vedomost->getZachislennye();
    cout << "\nЗачисленные на факультет " << vedomost->getFaculty() << ":" << endl;

    if (zach.empty()) {
        cout << "Нет зачисленных абитуриентов." << endl;
        return;
    }

    for (size_t i = 0; i < zach.size(); i++) {
        cout << "  " << i + 1 << ". " << zach[i].getInfo() << endl;
    }
}

// Главное меню
void displayMenu(Vedomost* vedomost) {
    cout << "\n=== СИСТЕМА ПРИЕМНАЯ КОМИССИЯ ===" << endl << endl;

    if (vedomost == nullptr) {
        cout << "1. Создать ведомость факультета" << endl;
        cout << "6. Выход" << endl;
    }
    else {
        cout << "1. Очистить ведомость и создать новую" << endl;
        cout << "2. Добавить абитуриента" << endl;
        cout << "3. Показать ведомость" << endl;
        cout << "4. Показать зачисленных" << endl;
        cout << "5. Ввести баллы абитуриента" << endl;
        cout << "6. Выход" << endl;
    }

    cout << "\nВыберите действие: ";
}

// Очистка экрана
void clearScreen() {
    system("cls");
}

// Основная функция
int main() {
    // Настройка русской кодировки
    setRussianEncoding();

    Vedomost* vedomost = nullptr;
    int choice = 0;

    do {
        clearScreen();
        displayMenu(vedomost);

        string input;
        getline(cin, input);

        // Проверяем, что ввод не пустой
        if (input.empty()) {
            cout << "Ошибка: введите число от 1 до 6." << endl;
            cout << "\nНажмите Enter для продолжения...";
            cin.get();
            continue;
        }

        // Преобразуем ввод в число
        bool valid = true;
        choice = 0;

        for (char c : input) {
            if (c < '0' || c > '9') {
                valid = false;
                break;
            }
            choice = choice * 10 + (c - '0');
        }

        if (!valid) {
            cout << "Ошибка: введите число от 1 до 6." << endl;
            cout << "\nНажмите Enter для продолжения...";
            cin.get();
            continue;
        }

        switch (choice) {
        case 1:
            createVedomost(vedomost);
            break;
        case 2:
            if (vedomost == nullptr) {
                cout << "Сначала создайте ведомость (выберите пункт 1)." << endl;
            }
            else {
                addAbiturient(vedomost);
            }
            break;
        case 3:
            if (vedomost != nullptr) {
                vedomost->showVedomost();
            }
            else {
                cout << "Ведомость не создана. Сначала создайте ведомость (выберите пункт 1)." << endl;
            }
            break;
        case 4:
            if (vedomost != nullptr) {
                showZachislennye(vedomost);
            }
            else {
                cout << "Ведомость не создана. Сначала создайте ведомость (выберите пункт 1)." << endl;
            }
            break;
        case 5:
            if (vedomost != nullptr) {
                enterScores(vedomost);
            }
            else {
                cout << "Ведомость не создана. Сначала создайте ведомость (выберите пункт 1)." << endl;
            }
            break;
        case 6:
            cout << "Выход из программы..." << endl;
            break;
        default:
            cout << "Неверный выбор. Пожалуйста, выберите действие от 1 до 6." << endl;
            break;
        }

        if (choice != 6) {
            cout << "\nНажмите Enter для продолжения...";
            cin.get();
        }

    } while (choice != 6);

    // Освобождаем память
    if (vedomost != nullptr) {
        delete vedomost;
        vedomost = nullptr;
    }

    return 0;
}