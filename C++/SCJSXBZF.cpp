#include"HeadFile.h"
#include<bits/stdc++.h>
using namespace std;
int main()
{
	string S;
	short I;
	getline(cin , S);
	string E = SCJSXBZF(S);
	for(I = 0;I <= E.length() - 1;I++)
	{
		if(E[I] != '\0')
		{
			cout << E[I];
		}
	}
    return 0;
}
