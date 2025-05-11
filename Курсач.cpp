#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <chrono>


using namespace std;

string getValidInput(string &input) {
	int i = 0;
	bool validInfo = true;
	while (i <= input.length()) {
		if (!((input[i] <= 57 &&  input[i] >= 48) || (input[i] <= 90 && input[i] >= 65) || (input[i] <= 122 && input[i] >= 97) || input[i] == 95)) {
			validInfo = false;
			break;
		}
		i++;
	}
	if (validInfo) {
		cout << "Использованы запрещенные символы!" << endl <<
		"Можно использовать только буквы, цифры и нижнее подчеркивание!" << endl;
	}
	else {
		return input;
	}
}

void SetCursorPosition(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void Error(string err) {
	SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cerr << err << endl;
	SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void CorrectInputTraning(fstream &text) {

	text.open("Traning.txt", ios::in);
	if (!text.is_open()) {
		Error("Ошибка! Не удалось открыть файл!");
		return;
	}

	string comp;
	getline(text, comp, '~');
	
	double fall = 0, all = 0;

	system("cls");

	char input;
	int pozition = 0;
	cout << comp << endl;

	SetCursorPosition(0, 0);
	auto startTime = chrono::high_resolution_clock::now();

	while (pozition < comp.length()) {
		input = _getch();

		if (input == 27) {
			cout << "\nВыход из тренировки" << endl;
			break;
		}

		else if (input == comp[pozition]) {
			SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); 
			cout << input;
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			pozition++;
			all++;
		}

		else {
			SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY); 
			cout << comp[pozition]; 
			SetCursorPosition(pozition, 0);
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			fall++;
			all++;
		}
	}
	auto endTime = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();
	auto speed = round((comp.length() / static_cast<double>(duration)) * 60);

	double prcor = 100 - (fall/all) * 100;

	text.close();
	cout << endl << "Поздравляем!Уровень пройден!" << endl;
	cout << "Процент корректного ввода: " << round(prcor) << "%" << endl;
	cout << "Скорость печати: " << speed << " символов в минуту." << endl;

}

void AddText(fstream &text) {
	
    if (!text.is_open()) {
		Error("Ошибка! Не удалось открыть файл!");
        return;
    }

    cout << "Добавьте текст для тренировки (закончив текст символом ~)" << endl;
    string newtext;
    getline(cin, newtext, '~');

    text.seekp(0, ios::end);
    text << newtext << '~';
    
    text.close();

    cout << "Текст успешно записан в файл" << endl;
}

  string hashing(string &pass) {
    int hash = 0;
	for (char c : pass) {
		hash = hash * 31 + c; 
	}
	string stringHash = to_string(hash);
	return stringHash;
}

void autorization(fstream &user) {
	while(true) {
	cout << "Нажмите соответсвующую клавишу для выбора опции"  << endl;
	cout << "1: Войти" << endl;
	cout << "2: Зарегистрироваться" << endl;
		char choose = _getch();

		if (choose == '1') {
			cout << "Введите логин и пароль:" << endl;

			cout << "Логин:" << endl;
			string login;
			getline(cin, login);
			getValidInput(login);

			cout << "Пароль:" << endl;
			string password;
			getline(cin, password);
			getValidInput(password);

			cout << "1: Войти" << endl;
			char choose = _getch();

			if (choose == '1') {

				if (!user.is_open()) {
					Error("Ошибка! Не удалось открыть файл!");
					return;
				}

				bool correctInfo = false;
				string inputInfo = login + "," + hashing(password);
				string fileInfo;

				while (getline(user, fileInfo)) {
					if (inputInfo == fileInfo) {
						correctInfo = true;
						break;
					}
				}

				if (correctInfo) {
					cout << "Успешный вход!" << endl;
					break;
					// TODO добавить следующие функции, т.е. переход к выбору тренировки или просмотра статистики
				}
				else {
					cout << "Неверный логин или пароль! Попробуйте снова: " << endl;
				}
			}
		}
	

		if (choose == '2') {
			cout << "Введите следующие данные:" << endl;

			cout << "Логин:" << endl;
			string login;
			getline(cin, login);
			getValidInput(login);

			cout << "Пароль:" << endl;
			string password;
			getline(cin, password);
			getValidInput(password);
			string hashedPassword = hashing(password);

			cout << "1: Зарегистрироваться" << endl;
			char choose = _getch();

			if (choose == '1') {
				if (!user.is_open()) {
					Error("Ошибка! Не удалось открыть файл!");
					return;
				}
				else {
					string line;
					bool exis = false;

					while (getline(user, line)) {
						size_t comPos = line.find(',');
						if (comPos != string::npos) {
							string teLogin = line.substr(0, comPos);
							if (teLogin == login) {
								exis = true;
								break;
							}
						}
					}
					if (exis) {
						cout << "Пользователь с таким логином уже существет!" << endl;
					}
					if(!exis) {
						user.clear();
						user.seekp(0, ios::end);

						string info = login + "," + hashedPassword;
						user << info << endl;

						cout << "Регистрация успешна!" << endl;
						break;
					}
				}
			}
		}
	}
}

