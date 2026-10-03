#include <bits/stdc++.h>
#include <omp.h>
using namespace std; using U=uint64_t; static inline int pc(U x){return __builtin_popcountll(x);} 
struct Data{int m;vector<pair<int,int>> pairs,outs;int eid[11][11];vector<U> cubes;vector<array<U,43>> G;};
Data build(){
 Data D;memset(D.eid,-1,sizeof(D.eid));for(int i=0,k=0;i<11;i++)for(int j=i+1;j<11;j++){D.eid[i][j]=k++;D.pairs.push_back({i,j});}
 vector<pair<int,int>> q3;for(int a=0;a<8;a++)for(int b:{1,2,4}){int c=a^b;if(a<c)q3.push_back({a,c});}
 U core=0;for(auto[a,b]:q3)core|=1ULL<<D.eid[a][b];
 int oi[55];fill(oi,oi+55,-1);for(int e=0;e<55;e++)if(!(core>>e&1)){oi[e]=D.outs.size();D.outs.push_back(D.pairs[e]);}D.m=D.outs.size();
 vector<pair<int,int>> lp;int leid[8][8];for(int i=0,k=0;i<8;i++)for(int j=i+1;j<8;j++){leid[i][j]=k++;lp.push_back({i,j});}
 unordered_set<U> base;base.reserve(1000);vector<int> p(8);iota(p.begin(),p.end(),0);do{U bm=0;for(auto[a,b]:q3){int x=min(p[a],p[b]),y=max(p[a],p[b]);bm|=1ULL<<leid[x][y];}base.insert(bm);}while(next_permutation(p.begin(),p.end()));
 unordered_set<U> cubes;cubes.reserve(140000);for(int sm=0;sm<(1<<11);sm++)if(pc((U)sm)==8){vector<int>S;for(int i=0;i<11;i++)if(sm>>i&1)S.push_back(i);for(U bm:base){U em=0;for(auto[a,b]:lp)if(bm>>leid[a][b]&1){int x=S[a],y=S[b];if(x>y)swap(x,y);em|=1ULL<<D.eid[x][y];}U om=0;for(int i=0;i<D.m;i++){auto[a,b]=D.outs[i];if(em>>D.eid[a][b]&1)om|=1ULL<<i;}cubes.insert(om);}}
 D.cubes.assign(cubes.begin(),cubes.end());
 vector<int> coord={0,1,2};vector<int> ov={8,9,10};
 do{for(int mask=0;mask<8;mask++){int q[8];for(int x=0;x<8;x++){int z=0;for(int b=0;b<3;b++)if(x>>coord[b]&1)z|=1<<b;q[x]=z^mask;}if(set<int>{q[0],q[1]}!=set<int>{0,1})continue;do{array<U,43> mp{};for(int i=0;i<D.m;i++){auto[a,b]=D.outs[i];int x=(a<8?q[a]:ov[a-8]);int y=(b<8?q[b]:ov[b-8]);if(x>y)swap(x,y);int j=oi[D.eid[x][y]];mp[i]=1ULL<<j;}D.G.push_back(mp);}while(next_permutation(ov.begin(),ov.end()));}}while(next_permutation(coord.begin(),coord.end()));
 sort(D.G.begin(),D.G.end());D.G.erase(unique(D.G.begin(),D.G.end()),D.G.end());
 cerr<<"m="<<D.m<<" cubes="<<D.cubes.size()<<" group="<<D.G.size()<<"\n";return D;
}
static inline U trans(U s,const array<U,43>&mp){U z=0;while(s){int i=__builtin_ctzll(s);z|=mp[i];s&=s-1;}return z;}
static inline U canon(U s,const vector<array<U,43>>&G){U c=s;for(auto &g:G)c=min(c,trans(s,g));return c;}
static inline U closure(const Data&D,U s){U full=(1ULL<<D.m)-1;while(1){U add=0;for(U om:D.cubes){U miss=om&~s;if(miss && !(miss&(miss-1)))add|=miss;}add&=~s;if(!add)return s;s|=add;if(s==full)return s;}}
int main(){Data D=build();

 vector<U> reps6v;reps6v.reserve(400000);for(int a=0;a<D.m-5;a++)for(int b=a+1;b<D.m-4;b++)for(int c=b+1;c<D.m-3;c++)for(int d=c+1;d<D.m-2;d++)for(int e=d+1;e<D.m-1;e++)for(int f=e+1;f<D.m;f++){U s=(1ULL<<a)|(1ULL<<b)|(1ULL<<c)|(1ULL<<d)|(1ULL<<e)|(1ULL<<f);reps6v.push_back(canon(s,D.G));}
sort(reps6v.begin(),reps6v.end());reps6v.erase(unique(reps6v.begin(),reps6v.end()),reps6v.end());cerr<<"reps6="<<reps6v.size()<<"\n";
 int T=omp_get_max_threads();vector<vector<U>> local(T);for(auto &v:local)v.reserve((reps6v.size()*37)/T+1000);
 auto t0=chrono::steady_clock::now();
 #pragma omp parallel for schedule(dynamic,32)
 for(long long ii=0;ii<(long long)reps6v.size();ii++){
   int tid=omp_get_thread_num();U s=reps6v[ii];U miss=((1ULL<<D.m)-1)^s;while(miss){U b=miss&-miss;miss^=b;local[tid].push_back(canon(s|b,D.G));}
 }
 vector<U> reps7;size_t total=0;for(auto &v:local)total+=v.size();reps7.reserve(total);for(auto &v:local)reps7.insert(reps7.end(),v.begin(),v.end());double tg=chrono::duration<double>(chrono::steady_clock::now()-t0).count();cerr<<"generated children="<<total<<" gen_sec="<<tg<<"\n";
 sort(reps7.begin(),reps7.end());reps7.erase(unique(reps7.begin(),reps7.end()),reps7.end()); { std::ofstream dump("n11_reps7.bin", std::ios::binary); uint64_t r7n=reps7.size(); dump.write(reinterpret_cast<char*>(&r7n), sizeof(r7n)); dump.write(reinterpret_cast<const char*>(reps7.data()), reps7.size()*sizeof(U)); }cerr<<"reps7="<<reps7.size()<<" dedup_sec="<<chrono::duration<double>(chrono::steady_clock::now()-t0).count()-tg<<"\n";
 auto t1=chrono::steady_clock::now();array<unsigned long long,44> hist{};U full=(1ULL<<D.m)-1;unsigned long long fullcount=0;U wit=0;int mx=0;
 #pragma omp parallel for schedule(dynamic,32)
 for(long long ii=0;ii<(long long)reps7.size();ii++){
   U c=closure(D,reps7[ii]);int p=pc(c);
   #pragma omp atomic
   hist[p]++; if(p>mx){
     #pragma omp critical
     {mx=max(mx,p);} } if(c==full){
     #pragma omp atomic
     fullcount++;
     #pragma omp critical
     {wit=reps7[ii];} }
 }
 double tc=chrono::duration<double>(chrono::steady_clock::now()-t1).count();cerr<<"closure_sec="<<tc<<" full_reps="<<fullcount<<" maxclosure="<<mx<<"\n";for(int i=0;i<44;i++)if(hist[i])cerr<<i<<":"<<hist[i]<<" ";cerr<<"\n";
 if(fullcount){cerr<<"FOUND percolating 7-seed orbit representative:";for(int i=0;i<D.m;i++)if(wit>>i&1)cerr<<" "<<i;cerr<<"\n";return 1;}
 cout<<"PASS: no 7-edge seed percolates. Thus wsat(K11,Q3)>=19 after the canonical first-edge reduction.\n";
}
