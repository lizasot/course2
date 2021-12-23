//общий класс юнитов
class UnitList;
class Unit
{
public:
    string name;
    int x = 1, y = 1;
    virtual void info() {
        cout << "Unit" << endl;
    }
    virtual void outputStats() {
        cout << "\nМестоположение [X][Y]: [" << x << "][" << y << "]" << endl;
    }
    virtual void draw() {
        cout << " ";
    }
    void sayName() {
        cout << name << endl;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    virtual void setXY(int newX, int newY) {
        x = newX;
        y = newY;
    }
    virtual void move(int x1, int y1) {
        x += x1;
        y += y1;
    }
    virtual int getRoleId() { return 0; };
    virtual void addUnit(UnitList*& list) { return; }
    virtual ~Unit() {}
};

// == Юниты ==
class Army : public Unit
{
public:
    int hp = 10,
        dmg = 5,
        speed = 1,
        mana = 1;
    int range = 1;
    virtual void outputStats() {
        if (name != "") cout << name << endl;
        cout << "Здоровье: " << hp << endl;
        cout << "Урон: " << dmg << endl;
        cout << "Ловкость: " << speed << endl;
        cout << "Мана: " << mana << endl;
        cout << "\nМестоположение [X][Y]: [" << x << "][" << y << "]" << endl;
    }
};

class Warrior : public Army
{
public:
    virtual void info() {
        cout << "Warrior" << endl;
    }
    virtual void draw() {
        cout << "W";
    }
    Warrior()
    {
        cout << "По-воински готов бегать по полю\n";
    }
    virtual ~Warrior() {}
};

class Shooter : public Army
{
public:
    virtual void info() {
        cout << "Shooter" << endl;
    }
    virtual void draw() {
        cout << "S";
    }
    Shooter()
    {
        cout << "По-стрелковски готов бегать по полю\n";
    }
    virtual ~Shooter() {}
};

class Mage : public Army
{
public:
    virtual void info() {
        cout << "Mage" << endl;
    }
    virtual void draw() {
        cout << "M";
    }
    Mage()
    {
        cout << "По-магически готов бегать по полю\n";
    }
    virtual ~Mage() {}
};

// Классы всех Воинов
class WarriorShield : public Warrior
{
    static int roleId;
public:
    void info() {
        cout << "Щитовик" << endl;
    }
    void draw() {
        cout << "\x1b[32mW\x1b[37m";
    }
    int getRoleId() {
        return roleId;
    }
};
int WarriorShield::roleId = 1;

class WarriorAssasin : public Warrior
{
    static int roleId;
public:
    void info() {
        cout << "Ассасин" << endl;
    }
    void draw() {
        cout << "\x1b[33mW\x1b[37m";
    }
    int getRoleId() {
        return roleId;
    }
};
int WarriorAssasin::roleId = 2;

// Классы всех Стрелков
class ShooterHunter : public Shooter
{
    static int roleId;
public:
    void info() {
        cout << "Охотник" << endl;
    }
    void draw() {
        cout << "\x1b[32mS\x1b[37m";
    }
    int getRoleId() {
        return roleId;
    }
};
int ShooterHunter::roleId = 3;

class ShooterSniper : public Shooter
{
    static int roleId;
public:
    void info() {
        cout << "Снайпер" << endl;
    }
    void draw() {
        cout << "\x1b[33mS\x1b[37m";
    }
    int getRoleId() {
        return roleId;
    }
};
int ShooterSniper::roleId = 4;

// Классы всех Магов
class MageSummoner : public Mage
{
    static int roleId;
public:
    void info() {
        cout << "Призыватель" << endl;
    }
    void draw() {
        cout << "\x1b[32mM\x1b[37m";
    }
    int getRoleId() {
        return roleId;
    }
};
int MageSummoner::roleId = 5;

class MageWizard : public Mage
{
    static int roleId;
public:
    void info() {
        cout << "Колдун" << endl;
    }
    void draw() {
        cout << "\x1b[33mM\x1b[37m";
    }
    int getRoleId() {
        return roleId;
    }
};
int MageWizard::roleId = 6;

//== Списки объектов ==
Unit* createIdUnit(int sw);

class UnitList
{
public:
    vector<Unit*> unitList;
    UnitList* fuse(UnitList* list) {
        UnitList* newList = list;
        size_t i;
        for (i = 0; i < unitList.size(); i++) newList->unitList.push_back(unitList[i]);
        return newList;
    }
    void info() {
        size_t i;
        for (i = 0; i < unitList.size(); ++i)
        {
            cout << i + 1 << ". ";
            unitList[i]->info();
            cout << "Местоположение: [" << unitList[i]->getX() << "][" << unitList[i]->getY() << "]" << endl;
        }
    }
    void draw() {
        size_t i;
        for (i = 0; i < unitList.size(); ++i)  unitList[i]->draw();
    }
    void sayName() {
        size_t i;
        for (i = 0; i < unitList.size(); ++i)  unitList[i]->sayName();
    }
    Unit* getUnit(int id) {
        return unitList[id - 1];
    }
    size_t getSize() {
        return unitList.size();
    }
    void addUnit(Unit* p) {
        unitList.push_back(p);
    }
    void deleteUnit(int id) {
        auto iter = unitList.cbegin();
        unitList.erase(iter + (id - 1));
    }
    UnitList* getClone() {
        UnitList* p = new UnitList;
        size_t i;
        for (i = 0; i < unitList.size(); i++)
        {
            Unit* temp = createIdUnit(unitList[i]->getRoleId());
            *temp = (*unitList[i]);
            p->unitList.push_back(temp);
        }
        return p;
    }
    ~UnitList() {
        size_t i;
        for (i = 0; i < unitList.size(); ++i)  delete unitList[i];
    }
};

// == База ==
Unit* menuCreateUnit(UnitList*& army, int x, int y);

//класс базы
class Base : public Unit
{
    static int roleId;
public:
    float hp;
    size_t maxUnit;
    UnitList* baseArmy;
    void info() {
        if (name != "") cout << name << endl;
        cout << "Здоровье: " << hp << endl;
        if (baseArmy) cout << "Текущее количество юнитов: " << baseArmy->getSize() << "/" << maxUnit << endl;
        else cout << "Текущее количество юнитов: 0/" << maxUnit << endl;
    }
    void addUnit(UnitList*& list) {
        bool check1 = false; //выход из подменю
        do {
            char sw1 = ' ';
            sw1 = ' ';
            check1 = false;
            cout << "\nВыберите куда поставить юнита:\n";
            cout << "1 2 3\n";
            cout << "4 B 5\n";
            cout << "6 7 8\n";
            cout << "[9] Оставить внутри базы\n";
            cout << "[0] Вернуться назад\n";
            cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

            cin >> sw1;
            while (cin.get() != '\n') { sw1 = ' '; };

            switch (sw1)
            {
            case '1': //[1] -1 -1
                menuCreateUnit(list, x - 1, y - 1);
                break;
            case '2': //[2] 0 -1
                menuCreateUnit(list, x, y - 1);
                break;
            case '3': //[3] 1 -1
                menuCreateUnit(list, x + 1, y - 1);
                break;
            case '4': //[1] -1 0
                menuCreateUnit(list, x - 1, y);
                break;
            case '5': //[2] 1 0
                menuCreateUnit(list, x + 1, y);
                break;
            case '6': //[3] -1 1
                menuCreateUnit(list, x - 1, y + 1);
                break;
            case '7': //[7] 0 1
                menuCreateUnit(list, x, y + 1);
                break;
            case '8': //[7] 1 1
                menuCreateUnit(list, x + 1, y + 1);
                break;
            case '9': //[9] Внутри базы
                if (!baseArmy || baseArmy->getSize() < maxUnit) menuCreateUnit(baseArmy, 0, 0);
                else {
                    cout << "Количество юнитов в базе достигло максимального количества.\n";
                    check1 = true;
                }
                break;
            case '0': //[0] Назад
                break;
            default:
                cout << "Ошибка! Пожалуйста, попробуйте снова\n";
                check1 = true;
                break;
            }
        } while (check1);
    }
    int getRoleId() {
        return roleId;
    }
    void draw() {
        cout << 'B';
    }
    Base();
};
Base::Base() {
    name = "";
    hp = 10;
    maxUnit = 10;
    x = 1;
    y = 1;
    baseArmy = new UnitList;
}
int Base::roleId = 7;

// == Ландшафт ==
class Landscape : public Unit
{
    static int roleId;
public:
    virtual void draw() {
        cout << "\x1b[32m*\x1b[37m";
    }
};
int Landscape::roleId = 8;

class Water : public Landscape
{
    static int roleId;
public:
    void draw() {
        cout << "\x1b[34m~\x1b[37m";
    }
};
int Water::roleId = 9;

class Bridge : public Landscape
{
    static int roleId;
public:
    void draw() {
        cout << "\x1b[33m=\x1b[37m";
    }
};
int Bridge::roleId = 10;