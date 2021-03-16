#include<stdio.h>
#include<stdlib.h>
#define INF 123456456
int chain(int a[],int len){
	int n=len;
	int m[n+1][n+1],s[n+1][n+1];
	int i,j,k;
	for(i=1;i<=n;i++)
		m[i][i]=0;
	int l;
	for(l=2;l<=n;l++){
		for(i=1;i<=n-l+1;i++){
			j = i+l-1;
			m[i][j] = INF;
			for(k=i;k<=j-1;k++){
				int sum=0,t;
				for(t=i;t<=j;t++)
					sum+=a[t];
				int q = m[i][k]+m[k+1][j]+sum;
				if(q<m[i][j]){
					m[i][j]=q;
					s[i][j]=k;
				}
			}
		}
	}
	return m[1][n];
}
int main(){
	int n;
	scanf("%d",&n);
	int a[n+1];
	int i;
	for(i=1;i<=n;i++){
		scanf("%d",&a[i]);
	}
	int ans = chain(a,n);
	printf("%d",ans);
}
