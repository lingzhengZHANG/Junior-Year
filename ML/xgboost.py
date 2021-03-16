import numpy as np


def load_data():
    x_train = np.load(r"data/train_data.npy")
    y_train = np.load(r"data/train_target.npy")
    x_test = np.load(r"data/test_data.npy")
    y_test = np.load(r"data/test_target.npy")
    return x_train,y_train,x_test,y_test

def sigmoid(x):
    return 1/(1+np.exp(-x))

def loss(pred, y, loss_type='mse'):
    return np.mean((y-pred) ** 2) / 2 if loss_type == 'mse' else - np.mean(y * np.log(pred + 1e-5) + (1-y) * np.log(1-pred + 1e-5))

def cal_g(pred, y):
    return (pred - y)

def cal_h(pred, y, loss_type):
    return np.array([1] * len(y)) if loss_type == 'mse' else (pred * (1 - pred))


class xgb():
    def __init__(self,lam=0.2, epsilon=0.01, threshold=0.01,
                 gamma=0.1, max_depth=10, max_iter=10, loss_type='log'):
        self.lam = lam
        self.threshold = threshold
        self.max_depth = max_depth
        self.loss_type = loss_type
        self.max_iter = max_iter
        self.epsilon = epsilon
        self.gamma = gamma
        self.forest = []

    def binSplitDataSet(self,dataSet, feature, value):
        index0 = np.where(dataSet[:,feature]<=value)[0]
        index1 = np.where(dataSet[:,feature]>value)[0]
        return index0,index1

    def fit(self,x_train,y_train):
        self.forest.append(0)
        predictions = self.forecast(x_train)
        self.g = cal_g(predictions,y_train)
        self.h = cal_h(predictions,y_train,self.loss_type)
        l = loss(predictions,y_train)
        count = 1
        while l>self.epsilon:
            if count>self.max_iter:
                break

            new_tree,depth = self.createTree(x_train,y_train,self.g,self.h,0)

            self.forest.append(new_tree)
            predictions = self.forecast(x_train)
            self.g = cal_g(predictions,y_train)
            self.h = cal_h(predictions,y_train,self.loss_type)
            l = loss(predictions,y_train,self.loss_type)
            count+=1

    def chooseBestSplit(self,x_train,y_train,g,h,depth):
        m,n = np.shape(x_train)
        bestS = 0; bestIndex = 0; bestValue = 0
        G,H = np.sum(g),np.sum(h)
        if len(np.unique(y_train))==1:
            return None,-G/(H+self.lam)

        for featIndex in range(n):
            for splitVal in set(x_train[:,featIndex]):
                index0,index1 = self.binSplitDataSet(x_train, featIndex, splitVal)
                Gl = np.sum(g[index0])
                Gr = np.sum(g[index1])
                Hl = np.sum(h[index0])
                Hr = np.sum(h[index1])
                gain = (Gl**2/(Hl + self.lam) + Gr**2/(Hr + self.lam)
                    - G**2 / (H + self.lam))/2 - self.gamma
                if gain > bestS:
                    bestS,bestIndex,bestValue = gain,featIndex,splitVal
        if bestS<self.threshold:
            return None,-G/(H+self.lam) # 停止
        return bestIndex,bestValue

    def createTree(self,x_train,y_train,g,h,depth):
        if depth>=self.max_depth:
            return np.mean(y_train),depth

        feature, value = self.chooseBestSplit(x_train,y_train,g,h,depth)

        if feature == None:
            return value,depth
        Tree = {}
        Tree['spInd'] = feature
        Tree['spVal'] = value
        Tree['depth'] = depth
    #将数据集分为两份，之后递归调用继续划分
        lSet, rSet = self.binSplitDataSet(x_train, feature, value)
        Tree['left'], lmax = self.createTree(x_train[lSet], y_train[lSet], g[lSet],h[lSet],depth+1)
        Tree['right'], rmax = self.createTree(x_train[rSet], y_train[rSet], g[rSet],h[rSet], depth+1)
        return Tree,max(lmax,rmax)

    def forecast(self, x_test):
        prediction = []
        for x in x_test:
            prediction.append(self.treeforecast(x))
        return np.array(prediction)

    def treeforecast(self, x):
        prediction = 0
        for tree in self.forest:
            while isinstance(tree, dict):
                if x[tree['spInd']] < tree['spVal']:
                    tree = tree['left']
                else:
                    tree = tree['right']
            try:
                prediction += tree
            except:
                print("wrong")
        if self.loss_type != 'mse':
            prediction = sigmoid(prediction)
        return prediction


if __name__ =="__main__":
    x_train,y_train,x_test,y_test = load_data()
    print(x_train)
    xg = xgb(lam=0.3,epsilon=0.05,threshold=0.01,gamma=1.5,max_depth=6,max_iter=8,loss_type='log')
    xg.fit(x_train,y_train)
    predictions = xg.forecast(x_train)
    print(predictions)
    print("Log Train: prediction accuracy: %.4f" % (np.sum((predictions > 0.5) == y_train) / float(len(y_train))))

    predictions = xg.forecast(x_test)
    print("Log Test: prediction accuracy: %.4f" % (np.sum((predictions > 0.5) == y_test) / float(len(y_test))))