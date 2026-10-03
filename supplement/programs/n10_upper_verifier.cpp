#include <bits/stdc++.h>
using namespace std; using U=uint64_t;
int main(){
 int n=10; vector<pair<int,int>> P; int eid[10][10]; memset(eid,-1,sizeof(eid)); for(int i=0,k=0;i<n;i++)for(int j=i+1;j<n;j++){eid[i][j]=k++;P.push_back({i,j});}
 vector<pair<int,int>> q3; for(int a=0;a<8;a++)for(int b=a+1;b<8;b++)if(__builtin_popcount((unsigned)(a^b))==1) q3.push_back({a,b});
 vector<pair<int,int>> LP; int leid[8][8]; for(int i=0,k=0;i<8;i++)for(int j=i+1;j<8;j++){leid[i][j]=k++;LP.push_back({i,j});}
 unordered_set<U> base; vector<int> perm(8); iota(perm.begin(),perm.end(),0); do{U m=0; for(auto[a,b]:q3)m|=1ULL<<leid[min(perm[a],perm[b])][max(perm[a],perm[b])]; base.insert(m);}while(next_permutation(perm.begin(),perm.end()));
 unordered_set<U> cubes; cubes.reserve(40000); for(int mask=0;mask<1<<10;mask++) if(__builtin_popcount((unsigned)mask)==8){vector<int>s;for(int i=0;i<10;i++)if(mask>>i&1)s.push_back(i);for(U bm:base){U c=0;for(auto[a,b]:LP)if(bm>>leid[a][b]&1)c|=1ULL<<eid[min(s[a],s[b])][max(s[a],s[b])];cubes.insert(c);}}
 vector<pair<int,int>> E0={{0,2},{0,4},{1,2},{1,3},{1,5},{1,9},{2,3},{2,6},{2,9},{3,5},{3,6},{3,7},{4,5},{4,6},{5,7},{6,7},{6,8},{7,8}};
 vector<pair<int,int>> add={{0,1},{3,9},{0,7},{1,7},{0,3},{2,5},{4,7},{1,6},{2,7},{5,6},{2,4},{3,4},{0,5},{0,6},{1,4},{5,9},{4,9},{6,9},{0,9},{7,9},{2,8},{5,8},{1,8},{3,8},{4,8},{0,8},{8,9}};
 U g=0; for(auto[a,b]:E0)g|=1ULL<<eid[min(a,b)][max(a,b)];
 int qfree=0; for(U c:cubes)if((c&~g)==0)qfree++; if(qfree){cerr<<"FAIL initial cube count="<<qfree<<"\n";return 1;}
 cout<<"initial Q3-free, edges="<<__builtin_popcountll(g)<<"\n";
 for(int step=0;step<(int)add.size();step++){auto[a,b]=add[step];U eb=1ULL<<eid[min(a,b)][max(a,b)];if(g&eb){cerr<<"FAIL repeated edge\n";return 1;} bool found=false; for(U c:cubes)if((c&eb) && ((c&~g)==eb)){found=true;break;} if(!found){cerr<<"FAIL step "<<step+1<<" edge "<<a<<","<<b<<"\n";return 1;} g|=eb; cout<<step+1<<": ("<<a<<","<<b<<") ok\n"; }
 cout<<"final edges="<<__builtin_popcountll(g)<<" complete="<<(g==((1ULL<<45)-1))<<"\nPASS\n";
}
