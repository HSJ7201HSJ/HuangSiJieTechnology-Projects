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
		cout << N << "不是素数";
	}
	else if(SS == true)
	{
		cout << N << "是素数";
	}
	return 0;
}
