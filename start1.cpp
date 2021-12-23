#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
using namespace std;

#include "composite.h"
#include "unitFactory.h"

//отрисовка объекта по айди
void drawUnit(int id)
{
	switch (id)
	{
	case 1:
		cout << "\x1b[32mW\x1b[37m";
		break;
	case 2:
		cout << "\x1b[33mW\x1b[37m";
		break;
	case 3:
		cout << "\x1b[32mS\x1b[37m";
		break;
	case 4:
		cout << "\x1b[33mS\x1b[37m";
		break;
	case 5:
		cout << "\x1b[32mM\x1b[37m";
		break;
	case 6:
		cout << "\x1b[33mM\x1b[37m";
		break;
	case 7:
		cout << "B";
		break;
	default:
		cout << "?";
		break;
	}
}

int idField = 0;
//класс поля
class Field
{
public:
	string name;
	int id;
	int maxX;
	int maxY;
	int maxObj = (maxX * maxY) / 2;
	UnitList* unitList;
	UnitList* baseList;
	UnitList* landList;
	Unit* biome;
	//надо пофиксить
	void recordObj(int**& Arr, UnitList*& list)
	{
		if (list)
		{
			for (size_t i = 0; i < list->getSize(); i++)
			{
				int tempX = list->getUnit(i + 1)->getX();
				int tempY = list->getUnit(i + 1)->getY();
				if ((tempY > 0) && (tempY <= maxY) && (tempX > 0) && (tempX <= maxX)) { //если внутри поля
					Arr[tempY - 1][tempX - 1] = list->getUnit(i + 1)->getRoleId();
				}
			}
		}
	}
	void show() {
		if ((unitList || baseList) && (unitList->getSize() + baseList->getSize() < maxObj))
		{
			cout << "Количество юнитов превышает допустимое значение!" << endl;
			return;
		}
		int** Arr = new int* [maxY];
		for (int i = 0; i < maxY; ++i)
		{
			Arr[i] = new int[maxX]();
		} //создаём двумерный массив

		//запоминаем, где стоят юниты
		if (unitList) recordObj(Arr, unitList);
		if (baseList) recordObj(Arr, baseList);
		if (landList) recordObj(Arr, landList);

		//вывод
		for (int i = 0; i < maxX + 2; i++) cout << 'X';
		cout << '\n';
		for (int j = 0; j < maxY; j++)
		{
			for (int i = 0; i < maxX; i++)
			{
				if (i == 0) cout << 'X';
				if (Arr[j][i]) drawUnit(Arr[j][i]);
				else if (biome) biome->draw();
				else cout << ' ';
				if (i == maxX - 1) cout << 'X' << endl;
			}
		}
		for (int i = 0; i < maxX + 2; i++) cout << 'X';
		cout << '\n';

		for (int i = 0; i < maxY; ++i)
		{
			delete[] Arr[i];
		}
		delete[] Arr; //удаляем двумерный массив
	}
	void setXY(int x, int y) {
		maxX = x;
		maxY = y;
	}
	void setUnitList(UnitList* listObj) {
		unitList = listObj;
	}
	void setBaseList(UnitList* listObj) {
		baseList = listObj;
	}
	void setLandList(UnitList* listObj) {
		landList = listObj;
	}
	void setBiome(Unit* obj) {
		biome = obj;
	}
	int getId()
	{
		return id;
	}
	Field(const Field& F)
	{
		if (F.name != "") name = F.name + "1";
		else name = "";
		id = ++idField;
		maxX = F.maxX;
		maxY = F.maxY;
		if (unitList) unitList = (F.unitList)->getClone();
		if (baseList) baseList = (F.baseList)->getClone();
		if (landList) landList = (F.landList)->getClone();

	}
	Field();
};

Field::Field() {
	name = "";
	id = ++idField;
	maxX = 10;
	maxY = 6;
	unitList = new UnitList;
	baseList = new UnitList;
	landList = new UnitList;
	biome = new Unit;
}

