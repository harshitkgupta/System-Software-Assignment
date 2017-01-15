function [C2,B1]=fun_rsa(J)
p=randi([1,255]);
while(~isprime(p))
    p=randi([1,255]);
end

q=randi([1,255]);
while((~isprime(q))| (p==q))
    q= randi([1,255]);
end

p
q

n=p*q
z=(p-1)*(q-1)

e=z;
while(gcd(e,z)~=1 |  e<2 | (z-1)<e)
    e=randi([2,z-1]);       
end

e

res=0;
d=1;
while(res~=1)
    res=mod(((d))*e,z);
    d=d+1;
end

d=d-1


[l m]=size(J)
for i=1:l
for j=1:m  

num=uint32(1) ;   
num1=uint32(J(i,j));

for k=1:e
num=mod(num*num1,n);
end

C(i,j)=(num) ;

end
end
C1=mod(C,n);
C2=uint8(C1);



for i=1:l
for j=1:m  
B(i,j)=uint32(1);    
for k=1:d
B(i ,j)=mod(B(i,j)*C(i,j),n);
end
end
end

B1=uint8(B);


