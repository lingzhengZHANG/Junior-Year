#!/usr/bin/env python
# -*- coding:utf-8 -*-
import numpy as np
import pandas as pd
import random
from sklearn.preprocessing import LabelEncoder
from sklearn.datasets import load_iris


iris = load_iris()
y = iris.target
x = iris.data
dataset = np.concatenate((x,y),axis=1)

#高斯核
def rbf(x1,x2,sigma):
    return np.exp(-(np.sum((x1-x2)**2))/(2*sigma**2))


def svmtrain(train,C,sigma,kernel,tol=np.e**-3,maxpass=10,g1g2=True):
    if g1g2==True:
        trainset =train.copy()
    elif g1g2==False:
        #print('here i came ')
        trainset = train.copy()
        trainset = np.delete(trainset, -2, axis=1)
        trainset = np.delete(trainset, -2, axis=1)
        #trainset = np.hstack((trainset[:,:-3],np.transpose([trainset[:,-1]])))

    X = trainset[:,:-1]
    y = trainset[:,-1]
    m = len(X)
    n = len(X[0])
    alphas = np.zeros(m)
    b = 0
    E = np.zeros(m)
    passes = 0
    eta = 0
    L = 0
    H = 0

    if kernel=='rbf':
        K = np.zeros((m,m))
        for i in range(m):
            for j in range(i,m):
                K[i,j] =  rbf(np.transpose([X[i,:]]),np.transpose([X[j,:]]),sigma)
                K[j,i] = K[i,j]
    elif kernel=='linear':
        K = X@X.T
    #print('thsi is K')
    #print(K)

    while passes <maxpass:
        changed_a = 0
        for i in range(m):
            E[i] = b + sum(alphas * y * K[:,i]) - y[i]  #求出损失
            if (y[i]*E[i]<-tol and alphas[i]<C) or (y[i]*E[i]>tol and alphas[i]>0):
                j = int(random.uniform(0, m))
                while j==i:
                    j = int(random.uniform(0, m))
                E[j] = b + sum(alphas * y * K[:,j]) - y[j] #损失

                alpha_i_old = alphas[i] #存下来
                alpha_j_old = alphas[j]         #利用smo算法，进行调整alpha和b的值

                if y[i]==y[j]:
                    L = max(0,alphas[j] + alphas[i] - C)
                    H = min(C,alphas[j] + alphas[i])
                else:
                    L = max(0,alphas[j]-alphas[i])
                    H = min(C,C+alphas[j]-alphas[i])
                if L==H:
                    continue
                eta = 2 * K[i,:]@K[j,:].T - K[i,:]@K[i,:].T -K[j,:]@K[i,:].T
                if eta>=0:
                    continue
                alphas[j] -= y[j] * (E[i]-E[j])/eta
                alphas[j] = min(H,alphas[j])
                alphas[j] = max(L,alphas[j])
                if abs(alphas[j]- alpha_j_old)<tol:
                    alphas[j] = alpha_j_old
                    continue
                alphas[i] += y[i] * y[j] * (alpha_j_old-alphas[j])
                b1 = b-  E[i] - y[i] * (alphas[i] - alpha_i_old) * K[i,:]@K[j,:].T - y[j] * (alphas[j]- alpha_j_old) * K[i,:]@K[j,:].T
                b2 = b - E[j] - y[i] * (alphas[i] - alpha_i_old) * K[i,:]@K[j,:].T- y[j] * (alphas[j]- alpha_j_old) * K[j,:]@K[j,:].T
                if alphas[i]>0 and alphas[i]<C:
                    b = b1
                elif alphas[j] >0 and alphas[j] <C:
                    b = b2
                else:
                    b = (b1+b2)/2

                changed_a+=1

        if changed_a==0:
            passes+=1
        else:
            passes = 0
    w = ((alphas * y).T @ X).T
    #print('thsi is alpha')
    #print(alphas)
    return b,alphas,w


def svmpredict(trainset,predict,w,b,alphas,sigma,kernel,g1g2=True):
    if g1g2==False:
        data = trainset.copy()
        data = np.delete(data,-2,axis=1)
        data = np.delete(data, -2, axis=1)
        test = predict.copy()
        test = np.delete(test, -1, axis=1)
        test = np.delete(test, -1, axis=1)
    else:
        data = trainset.copy()
        test = predict.copy()
    train = data[:,:-1]
    label = data[:,-1]
    X= test
    m = len(X)
    #p = np.transpose([np.zeros(m)])
    #pred = np.transpose([np.zeros(m)])
    if kernel=='rbf':
        X1 = np.transpose([np.sum(X**2,axis=1)])    #求测试集的模
        X2 = np.sum(train**2,axis=1)    #求训练集的模
        temp = X2 - 2 * X @ train.T
        K = -(temp+X1)      #训练集和测试的高斯核
        K = np.exp(K/(2*sigma**2))
        K = label*K
        K = alphas*K
        p = np.sum(K,axis=1)
    elif kernel=='linear':
        p = X @ np.transpose([w])+np.transpose([b])
    #print(p)
    for i in range(len(p)):
        if p[i]>=0:
            p[i] = 1
        else:
            p[i] = -1
    return p



if __name__ =='__main__':
    def load_data(file_path):
        df = pd.read_csv(file_path, sep=';')
        df = preprocess(df)
        dataset = df.values
        dataset = dataset.astype(float)
        random.shuffle(dataset)
        for i in range(len(dataset)):
            dataset[i, -1] = 1 if dataset[i, -1] >= 10 else -1
        trainsize = len(dataset) * 7 // 10
        train = dataset[:trainsize]
        test = dataset[trainsize:]
        return train,test
    def preprocess(df):
        col = []
        for i in range(df.shape[1]):
            if df.dtypes[i] == 'object':
                col.append(df.columns[i])
        if col is not None:
            for c in col:
                df[c] = LabelEncoder().fit_transform(df[c])
        return df


    def calcu(plabel, olabel):  # plabel指预测的，olabel指原来的
        TP = 0
        FP = 0
        FN = 0
        TN = 0
        for i in range(len(plabel)):
            if olabel[i] == 1:
                if plabel[i] == 1:
                    TP += 1
                else:
                    FN += 1
            else:
                if plabel[i] == 1:
                    FP += 1
                else:
                    TN += 1
        P = TP / (TP + FP)
        R = TP / (TP + FN)
        F1 = 2 * P * R / (P + R)
        return F1
    train,test = load_data('student-mat.csv')

    b,alpha,w = svmtrain(train,0.6,2,'rbf',g1g2=False)
    #print('this is b and a')
    #print(b,alpha)
    ans  = svmpredict(train,test[:,:-1],w,b,alpha,2,'rbf',g1g2=False)
    print('gaussian kernel svm,F1 = ', calcu(ans, test[:, -1]))

    print(ans)
    #check accuracy
    s = 0
    for i in range(len(test)):
        if ans[i] == test[i,-1]:
            s+=1
    print('accurancy = ',s/len(test))
