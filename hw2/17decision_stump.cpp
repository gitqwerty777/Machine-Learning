#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

struct data{
  data(double xx, int yy){
    x = xx; y = yy;
  }
  double x; int y;
};

bool datacmp(data a, data b){
    return a.x < b.x;
}

std::vector<data> d;
int size = 20;
double avgein;
double avgeout;

inline double abs(double n){
  return n>0?n:-n;
}

int main(int argc, char* argv[]){
  for(int t = 0; t < 5000; t++){
    srand(time(NULL)+9527*t);
    //generate data
    d.clear();
    for(int i = 0; i < size; i++){
      double n = double(rand()) / 2147483647;
      int r = rand();
      if(r % 2)
	n = -n;
      int output = (n > 0)?1:-1;
      if(r % 10 == 1 || r % 10 == 2)
	output = -output;
      d.push_back(data(n, output));
    }
    //do algorithm
    std::sort(d.begin(), d.end(), datacmp);
    std::vector<int> bests, besttheta;
    bests.clear();
    besttheta.clear();
    int besterror; 
    int nowerror; double nowtheta = double(1+d[19].x)/2;
    nowerror = 0;
    for(int i = 0; i < 20; i++)//default s = +, theta = 1
      if(d[i].y == 1)
	nowerror++;
    if(nowerror > 10){
      bests.push_back(-1);
      besterror = 20-nowerror;
      besttheta.push_back(nowtheta);
    } else if(nowerror < 10){
      bests.push_back(1);
      besterror = nowerror;
      besttheta.push_back(nowtheta);
    } else {
      besterror = nowerror;
      bests.push_back(1);
      besttheta.push_back(nowtheta);
      bests.push_back(-1);
      besttheta.push_back(nowtheta);
    }
    for(int i = 19; i >= 0; i--){
      if(i != 0)
	nowtheta = double(d[i].x + d[i-1].x) / 2;
      else
	nowtheta = double(d[0].x - 1)/2;
      if(d[i].y == 1){//
	nowerror--;
	if(nowerror < besterror){
	  besterror = nowerror;
	  bests.clear();
	  besttheta.clear();
	  bests.push_back(1);
	  besttheta.push_back(nowtheta);
	} else if(nowerror == besterror){
	  bests.push_back(1);
	  besttheta.push_back(nowtheta);
	}
      } else {
	nowerror++;
	if(20 - nowerror < besterror){
	  besterror = 20-nowerror;
	  bests.clear();
	  besttheta.clear();
	  bests.push_back(-1);
	  besttheta.push_back(nowtheta);
	} else if(20 - nowerror == besterror){
	  bests.push_back(-1);
	  besttheta.push_back(nowtheta);
	}
      }
    }
    //random get the best ein
    int ri = rand() % bests.size();
    double ein = double(besterror) / 20;
    double eout = 0.5+0.3*bests[ri]*(abs(besttheta[ri]) - 1);
    avgein += ein;
    avgeout += eout;
  }
  avgein /= 5000;
  avgeout /= 5000;
  printf("avgein = %lf\n avgeout = %lf\n", avgein, avgeout);
  return 0;
}
