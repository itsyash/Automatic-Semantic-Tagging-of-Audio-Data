
def parse(filename):
	f=open(filename)
	lines=f.readlines()
	names=[]
	vectors=[]
	labels=[]
	for x in lines:
		if x.find("#I") is not -1 and len(x)>1:
			names.append(int(x[3:-1]))
		elif x.find("#I") is -1 and len(x)>1:
			temp=x[:-1].split(' ')
			vectors.append(temp[1:])
			labels.append(int(temp[0]))
	#print names
	print vectors
	#print labels

	sz=0
	indexlist=[]
	for x in vectors:
		l=[]
		for y in x:
			temp=y.split(':')
			if int(temp[0])>sz:
				sz=int(temp[0])
			l.append(temp)
		indexlist.append(l)

	#print indexlist
	#print sz


	feature_vectors=[]
	for x in indexlist:
		l=[0]*(sz+1)
		for y in x:
			l[int(y[0])]=float(y[1])
		feature_vectors.append(l)

	print feature_vectors

parse('input')
