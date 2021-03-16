import numpy as np
import time
def get_gradient(G, x, c):
    #return 2 * np.matmul(G, x).reshape(-1)+c
    return np.matmul(G, x).reshape(-1) + c
    #return G@x+c

def get_f(G, x, c):
    #return np.dot(x.reshape(-1), np.matmul(G, x)).reshape(-1) + np.dot(x.reshape(-1), c.reshape(-1))
    return np.dot(x.reshape(-1), np.matmul(G, x)).reshape(-1) /2+ np.dot(x.reshape(-1), c.reshape(-1))
    #return 1/2*x.T@G@x+c.T@x

def get_alpha(G, x, c, dk,step=0.01,ro=0.25, sigma=0.75):
    # 区间[0,10]
    alpha_bar = step
    while(get_f(G,x + alpha_bar * d,c) < get_f(G,x,c)):
        alpha_bar += step
    # alpha_bar = 100
    fi0 = get_f(G, x, c)
    gradient = get_gradient(G, x, c)
    fi0_gra = gradient.T @ dk
    a1 = 0
    a2 = alpha_bar
    fi1 = fi0
    fi1_gra = fi0_gra
    alpha = a2 / 2
    while(1):
        fi = get_f(G, x + alpha * dk, c)
        if (fi > fi0 + ro * alpha * fi0_gra):
            alpha_new = a1 + (a1 - alpha) ** 2 * fi1_gra / ((fi1 - fi) - (a1 - alpha) * fi1_gra) /2
            a2 = alpha
            alpha = alpha_new
            continue
        fi_gra =  get_gradient(G, x + alpha * dk, c).T @ dk
        if (fi_gra > sigma * fi0_gra):
            return alpha  # alpha_k = alpha
        if (fi1_gra - fi_gra ==0):
            return alpha
        alpha_new = alpha - (a1 - alpha) * fi_gra / (fi1_gra - fi_gra)
        a1 = alpha
        alpha = alpha_new
        fi1 = fi
        fi1_gra = fi_gra


if __name__ == '__main__':

    G = np.array([[1.6989, 1.2013], [1.2013, 0.9552]])
    G  =G.T@G
    eps = 0.001
    x = np.array([0, 0], dtype=np.float64)
    c = np.array([0.0089,0.9480], dtype=np.float64)
    gradient = get_gradient(G, x, c)


    k = 0
    d = -get_gradient(G,x,c)
    s = time.time()
    while (np.linalg.norm(d) > eps):
        alpha = get_alpha(G,x,c,d)
        x = x + alpha * d
        d = -get_gradient(G,x,c)
        k+=1
    t = time.time()
    print("time is ",t-s)
    print("answer is", get_f(G,x,c))
    print("Iteration is",k)