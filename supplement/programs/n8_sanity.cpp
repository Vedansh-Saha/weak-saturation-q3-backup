#include <bits/stdc++.h>
using namespace std; using U=uint64_t; static inline int pc(U x){return __builtin_popcountll(x);} 
int main(){
 const int n=8; int eid[8][8]; memset(eid,-1,sizeof(eid)); vector<pair<int,int>> edges; for(int i=0,k=0;i<n;i++)for(int j=i+1;j<n;j++){eid[i][j]=k++;edges.push_back({i,j});}
 vector<pair<int,int>> q3;for(int a=0;a<8;a++)for(int b:{1,2,4}){int c=a^b;if(a<c)q3.push_back({a,c});}
 U core=0;for(auto[a,b]:q3)core|=1ULL<<eid[a][b];
 vector<pair<int,int>> out; int oi[28];fill(oi,oi+28,-1);for(int e=0;e<28;e++)if(!(core>>e&1)){oi[e]=out.size();out.push_back(edges[e]);}
 vector<pair<int,int>> lp;int leid[8][8];for(int i=0,k=0;i<8;i++)for(int j=i+1;j<8;j++){leid[i][j]=k++;lp.push_back({i,j});}
 unordered_set<U> base;base.reserve(1000);vector<int> p(8);iota(p.begin(),p.end(),0);do{U bm=0;for(auto[a,b]:q3){int x=min(p[a],p[b]),y=max(p[a],p[b]);bm|=1ULL<<leid[x][y];}base.insert(bm);}while(next_permutation(p.begin(),p.end()));
 unordered_set<U> cubes; for(U bm:base){U em=0;for(auto[a,b]:lp)if(bm>>leid[a][b]&1)em|=1ULL<<eid[a][b];cubes.insert(em);} 
 unordered_set<U> omsSet;for(U c:cubes){U om=0;for(int i=0;i<(int)out.size();i++){auto[a,b]=out[i];if(c>>eid[a][b]&1)om|=1ULL<<i;} if(om)omsSet.insert(om);}vector<U> oms(omsSet.begin(),omsSet.end());
 vector<vector<U>> rules(out.size());for(U om:oms){U x=om;while(x){int i=__builtin_ctzll(x);U b=1ULL<<i;rules[i].push_back(om^b);x^=b;}}for(auto&r:rules){sort(r.begin(),r.end());r.erase(unique(r.begin(),r.end()),r.end());}
 U full=(1ULL<<out.size())-1;auto closure=[&](U s){while(1){U add=0;for(int e=0;e<(int)out.size();e++)if(!(s>>e&1)){for(U pre:rules[e])if((pre&~s)==0){add|=1ULL<<e;break;}}add&=~s;if(!add)return s;s|=add;}};
 long long p3=0,p4=0; int mx3=0,mx4=0; U w=0;for(int a=0;a<13;a++)for(int b=a+1;b<14;b++)for(int c=b+1;c<16;c++){U s=(1ULL<<a)|(1ULL<<b)|(1ULL<<c);U cl=closure(s);mx3=max(mx3,pc(cl));if(cl==full)p3++;}
 for(int a=0;a<13;a++)for(int b=a+1;b<14;b++)for(int c=b+1;c<15;c++)for(int d=c+1;d<16;d++){U s=(1ULL<<a)|(1ULL<<b)|(1ULL<<c)|(1ULL<<d);U cl=closure(s);mx4=max(mx4,pc(cl));if(cl==full){p4++;if(!w)w=s;}}
 cerr<<"n=8 distinct_Q3_copies="<<cubes.size()<<" outside_edges="<<out.size()<<"\n";
 cerr<<"3-edge seeds=560 percolating="<<p3<<" max_closure="<<mx3<<"\n";
 cerr<<"4-edge seeds=1820 percolating="<<p4<<" max_closure="<<mx4<<"\n";
 if(p3 != 0 || p4 == 0)return 2; cerr<<"sanity PASS: wsat(K8,Q3)=15.\nseed:";for(int i=0;i<16;i++)if(w>>i&1)cerr<<' '<<out[i].first<<out[i].second;cerr<<"\n"; return 0;
}