//== Список команд == 
void moveUnit(Field& field, int id)
{
	Unit* hero = field.unitList->getUnit(id);
	bool check1 = false; //выход из подменю
	do {
		system("cls");
		field.show();
		char sw1 = ' ';
		check1 = true;
		sw1 = ' ';
		cout << "\nВыберите куда шагнуть юниту:\n";
		cout << "1 2 3\n";
		cout << "4 Х 5\n";
		cout << "6 7 8\n";
		cout << "[0] Вернуться назад\n";
		cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

		cin >> sw1;
		while (cin.get() != '\n') { sw1 = ' '; };

		switch (sw1)
		{
		case '1': //[1] -1 -1
			hero->move(-1, -1);
			break;
		case '2': //[2] 0 -1
			hero->move(0, -1);
			break;
		case '3': //[3] 1 -1
			hero->move(1, -1);
			break;
		case '4': //[1] -1 0
			hero->move(-1, 0);
			break;
		case '5': //[2] 1 0
			hero->move(1, 0);
			break;
		case '6': //[3] -1 1
			hero->move(-1, 1);
			break;
		case '7': //[7] 0 1
			hero->move(0, 1);
			break;
		case '8': //[7] 1 1
			hero->move(1, 1);
			break;
		case '0': //[0] Назад
			check1 = false;
			break;
		default:
			cout << "Ошибка! Пожалуйста, попробуйте снова\n";
			break;
		}
	} while (check1);
}

void newField(Field& field)
{
	int x, y;
	cout << "Создайте поле размера X и Y (два целых числа):" << endl;
	cin >> x >> y;
	field.setXY(x, y);
}

Unit* createIdUnit(int sw)
{
	RoleUnitFactory newUnit;

	WShieldFactory shield;
	WAssasinFactory assasin;
	SHunterFactory hunter;
	SSnipernFactory sniper;
	MSummonerFactory summoner;
	MWizardFactory wizard;
	Unit* p = 0;
	switch (sw)
	{
	case 1: //[1] Щитовик
		p = newUnit.createUnits(shield);
		break;
	case 2: //[2] Ассасин
		p = (newUnit.createUnits(assasin));
		break;
	case 3: //[3] Охотник
		p = (newUnit.createUnits(hunter));
		break;
	case 4: //[4] Снайпер
		p = (newUnit.createUnits(sniper));
		break;
	case 5: //[5] Призыватель
		p = (newUnit.createUnits(summoner));
		break;
	case 6: //[6] Колдун
		p = (newUnit.createUnits(wizard));
		break;
	case 7: //[7] База
		p = (newUnit.createUnits(7));
		break;
	case 8: //[8] Травка
		p = (newUnit.createUnits(8));
		break;
	case 9: //[9] Водичка
		p = (newUnit.createUnits(9));
		break;
	case 10: //[10] Брёвна
		p = (newUnit.createUnits(10));
		break;
	case 0: //[0] Назад
		break;
	default:
		break;
	}
	return p;
}

Unit* createIdUnit(int sw, int x, int y)
{
	Unit* p = createIdUnit(sw);
	p->setXY(x, y);
	return p;
}

void menuCreateUnit(UnitList*& army)
{
	bool check1 = false;
	char sw1 = ' ';
	do {
		check1 = false;
		sw1 = ' ';
		cout << "\nВыберите класс юнита:\n";
		cout << "\x1b[32m[1]\x1b[0m Щитовик\n";
		cout << "\x1b[32m[2]\x1b[0m Ассасин\n";
		cout << "\x1b[32m[3]\x1b[0m Охотник\n";
		cout << "\x1b[32m[4]\x1b[0m Снайпер\n";
		cout << "\x1b[32m[5]\x1b[0m Призыватель\n";
		cout << "\x1b[32m[6]\x1b[0m Колдун\n";
		cout << "\x1b[32m[0]\x1b[0m Вернуться назад\n";
		cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

		cin >> sw1;
		while (cin.get() != '\n') { sw1 = ' '; };

		switch (sw1)
		{
		case '1': //[1] Щитовик
			army->addUnit(createIdUnit(1));
			break;
		case '2': //[2] Ассасин
			army->addUnit(createIdUnit(2));
			break;
		case '3': //[3] Охотник
			army->addUnit(createIdUnit(3));
			break;
		case '4': //[4] Снайпер
			army->addUnit(createIdUnit(4));
			break;
		case '5': //[5] Призыватель
			army->addUnit(createIdUnit(5));
			break;
		case '6': //[6] Колдун
			army->addUnit(createIdUnit(6));
			break;
		case '0': //[0] Назад
			break;
		default:
			cout << "Ошибка! Пожалуйста, попробуйте снова\n";
			check1 = true; //цикл пойдёт заново
			break;
		}
	} while (check1);
}

