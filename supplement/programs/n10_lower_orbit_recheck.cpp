#include <bits/stdc++.h>
using namespace std;
using U=uint64_t;

struct Rule{U p;};

int main(){
    const int n=10;
    vector<pair<int,int>> pairs; int eid[10][10]; memset(eid,-1,sizeof(eid));
    for(int i=0,k=0;i<n;i++)for(int j=i+1;j<n;j++){eid[i][j]=k++;pairs.push_back({i,j});}
    vector<pair<int,int>> lp; int leid[8][8]; memset(leid,-1,sizeof(leid));
    for(int i=0,k=0;i<8;i++)for(int j=i+1;j<8;j++){leid[i][j]=k++;lp.push_back({i,j});}
    vector<pair<int,int>> q3;
    for(int a=0;a<8;a++)for(int bit: {1,2,4}){int b=a^bit;if(a<b)q3.push_back({a,b});}
    unordered_set<U> base; base.reserve(1000);
    vector<int> p(8); iota(p.begin(),p.end(),0);
    do{U m=0;for(auto[a,b]:q3){int x=min(p[a],p[b]),y=max(p[a],p[b]);m|=1ULL<<leid[x][y];}base.insert(m);}while(next_permutation(p.begin(),p.end()));
    vector<U> cubes; cubes.reserve(40000); unordered_set<U> seenC; seenC.reserve(40000);
    for(int mask=0;mask<(1<<10);mask++) if(__builtin_popcount((unsigned)mask)==8){vector<int>S;for(int i=0;i<10;i++)if(mask>>i&1)S.push_back(i);for(U bm:base){U gm=0;for(auto[a,b]:lp)if(bm>>leid[a][b]&1)gm|=1ULL<<eid[min(S[a],S[b])][max(S[a],S[b])];if(seenC.insert(gm).second)cubes.push_back(gm);}}
    bool inbase[45]={0};for(auto[a,b]:q3)inbase[eid[a][b]]=1;
    vector<int> outs; int oi[45]; fill(oi,oi+45,-1); for(int e=0;e<45;e++)if(!inbase[e]){oi[e]=outs.size();outs.push_back(e);} 
    vector<vector<U>> rules(outs.size());
    for(U c:cubes){U om=0;for(int i=0;i<33;i++)if(c>>outs[i]&1)om|=1ULL<<i;for(int i=0;i<33;i++)if(om>>i&1)rules[i].push_back(om^(1ULL<<i));}
    for(auto&r:rules){sort(r.begin(),r.end());r.erase(unique(r.begin(),r.end()),r.end());}


    struct Perm {int v[10]; U emap[33];};
    vector<Perm> G;
    vector<int> bp={0,1,2};
    do{
      for(int mask=0;mask<8;mask++){
        int q[8];
        for(int x=0;x<8;x++){
          int y=0; for(int b=0;b<3;b++) if(mask>>b&1) y|=1<<b;
          int z=0; for(int b=0;b<3;b++) if(x>>bp[b]&1) z|=1<<b;
          q[x]=z^y;
        }
        if(!(set<int>{q[0],q[1]}==set<int>{0,1})) continue;
        for(int sw=0;sw<2;sw++){
          Perm P{};
          for(int x=0;x<8;x++)P.v[x]=q[x];
          P.v[8]=sw?9:8; P.v[9]=sw?8:9;
          for(int i=0;i<33;i++){auto [a,b]=pairs[outs[i]];int x=P.v[a],y=P.v[b]; if(x>y)swap(x,y);int oe=oi[eid[x][y]];P.emap[i]=1ULL<<oe;}
          G.push_back(P);
        }
      }
    }while(next_permutation(bp.begin(),bp.end()));
    sort(G.begin(),G.end(),[](const Perm&a,const Perm&b){for(int i=0;i<33;i++)if(a.emap[i]!=b.emap[i])return a.emap[i]<b.emap[i];return false;});
    G.erase(unique(G.begin(),G.end(),[](const Perm&a,const Perm&b){for(int i=0;i<33;i++)if(a.emap[i]!=b.emap[i])return false;return true;}),G.end());
    cerr<<"group="<<G.size()<<" cubes="<<cubes.size()<<" base="<<base.size()<<"\n";

    auto transform=[&](U s,const Perm&P){U z=0;while(s){U b=s&-s;int i=__builtin_ctzll(s);for(int j=0;j<33;j++){if(P.emap[i]>>j&1){z|=1ULL<<j;break;}}s^=b;}return z;};
    auto canon=[&](U s){U c=s;for(auto&P:G)c=min(c,transform(s,P));return c;};
    auto closure=[&](U s){
      U full=(1ULL<<33)-1;
      while(true){U add=0;for(int e=0;e<33;e++)if(!(s>>e&1)){bool ok=false;for(U r:rules[e]){if((r&~s)==0){ok=true;break;}}if(ok)add|=1ULL<<e;}add&=~s;if(!add)return s;s|=add;if(s==full)return s;}
    };

    unordered_set<U> reps; reps.reserve(60000);

    long long total=0;
    for(int a=0;a<33-5;a++)for(int b=a+1;b<33-4;b++)for(int c=b+1;c<33-3;c++)for(int d=c+1;d<33-2;d++)for(int e=d+1;e<33-1;e++)for(int f=e+1;f<33;f++){
      U s=(1ULL<<a)|(1ULL<<b)|(1ULL<<c)|(1ULL<<d)|(1ULL<<e)|(1ULL<<f); ++total;
      U cc=canon(s); reps.insert(cc);
    }
    cerr<<"all="<<total<<" reps="<<reps.size()<<"\n";
    long long fullcount=0; U witness=0; map<int,long long> hist;
    for(U s:reps){U c=closure(s);int pc=__builtin_popcountll(c);hist[pc]++;if(pc==33){fullcount++;witness=s;}}
    cerr<<"full reps="<<fullcount<<"\n";
    for(auto [k,v]:hist) cerr<<k<<":"<<v<<" ";
    cerr<<"\n";
    if(fullcount){
      cout<<"FOUND\n";for(int i=0;i<33;i++)if(witness>>i&1){auto[a,b]=pairs[outs[i]];cout<<a<<","<<b<<"\n";}
      return 1;
    }
    cout<<"PASS: no 6-edge seed percolates, hence wsat(10,Q3)>=18 under the canonical first-edge reduction.\n";
}
