#include<iostream>
#include<vector>
using namespace std;

int minimum(int a, int b, int c){
	int tmp = a < b ? a : b;
	return tmp < c ? tmp : c;
}

int LevenshteinDist(string &s1, string &s2){
	//���ַ���s1Ϊ��׼�������ַ���s2������Levenshtein���룻��˱���Levenshtein��������鳤��Ϊ�ַ���s1�ĳ���+1
	vector<int> levenshteinDistsForLastLetter = vector<int>(s1.length() + 1);
	vector<int> levenshteinDistsForCurrentLetter = vector<int>(s1.length() + 1);
	//��ʼ��levenshteinDistsForLastLetter���飨���߼��������Ϊ���ַ�����s1��Levenshtein���룩
	for (auto i = 0; i < s1.length() + 1; i++)
	{
		levenshteinDistsForLastLetter[i] = i;
	}
	for (auto i = 0; i < s2.length(); i++){//���ַ���s2�е�ÿ���ַ���Ҫ���㣬��������ѭ���Ĵ�����s2.length
		for (auto j = 0; j < levenshteinDistsForLastLetter.size(); j++){//s2�е�ÿ���ַ���Ҫ�ͻ�׼�ַ�����Ҳ����s1���Ƚϣ�����ڲ�ѭ���Ĵ�����s1.length+1����Ϊ��Ҫ���ǵ�һ���ַ�ǰ�ġ��ա��ַ���
			if (j == 0){//����ǡ��ա��ַ����Ǳ�Ȼ��ִ��i�β������������ǰһ���ַ���Levenshtein�����1
				levenshteinDistsForCurrentLetter[j] = levenshteinDistsForLastLetter[j] + 1;
				continue;
			}
			else{
				int cost = s1[j - 1] == s2[i] ? 0 : 1;
				//��̬�滮��ȡ����������С��
				//levenshteinDistsForLastLetter[j] + 1������i-1���ַ�Ϊֹ���ַ����뵽��j���ַ�Ϊֹ�Ļ�׼�ַ���֮���Levenshtein������ϱ��ַ�
				//levenshteinDistsForCurrentLetter[j - 1] + 1������i���ַ�Ϊֹ���ַ����뵽��j-1���ַ�Ϊֹ�Ļ�׼�ַ���֮���Levenshtein������ϻ�׼�ַ����е�j���ַ�
				//levenshteinDistsForLastLetter[j - 1] + cost������i-1���ַ�Ϊֹ���ַ����뵽��j-1���ַ�Ϊֹ�Ļ�׼�ַ���֮���Levenshtein������ϱ��ַ��ͻ�׼�ַ����е�j���ַ�֮���Levenshtein����
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