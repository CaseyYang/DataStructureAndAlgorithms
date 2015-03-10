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

//�������ºܶ࣬ʵ�ַ������ֶ��У�Ҳ����ȫ�־�̬����ʵ�ֵ�
//����һЩ�۵㣬�Լ��ҵ�����ǣ�ȫ�־�̬��Ա�����ڳ���һ��ʼ���ʼ�����޷�ʹ�ò�ͬ��ֵ�����г�ʼ����(�������Ҫ��)
//ȫ�־�̬��Ա������ʼ��˳���������캯�������׳�һЩ����������
//ʹ�þֲ���̬��Ա��һ�ֱȽ����ϱȽϹ㷺��д�����ô����ܹ�ʹ��ģ�壬Ҳ��ʹ�ü̳ж�̬

//����ģʽ
class Singleton1{
public:
	static Singleton1& Instance()
	{
		static Singleton1 s_Instance;
		return s_Instance;
	}
protected:
	Singleton1() {
		cout << "Singleton1���캯�������ã�" << endl;
	}
	~Singleton1() {}
private:
	Singleton1(const Singleton1& rhs) {}
	Singleton1& operator = (const Singleton1& rhs) {
		return *this;
	}
};

//����ģʽ��ģ����
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
//����ģʽģ������÷�
class SingletonInstance :public Singleton2 < SingletonInstance > {
public:
	static int count;
protected:
	friend class Singleton2<SingletonInstance>;
	SingletonInstance(){
		++count;
		cout << "SingletonInstace���캯����" << count << "������" << endl;
	}
	SingletonInstance(const SingletonInstance& orig){}
	SingletonInstance& operator = (const SingletonInstance& rhs) {}
};
int SingletonInstance::count = 0;

int main(){
	SingletonInstance::Instance();
	SingletonInstance::Instance();
}