#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <memory>
class Component
{
protected:
	Component* _parent{ nullptr };
public:
	Component() { std::cout << "Component constructor\n"; }
	virtual ~Component() { std::cout << "Component destructor!" << std::endl; }
	void SetParent(Component* parent) {
		this->_parent = parent;
	}
	Component* GetParent() const {
		return this->_parent;
	}
	virtual void Add(std::shared_ptr<Component>& sharedPtrComponent) {}
	virtual void Remove(std::shared_ptr<Component>& sharedPtrComponent) {}
	virtual bool IsComposite() const {
		return false;
	}
	virtual std::string Operation() const = 0;
};

class Leaf :public Component
{
public:
	std::string Operation() const final {
		return "Leaf";
	}
};

class Composite :public Component
{
protected:
	std::list<std::shared_ptr<Component>> _children;
public:
	void Add(std::shared_ptr<Component>& sharedPtrComponent) override {
		this->_children.push_back(std::shared_ptr<Component>(sharedPtrComponent));
		sharedPtrComponent->SetParent(this);
	}
	void Remove(std::shared_ptr<Component>& sharedPtrComponent) override {
		auto iterator = _children.begin();
		for (; iterator != _children.end(); ++iterator) {
			if (iterator->get() == sharedPtrComponent.get()) {
				break;
			}
		}
		_children.erase(iterator);
	}
	bool IsComposite() const override {
		return true;
	}
	std::string Operation() const override {
		std::string result;
		for (auto it = _children.begin(); it != _children.end(); ++it) {
			if (*it == _children.back()) {
				result += (*it)->Operation();
			}
			else {
				result += (*it)->Operation() + "+";
			}
		}
		return "Branch(" + result + ")";
	}
};

void ClientCode(Component* component) {
	std::cout << "RESULT: " << component->Operation();
}

int main()
{
	//std::shared_ptr<Component> simple = std::make_shared<Leaf>();
	//std::cout << "Client: I've got a simple component:\n";
	//ClientCode(simple.get());
	//std::cout << "\n\n";
	///**
	// * ...as well as the complex composites.
	// */

	//std::shared_ptr<Component> tree = std::make_shared<Composite>();
	//std::shared_ptr<Component> branch1 = std::make_shared<Composite>();
	//std::shared_ptr<Component> branch2 = std::make_shared<Composite>();

	//std::shared_ptr<Component> leaf_1 = std::make_shared<Leaf>();
	//std::shared_ptr<Component> leaf_2 = std::make_shared<Leaf>();
	//std::shared_ptr<Component> leaf_3 = std::make_shared<Leaf>();

	//branch1->Add(leaf_1); 
	//branch1->Add(leaf_2);
	//branch2->Add(leaf_3);

	//tree->Add(branch1);
	//tree->Add(branch2);
	//tree->Remove(branch2);
	//std::cout << "Client: Now I've got a composite tree:\n";
	//ClientCode(tree.get());
	//std::cout << "\n\n";
	uint64_t a = 10;
	std::cout << sizeof(a) << std::endl;
}