Unit* menuCreateUnit(UnitList*& army, int x, int y)
{
	RoleUnitFactory newUnit;

	WShieldFactory shield;
	WAssasinFactory assasin;
	SHunterFactory hunter;
	SSnipernFactory sniper;
	MSummonerFactory summoner;
	MWizardFactory wizard;
	Unit* p = 0;
	bool check1 = false;
	char sw1 = ' ';
	do {
		check1 = false;
		sw1 = ' ';
		cout << "\nВыберите класс юнита:\n";
		cout << "\x1b[32m[1]\x1b[0m Щитовик\n";
		cout << "\x1b[32m[2]\x1b[0m Ассасин\n";
		cout << "\x1b[32m[3]\x1b[0m Охотник\n";
		cout << "\x1b[32m[4]\x1b[0m Снайпер\n";
		cout << "\x1b[32m[5]\x1b[0m Призыватель\n";
		cout << "\x1b[32m[6]\x1b[0m Колдун\n";
		cout << "\x1b[32m[0]\x1b[0m Вернуться назад\n";
		cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

		cin >> sw1;
		while (cin.get() != '\n') { sw1 = ' '; };

		switch (sw1)
		{
		case '1': //[1] Щитовик
			p = newUnit.createUnits(shield);
			army->addUnit(p);
			break;
		case '2': //[2] Ассасин
			p = newUnit.createUnits(assasin);
			army->addUnit(p);
			break;
		case '3': //[3] Охотник
			p = newUnit.createUnits(hunter);
			army->addUnit(p);
			break;
		case '4': //[4] Снайпер
			p = newUnit.createUnits(sniper);
			army->addUnit(p);
			break;
		case '5': //[5] Призыватель
			p = newUnit.createUnits(summoner);
			army->addUnit(p);
			break;
		case '6': //[6] Колдун
			p = newUnit.createUnits(wizard);
			army->addUnit(p);
			break;
		case '0': //[0] Назад
			break;
		default:
			cout << "Ошибка! Пожалуйста, попробуйте снова\n";
			check1 = true; //цикл пойдёт заново
			break;
		}
		(army->getUnit(army->getSize()))->setXY(x, y);
	} while (check1);
	return p;
}

