from numpy import *


'''
filename='hw3_train.dat' #文件目录
def loadDataSet():   #读取数据（这里只有两个特征）
    dataMat = []
    labelMat = []
    fr = open(filename)
    for line in fr.readlines():
        lineArr = line.strip().split()
        dataMat.append([1.0, float(lineArr[0]), float(lineArr[1])])   #前面的1，表示方程的常量。比如两个特征X1,X2，共需要三个参数，W1+W2*X1+W3*X2
        labelMat.append(int(lineArr[2]))
    return dataMat,labelMat
'''


def loadDataSet():
    x_train= load('data/train_data.npy')
    y_train = load('data/train_target.npy')
    #print(x_train)
    #print(y_train)
    return x_train,y_train
def sigmoid(inX):  # sigmoid函数
    return 1.0 / (1 + exp(-inX))


def gradAscent(x_train, y_train):
    m, n = shape(x_train)
    alpha = 0.001
    maxCycles = 500
    weights = ones((n, 1))
    for k in range(maxCycles):
        h = sigmoid(dot(x_train,weights))
        error = (y_train.reshape(len(y_train),1) - h)
        weights = weights + alpha * dot(x_train.transpose(),error)
    return weights


def stocGradAscent(x_train, y_train):
    #dataMatrix = mat(dataMat)
    #classLabels = labelMat
    m, n = shape(x_train)
    alpha = 0.01
    maxCycles = 100
    #weights = ones((n, 1))
    weights = ones((n,1))
    for k in range(maxCycles):
        for i in range(m):  # 遍历计算每一行
            h = sigmoid(sum(dot(x_train[i].reshape(1,len(x_train[0])) ,weights)))
            error = y_train[i] - h
            weights = weights + alpha * error * x_train[i].reshape(len(x_train[i]),1)
    #print(weights.shape)
            #weights = weights + alpha * dot(x_train[i].transpose(),error)
    return weights


def stocGradAscent1(dataMat, labelMat):  # 改进版随机梯度上升，在每次迭代中随机选择样本来更新权重，并且随迭代次数增加，权重变化越小。
    dataMatrix = mat(dataMat)
    classLabels = labelMat
    m, n = shape(dataMatrix)
    weights = ones((n, 1))
    maxCycles = 500
    for j in range(maxCycles):  # 迭代
        dataIndex = [i for i in range(m)]
        for i in range(m):  # 随机遍历每一行
            alpha = 4 / (1 + j + i) + 0.0001  # 随迭代次数增加，权重变化越小。
            randIndex = int(random.uniform(0, len(dataIndex)))  # 随机抽样
            h = sigmoid(sum(dataMatrix[randIndex] * weights))
            error = classLabels[randIndex] - h
            weights = weights + alpha * error * dataMatrix[randIndex].transpose()
            del (dataIndex[randIndex])  # 去除已经抽取的样本
    return weights


def loadDataSet1():
    x_test = load('data/test_data.npy')
    y_test = load('data/test_target.npy')
    print(x_test)
    print(y_test)
    return x_test,y_test

def plotBestFit(weights):  # 画出最终分类的图
    import matplotlib.pyplot as plt
    dataMat, labelMat = loadDataSet()
    dataArr = array(dataMat)
    n = shape(dataArr)[0]
    xcord1 = []
    ycord1 = []
    xcord2 = []
    ycord2 = []
    for i in range(n):
        if int(labelMat[i]) == 1:
            xcord1.append(dataArr[i, 1])
            ycord1.append(dataArr[i, 2])
        else:
            xcord2.append(dataArr[i, 1])
            ycord2.append(dataArr[i, 2])
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(xcord1, ycord1, s=30, c='red', marker='s')
    ax.scatter(xcord2, ycord2, s=30, c='green')
    x = arange(-3.0, 3.0, 0.1)
    y = (-weights[0] - weights[1] * x) / weights[2]
    ax.plot(x, y)
    plt.xlabel('X1')
    plt.ylabel('X2')
    plt.show()

def calculateac(weights):
    x_test,y_test = loadDataSet()
    y_predict = sigmoid(dot(x_test,weights))
    #print(y_test)
    #print(y_test.shape)
    #all_len = len(y_predict)
    accuracy = 0
    #y_predict = y_predict.reshape((len(y_predict,)))
    for i in range(len(y_predict)):
        if y_predict[i]>=0.5:
            y_predict[i] = 1
        else:
            if y_predict[i] <0.5:
                y_predict[i] = 0
        if y_predict[i]==y_test[i]:
            accuracy+=1
    print('testset_accuracy=',accuracy/len(y_predict))
def main():
    #filepath = 'C:/Users/13501/Documents/Tencent Files/1350163822/FileRecv/data/LR'
    dataMat, labelMat = loadDataSet()
    #print(dataMat.shape)
    #print(labelMat.shape)
    #weights = gradAscent(dataMat, labelMat)
    weights = stocGradAscent(dataMat,labelMat)
    #print(weights.shape)
    plotBestFit(weights)
    calculateac(weights)

if __name__ == '__main__':
    main()