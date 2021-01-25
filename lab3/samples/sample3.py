# 
这是一段注释,将不会被识别    
sample3.qy 测试样3, 
它计算一个斐波拉契数列Fibonacci的值
#

input n;                        # 输入一个正整数值 #
if(n<0):
    print(error)                # 打印错误 #
else:
    if((n==0) or (n==1)):       # 逻辑判断 #
        print(1)
    else:                       # else分支 # 
        a=1;
        b=1;
        i=2;
        while(i<=n):            # while循环 #
            res=a+b;
            a=b;
            b=res;
            i=i+1
        end;
        print(res)              # 打印字符 #     
    end
end