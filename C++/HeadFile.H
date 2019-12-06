#ifndef HeadFile_H_INCLUDED
#define HeadFile_H_INCLUDED
#include<bits/stdc++.h>
using namespace std;
short TTZPG(short T , short AH[10])
{
	short N , I;
	T += 30;
	for(I = 0;I <= 9;I++)
	{
		if(AH[I] <= T)
		{
			N++;
		}
	}
	return N;
}
void SXHS(short I)
{
    long long N1 , N2 , A;
    short J;
    for (N1 = pow(10 , I - 1); N1 < pow(10 , I); N1++)
    {
        N2 = 0;
        for (J = 0;J <= I - 1;J++)
        {
            A = pow(10 , J);
            A = N1 / A;
            A = A % 10;
            A = pow(A , I);
            N2 = N2 + A;
        }
        if (N1 == N2)
        {
        	cout << N1 << " ";
		}
    }
    return;
}
bool SSTF(short N)
{
	short I;
	if(N == 2)
	{
		return true;
	}
	for(I = 0;I <= N - 1;I++)
	{
		if(N % 2 == 0)
		{
			return false;
		}
	}
	return true;
}
void CFB(short N)
{
	short I , J;
	for(I = 1;I <= N;I++)
	{
		for(J = 1;J <= I;J++)
		{
			cout << J << " * " << I << " = " << J * I << " ";
		}
		cout << endl;
	}
	return;
}
int FBNQSL(short N)
{
	if(N == 1)
	{
		return 1;
	}
	else if(N <= 0)
	{
		return 0;
	}
	else
	{
		return FBNQSL(N - 1) + FBNQSL(N - 2);
	}
}
short YWZMGS(string S)
{
	short I , Num = 0;
	short A[S.length()];
	for(I = 0;I <= S.length();I++)
	{
		A[I] = S[I];
	}
	for(I = 0;I <= S.length();I++)
	{
		if(A[I] >= 65 && A[I] <= 90)
		{
			Num++;
		}
		if(A[I] >= 97 && A[I] <= 122)
		{
			Num++;
		}
	}
	return Num;
}
void ZFCFX(string S)
{
	
	
	short I;
	for(I = S.length() - 1;I >= 0;I--)
	{
		cout << S[I];
	}
	return;
}
void ZFCLJ(string S1 , string S2)
{
	
	short I;
	for(I = 0;I <= S1.length() - 1;I++)
	{
		cout << S1[I];
	}
	for(I = 0;I <= S2.length() - 1;I++)
	{
		cout << S2[I];
	}
	return;
}
string SCJSXBZF(string S)
{
	short I;
	for(I = 0;I <= S.length() - 1;I++)
	{
		if(I % 2 == 0)
		{
			S[I] = '\0';
		}
	}
	return S;
}
void ZFCCMSC(short M , string S)
{
	short I;
	for(I = M;I <= S.length() - 1;I++)
	{
		cout << S[I];
	}
	return;
}
#endif
