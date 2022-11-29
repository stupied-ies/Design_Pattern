// https://refactoring.guru/design-patterns/prototype
// https://refactoring.guru/design-patterns/prototype/cpp/example
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

enum Type {
	PROTOTYPE_1 = 0,
	PROTOTYPE_2
};

class Entity
{
private:
	int m_old;
public:
	Entity() : m_old(10) { std::cout << "Entity default Constructor\n"; }
	Entity(int old) : m_old(old) {}
	int GetData() {
		return m_old;
	}
	~Entity() { 
		std::cout << "Entity Deconstructor: " << m_old << "\n"; 
	}
};
class Prototype
{
protected:
	std::string _prototype_name;
public:
	Prototype() {
		std::cout << "Prototype: "<< _prototype_name << "\n";
	};
	Prototype(std::string prototype_name) :_prototype_name(prototype_name) {}
	virtual ~Prototype(){}
	virtual std::unique_ptr<Prototype> clone()const = 0;
	virtual void Display() const = 0;
};
class ConcretePrototype1 :public Prototype
{
private:
	int		_prototype_field1;
	std::shared_ptr<Entity> _prototype_field2;
public:
	ConcretePrototype1(std::string prototype_name, int prototype_field1, std::shared_ptr<Entity> prototype_field2)
	: Prototype(prototype_name), _prototype_field1(prototype_field1), _prototype_field2(prototype_field2)
	{}
	ConcretePrototype1(const ConcretePrototype1& other)
		: Prototype(other._prototype_name), _prototype_field1(other._prototype_field1), _prototype_field2(other._prototype_field2)
	{}
	~ConcretePrototype1() {
		std::cout << "ConcretePrototype1 deconstrctor\n";
	}
	std::unique_ptr<Prototype> clone() const override{
		return std::make_unique<ConcretePrototype1>(*this);
	}
	void Display() const override {
		std::cout << "Call Method from " << _prototype_name << " with field : " << _prototype_field1 
			<< " and " << _prototype_field2->GetData() << std::endl;
	}
};
class ConcretePrototype2 :public Prototype
{
private:
	int		_prototype_field1;
	std::unique_ptr<Entity> _prototype_field2;
public:
	ConcretePrototype2(std::string prototype_name, int prototype_field1, std::unique_ptr<Entity> prototype_field2)
		: Prototype(prototype_name), _prototype_field1(prototype_field1), _prototype_field2(std::move(prototype_field2))
	{}
	ConcretePrototype2(const ConcretePrototype2& other)
		:Prototype(other._prototype_name), _prototype_field1(other._prototype_field1), _prototype_field2(std::make_unique<Entity>(_prototype_field1))
	{}
	~ConcretePrototype2() {
		std::cout << "ConcretePrototype2 deconstrctor\n";
	}
	std::unique_ptr<Prototype> clone() const override {
		return std::make_unique<ConcretePrototype2>(*this);
	}
	void Display() const override {
		std::cout << "Call Method from " << _prototype_name << " with field : " << _prototype_field1
			<< " and " << _prototype_field2->GetData() << std::endl;
	}
};
class PrototypeFactory
{
private:
	std::unordered_map<Type, std::unique_ptr<Prototype>, std::hash<int>> _prototypes;
public:
	PrototypeFactory() {
		_prototypes[Type::PROTOTYPE_1] = std::make_unique<ConcretePrototype1>("PROTOTYPE_1 ", 50, std::make_shared<Entity>(50));
		_prototypes[Type::PROTOTYPE_2] = std::make_unique<ConcretePrototype2>("PROTOTYPE_2 ", 60, std::make_unique<Entity>(60));
	}
	~PrototypeFactory() {
		std::cout << "PrototypeFactory deconstrctor\n";
	}
	std::unique_ptr<Prototype> CreatePrototype(Type type) {
		return _prototypes[type]->clone();
	}
};
void Client(PrototypeFactory& prototype_factory) {
	std::cout << "Let's create a Prototype 1\n";
	std::unique_ptr<Prototype> prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_1);
	prototype->Display();
	std::cout << "\n";
	std::cout << "Let's create a Prototype 2 \n";
	prototype.reset();
	prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_2);
	prototype->Display();
}
int main()
{
	{
		std::unique_ptr<PrototypeFactory> prototype_factory = std::make_unique<PrototypeFactory>();
		Client(*prototype_factory);
	}
	system("pause");


}
