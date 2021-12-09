
print( "x^k mod m")
x=int(input("x: "))
k=int(input("k: "))
m=int(input("m: "))

q=2
rem=0
start=k
count=0
mod_result=x%m
mul=1
while(q!=0):
    rem=start%2
    q=int(start/2)
    start=q
    if count>0:
        mod_result=(mod_result*mod_result)%m
    if rem==1:
        print("mr="+str(mod_result))
        mul=mul*mod_result
        if mul>m:
            mul=mul%m
    count+=1
    
print("mul= "+str(mul))
print(str(x)+"^"+str(k)+" mod "+str(m)+" = "+str(mul%m))
        
