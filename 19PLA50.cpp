#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

double w[5];
int correct_count = 0;

struct xy{
  double x[5];
  bool y;
  xy(double a, double b, double c, double d, double e, bool bl){
    x[0] = a;    x[1] = b;    x[2] = c;    x[3] = d;    x[4] = e;
    y = bl;
  }
  bool check_sign(){
    double ans = 0;
    for(int i = 0; i < 5; i++)
      ans += w[i] * x[i];
    bool anssign = (ans>0)?true:false;
    return (anssign && y) || (!anssign && !y);
  }
  void correct_mistake(){
    correct_count++;
    if(y){//new w = w + x
      for(int i = 0; i < 5; i++)
	w[i] += x[i];
    } else {//w = w - x;
      for(int i = 0; i < 5; i++)
	w[i] -= x[i];
    }
  }
};

int main(){
  FILE *fin = fopen("pocket_hw1_18_train.dat", "r");
  if(!fin)
    printf("file open error\n");

  std::vector<struct xy> data;
  double x[5];  int y;
  
  while(fscanf(fin, "%lf %lf %lf %lf %d", &x[0], &x[1], &x[2], &x[3],&y) == 5){
    xy temp(x[0], x[1], x[2], x[3], 1, (y > 0));
    data.push_back(temp);
  }

  long errornum = 0;
  long testnum = 0;
  int size = data.size();
  int* cycle = (int*)malloc(sizeof(int) * size);
  bool* isused = (bool*)malloc(sizeof(bool) * size);
  int selected;
  for(int j = 0; j < 2000; j++){
    srand(time(NULL));
    for(int i = 0; i < size; i++)
      isused[i] = false;
    for(int i = 0; i < size; i++){
      selected = rand() % size;
      while(isused[selected]){
	selected++;
	if(selected >= size)
	  selected -= size;
      }
      isused[selected] = true;
      cycle[i] = selected;
    }
    int rindex, nowindex = 0, cormistake = 0;
    while(cormistake < 50){//not the same sign-> mistake
      rindex = cycle[nowindex];
      if(!data[rindex].check_sign()){
	data[rindex].correct_mistake();
	cormistake++;
      }
      if(++nowindex >= size)
	nowindex -= size;
    }
      
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
return 0;
}
