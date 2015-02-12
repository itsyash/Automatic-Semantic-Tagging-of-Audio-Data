i% prophet Mohammed said [ALLAH will help any one helped his/her brother/sister] PBUH
%This code to apply LDA (Linear Discriminant Analysis) 
% for any information please send to engalaatharwat@hotmail.com
%Egypt - HICIT - +20106091638

% This example deals with 2 classes
c1=[0.001 0.002;0.002 0.003;0.003 0.003;0.004 0.005;0.005 0.005]  % the first class 5 observations
c2=[0.001 0;0.002 0.001;0.003 0.001;0.003 0.002;0.005 0.003;0.006 0.005] % the second class 6 observations
scatter(c1(:,1),c1(:,2),6,'r'),hold on;
scatter(c2(:,1),c2(:,2),6,'b');


% Number of observations of each class
n1=size(c1,1)
n2=size(c2,1)

%Mean of each class
mu1=mean(c1)
mu2=mean(c2)

% Average of the mean of all classes
mu=(mu1+mu2)/2

% Center the data (data-mean)
d1=c1-repmat(mu1,size(c1,1),1)
d2=c2-repmat(mu2,size(c2,1),1)


% Calculate the within class variance (SW)
s1=d1'*d1
s2=d2'*d2
sw=s1+s2
invsw=inv(sw)

% in case of two classes only use v
% v=invsw*(mu1-mu2)'

% if more than 2 classes calculate between class variance (SB)
sb1=n1*(mu1-mu)'*(mu1-mu)
sb2=n2*(mu2-mu)'*(mu2-mu)
SB=sb1+sb2
v=invsw*SB

% find eigne values and eigen vectors of the (v)
[evec,eval]=eig(v)

% Sort eigen vectors according to eigen values (descending order) and
% neglect eigen vectors according to small eigen values
% v=evec(greater eigen value)
% or use all the eigen vectors

% project the data of the first and second class respectively
y2=c2*v
y1=c1*v