void adminAction(fstream& user) {
	cout << "1: Добавить пользователя" << endl;
	cout << "2: Удалить пользователя " << endl;
	cout << "3: Изменить данные пользователя" << endl;
	cout << "4: Изменить текст для тренировки" << endl;
	char choose = _getch();

	if (choose == 1) {
		user.open("User.txt", ios::in | ios::out);
		if (!user.is_open()) {
			Error("Ошибка! Не удалось открыть файл!");
			return;
		}

		cout << "Введите логин нового пользователя:" << endl;
		string newLogin;
		getline(cin, newLogin);
		getValidInput(newLogin);

		cout << "Введите пароль нового пользователя:" << endl;
		string newPassword;
		getline(cin, newPassword);
		getValidInput(newPassword);
		string hashedPassword = hashing(newPassword);

		user << newLogin << "," << hashedPassword << endl;
		cout << "Пользователь успешно добавлен." << endl;
		user.close();
	}

	if (choose == 2) {
		user.open("User.txt", ios::in | ios::out);
		if (!user.is_open()) {
			Error("Ошибка! Не удалось открыть файл!");
			return;
		}

		/*   cout << user.rdbuf(); можно реализовать через эту функцию вывод user*/

		string userInfo;
		while (getline(user, userInfo)) {
			int i = 1;
			cout << i << ": " << userInfo << endl;
			i++;
		}


		cout << "Введите логин пользователя, которого хотите удалить" << endl;
		string loginToDelete;
		getline(cin, loginToDelete);

		fstream temp;
		temp.open("temp.txt", ios::out);
		if (!temp.is_open()) {
			SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			Error("Ошибка!Не удалось открыть временный файл");
			return;
		}

		string loginNoDelete;
		while (getline(user, loginNoDelete, ' ')) { // Проверить корректно ли работает считывание логина
			if (loginNoDelete == loginToDelete) {
				continue;
			}
			temp << loginNoDelete << endl;
		}

		temp.close();
		user.close();

		if (remove("User.txt") != 0) {
			Error("Ошибка! Не удалось удалить файл User.txt");
		}
		else if (rename("temp.txt", "User.txt") != 0) {
			Error("Ошибка! Не удалось переименовать файл temp.txt");
		}
		else {
			cout << "Пользователь успешно удален." << endl;
		}



	}
	if (choose == 3) {
		user.open("User.txt", ios::in | ios::out);
		if (!user.is_open()) {
			Error("Ошибка! Не удалось открыть файл!");
			return;
		}

		cout << "Введите логин пользователя, данные которого хотите изменить: " << endl;
		string loginToEdit;
		getline(cin, loginToEdit);

		fstream temp;
		temp.open("temp.txt", ios::out);
		if (!temp.is_open()) {
			Error("Ошибка!Не удалось открыть временный файл");
			return;
		}

		string userInfo;
		bool found = false;
		while (getline(user, userInfo)) {
			size_t pos = userInfo.find(loginToEdit);
			if (pos != string::npos) {
				found = true;
				cout << "Введите новый логин:" << endl;
				string newLogin;
				getline(cin, newLogin);
				getValidInput(newLogin);

				cout << "Введите новый пароль:" << endl;
				string newPassword;
				getline(cin, newPassword);
				getValidInput(newPassword);
				string hashedPassword = hashing(newPassword);

				temp << newLogin << " , " << hashedPassword << endl;
			}
			else {
				temp << userInfo << endl;
			}
		}

		if (!found) {
			cout << "Пользователь с таким логином не найден." << endl;
		}
		else {
			cout << "Данные пользователя успешно изменены." << endl;
		}

		temp.close();
		user.close();

		if (remove("User.txt") != 0) {
			Error("Ошибка! Не удалось удалить файл User.txt");
		}
		else if (rename("temp.txt", "User.txt") != 0) {
			Error("Ошибка! Не удалось переименовать файл temp.txt");
		}
	}
	if (choose == 4) {
		fstream text("TrainingText.txt", ios::in | ios::out | ios::app);
		AddText(text);
	}
}

