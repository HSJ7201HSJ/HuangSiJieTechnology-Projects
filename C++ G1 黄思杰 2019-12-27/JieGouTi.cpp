#include"C++ Head File 2019-12-27.h"
#include<bits/stdc++.h>
using namespace std;
int main()
{
	system("color 06");
	short N , I;
	cin >> N;
	Student S[N];
	for(I = 0;I <= N - 1;I++)
	{
		cin >> S[I].N >> S[I].A;
	}
	Student R = Stu(S , N);
	cout << R.N << " " << R.A;
	return 0;
}
