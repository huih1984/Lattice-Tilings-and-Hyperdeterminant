import itertools
import numpy as np
# 根据轮换得到排列
# sample：index =5,6,7,8 tuple = 1,2,3， len = 4, new_index = (6,7,5,8)
f = open("out.txt", "w")
# 0,1值，存放第i行的第j个元素，标记元素是否已被使用过
l0 = np.zeros((80))#记录1,2行被标记元素
l1 = np.zeros((80))#记录3,4行被标记元素
m = 4
n = 4
cnt = 0
dimension = m * n
class Index(object):
    def __init__(self):
        return

    def set(self, cols):
        self.cols = cols.copy()

class IndexSet(object):
    def __init__(self, row1_index):
        self.row1_index = row1_index
        self.sets = list()
        return


def get_tran_(index, tuple, len):
    # 根据轮换，获取排列
    new_index = Index()
    new_index.set(index.cols)
    for i in range(len-1):
        new_index.cols[tuple[i]] = index.cols[tuple[i+1]]
        new_index.cols[tuple[len - 1]] = index.cols[tuple[0]]
    return new_index

# 获取行排列的逆序数与本身符号的乘积
def get_sign(permdata, dimension):
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

elementSets =[]
indexSet = list()
pfaffian = np.zeros([4,80], np.uint)
def compute_all_element():
    # m 行数
    # n 列数
    # 存放列元素的集合
    # indexSet = list()
    for i in range(m):
        for j in range(1, n - 2):
            index = Index()
            index.set([i * n + j, i * n + j + 1, i * n + j + 2, i * n + j + 3])
            indexSet.append(index)
            tuple1 = [[0, 2]]
            tuple2 = [[1, 3]]
            for ind in range(0, 1):
                index = get_tran_(index, tuple1[ind], 2)
                index = get_tran_(index, tuple2[ind], 2)
                indexSet.append(index)

    for i in range(0, m-3):
        for j in range(1, n + 1):
            index = Index()
            index.set([i * n + j, i * n + j + 1 * n, i * n + j + 2 * n, i * n + j + 3 * n])
            indexSet.append(index)
            tuple1 = [[0, 2]]
            tuple2 = [[1, 3]]
            for ind in range(0, 1):
                index = get_tran_(index, tuple1[ind], 2)
                index = get_tran_(index, tuple2[ind], 2)
                indexSet.append(index)
    for ind in range(len(indexSet)):
        print(indexSet[ind].cols)

    # 将数据按照第一行元素放入不同list
    global elementSets
    for ind in range(dimension + 1):
        elementSets.append(set())

    set_len = len(indexSet)
    for ind in range(set_len):
        elementSets[indexSet[ind].cols[0]].add(indexSet[ind])
    return

def dfs(i):
    global cnt
    global elementSets, indexSet
    global l0, l1
    if i==dimension/2:
        for i1 in range(4):
            for j1 in range(int(dimension/2)):
                f.write("%2d " % (pfaffian[i1,j1]))
            f.write("\n")
        f.write("\n")
        cnt = cnt + 1#get_sign(pfaffian)
    else:
        for ind_s in range(i, len(indexSet)):
            if l0[indexSet[ind_s].cols[0]] != 0 or l0[indexSet[ind_s].cols[1]] != 0 or l1[indexSet[ind_s].cols[2]] != 0 or l1[indexSet[ind_s].cols[3]] != 0:
                continue
            if i >= 1 and indexSet[ind_s].cols[0] < pfaffian[0, i - 1]:
                continue
            pfaffian[0, i] = indexSet[ind_s].cols[0]
            pfaffian[1, i] = indexSet[ind_s].cols[1]
            pfaffian[2, i] = indexSet[ind_s].cols[2]
            pfaffian[3, i] = indexSet[ind_s].cols[3]
            l0[indexSet[ind_s].cols[0]] = 1
            l0[indexSet[ind_s].cols[1]] = 1
            l1[indexSet[ind_s].cols[2]] = 1
            l1[indexSet[ind_s].cols[3]] = 1
            dfs(i + 1)
            l0[indexSet[ind_s].cols[0]] = 0
            l0[indexSet[ind_s].cols[1]] = 0
            l1[indexSet[ind_s].cols[2]] = 0
            l1[indexSet[ind_s].cols[3]] = 0


if __name__ == '__main__':
    compute_all_element()
    dfs(0)
    f.write("%d" % cnt)