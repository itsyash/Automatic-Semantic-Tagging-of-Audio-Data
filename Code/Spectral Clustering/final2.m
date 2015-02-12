load dataBig.mat    
load labelsBig.mat
load dataTest.mat
load labelsTest.mat

N = size(data,1); % Size of Train Set
M = size(Tdata,1); % Size of Test Set
NUM_LABELS = 31; % Number of Labels

train_data=transpose(data);
test_data=transpose(Tdata);

train_labels = transpose(labels); % Labels of Train Set
test_labels = transpose(Tlabels); % Labels of Test Set

% calculate the affinity / similarity matrix
affinity = CalculateAffinity(train_data);
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
k = 5;
nEigVec = eigVectors(:,(size(eigVectors,1)-(k-1)): size(eigVectors,1));

% construct the normalized matrix U from the obtained eigen vectors
for i=1:size(nEigVec,1)
    n = sqrt(sum(nEigVec(i,:).^2));    
    U(i,:) = nEigVec(i,:) ./ n; 
end

% perform kmeans clustering on the matrix U
clusters = 5;
[IDX,C] = kmeans(U,clusters); 

% plot the eigen vector corresponding to the largest eigen value
%figure,plot(IDX)
figure,
hold on;
for i=1:size(IDX,1)
    if IDX(i,1) == 1
        plot(train_data(i,1),train_data(i,2),'m+');
    elseif IDX(i,1) == 2
        plot(train_data(i,1),train_data(i,2),'g+');
    else
        plot(train_data(i,1),train_data(i,2),'b+');        
    end
end
hold off;
title('Clustering Results using K-means');
grid on;shg

score = zeros(M,NUM_LABELS); % Matrix to store score (for every class) for all test points.
for cluster_id=1:clusters
    n=1;
    trainData = [];
    testData = [];
    % Group features belonging to same cluster for each data point.
    for i=1:size(IDX,1)
        if IDX(i,1) == cluster_id
            trainData(n,:) = train_data(i,:);
            testData(n,:) = test_data(i,:);
        n=n+1;
        end
    end
    cluster_id=cluster_id+1;
    % Final train and test sets with correlated features grouped together.
    trainData = transpose(trainData);
    testData = transpose(testData);
    % Classification Model
    tree = fitcknn(trainData,train_labels,'NumNeighbors',5);
    [predict_labels, test_scores] = predict(tree,testData);
    % Avg Score Matrix for each data point.
    score = score + test_scores;
    % Write Final Train and Test Sets to files.
    save(strcat('train_data',int2str(cluster_id-1)),'trainData');    
    save(strcat('test_data',int2str(cluster_id-1)),'testData');
end


final_labels = [];
for i=1:size(score,1)
    [max_value,idx] = max(score(i,:)); % Find index(label) with max score for each data point.
    final_labels = [final_labels idx];
end

% Predicted label for each test point.
final_labels = transpose(final_labels);

cnt=0; 
for i=1:M
    if test_labels(i) == final_labels(i)
        cnt = cnt+1;
    end
end

accuracy = cnt/M;
cnt
M
accuracy
