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
//C++��׼�е�����ָ����auto_ptr
//ԭ�����ӣ�http://blog.csdn.net/xt_xiaotian/article/details/5714477
void TestAutoPtr(){
	//��������ָ��
	auto_ptr<Simple> aptr1(new Simple(1));
	auto_ptr<Simple> aptrMirror(new Simple(1));
	//ʹ������ָ��
	if (aptr1.get()){
		aptr1->PrintSomething();//auto_ptr��һ���࣬���������->���ԡ�ģ�⡱ָ�������ָ����ĳ�Ա
		aptr1.get()->info_extend += "Addition";//get()������������ָ��������ָ��
		aptr1->PrintSomething();
		(*aptr1).info_extend += " other";//(*aptr1)�õ�����ָ����ָ����
		aptr1->PrintSomething();
	}
	//����ָ����Ϊ����ģʽ��ͬһ������ͬʱֻ����һ������ָ�����
	//������ʹ�ø�ֵ�����=�����Ǹ��ƹ��캯��������ʹǰһ��ָ����������ָ��ʧЧ
	if (aptr1.get()){
		auto_ptr<Simple> aptr2 = aptr1;
		cout << "aptr1�Ƿ�ʧЧ��" << (aptr1.get() == NULL) << endl;//aptr1ͨ���ȺŸ�ֵ��aptr2��aptr1ʧЧ
		cout << "aptr2�Ƿ�ʧЧ��" << (aptr2.get() == NULL) << endl;
		auto_ptr<Simple> aptr3(aptr2);
		cout << "aptr2�Ƿ�ʧЧ��" << (aptr2.get() == NULL) << endl;//aptr2ͨ�����ƹ��캯������aptr3��aptr2ʧЧ
		cout << "aptr3�Ƿ�ʧЧ��" << (aptr3.get() == NULL) << endl;
		aptr1 = aptr3;
	}
	//�ͷ�����ָ��
	Simple* rawPtr = aptr1.release();
	cout << "aptr1�Ƿ�ʧЧ��" << (aptr1.get() == NULL) << endl;//release�������ͷŶ����ָ������Ȩ�������ͷŶ�����ռ�ڴ档�����Ͽ�ͨ���ͷų�����ָ�����
	aptrMirror.reset();//reset�������ͷŶ�����ռ�ڴ棬������������������������˴�������������������
}

//C++ Primer�е�����ָ����
template <typename T>
class HasPtr;
//U_Ptr������������Ա������ָ������ָ������ü��������ü�����¼���ж���HasPtrʵ�������˸ö���
//ÿ��HasPtr��Ĺ��캯�����������ƹ���͸�ֵ�������������ʱ�����ü�����1
//����һ��HasPtrʵ��������ʱ�����ü�����1�������ü���Ϊ0ʱ������U_Ptr����������
//Ϊ�˷�ֹ�ⲿ���ã�U_ptr�����г�Ա����Ϊprivate��ֻ������HasPtr���Ե���
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
//HasPtr��Ĺؼ��Ǹ��ಢ��ֱ�ӹ����ڴ棬���ǰ���һ��U_Ptr��ʵ����ָ�룬�ɸ�U_Ptr��������ڴ棻
//������HasPtr���ڸ��ƻ�ֵ�����У������´�����HasPtr�����ָ��ͬһ��U_Ptrʵ����ֻ�е�U_Ptrʵ���ڲ�����Ϊ0ʱ���ڴ�Ż������ͷ�
//������һ��HasPtrʵ�������Ժ�����HasPtrʵ�����֡�����ָ�롱
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
	//ע�⣺����Ҫ�ȶ�Ŀ�����ü�����1���ٿ��Ǳ�������ü�����1��
	//��������ֵ������ʱ�����������U_Ptr�����ü���Ϊ0���������޷���ɸ�ֵ�����
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
	//ֻ�е�ָ���������ü���Ϊ��ʱ�����ͷŶ����ڴ棻����ֻ�ͷŸ�HasPtr������
	if (innerPtr->count == 0){
		delete innerPtr;
	}
}
//����C++ģ���ຯ�������Ͷ�������������Ľ��ͣ�http://www.cnblogs.com/qlwy/archive/2012/03/21/2410045.html
//���۾��ǣ�ģ����ĺ��������Ͷ��岻�ܷ����������ļ���
//�����������˷��ֿ��Է������ķ�����δ���ԣ�http://bbs.csdn.net/topics/320028181

int main(){
	//TestAutoPtr();
	int *p = new int(3);
	HasPtr<int> sPtr1(p, *p);
	HasPtr<int> sPtr2(sPtr1);
	return 0;
}