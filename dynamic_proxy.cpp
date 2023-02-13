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
	virtual void Request() = 0;
	virtual void Request(int a) = 0;
};
using SubjectPtr = std::shared_ptr<Subject>;

class RealSubject : public Subject
{
public:
	virtual void Request() final {
		std::cout << "RealSubject: Handling request\n";
	}
	virtual void Request(int a) final {
		std::cout << "RealSubject: Handling request : \t " << a << std::endl;
	}
	void Request2()  {
		std::cout << "RealSubject: Handling request\n";
	}
};
using RealSubjectPtr = std::shared_ptr<RealSubject>;

template <typename T, typename F, typename ... args>
class Proxy
{
private:
	std::shared_ptr<T> _realSubjectPtr;
	F (T::*_fun)(args...);
private:
	void Init() {
		_realSubjectPtr.reset( new RealSubject);
	}
	bool CheckAcess() const {
		std::cout << "Proxy: Checking access prior to firing a real request.\n";
		return true;
	}
	void LogAccess() const {
		std::cout << "Proxy: Logging the time of request.\n";
	}

public:
	Proxy(std::shared_ptr<T> realSubjectPtr, F (T::* f)(args...)) : _realSubjectPtr(realSubjectPtr), _fun(f) {}
	~Proxy() {}

	template <typename ... args>
	void doSomeThing(args ... params) {
		if (this->CheckAcess()) {
			if (_realSubjectPtr.get() == nullptr)
				Init();
			(_realSubjectPtr.get()->*_fun)(std::forward<args>(params) ...);
			LogAccess();
		}
	}
};
template <typename T, typename F, typename ... args>
std::shared_ptr<Proxy<T, F, args ... >> create(std::shared_ptr<T> realSubjectPtr, F (T::* f)(args ...)) {
	return std::make_shared<Proxy<T, F, args ... >>(realSubjectPtr, f);
}

template <typename T, typename ... args>
std::shared_ptr<T> createPtr(args&& ... params) {
	return std::make_shared<T>(std::forward<args>(params)...);
}

int main()
{
	RealSubjectPtr realSubject = createPtr<RealSubject>();
	auto proxyPtr = create(realSubject, &RealSubject::Request2);
	proxyPtr->doSomeThing();
}

