int total(int i){

  if(i==1)
    return 1;
  else if(i==2)
    return 3;
  else
    return total(i-1)+2*total(i-2);
}

int unique(int i){
  if(i==1)
    return 1;
  else if(i==2)
    return 3;
  else if(i==3)
    return 1;
  else if(i==4)
    return 5;
  else
    return unique(i-2)+2*unique(i-4);
}

int main(void){

  int N;
  int D;
  int A;

  D=total(N);
  A=unique(N);

  return 0;
}
