#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

double w[5];
long long correct_count = 0;

struct xy{
  double x[5];
  bool y;
  xy(double a, double b, double c, double d, double e, bool bl){
    x[0] = a;
    x[1] = b;
    x[2] = c;
    x[3] = d;
    x[4] = e;
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
	w[i] += 0.5 * x[i];
    } else {//w = w - x;
      for(int i = 0; i < 5; i++){
	w[i] -= 0.5 * x[i];
      }
    }
    //    printf("now w = (%lf, %lf, %lf, %lf)\n", w[0], w[1], w[2], w[3]);
    //sleep(3);
  }
};

int main(){
  FILE *fin = fopen("hw1_15_train.dat", "r");//??
  if(!fin){
    printf("file open error\n");
  }
  std::vector<struct xy> data;
  double x[5];  int y;
  
  while(fscanf(fin, "%lf %lf %lf %lf %d", &x[0], &x[1], &x[2], &x[3], &y) == 5){
    xy temp(x[0], x[1], x[2], x[3], 1, (y > 0));
    data.push_back(temp);
  }

  int size = data.size();
  double avgmistake;
  int* cycle = (int*)malloc(sizeof(int) * size);
  bool* isused = (bool*)malloc(sizeof(bool) * size);
  int selected;
  srand(time(NULL));
  for(int i = 0; i < 2000; i++){
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
    int rindex;
    int nowindex = 0;
    int nomistake = 0;
    w[0] = w[1] = w[2] = w[3] = w[4] = 0;
    while(nomistake < size){//not the same sign-> mistake
      rindex = cycle[nowindex];
      if(!data[rindex].check_sign()){
	data[rindex].correct_mistake();
	printf("stop at mistake %d\n", nomistake);
	nomistake = 1;
      } else {
	nomistake++;
      }
      if(++nowindex >= size)
	nowindex -= size;
    }
  }
  printf("avg mistake = %lf\n", double(correct_count) / 2000);
}
