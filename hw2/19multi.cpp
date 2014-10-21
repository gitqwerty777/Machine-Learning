#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <string.h>
using namespace std;

struct data{
  data(double* xx, int yy){
    x = (double*)malloc(sizeof(double)*9);
    memcpy(x, xx, sizeof(double)*9);
    y = yy;
  }
  double* x; int y;
};

int cmpvalue;
bool datacmp(data a, data b){
    return a.x[cmpvalue] < b.x[cmpvalue];
}

std::vector<data> d;
int size;

struct h{
  h(int ss, double tt, int ii){
    s = ss; theta = tt; i = ii;
  }
  int s, i;
  double theta;
};

int main(int argc, char* argv[]){
  //load data
  FILE *f = fopen("hw2_train.dat", "r");
  double v[9]; int output; size = 0;
  while(fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %d", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8], &output) == 10){
    size++;
    d.push_back(data(v, output));
  }
  fclose(f);
  //do algorithm
  std::vector<h> best;
  int besterror = size;
  for(int j = 0; j < 9; j++){//i
    cmpvalue = j;
    std::sort(d.begin(), d.end(), datacmp);
    for(int i = 0; i <= size; i++){//theta
      //s = 1 and -1
      double nowtheta;
      if(i != 0 && i != size)
	nowtheta = double(d[i].x[j] + d[i-1].x[j]) / 2;
      else{
	if(i == 0)
	  nowtheta = double(d[0].x[j] - 1) / 2;
	else
	  nowtheta = double(d[size-1].x[j] + 1) / 2;
      }
      int errnum = 0;
      for(int t = 0; t < size; t++){
	if((d[t].x[j] - nowtheta) * d[t].y <= 0)
	  errnum++;
      }
      if(errnum < besterror){
	besterror = errnum;
	best.clear();
	best.push_back(h(1, nowtheta, j));
      } else if(errnum == besterror){
	best.push_back(h(1, nowtheta, j));
      }
      errnum = size - errnum;
      if(errnum < besterror){
	besterror = errnum;
	best.clear();
	best.push_back(h(-1, nowtheta, j));
      } else if(errnum == besterror){
	best.push_back(h(-1, nowtheta, j));
      }
    }
  }
  //random get the best ein
  double ri = rand() % best.size();
  h rans = best[ri];
  double ein = double(besterror) / size;
  printf("ein = %lf\n", ein);
  
  //read test
  int errcount = 0;
  f = fopen("hw2_test.dat", "r");
  size = 0;
  while(fscanf(f, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %d", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8],  &output) == 10){
    size++;
    if(double(rans.s) * (v[rans.i] - rans.theta) * output < 0)
      errcount++;
  }
  fclose(f);

  double eout = double(errcount) / size;
  printf("eout = %lf\n", eout);
  return 0;
}
