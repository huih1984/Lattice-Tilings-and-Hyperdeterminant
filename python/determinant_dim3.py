import numpy as np
import itertools
import random
import os
m = 1
n = 6
dimension = m*n
def random_matrixA():
    matrix = np.zeros([dimension, dimension, dimension], np.int)
    for i in range(dimension):
        for j in range(dimension):
            for k in range(dimension):
                    if (
                            (np.mod(i, 3)+1 == np.mod(j, 3)  and np.mod(j, 3) +1 == np.mod(k, 3))
                    # (np.mod(i, 3)+1 == np.mod(j, 3)  and np.mod(k, 3)+1  == np.mod(l, 3))
                    ): #or (i+n == j and j+n == k and k+n == l):
                        matrix[i,j,k] = 1#np.random.random_integers(3)# 2
                        matrix[j, k, i] =  - matrix[i,j,k]
                        matrix[k, i, j] =  matrix[i,j,k]
    return matrix

def coMatrix(A, i,j,k):
    '''
    i,j,k,l的余子式
    :param A:
    :param i:
    :param j:
    :param k:
    :return:
    '''
    out_dim = A.shape[0]
    matrix = np.zeros([out_dim - 1, out_dim - 1, out_dim - 1], np.int)
    for s1 in range(out_dim - 1):
        for s2 in range(out_dim - 1):
            for s3 in range(out_dim - 1):
                    if s1 >= i:
                        s11 = s1 + 1
                    else:
                        s11 = s1
                    if s2 >= j:
                        s22 = s2 + 1
                    else:
                        s22 = s2
                    if s3 >= k:
                        s33 = s3 + 1
                    else:
                        s33 = s3
                    matrix[s1, s2, s3] = A[s11, s22, s33]
    return matrix
deter_deep = 0
element_list = []

def append_current_index(j,k):
    j_list = []
    k_list = []
    for ind in range(len(element_list)):
        j_list.append(element_list[ind][1])
        k_list.append(element_list[ind][2])
    indj = 0
    indk = 0
    for ind in range(m * n):
        if ind in j_list:
            continue
        elif ind not in j_list and indj < j:
            indj += 1
        elif indj == j:
            current_j = ind
            break

    for ind in range(m * n):
        if ind in k_list:
            continue
        elif ind not in k_list and indk < k:
            indk += 1
        elif indk == k:
            current_k = ind
            break
    element_list.append((deter_deep, current_j, current_k))

def determinant2(matrix):
    global deter_deep
    global element_list
    '''

    :param matrix:
    :return:
    '''
    if matrix.shape[0] == 1:
        if matrix[0,0,0] != 0:
            append_current_index(0, 0)
            for ind in range(3):
                for ind2 in range(m*n):
                    print(element_list[ind2][ind], end=' ')
                print('')
            print('\n', end='')
            # print((element_list))
            element_list.pop()
        return matrix[0,0,0]
    else:
        for i in range(matrix.shape[0]):
            all_zero = True
            for j in range(matrix.shape[0]):
                if not all_zero:
                    break
                for k in range(matrix.shape[0]):
                    if matrix[i, j, k] != 0:
                        all_zero = False
                        break
            if all_zero:
                return 0
        for j in range(matrix.shape[0]):
            all_zero = True
            for i in range(matrix.shape[0]):
                if not all_zero:
                    break
                for k in range(matrix.shape[0]):
                    if matrix[i, j, k] != 0:
                        all_zero = False
                        break
            if all_zero:
                return 0
        for k in range(matrix.shape[0]):
            all_zero = True
            for j in range(matrix.shape[0]):
                if not all_zero:
                    break
                for i in range(matrix.shape[0]):
                    if matrix[i, j, k] != 0:
                        all_zero = False
                        break
            if all_zero:
                return 0
        det = 0
        for j in range(matrix.shape[0]):
            for k in range(matrix.shape[0]):
                    if matrix[0,j,k] != 0:
                        append_current_index(j, k)
                        deter_deep += 1
                        temp = determinant2(coMatrix(matrix, 0,j,k))*np.power(-1, j+k)
                        if temp != 0:
                            det += matrix[0,j,k]*temp
                        element_list.pop()
                        deter_deep -= 1

        return det

