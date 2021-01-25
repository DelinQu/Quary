#
  这是测试样例2
  一个while-if-else分支测试
  程序求和0~10的平方和,
  输入一个值,判断sum和他的大小关系
#
input y;
i=0;
sum=0;
while(i<=10):
  sum=sum+i*i;
  i=i+1;
  print(sum)
end;

if(sum>y):
  print(0)
else:
  print(1)
end
