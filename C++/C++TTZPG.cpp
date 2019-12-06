#include"HeadFile.h"
#include<bits/stdc++.h>
using namespace std;
int main()
{
	short I;
	short AH[10];
	for(I = 0;I <= 9;I++)
	{
		cin >> AH[I];
	}
	short T;
	cin >> T;
	short N = TTZPG(T , AH);
	cout << N;
	return 0;
}
