#include"HeadFile.h"
#include<bits/stdc++.h>
using namespace std;
int main()
{
	short X , J;
	J = 1;
	cin >> X;
	while(J <= X)
	{
		cout << FBNQSL(J) << " ";
		J++;
	}
	return 0;
}
