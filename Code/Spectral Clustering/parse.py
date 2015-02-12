f=open('1test')
lines=f.readlines()
lines=filter(lambda x: x.find("#I") is -1 and len(x)>1,lines)
a=map(lambda x: x[:-1],lines)
b=map(lambda x:[x],a)
c=map(lambda x:x[0].split(' ') ,b)
labels=map(lambda x: int(x[0]),c)
d=map(lambda x:x[1:],c)
e=map(lambda x: map(lambda y:y.split(':'),x),d)
temp=map(lambda x: map(lambda y:int(y[0]),x),e)
sz=max(reduce(lambda x,y:x+y,temp))
f=map(lambda x:map(lambda y:(int(y[0]),float(y[1])),x),e)

feature_vectors=[]

idx = 0

for x in f:
 l=[0]*(sz+1)
 for y in x:
		l[y[0]-1]=y[1]
 #l[sz] = labels[idx]
 idx = idx+1
 feature_vectors.append(l)

print feature_vectors
print labels

for x in feature_vectors:
  feature = ""
  for y in x[:-1]:
   feature  = feature + str(y) + ","
  feature =feature + str(x[-1])
  print feature
