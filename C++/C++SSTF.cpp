#include"HeadFile.h"
#include<bits/stdc++.h>
using namespace std;
int main()
{
	short N;
	cin >> N;
	bool SS = SSTF(N);
	if(SS == false)
	{
		cout << N << "��������";
	}
	else if(SS == true)
	{
		cout << N << "������";
	}
	return 0;
}
