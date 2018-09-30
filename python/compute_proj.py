import itertools
import random
import numpy as np
dimension = 8
cnt = 0
indexSystem = np.zeros((4,80), np.uint)
peritems = np.zeros((4,32), np.uint)
l0 = np.zeros((80))
l1 = np.zeros((80))
l2 = np.zeros((80))
l3 = np.zeros((80))
f = open("out.txt", "w")
#随机得到一个[1,dimension]排列
def random_permutation(compare_per=list(range(dimension))):
    while(1):
        li=list(range(dimension))
        random.shuffle(li)
        gotted = is_per_legal(li, compare_per)
        if gotted:
            break
    # print(li)
    return li

def is_per_legal(per, compare_per=list(range(dimension))):
    gotted = True
    for i in range(len(per)):
        if per[i] == compare_per[i]:
            gotted = False
            break
    return gotted

def one_matrix():
    per2 = random_permutation()
    while(1):
        per3 = random_permutation()
        if is_per_legal(per3, per2):
            break
    while (1):
        per4 = random_permutation()
        if is_per_legal(per4, per2) and is_per_legal(per4, per3):
            break
    randArray = np.array([list(range(dimension)),per2, per3, per4])
    return randArray



def get_sign( permdata):
    sign = 1
    for i in range(4):
        for j in range(dimension):
            for k in range(j+1,dimension):
                if permdata[i,k] > permdata[i,j]:
                    sign = sign*-1
    for j in range(dimension):
        for i in range(4):
            for k in range(i+1, 4):
                if permdata[k,j] > permdata[i,j]:
                    sign = sign*-1
    return sign

def dfs(i):
    global cnt
    global indexSystem
    global l0, l1, l2, l3
    if i==dimension:
        for i1 in range(4):
            for j1 in range(dimension):
                # print(str(indexSystem[i,j]) + ' ')
                # print("%2d " % (indexSystem[i,j]), file=f)
                f.write("%2d " % (indexSystem[i1,j1]))
            f.write("\n")
        f.write("\n")
        cnt = cnt + get_sign(indexSystem)
    else:
        for i1 in range(i,32):
            if l0[peritems[0,i1]] != 0 or l1[peritems[1,i1]] != 0 or l2[peritems[2,i1]] != 0 or l3[peritems[3,i1]] != 0 or peritems[0, i1] != i:
                continue
            indexSystem[0, i] = peritems[0, i1]
            indexSystem[1, i] = peritems[1, i1]
            indexSystem[2, i] = peritems[2, i1]
            indexSystem[3, i] = peritems[3, i1]
            l0[indexSystem[0, i]] = 1
            l1[indexSystem[1, i]] = 1
            l2[indexSystem[2, i]] = 1
            l3[indexSystem[3, i]] = 1
            dfs(i + 1)
            l0[indexSystem[0, i]] = 0
            l1[indexSystem[1, i]] = 0
            l2[indexSystem[2, i]] = 0
            l3[indexSystem[3, i]] = 0

if __name__ == '__main__':
    randArray = one_matrix()
    for i in range(4):
        for j in range(8):
            peritems[i, j] = randArray[i, j]
    
    for j in range(8):
        peritems[0, j + 8] = randArray[1, j]
        peritems[1, j + 8] = randArray[0, j]
        peritems[2, j + 8] = randArray[3, j]
        peritems[3, j + 8] = randArray[2, j]

    for j in range(8):
        peritems[0, j + 16] = randArray[3, j]
        peritems[1, j + 16] = randArray[2, j]
        peritems[2, j + 16] = randArray[1, j]
        peritems[3, j + 16] = randArray[0, j]

    for j in range(8):
        peritems[0, j + 24] = randArray[2, j]
        peritems[1, j + 24] = randArray[3, j]
        peritems[2, j + 24] = randArray[0, j]
        peritems[3, j + 24] = randArray[1, j]

    cnt = 0
    dfs(0)
    print(cnt)