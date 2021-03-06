#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

double w[5];

struct wvec{
  wvec(double* inw, int mn){
    for(int i = 0; i < 5; i++)
      w[i] = inw[i];
    misnum = mn;
  }
  double w[5];
  int misnum;
};

std::vector<wvec> warr;
long long correct_count = 0;
std::vector<struct xy> data;
int size;//data size

struct xy{
  double x[5];
  bool y;
  xy(double a, double b, double c, double d, double e, bool bl){
    x[0] = a; x[1] = b; x[2] = c; x[3] = d; x[4] = e;
    y = bl;
  }
  bool check_sign(double* nw){
    double ans = 0;
    for(int i = 0; i < 5; i++)
      ans += nw[i] * x[i];
    bool anssign = (ans>0)?true:false;
    return (anssign && y) || (!anssign && !y);
  }
  bool check_sign(){
    double ans = 0;
    for(int i = 0; i < 5; i++)
      ans += w[i] * x[i];
    bool anssign = (ans>0)?true:false;
    return (anssign && y) || (!anssign && !y);
  }
  void correct_mistake(){
    if(y){//new w = w + x
      for(int i = 0; i < 5; i++)
	w[i] = w[i] + x[i];
    } else {//w = w - x;
      for(int i = 0; i < 5; i++)
	w[i] = w[i] - x[i];
    }
  }
  void record_w(){
    int mn = 0;
    for(int i = 0; i < size; i++){
      if(!data[i].check_sign())
	mn++;
    }
    warr.push_back(wvec(w, mn));
  }
};

int main(){
  FILE *fin = fopen("pocket_hw1_18_train.dat", "r");
  if(!fin)
    printf("file open error\n");

  double x[5];  int y;
  while(fscanf(fin, "%lf %lf %lf %lf %d", &x[0], &x[1], &x[2], &x[3], &y) == 5){
    xy temp(x[0], x[1], x[2], x[3], 1, (y > 0));
    data.push_back(temp);
  }

  size = data.size();
  int selected;  int nowindex;
  long testnum = 0;
  long errornum = 0; // test correct
  for(int t = 0; t < 2000; t++){
    srand(time(NULL)+t*t*9527);
    int update = 0;
    warr.clear();
    w[0] = w[1] = w[2] = w[3] = w[4] = 0;
    while(update < 50){
      nowindex = rand() % size;
      while(data[nowindex].check_sign())//if all no mistake, loop
	nowindex = rand() % size;
      data[nowindex].correct_mistake();
      data[nowindex].record_w();
      update++;
    }

    //find minimum
    int minmn = size;
    int minindex = 0;
    for(int i = 0; i < 50; i++)
      if(warr[i].misnum < minmn){
	minmn = warr[i].misnum;
	minindex = i;
      }
    for(int i = 0; i < 5; i++)
      w[i] = warr[minindex].w[i];
    printf("w = [%lf, %lf, %lf, %lf, %lf]\n", w[0], w[1], w[2], w[3], w[4]);
    
    //test
    FILE* ftest = fopen("pocket_hw1_18_test.dat", "r");
    while(fscanf(ftest, "%lf %lf %lf %lf %d", &x[0], &x[1], &x[2], &x[3], &y) == 5){
      xy temp(x[0], x[1], x[2], x[3], 1, (y > 0));
      if(!temp.check_sign())
	errornum++;
      testnum++;
    }
    fclose(ftest);
  }
  printf("errnum = %ld testnum = %ld error rate = %lf\n", errornum, testnum,double(errornum) / testnum);
}
