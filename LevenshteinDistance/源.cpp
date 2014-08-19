#include<iostream>
#include<vector>
using namespace std;

int minimum(int a, int b, int c){
	int tmp = a < b ? a : b;
	return tmp < c ? tmp : c;
}

int LevenshteinDist(string &s1, string &s2){
	//设字符串s1为基准，计算字符串s2到它的Levenshtein距离；因此保存Levenshtein距离的数组长度为字符串s1的长度+1
	vector<int> levenshteinDistsForLastLetter = vector<int>(s1.length() + 1);
	vector<int> levenshteinDistsForCurrentLetter = vector<int>(s1.length() + 1);
	//初始化levenshteinDistsForLastLetter数组（其逻辑可以理解为空字符串到s1的Levenshtein距离）
	for (auto i = 0; i < s1.length() + 1; i++)
	{
		levenshteinDistsForLastLetter[i] = i;
	}
	for (auto i = 0; i < s2.length(); i++){//对字符串s2中的每个字符都要计算，因此最外层循环的次数是s2.length
		for (auto j = 0; j < levenshteinDistsForLastLetter.size(); j++){//s2中的每个字符都要和基准字符串（也就是s1）比较，因此内层循环的次数是s1.length+1（因为还要考虑第一个字符前的“空”字符）
			if (j == 0){//如果是“空”字符，那必然是执行i次插入操作，即比前一个字符的Levenshtein距离加1
				levenshteinDistsForCurrentLetter[j] = levenshteinDistsForLastLetter[j] + 1;
				continue;
			}
			else{
				int cost = s1[j - 1] == s2[i] ? 0 : 1;
				//动态规划：取下面三个最小者
				//levenshteinDistsForLastLetter[j] + 1：到第i-1个字符为止的字符串与到第j个字符为止的基准字符串之间的Levenshtein距离加上本字符
				//levenshteinDistsForCurrentLetter[j - 1] + 1：到第i个字符为止的字符串与到第j-1个字符为止的基准字符串之间的Levenshtein距离加上基准字符串中第j个字符
				//levenshteinDistsForLastLetter[j - 1] + cost：到第i-1个字符为止的字符串与到第j-1个字符为止的基准字符串之间的Levenshtein距离加上本字符和基准字符串中第j个字符之间的Levenshtein距离
				levenshteinDistsForCurrentLetter[j] = minimum(levenshteinDistsForLastLetter[j] + 1, levenshteinDistsForCurrentLetter[j - 1] + 1, levenshteinDistsForLastLetter[j - 1] + cost);
			}
		}
		levenshteinDistsForLastLetter = vector<int>(levenshteinDistsForCurrentLetter);
	}
	return levenshteinDistsForLastLetter.back();
}

int main(){
	string s1 = "gambol", s2 = "gumbo";
	cout<<LevenshteinDist(s1, s2)<<endl;
	return 0;
}