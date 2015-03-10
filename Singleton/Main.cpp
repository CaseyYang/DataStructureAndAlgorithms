#include<iostream>
#include<list>
using namespace std;

class Simple {
public:
	Simple(int param = 0) {
		number = param;
		cout << "Simple: " << number << std::endl;
	}
	~Simple() {
		cout << "~Simple: " << number << std::endl;
	}
	void PrintSomething() {
		cout << "PrintSomething: " << info_extend.c_str() << std::endl;
	}
	string info_extend;
	int number;
};

//网上文章很多，实现方法各种都有，也有用全局静态变量实现的
//网上一些观点，以及我的理解是：全局静态成员变量在程序一开始便初始化，无法使用不同的值来进行初始化；(这点是主要的)
//全局静态成员变量初始化顺序不依赖构造函数，容易出一些依赖性问题
//使用局部静态成员是一种比较网上比较广泛的写法，好处是能够使用模板，也能使用继承多态

//单例模式
class Singleton1{
public:
	static Singleton1& Instance()
	{
		static Singleton1 s_Instance;
		return s_Instance;
	}
protected:
	Singleton1() {
		cout << "Singleton1构造函数被调用！" << endl;
	}
	~Singleton1() {}
private:
	Singleton1(const Singleton1& rhs) {}
	Singleton1& operator = (const Singleton1& rhs) {
		return *this;
	}
};

//单例模式的模板类
template <typename T>
class Singleton2
{
public:
	static T& Instance()
	{
		static T s_Instance;
		return s_Instance;
	}
protected:
	Singleton2() {
	}
	~Singleton2() {}
private:
	Singleton2(const Singleton2& rhs) {}
	Singleton2& operator = (const Singleton2& rhs) {}
};
//单例模式模板类的用法
class SingletonInstance :public Singleton2 < SingletonInstance > {
public:
	static int count;
protected:
	friend class Singleton2<SingletonInstance>;
	SingletonInstance(){
		++count;
		cout << "SingletonInstace构造函数第" << count << "被调用" << endl;
	}
	SingletonInstance(const SingletonInstance& orig){}
	SingletonInstance& operator = (const SingletonInstance& rhs) {}
};
int SingletonInstance::count = 0;

int main(){
	SingletonInstance::Instance();
	SingletonInstance::Instance();
}