#include<iostream>
#include<memory>
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
//C++标准中的智能指针类auto_ptr
//原文链接：http://blog.csdn.net/xt_xiaotian/article/details/5714477
void TestAutoPtr(){
	//构造智能指针
	auto_ptr<Simple> aptr1(new Simple(1));
	auto_ptr<Simple> aptrMirror(new Simple(1));
	//使用智能指针
	if (aptr1.get()){
		aptr1->PrintSomething();//auto_ptr是一个类，重载运算符->可以“模拟”指针调用所指对象的成员
		aptr1.get()->info_extend += "Addition";//get()函数返回智能指针对象的裸指针
		aptr1->PrintSomething();
		(*aptr1).info_extend += " other";//(*aptr1)得到智能指针所指对象
		aptr1->PrintSomething();
	}
	//智能指针类为单例模式，同一个对象同时只能由一个智能指针管理
	//无论是使用赋值运算符=、还是复制构造函数，都会使前一个指向对象的智能指针失效
	if (aptr1.get()){
		auto_ptr<Simple> aptr2 = aptr1;
		cout << "aptr1是否失效：" << (aptr1.get() == NULL) << endl;//aptr1通过等号赋值给aptr2后，aptr1失效
		cout << "aptr2是否失效：" << (aptr2.get() == NULL) << endl;
		auto_ptr<Simple> aptr3(aptr2);
		cout << "aptr2是否失效：" << (aptr2.get() == NULL) << endl;//aptr2通过复制构造函数赋给aptr3后，aptr2失效
		cout << "aptr3是否失效：" << (aptr3.get() == NULL) << endl;
		aptr1 = aptr3;
	}
	//释放智能指针
	Simple* rawPtr = aptr1.release();
	cout << "aptr1是否失效：" << (aptr1.get() == NULL) << endl;//release函数：释放对象的指针所有权，并不释放对象所占内存。对象认可通过释放出的裸指针调用
	aptrMirror.reset();//reset函数：释放对象所占内存，对于类对象会调用析构函数。此处会输出析构函数内语句
}

//C++ Primer中的智能指针类
template <typename T>
class HasPtr;
//U_Ptr类中有两个成员变量：指向对象的指针和引用计数。引用计数记录了有多少HasPtr实例引用了该对象；
//每当HasPtr类的构造函数（包括复制构造和赋值运算符）被调用时，引用计数加1
//而当一个HasPtr实例被析构时，引用计数减1；当引用计数为0时，调用U_Ptr的析构函数
//为了防止外部调用，U_ptr类所有成员被设为private，只有友类HasPtr可以调用
template <typename T>
class U_Ptr{
public:
	friend class HasPtr < T > ;
	T *rawPtr;
	size_t count;
	U_Ptr(T *p) :rawPtr(p), count(1){
	}
	~U_Ptr(){
		delete rawPtr;
	}
};
//HasPtr类的关键是该类并不直接管理内存，而是包含一个U_Ptr类实例的指针，由该U_Ptr对象管理内存；
//这样当HasPtr类在复制或赋值过程中，所有新创建的HasPtr类对象都指向同一个U_Ptr实例，只有当U_Ptr实例内部计数为0时，内存才会真正释放
//避免了一个HasPtr实例析构以后其他HasPtr实例出现“悬垂指针”
template <typename T>
class HasPtr{
public:
	HasPtr(T*p, T &value);
	HasPtr(const HasPtr<T> &orig);
	HasPtr operator=(const HasPtr<T> &orig);
	~HasPtr();
private:
	U_Ptr<T> *innerPtr;
	T value;
};
template <typename T>
HasPtr<T>::HasPtr(T*p, T &value) :innerPtr(new U_Ptr<T>(p)), value(value){
};
template <typename T>
HasPtr<T>::HasPtr(const HasPtr<T> &orig) :innerPtr(orig.innerPtr), value(orig.value){
	++innerPtr->count;
}
template <typename T>
HasPtr<T> HasPtr<T>::operator=(const HasPtr<T> &orig){
	//注意：首先要先对目标引用计数加1，再考虑本身的引用计数减1，
	//否则当自身赋值给自身时，会出现自身U_Ptr的引用计数为0被析构而无法完成赋值的情况
	++orig.innerPtr;
	if (--innerPtr == 0){
		delete innerPtr;
	}
	innerPtr = orig.innerPtr;
	value = orig.value;
	return *this;
};
template <typename T>
HasPtr<T>::~HasPtr(){
	//只有当指向对象的引用计数为零时，才释放对象内存；否则只释放该HasPtr对象本身
	if (innerPtr->count == 0){
		delete innerPtr;
	}
}
//关于C++模板类函数声明和定义分离后编译出错的解释：http://www.cnblogs.com/qlwy/archive/2012/03/21/2410045.html
//结论就是：模板类的函数声明和定义不能分离在两个文件中
//但是另外有人发现可以分离编译的方法，未尝试：http://bbs.csdn.net/topics/320028181

int main(){
	//TestAutoPtr();
	int *p = new int(3);
	HasPtr<int> sPtr1(p, *p);
	HasPtr<int> sPtr2(sPtr1);
	return 0;
}