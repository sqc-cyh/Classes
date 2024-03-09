#include<stdio.h>
#include<math.h>
#define P 1000000007
#define N 10005
int min(int a,int b){return a<b?a:b;}
int f[N][25][2];
int n;
int main()
{
    f[0][0][1] = f[1][1][0] = f[1][1][1] = 1;
    scanf("%d", &n);
    for(int i = 2; i <= n; i++)
    {
        for(int j = log2(i+1)/2; j <= 2*log2(i+1); j++)
        {
            for(int k1 = 0; k1 <= i - 1; k1++)
            {
                int k2 = i - 1 - k1;
                f[i][j][0] += f[k1][j - 1][1] * f[k2][j - 1][1];//父亲是红色，两个儿子都是黑色 
                f[i][j][1] += f[k1][j][0] * f[k2][j][0] + f[k1][j][0] * f[k2][j - 1][1];//父亲是黑色，两个儿子是红色或者左红右黑 
                f[i][j][1] += f[k1][j - 1][1] * f[k2][j][0] + f[k1][j - 1][1] * f[k2][j - 1][1];//父亲是黑色，左黑右红或者两个儿子是黑色 
            }
        }
    }
    int sum = 0;
    for(int i = 0; i <= min(n, 20); i++)
        sum += f[n][i][1];
    printf("%d\n", sum);
}
