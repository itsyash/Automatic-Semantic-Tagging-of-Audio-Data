load traindata.mat    
load labels.mat
load testdata.mat

testdata=transpose(data);
data=transpose(data);

% calculate the affinity / similarity matrix
affinity = CalculateAffinity(data);
figure,imshow(affinity,[]), title('Affinity Matrix')

% compute the degree matrix
for i=1:size(affinity,1)
    D(i,i) = sum(affinity(i,:));
end

% compute the normalized laplacian / affinity matrix (method 1)
%NL1 = D^(-1/2) .* L .* D^(-1/2);
for i=1:size(affinity,1)
    for j=1:size(affinity,2)
        NL1(i,j) = affinity(i,j) / (sqrt(D(i,i)) * sqrt(D(j,j)));  
    end
end

% compute the normalized laplacian (method 2)  eye command is used to
% obtain the identity matrix of size m x n
% NL2 = eye(size(affinity,1),size(affinity,2)) - (D^(-1/2) .* affinity .* D^(-1/2));

% perform the eigen value decomposition
[eigVectors,eigValues] = eig(NL1);

% select k largest eigen vectors
k = 3;
nEigVec = eigVectors(:,(size(eigVectors,1)-(k-1)): size(eigVectors,1));

% construct the normalized matrix U from the obtained eigen vectors
for i=1:size(nEigVec,1)
    n = sqrt(sum(nEigVec(i,:).^2));    
    U(i,:) = nEigVec(i,:) ./ n; 
end

% perform kmeans clustering on the matrix U
[IDX,C] = kmeans(U,3); 

% plot the eigen vector corresponding to the largest eigen value
%figure,plot(IDX)
figure,
hold on;
for i=1:size(IDX,1)
    if IDX(i,1) == 1
        plot(data(i,1),data(i,2),'m+');
    elseif IDX(i,1) == 2
        plot(data(i,1),data(i,2),'g+');
    else
        plot(data(i,1),data(i,2),'b+');        
    end
end
hold off;
title('Clustering Results using K-means');
grid on;shg


for kCur=1:3
    n=1;
    trainData = [];
    testData = [];
    for i=1:size(IDX,1)
        if IDX(i,1) == kCur
            trainData(n,:) = data(i,:);
            testData(n,:) = testdata(i,:);
        n=n+1;
        end
    end
    kCur=kCur+1;
    trainData = transpose(trainData);
    trainData   
    testData = transpose(testData);
    testData
    save(strcat('train_data',int2str(kCur-1)),'trainData');    
    save(strcat('test_data',int2str(kCur-1)),'testData');
end
