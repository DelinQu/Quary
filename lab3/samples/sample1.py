#
  这是测试样例1
  一个if分支测试
#
input x;
if 0 < x:
  fact = 0;
  if(x>2):
    fact = 1;
    if(x>3):
      fact = 2
    end
  end
else:
  fact = 3;
  print(fact)
end
