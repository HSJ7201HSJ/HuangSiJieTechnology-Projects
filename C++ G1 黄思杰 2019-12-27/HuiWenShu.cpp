#include"C++ Head File 2019-12-27.h"
#include<bits/stdc++.h>
using namespace std;
int main()
{
	system("color 06");
	long long N;
	cin >> N;
	bool TF = HuiWenShu(N);
	if(TF == true)cout << "Yes��";
	else cout << "No��";
	return 0;
}
