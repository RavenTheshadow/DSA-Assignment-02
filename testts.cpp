#include <bits/stdc++.h>

using namespace std;

int MAXSIZE = 10000;
vector <vector <int>> dp(1000 + 1, vector <int>(1000 + 1, 0));
int C (const int &k, const int &n)		{
	if(k == 0 || k == n) return 1;
    
    if(!dp[n][k]) dp[n][k] = (C(k, n - 1) % MAXSIZE + C (k - 1, n - 1) % MAXSIZE ) % MAXSIZE;
	
    return dp[n][k];
}

int main(){
    cout << C(3, 50);
}