int main()
{
	setlocale(0, "");

	Field f1;
	newField(f1);
	bool check = true; //выход из меню
	bool check1 = false; //выход из подменю
	Field f2 = f1;
	UnitList* army = f1.unitList;
	UnitList* base = f1.baseList;


	//false - заканчивает цикл, приводя непосредственно к выходу
	do {
		f1.show();
		char sw = ' '; //переключатель главного меню
		char sw1 = ' '; //переключатель саб-меню
		int id = 0;
		cout << "\nВыберите нужный раздел: \n";
		cout << "\x1b[32m[1]\x1b[0m Добавить юнита\n";
		cout << "\x1b[32m[2]\x1b[0m Удалить юнита\n";
		cout << "\x1b[32m[3]\x1b[0m Выбрать юнита\n";
		cout << "\x1b[32m[4]\x1b[0m Сохранить текущее поле\n";
		cout << "\x1b[32m[5]\x1b[0m Вывести сохранённое поле\n";
		cout << "\x1b[32m[6]\x1b[0m Установить базу\n";
		cout << "\x1b[32m[7]\x1b[0m Создать юнита на базе\n";
		cout << "\x1b[32m[8]\x1b[0m Редактировать ландшафт\n";
		cout << "\x1b[32m[0]\x1b[0m Закрыть программу\n";
		cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

		cin >> sw;
		while (cin.get() != '\n') { sw = ' '; }; //если строка содержит более одного символа, возвращается ошибка

		switch (sw)
		{

		case '1': //[1] Добавить юнита
			system("cls");
			menuCreateUnit(army);
			break;

		case '2': //[2] Удалить юнита
			if (!army) break;
			army->info();
			cout << "\n[0] Назад" << endl;
			cout << "Выберите номер юнита, которого необходимо удалить:";
			cin >> id;
			if (!id) break;
			army->deleteUnit(id);
			system("cls");
			break;

		case '3': //[3] Выбрать юнита
			if (!army) break;
			do {
				check1 = false;
				sw1 = ' ';
				cout << endl;
				army->info();
				cout << "\n[0] Вернуться назад" << endl;
				cout << "Выберите номер юнита, которого хотите рассмотреть:" << endl;
				int id;
				cin >> id;
				if (id && (army->getUnit(id)))
				{
					army->getUnit(id)->outputStats();
					cout << "\x1b[32m[1]\x1b[0m Передвижение по карте\n";
					cout << "\x1b[32m[0]\x1b[0m Вернуться назад\n";
					cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

					cin >> sw1;
					while (cin.get() != '\n') { sw1 = ' '; };

					switch (sw1)
					{
					case '1': //[1] Передвижение по карте
						moveUnit(f1, id);
						break;
					case '0': //[0] Назад
						break;
					default:
						cout << "Ошибка! Пожалуйста, попробуйте снова\n";
						check1 = true; //цикл пойдёт заново
						break;
					}
				}
			} while (check1);
			system("cls");
			break;

		case '4': //[4] Сохранить текущее поле
			system("cls");
			f2.setUnitList((f1.unitList)->getClone());
			f2.setBaseList((f1.baseList)->getClone());
			cout << "Поле было сохранено!" << endl;
			break;

		case '5': //[5] Вывод сохранённого поля
			system("cls");
			cout << "Ранее сохранённое поле: " << endl;
			f2.show();
			break;

		case '6': //[6] Установить базу
			system("cls");
			cout << "Введите координаты: " << endl;
			int x, y;
			cin >> x >> y;
			base->addUnit(createIdUnit(7, x, y));
			break;

		case '7': //[7] Создание юнитов (через базу)
			base->info();
			cout << "\n[0] Назад" << endl;
			cout << "Выберите базу, которая создаст юнита: ";
			cin >> id;
			if (!id) break;
			(base->getUnit(id))->addUnit(army);
			system("cls");
			break;

		case '8': //[8] Редактирование ландшафта !!Надо доделать (мб сделать отсюда доступ к интерфейсу? ряльно так же удобнее)
			do {
				check1 = false;
				sw1 = ' ';
				cout << endl;
				cout << "[1] Установить травяной биом" << endl;
				cout << "[2] Удалить биом" << endl;
				cout << "[3] Установить водичку" << endl;
				cout << "[4] Установить брёвна/мост" << endl;
				cout << "[5] Установить травку (если она не установлена как биом)" << endl;
				cout << "[6] Удалить объект" << endl;
				cout << "[0] Вернуться назад" << endl;
				cout << "Введите число, чтобы выполнить нужное действие: " << endl;
				cin >> sw1;
				while (cin.get() != '\n') { sw1 = ' '; };

				switch (sw1)
				{
				case '1': //[1] Установить травяной биом
					moveUnit(f1, id);
					break;
				case '0': //[0] Назад
					break;
				default:
					cout << "Ошибка! Пожалуйста, попробуйте снова\n";
					check1 = true; //цикл пойдёт заново
					break;
				}
			} while (check1);
			system("cls");
			break;

		case '0': //[0] Закрыть программу
			cout << "Выход из программы...\n";
			check = false; //выход из цикла
			break;
		default: //в случае, если введено что-то иное
			cout << "Ошибка! Пожалуйста, попробуйте снова\n";
			break;
		}
	} while (check);
	return 0;
}