import numpy as np



def conv() : 
    f_dims = np.random.randint(1,10,2)
    f = np.random.rand(f_dims[0], f_dims[1])
    g_dims = np.random.randint(1,10,2)
    g = np.random.rand(g_dims[0], g_dims[1])

    h = np.conv2d(f,g,mode='full'); 

    #print f
    


if __name__ == "__main__" : 
    conv()

