#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

double w[5];
double nw[5];//new w
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
	nw[i] = w[i] + x[i];
    } else {//w = w - x;
      for(int i = 0; i < 5; i++)
	nw[i] = w[i] - x[i];
    }
  }
  bool choose_better_w(){
    if(w[0] + w[1] + w[2] + w[3] + w[4] == double(0)){
      memcpy(w, nw, sizeof(w));
      return true;
    }
    int on = 0, nn = 0;
    for(int i = 0; i < size; i++){
      if(!data[i].check_sign())
	on++;
      if(!data[i].check_sign(nw))
	nn++;
    }
    if(on > nn){
      memcpy(w, nw, sizeof(w));
      return true;
    }
    return false;
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
  for(int i = 0; i < 2000; i++){
    srand(time(NULL));
    int update = 0;
    w[0] = w[1] = w[2] = w[3] = w[4] = 0;
    while(update < 50){
      nowindex = rand() % size;
      if(!data[nowindex].check_sign()){
	data[nowindex].correct_mistake();
	data[nowindex].choose_better_w();
      }
      update++;
    }
    //printf("w = [%lf, %lf, %lf, %lf, %lf]\n", w[0], w[1], w[2], w[3], w[4]);

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
