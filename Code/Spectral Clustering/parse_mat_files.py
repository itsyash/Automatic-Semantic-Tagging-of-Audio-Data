# Convert mat file of data points (having corelated features) into input files for Passive-agressive Algo.
import scipy.io
mat1 = scipy.io.loadmat('test_data2.mat')
mat2 = scipy.io.loadmat('labels.mat')
data = mat1['testData']
labels = mat2['labels'][0]

idx=0
for x in data:
  feature = str(labels[idx]) + " "
  feature_number=1
  for y in x[:-1]:
   feature  = feature + str(feature_number) + ":" + str(y) + " "
   feature_number=feature_number+1
  feature =feature + str(feature_number) + ":" + str(x[-1])
  print feature
  idx=idx+1
