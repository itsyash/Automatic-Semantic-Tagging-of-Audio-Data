file_features = open('feature_vectors.txt')
file_labels = open('labels.txt')
file_output = open('svm_features.txt','w')
features  = file_features.readlines()
labels = file_labels.readlines()
for feature,label in zip(features,labels):
	parts = feature.split()[1:]
	count=1
	file_output.write(label.split()[1])
	for p in parts:
		s = str(count) + ":" + p
		file_output.write(" "+s)
		count = count+1
	file_output.write("\n")