struct UserData {
	string login;
	string hashedPassword;
	int currentLevel;
	double averageSpeed;
	double accuracy;
};


void saveUserData(const UserData& userData) {
	ofstream userFile("UserData.txt", ios::app);
	if (!userFile.is_open()) {
		cerr << "Ошибка! Не удалось открыть файл для записи данных пользователя" << endl;
		return;
	}
	userFile << userData.login << "," << userData.hashedPassword << ","
		<< userData.currentLevel << "," << userData.averageSpeed << ","
		<< userData.accuracy << endl;
	userFile.close();
}

bool loadUserData(UserData& userData, const string& login) {
	ifstream userFile("UserData.txt");
	if (!userFile.is_open()) {
		cerr << "Ошибка! Не удалось открыть файл для чтения данных пользователя" << endl;
		return false;
	}

	string tempLogin, tempPassword;
	int level;
	double speed, acc;
	char delimiter;

	while (userFile >> tempLogin >> delimiter >> tempPassword >> delimiter >>
		level >> delimiter >> speed >> delimiter >> acc) {
		if (tempLogin == login) {
			userData = { tempLogin, tempPassword, level, speed, acc };
			userFile.close();
			return true;
		}
	}
	userFile.close();
	return false;
}

void trainingMode(UserData& userData) {
	cout << "Выберите режим тренировки:" << endl;;
	cout << "1: Курс" << endl;
	cout << "2: Случайный текст" << endl;
	char choice = _getch();

	if (choice == '1') {
		// Логика для прохождения курса
		cout << "Вы на уровне: " << userData.currentLevel << endl;
		// ... реализация логики прохождения уровня
		userData.currentLevel++; // предположим, что уровень пройден успешно
	}
	else if (choice == '2') {
		// Логика для тренировки со случайным текстом
		cout << "Тренировка со случайным текстом\n";
		// ... реализация логики выбора случайного текста
	}
	else {
		cout << "Некорректный выбор!" << endl;
	}
}

void viewStatistics(const UserData& userData) {
	cout << "Статистика пользователя: " << userData.login << endl;
	cout << "Текущий уровень: " << userData.currentLevel << endl;
	cout << "Средняя скорость: " << userData.averageSpeed << " символов в минуту" << endl;
	cout << "Точность: " << userData.accuracy << "%" << endl;
}

void userAction(UserData& userData) {
	while (true) {
		cout << "Выберите действие: " << endl;
		cout << "1: Просмотр статистики" << endl;
		cout << "2: Выбор режима тренировки" << endl;
		cout << "3: Выход" << endl;
		char choice = _getch();

		if (choice == '1') {
			viewStatistics(userData);
		}
		else if (choice == '2') {
			trainingMode(userData);
		}
		else if (choice == '3') {
			saveUserData(userData);
			break;
		}
		else {
			cout << "Некорректный выбор!" << endl;
		}
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "1.Пользователь: " << endl << "2.Администратор: " << endl;
	char sel = _getch();
	if (sel == '1') {
		fstream user("User.txt", ios::in | ios::out | ios::app);
		if (!user.is_open()) {
			cerr << "Ошибка: не удалось открыть файл User.txt" << endl;
			return 1;
		}

		autorization(user);

		user.close();
	}
	if (sel == '2') {
		fstream admin("Admin.txt", ios::in | ios::out | ios::app);
		if (!admin.is_open()) {
			cerr << "Ошибка: не удалось открыть файл User.txt" << endl;
			return 1;
		}

		autorization(admin);

		admin.close();
	}

	

	/*fstream text("Traning.txt", ios::app | ios::in);
	AddText(text);
	text.clear();

	CorrectInputTraning(text);*/

	return 0;
}

