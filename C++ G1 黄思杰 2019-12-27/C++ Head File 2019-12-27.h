#ifndef C++_Head_File_2019-12-27_H_INCLUDED
#define C++_Head_File_2019-12-27_H_INCLUDED
#include<bits/stdc++.h>
using namespace std;
bool HuiWenShu(long long N)
{
	short G = 0 , S = 0 , B = 0 , Q = 0 , W = 0;
	W = N / 10000;
	Q = N /1000 % 10;
	B = N / 100 % 10;
	S = N / 10 % 10;
	G = N % 10;
	if(G != W || S != Q)return false;
	return true;
}
void ZhuanZhiJuZhen()
{
	short N[4][4];
	cin >> N[0][0] >> N[1][0] >> N[2][0] >> N[3][0] >> N[0][1] >> N[1][1] >> N[2][1] >> N[3][1]>> N[0][2] >> N[1][2] >> N[2][2] >> N[3][2] >> N[0][3] >> N[1][3] >> N[2][3] >> N[3][3];
	short I , J;
	for(I = 0;I <= 3;I++)
	{
		for(J = 0;J <= 3;J++)
		{
			cout << N[I][J] << " ";
		}
		cout << endl;
	}
}
struct Student
{
	string N;
	short A;
};
struct Student Stu(Student S[] , short N)
{
	Student M;
	short I;
	M = S[0];
	for(I = 1;I <= N - 1;I++)
	{
		if(S[I].A > M.A)M = S[I];
	}
	return M;
}
struct STUdent
{
	short I;
	short S;
};
struct STUdent STU(STUdent SS[] , short N)
{
	STUdent M;
	short I;
	M = SS[0];
	for(I = 1;I <= N - 1;I++)
	{
		if(SS[I].S > M.S)M = SS[I];
	}
	return M;
}
string CZFC(string S1 , string S2 , short N)
{
	string R;
	short I;
	for(I = 0;I <= N - 1;I++)
	{
		R[I] = S1[I];
	}
	for(I = N;I <= N + S2.length() - 1;I++)
	{
		R[I] = S2[I];
	}
	for(I = N + S2.length();I <= S1.length() - S2.length() - 2;I++)
	{
		R[I] = S1[I];
	}
	return R;
}
#endif
