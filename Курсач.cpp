#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <conio.h>
#include <windows.h>


using namespace std;

void SetColor(int color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void CorrectInput(fstream &text) {

	text.open("Traning.txt", ios::in);
	if (!text.is_open()) {
		SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY); 
		cerr << "Ошибка!Не удалось открыть файл";
		SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return;
	}

	string comp;
	getline(text, comp, '~');

	char input;
	int pozition = 0;
	cout << comp << endl;
	while (pozition < comp.length()) {
		input = _getch();

		if (input == 27) {
			cout << "\nВыход из тренировки" << endl;
			break;
		}

		else if (input == comp[pozition]) {
			cout << input;
			pozition++;
		}

		else {
			SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY); 
			cout << input; 
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			pozition++;
		}
		
	}

	text.close();
}

void AddText(fstream &text) {
	
    if (!text.is_open()) {
        SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        cerr << "Ошибка! Не удалось открыть файл для записи." << endl;
        SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
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

unsigned int hashing(const string &pass) {
	unsigned int hash = 0;
	for (char c : pass) {
		hash = hash * 31 + c; 
	}
	return hash;
}

void autorization(fstream &user) {

	cout << "Нажмите соответсвующую клавишу для выбора опции"  << endl;
	cout << "1: Войти" << endl;
	cout << "2: Зарегистрироваться" << endl;
		char choose = _getch();

	if (choose == 1) {
		 cout << "Введите логин и пароль:" << endl;
		 cout << "Логин:" << endl;
			 string login;
			 getline(cin, login);
		 cout << "Пароль" << endl;
			 string password;
			 getline(cin, password);
		 cout << "1: Войти" << endl;
			 char choose = _getch();
			 if (choose == 1) {
				 user.open("User.txt", ios::in);
				 if (!user.is_open()) {
					 SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
					 cerr << "Ошибка!Не удалось открыть файл";
					 SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					 return;
				 }
				 bool correctInfo = false;
				 string inputInfo = login + " , " + password;
				 string fileInfo;
				 while (getline(user, fileInfo)) {
					 if (inputInfo == fileInfo) {
						 correctInfo = true;
						 break;
					 }
				}
				 user.close();

				 if (correctInfo) {
					 cout << "Успешный вход!" << endl; // TODO добавить следующие функции, т.е. переход к выбору тренировки или просмотра статистики
				 }
				 else {
					 cout << "Неверный логин или пароль!" << endl; // TODO реализовать возможность повторного ввода данных
				 }

			 }
	}

	if (choose == 2) {
		cout << "Введите следующие данные:" << endl;
		cout << "Логин:" << endl;
			string login;
			getline(cin, login);
		cout << "Пароль:" << endl;
			string password;
			getline(cin, password);
			unsigned int hashedPassword = hashing(password);

		cout << "1: Зарегистрироваться" << endl;
			char choose = _getch();
			if (choose == 1) {
				user.open("User.txt", ios::app);
				if (!user.is_open()) {
					SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
					cerr << "Ошибка!Не удалось открыть файл";
					SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					return;
				}
				else {
					string info = login + " , " + to_string(hashedPassword);
					user << info << endl;
				}

				user.close();
			}

	}

}

void adminAction(fstream &user) {
	 cout << "1: Добавить пользователя" << endl;
	 cout << "2: Удалить пользователя " << endl;
	 cout << "3: Изменить данные пользователя" << endl;
	 cout << "4: Изменить текст для тренировки" << endl;
	 char choose = _getch();

	 if (choose == 1) {
		 user.open("User.txt", ios::in | ios::out);

		 user.close();
	 }
	 if (choose == 2) {
		 user.open("User.txt", ios::in | ios::out);
		 if (!user.is_open()) {
			 SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			 cerr << "Ошибка!Не удалось открыть файл";
			 SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
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
			 cerr << "Ошибка!Не удалось открыть временный файл";
			 SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			 return;
		 }

		 string loginNoDelete;
		 while (getline(user, loginNoDelete, ' ')) { // проверить корректно ли работает считываение только логина, без пароля
			 if (loginNoDelete == loginToDelete) {
				 continue;
			 }
			 temp << loginNoDelete << endl;
		 }

		 temp.close();
		 user.close();

		 if (remove("User.txt") != 0) {
			 SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			 cerr << "Ошибка! Не удалось удалить файл User.txt" << endl;
			 SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		 }
		 else if (rename("temp.txt", "User.txt") != 0) {
			 SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			 cerr << "Ошибка! Не удалось переименовать файл temp.txt" << endl;
			 SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		 }
		 else {
			 cout << "Пользователь успешно удален." << endl;
		 }
		
		 
		
	 }
	 if (choose == 3) {
		 user.open("User.txt", ios::in | ios::out);

		 user.close();
	 }
	 // TODO добавить/вставить функцию 4
}

int main()
{
	setlocale(LC_ALL, "RU");
	fstream text("Traning.txt", ios::app | ios::in);
	AddText(text);
	text.clear();

	CorrectInput(text);

	return 0;
}

