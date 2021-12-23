// == Абстрактная фабрика для создания юнитов ==

// Абстрактная фабрика для производства воинов
class WarriorFactory
{
public:
    virtual Warrior* createWarrior() = 0;
    virtual ~WarriorFactory() {}
};

// Абстрактная фабрика для производства стрелков
class ShooterFactory
{
public:
    virtual Shooter* createShooter() = 0;
    virtual ~ShooterFactory() {}
};

// Абстрактная фабрика для производства магов
class MageFactory
{
public:
    virtual Mage* createMage() = 0;
    virtual ~MageFactory() {}
};

// Фабрика для создания Саппорта-Воина
class WShieldFactory : public WarriorFactory
{
public:
    Warrior* createWarrior() {
        return new WarriorShield;
    }
};

// Фабрика для создания Саппорта-Стрелка
class SHunterFactory : public ShooterFactory
{
public:
    Shooter* createShooter() {
        return new ShooterHunter;
    }
};

// Фабрика для создания Саппорта-Мага
class MSummonerFactory : public MageFactory
{
public:
    Mage* createMage() {
        return new MageSummoner;
    }
};

// Фабрика для создания Воина-Дамагера
class WAssasinFactory : public WarriorFactory
{
public:
    Warrior* createWarrior() {
        return new WarriorAssasin;
    }
};

// Фабрика для создания Стрелка-Дамагера
class SSnipernFactory : public ShooterFactory
{
public:
    Shooter* createShooter() {
        return new ShooterSniper;
    }
};

// Фабрика для создания Мага-Дамагера
class MWizardFactory : public MageFactory
{
public:
    Mage* createMage() {
        return new MageWizard;
    }
};

// Создание конкретных ролей
class RoleUnitFactory
{
public:
    Unit* createUnits(WarriorFactory& factory) {
        return factory.createWarrior();
    }
    Unit* createUnits(ShooterFactory& factory) {
        return factory.createShooter();
    }
    Unit* createUnits(MageFactory& factory) {
        return factory.createMage();
    }
    Unit* createUnits(int roleId) {
        switch (roleId)
        {
        case 7:
            return new Base;
        case 8:
            return new Landscape;
        case 9:
            return new Water;
        case 10:
            return new Bridge;
        default:
            return new Unit;
            break;
        }
    }
};