def determinant(matrix):
    non_zeros = []
    for i in range(matrix.shape[0]):
        for j in range(matrix.shape[0]):
            for k in range(matrix.shape[0]):
                if matrix[i,j,k] != 0:
                    non_zeros.append([i,j,k])
    deter = 0
    array = np.arange(0, matrix.shape[0])
    for i in itertools.permutations(array, matrix.shape[0]):
        next = True
        for ind in range(matrix.shape[0]):
            next = True
            for ind_non_zeros in range(len(non_zeros)):
                if non_zeros[ind_non_zeros][1] == i[ind] and non_zeros[ind_non_zeros][0] == ind:
                    next = False
            if next == True:
                continue
        if next == True:
            continue
        for j in itertools.permutations(array, matrix.shape[0]):
            next = True
            for ind in range(matrix.shape[0]):
                next = True
                for ind_non_zeros in range(len(non_zeros)):
                    if non_zeros[ind_non_zeros][2] == j[ind] and non_zeros[ind_non_zeros][1] == i[ind] and non_zeros[ind_non_zeros][0] == ind:
                        next = False
                if next == True:
                    continue
            if next == True:
                continue
            factor = 1
            for s in range(matrix.shape[0]):
                if factor == 0:
                    break
                factor *= matrix[s, i[s], j[s]]
            factor *= inverse_number(i)*inverse_number(j)
            deter += factor
    return deter

def codet(A):
    coA = np.zeros([dimension, dimension, dimension], np.int)
    for i in range(dimension):
        for j in range(dimension):
            for k in range(dimension):
                    if i > j:
                        coA[i, j, k] = - coA[j,k,i]
                    elif i > k:
                        coA[i, j, k] = coA[k,i,j]
                    else:
                        matrix = np.zeros([dimension - 1, dimension - 1, dimension - 1], np.int)
                        for s1 in range(dimension - 1):
                            for s2 in range(dimension - 1):
                                for s3 in range(dimension - 1):
                                    if s1 >= i:
                                        s11 = s1 + 1
                                    else:
                                        s11 = s1
                                    if s2 >= j:
                                        s22 = s2 + 1
                                    else:
                                        s22 = s2
                                    if s3 >= k:
                                        s33 = s3 + 1
                                    else:
                                        s33 = s3
                                    matrix[s1,s2,s3] = A[s11,s22,s33]
                        coA[i,j,k] = determinant2(matrix)*np.power(-1, i+j+k)
    return coA

def inverse_number(string):
    ans = 1
    for i in range(len(string)):
        for j in range(i):
            if string[j] > string[i]:
                ans *= -1
    return ans

A = random_matrixA()
# print(('det(A)=',determinant(A)))
print(('det(A)=',determinant2(A)))
for i in range(dimension):
    for j in range(dimension):
        for k in range(dimension):
            if A[i,j,k] != 0:
                print((i,j,k))
# print(('A', A))
#
# coA = random_matrixB()#
# for i in range(dimension):
#     for j in range(dimension):
#         for k in range(dimension):
#             for l in range(dimension):
#                 if coA[i,j,k,l] != 0:
#                     print((i,j,k,l))
# print(('coA', coA))
# def multip(A,B):
#     C = np.zeros([dimension, dimension], np.int64)
#     for i in range(dimension):
#         for j in range(dimension):
#             for s1 in range(dimension):
#                 for s2 in range(dimension):
#                     for s3 in range(dimension):
#                         C[i,j] += A[i,s1,s2,s3]*B[j,s1,s2,s3]
#     return C
#
# C = multip(A, coA)
# print(('C', C))
# det_c = np.linalg.det(C)
# print(('det(A)=',determinant2(A), 'det(coA)=',determinant2(coA)))
# print(('det(C)=',det_c))

