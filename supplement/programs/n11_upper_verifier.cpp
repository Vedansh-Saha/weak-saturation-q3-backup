#include <bits/stdc++.h>
using namespace std; using U=uint64_t;
int main(){
 const int n=11; vector<pair<int,int>> E; int eid[11][11]; memset(eid,-1,sizeof(eid));
 for(int i=0,k=0;i<n;i++)for(int j=i+1;j<n;j++){eid[i][j]=k++;E.push_back({i,j});}
 vector<pair<int,int>> q; for(int a=0;a<8;a++)for(int b:{1,2,4}){int c=a^b;if(a<c)q.push_back({a,c});}
 vector<pair<int,int>> L; int lid[8][8]; memset(lid,-1,sizeof(lid));
 for(int i=0,k=0;i<8;i++)for(int j=i+1;j<8;j++){lid[i][j]=k++;L.push_back({i,j});}
 unordered_set<U> base;base.reserve(1000); vector<int> p(8);iota(p.begin(),p.end(),0);
 do{U m=0;for(auto [a,b]:q){int x=min(p[a],p[b]),y=max(p[a],p[b]);m|=1ULL<<lid[x][y];}base.insert(m);}while(next_permutation(p.begin(),p.end()));
 assert(base.size()==840);
 vector<U> cubes;cubes.reserve(138600);unordered_set<U> seen;seen.reserve(138600);
 for(int sm=0;sm<(1<<n);sm++) if(__builtin_popcount((unsigned)sm)==8){vector<int>S;for(int i=0;i<n;i++)if(sm>>i&1)S.push_back(i);for(U bm:base){U cm=0;for(auto [a,b]:L)if(bm>>lid[a][b]&1){int x=S[a],y=S[b];if(x>y)swap(x,y);cm|=1ULL<<eid[x][y];}if(seen.insert(cm).second)cubes.push_back(cm);}}
 cerr<<"cubes="<<cubes.size()<<"\n";
 vector<pair<int,int>> init={{0,2},{0,4},{1,3},{1,5},{2,3},{2,6},{3,7},{4,5},{4,6},{5,7},{6,7},
 {3,5},{3,8},{3,10},{4,8},{6,8},{7,10},{8,9},{9,10}};
 auto emask=[&](pair<int,int>e){auto[a,b]=e;if(a>b)swap(a,b);return 1ULL<<eid[a][b];};
 U g=0;for(auto e:init)g|=emask(e);assert(__builtin_popcountll(g)==19);
 long long qfree=0;for(U c:cubes)if((c&~g)==0)qfree++;cout<<"initial_edges="<<__builtin_popcountll(g)<<" initial_Q3="<<qfree<<"\n";assert(qfree==0);
 vector<pair<int,int>> add={{0,1},{0,8},{2,5},{0,7},{0,9},{1,6},{1,8},{0,3},{0,5},{0,6},{0,10},{1,2},{1,4},{1,7},{1,9},{1,10},{2,4},{2,7},{2,8},{2,9},{2,10},{3,4},{3,6},{3,9},{4,7},{4,9},{4,10},{5,6},{5,8},{5,9},{5,10},{6,9},{6,10},{7,8},{7,9},{8,10}};
 assert(add.size()==36);
 int k=0;for(auto e:add){++k;U b=emask(e);assert(!(g&b));bool ok=false;for(U c:cubes)if((c&b) && (c&~g)==b){ok=true;break;}if(!ok){cerr<<"no witness step "<<k<<" edge "<<e.first<<","<<e.second<<"\n";return 2;}g|=b;cout<<k<<":"<<e.first<<","<<e.second<<" ok\n";}
 cout<<"final_edges="<<__builtin_popcountll(g)<<" complete="<<(g==((1ULL<<55)-1))<<"\n";assert(g==((1ULL<<55)-1));cout<<"PASS\n";
}
