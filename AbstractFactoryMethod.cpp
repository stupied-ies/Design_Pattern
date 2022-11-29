// https://refactoring.guru/design-patterns/abstract-factory
// https://refactoring.guru/design-patterns/abstract-factory/cpp/example
#include <iostream>
#include <string>
#include <memory>
class Button
{
public:
	virtual ~Button() {}
	virtual void click() = 0;
};
class CheckBox
{
public:
	virtual ~CheckBox() {}
	virtual void paint() = 0;
};
class WinButton : public Button
{
public:
	~WinButton() { std::cout << "WinButton is deconstructed\n"; }
	void click() override {
		std::cout << "WinButton clicked" << std::endl;
	};
};
class MacButton : public Button
{
public:
	~MacButton() { std::cout << "MacButton is deconstructed\n"; }
	void click() override {
		std::cout << "MacButton clicked" << std::endl;
	};
};
class WinCheckBox :public CheckBox
{
public:
	~WinCheckBox() { std::cout << "WinCheckBox is deconstructed\n"; }
	void paint() override {
		std::cout << "WinCheckBox painted" << std::endl;
	};
};
class MacCheckBox :public CheckBox
{
public:
	~MacCheckBox() { std::cout << "MacCheckBox is deconstructed\n"; }
	void paint() override {
		std::cout << "MacCheckBox painted" << std::endl;
	};
};

class GUIFactory
{
public:
	virtual ~GUIFactory() {}
	virtual Button* createButton() = 0;
	virtual CheckBox* createCheckBox() = 0;
};
class winGUIFactory : public GUIFactory {
public:
	~winGUIFactory() { std::cout << "winGUIFactory is deconstructed\n"; }
	Button* createButton() override {
		return new WinButton();
	};
	CheckBox* createCheckBox() override {
		return new WinCheckBox();
	};
};
class MacGUIFactory :public GUIFactory {
public:
	~MacGUIFactory() { std::cout << "MacGUIFactory is deconstructed\n"; }
	Button* createButton() override {
		return new MacButton();
	};
	CheckBox* createCheckBox() override {
		return new MacCheckBox();
	};
};

class Application
{
public:
	explicit Application(GUIFactory* factory) :m_factory(std::unique_ptr<GUIFactory>(factory)) {}
	~Application() { std::cout << "Application is deconstructed!" << std::endl; }
	void createUI() {
		m_button = std::unique_ptr<Button>(m_factory->createButton());
		m_checkBox = std::unique_ptr<CheckBox>(m_factory->createCheckBox());
	}
	void doStuff() {
		m_button->click();
		m_checkBox->paint();
	}
private:
	std::unique_ptr<GUIFactory> m_factory;
	std::unique_ptr<Button> m_button;
	std::unique_ptr<CheckBox> m_checkBox;
};


int main()
{
	std::string OS = "Mac";
	if (OS == "Win") {
		GUIFactory* factory = new winGUIFactory();
		Application app(factory);
		app.createUI();
		app.doStuff();
	}
	else if (OS == "Mac") {
		GUIFactory* factory = new MacGUIFactory();
		Application app(factory);
		app.createUI();
		app.doStuff();

	}
}
