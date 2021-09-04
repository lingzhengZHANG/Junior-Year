import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def initCenters(dataSet , k):
    numSamples , dim = dataSet.shape
    centers = np.zeros((k,dim))
    for i in range(k):
        index = int (np.random.uniform(0,numSamples))
        centers[i, :] = dataSet[index,  :]
    # numSamples, dim = dataSet.shape
    # centers = np.zeros((k, dim))
    # for i in range(k):
    #     centers[i,0] = (44-5.25)*np.random.random() + 5.25
    #     centers[i,1] = (41.89-5.06) *np.random.random() + 5.06
    return centers

def Dist2Centers(sample,centers):
    k= centers.shape[0]
    dis2cents = np.zeros(k)
    for i in range(k):
        dis2cents[i] = np.sqrt(np.sum(np.power(sample - centers[i,:],2)))
    return dis2cents

#def kmeans(dataSet , k, iterNum):
def kmeans(dataSet, k):
    numSamples = dataSet.shape[0]
    iterCount = 0

    clusterAssignment = np.zeros(numSamples)
    clusterChanged = True
    centers= initCenters(dataSet,k)
    #i = 0
    while clusterChanged:
        #i+=1
        #clusterChanged = False
        clusterChanged = False
        #iterCount = iterCount+1
        for i in range(numSamples):

            dis2cent =  Dist2Centers(dataSet[i,:],centers)
            minIndex = np.argmin(dis2cent)

            if clusterAssignment[i] != minIndex:
                clusterChanged = True
                clusterAssignment[i] = minIndex

        for j in range(k):
            pointsInCluster = dataSet[np.nonzero(clusterAssignment[:]== j )[0]]

            # now = np.mean(pointsInCluster,axis = 0)
            # if (now==centers[j,:]).all():
            #     clusterChanged = False
            # centers[j,:] = now
            centers[j,:] = np.mean(pointsInCluster,axis = 0)
    #print('at all',i,'times')
    return centers, clusterAssignment

def km(data,k):
    #print(" clustering")
    dataSet = np.mat(data)

    #centers_result, clusterAssignment_result = kmeans(dataSet , k ,1000)
    centers_result, clusterAssignment_result = kmeans(dataSet, k)
    DBI(centers_result, clusterAssignment_result, data)
    print("showing the center coordinate")
    print(centers_result)
    showCluster(dataSet , k , centers_result,clusterAssignment_result)
    np.set_printoptions(precision=3, suppress=True)
    ans = np.array([int(x) for x in clusterAssignment_result])
    #ans=np.transpose([ans])
    #c = np.hstack((ans,data))
    return ans

def cal_dist(x,y):
    return np.sqrt(np.sum(np.power(x - y, 2)))

def cal_avg(centers_result,clusterAssignment_result,data):
    #print(np.concatenate((data,clusterAssignment_result.reshape(len(clusterAssignment_result),1)),axis=1))
    avg = np.zeros(len(centers_result))
    for i in range(len(centers_result)):
        index = np.where(clusterAssignment_result == i)
        #print("class",i)
        for j in index[0]:
            avg[i] += cal_dist(centers_result[i], data[j])
            #print(data[j])
            #print(cal_dist(centers_result[i], data[j]))
            #print(avg[i])
        #print("this is inital avg",avg[i])
        avg[i] /= len(index[0])
    #print("thisi si avg",avg)
    return avg

def cal_dcen(centers):
    l = len(centers)
    ans = np.zeros((l,l))
    for i in range(l):
        for j in range(l):
            if i==j:
                continue
            else:
                ans[i,j] = cal_dist(centers[i],centers[j])
                ans[j,i] = cal_dist(centers[j],centers[i])
    return ans

def cal_dbi(avg,dcen):
    dbi = 0
    k = len(dcen)
    for i in range(k):
        m=0
        for j in range(k):
            if i==j:
                continue
            temp = (avg[i]+avg[j])/dcen[i,j]
            m = max(temp,m)
        dbi+=m
    return dbi/k

def DBI(centers_result,clusterAssignment_result,data):
    avg = cal_avg(centers_result,clusterAssignment_result,data)
    dcen = cal_dcen(centers_result)
    dbi = cal_dbi(avg,dcen)
    print("dbi is:",dbi)

def showCluster(dataSet, k , centers, clusterAssignment):
    numSamples, dim = dataSet.shape
    mark = ['or','ob','og']

    for i in range(numSamples):
        markIndex = int(clusterAssignment[i])
        plt.plot(dataSet[i,0],dataSet[i,1],mark[markIndex])

    mark = ['Dr','Db','Dg']
    for i in range(k):
        plt.plot(centers[i,0], centers[i,1],mark[i],markersize = 17)
    plt.show()


if __name__=='__main__':
    a = np.load(r"k-means/k-means.npy",allow_pickle=True)
    data = a.tolist()
    feature0 = data['class_0']
    feature1 = data['class_1']
    feature2 = data['class_2']
    label0 = np.zeros((len(feature0),1))
    label1 = np.ones((len(feature1),1))
    label2 = np.array([2]*len(feature2)).reshape(len(feature2),1)
    #data = np.concatenate((feature0,feature1))
    data0 = np.concatenate((feature0,label0),axis=1)
    data1 = np.concatenate((feature1,label1),axis=1)
    data2 = np.concatenate((feature2,label2),axis=1)
    datalabel = np.concatenate((data0,data1,data2))
    data = datalabel[:,:-1]
    km(data,3)
    #showinitial(data)
