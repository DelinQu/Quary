int a[5];
int globals;

void quicksort(int a[], int left, int right) {
  int localvala;
  int i;
  int j;
  int key;
  int temp;
  if (left < right) {
    int localvalb;
    i = left;
    j = right;
    key = a[left];
    while (i < j) {
      int localvalc;
      while (a[i] < key) {
      	i = i + 1;
      }
      while (a[j] > key) {
      	j = j - 1;
      }
      if (i < j) {
	temp = a[i];
	a[j] = a[i];
	a[i] = temp;
	localvalc = localvalc + 1;
      }
    }
    temp = a[left];
    a[left] = a[j];
    a[j] = temp;
    quicksort(a, left, j-1);
    quicksort(a, j+1, right);
  }
}
int mid(int tem) {
  int j;
  int temp;
  temp = 0;
  /* for (j = 0; j < tem; j++) { */
  /*   temp = temp + j; */
  /* } */
  return temp;
}

int mida(int num, int i) {
  int result;
  int j;
  if (i > 4) {
    result = mid(i);
  } /* else { */
  /*   for (j = 0; j < i ; j++) { */
  /*     result = (result * j) + num; */
  /*   } */
  /* } */
  return result;
}

void main(void) {
  int i;
  int num;

  i = 0;
  num = 5;
  while (i < num) {
    a[i] = mida(num, i);
    i = i+1;
  }
  quicksort(a, 0, num - 1);
}
