//https://refactoring.guru/design-patterns/proxy/cpp/example
//https://refactoring.guru/design-patterns/proxy
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <memory>

class Subject
{
public:
	Subject() { std::cout << "Subject constructor\n"; }
	virtual ~Subject() { std::cout << "Subject destructor\n"; }
	virtual void Request()  = 0;
};

class RealSubject : public Subject
{
public:
	void Request() override {
		std::cout << "RealSubject: Handling request\n";
	}
};

class Proxy : public Subject
{
private:
	std::unique_ptr<RealSubject> _real_subject;
private:
	void Init() {
		_real_subject.reset(new RealSubject);
	}
	bool CheckAcess() const {
		std::cout << "Proxy: Checking access prior to firing a real request.\n";
		return true;
	}
	void LogAccess() const {
		std::cout << "Proxy: Logging the time of request.\n";
	}
public:
	Proxy() {}
	Proxy(RealSubject* real_subject) : _real_subject{ real_subject } {}
	Proxy(std::unique_ptr<RealSubject>& real_subject) : _real_subject(std::move(real_subject)) {}
	~Proxy() override {}
	void Request() override {
		if (this->CheckAcess()) {
			if (_real_subject.get() == nullptr)
				Init();
			_real_subject->Request();
			LogAccess();
		}
	}
};

void ClientCode( Subject& subject) {
	// ...
	subject.Request();
	// ...
}

int main()
{
	std::unique_ptr<RealSubject> real_subject(new RealSubject);
	std::cout << "Client: Executing the same client code with a proxy:\n";
	std::unique_ptr<Proxy> proxy(new Proxy(real_subject));
	ClientCode(*proxy);
}

