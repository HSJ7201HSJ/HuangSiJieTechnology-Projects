#include"C++ Head File 2019-12-27.h"
#include<bits/stdc++.h>
using namespace std;
int main()
{
	system("color 06");
	string S1 , S2;
	short N , I;
	getline(cin , S1);
	getline(cin , S2);
	cin >> N;
	string R = CZFC(S1 , S2 , N);
	for(I = 0;I <= R.length();I++)
	{
		cout << R[I];
	}
	return 0;
}
