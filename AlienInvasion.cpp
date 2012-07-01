#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <time.h>

using namespace std;

struct point{
  int x,y;
  point() {}
  point(int x_,int y_) {x=x_; y=y_;}
};

void count_path(const vector<string>& map,int path[][25],point xy){
  int x=xy.x;
  int y=xy.y;

  for (int i=0; i<25; ++i)
    for (int j=0; j<25; ++j)
      path[i][j]=-1;

  point wave[25*25];
  int start=0,end=0;
  path[y][x]=0;
  wave[end++]=point(x,y);

  point shifts[4]={
    point(-1,0),
    point(1,0),
    point(0,-1),
    point(0,1)
  };

  while (start != end) {
    point curr_p=wave[start++];
    int curr_len=path[curr_p.y][curr_p.x] + 1;

    for (int i=0; i<4; ++i) {

      point p=point(curr_p.x+shifts[i].x,curr_p.y+shifts[i].y);

      if (map[p.y][p.x] != 'W' &&
          path[p.y][p.x] == -1){
        path[p.y][p.x]=curr_len;
        wave[end++]=p;
      }
    }
  }
}

int invasionTime(const vector<string>& map,int x,int y){
  point locations[8];
  locations[0]=point(x,y);
  size_t locnum=1;
  size_t i,j;

  for (i=0; i<map.size(); ++i) {
    const string& s=map[i];
    for (j=0; j<s.size(); ++j) {
      if (s[j] == 'X')
        locations[locnum++]=point(j,i);
    }
  }
  if (locnum==1)
    return 0;

  int distances[8][8];

  int path[25][25];
  for (i=0; i<locnum; ++i) {
    count_path(map,path,locations[i]);
    for (j=0; j<locnum; ++j) {
      distances[i][j]=path[locations[j].y][locations[j].x];
      if ( distances[i][j]==-1 )
        return -1;
    }
  }

  int min_path=25*25;
  int perm[8];
  for (i=0; i<locnum; ++i) {
    perm[i]=i;
  }
  while( next_permutation(perm+1,perm+locnum) ){
    int curr_path=0;
    for (i=1; i<locnum; ++i) {
      curr_path += distances[perm[i-1]][perm[i]];
    }
    if (curr_path < min_path)
      min_path=curr_path;
  }

  return min_path;
}

string String[] ={
 string("WWWWWWWWWWWWWWWWWWWW"),
 string("W                  W"),
 string("W WWW  WWWWWWWWWW  W"),
 string("W W         W      W"),
 string("W W X       W      W"),
 string("W WWWWW     W X    W"),
 string("W W         WWWWWWWW"),
 string("W WWWWWWWW  W    X W"),
 string("W      X W         W"),
 string("WWWWWWWWWWWWWWWWWWWW")
};

int cdecl main(void){
  vector<string> Map(String,String+sizeof(String)/sizeof(*String));
  int x=1;
  int y=1;

  clock_t start, finish;
  start=clock();
  int result;
  for (int i=0; i<1000; ++i)
    result=invasionTime(Map,x,y);
  finish=clock();

  cout<<"result: "<<
    result<<endl;
  cout<<"time (microseconds) per one call: "<<
    (int)((double)(finish-start)/CLOCKS_PER_SEC*1000)<<endl;
  return 0;
}
