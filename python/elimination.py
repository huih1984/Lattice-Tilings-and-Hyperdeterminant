import numpy as np
from determinant import inverse_number
f = open('out.txt', 'r')
all_ = []
one_ = []
dim = 4
while(1):
    line_data = f.readline()
    if line_data == '':
        break
    if line_data == '\n':
        one_ = np.array(one_, dtype=int)
        all_.append(one_)
        one_ = []
        continue
    line_data_list = line_data.split(' ')
    line_data_list.pop(-1)
    one_.append(line_data_list)
f.close()
arr = []
account = []
for ind in range(len(all_)):
    sign = 1
    for ind_temp in range(4):
        sign *= inverse_number(all_[ind][ind_temp])
    for ind2 in range(dim):
        min_ind = 0
        for ind3 in range(4):
            if all_[ind][ind3, ind2] < all_[ind][min_ind, ind2]:
                min_ind = ind3
        temp = all_[ind][0, ind2]
        all_[ind][0, ind2] = all_[ind][min_ind, ind2]
        all_[ind][1, ind2] = all_[ind][np.mod((min_ind + 1), 4), ind2]
        all_[ind][2, ind2] = all_[ind][np.mod((min_ind + 2), 4), ind2]
        all_[ind][3, ind2] = all_[ind][np.mod((min_ind + 3), 4), ind2]
        sign *= np.power(-1, min_ind)
    for ind2 in range(dim):
        for ind3 in range(ind2 + 1, dim):
            for ind4 in range(4):
                if all_[ind][ind4, ind3] < all_[ind][ind4, ind2]:
                    for ind5 in range(4):
                        temp = all_[ind][ind4, ind2]
                        all_[ind][ind4, ind2] = all_[ind][ind4, ind3]
                        all_[ind][ind4, ind3] = temp
                    break
    exist_ = False
    for arr_ind in range(len(arr)):
        all_equal = True
        for ind_one in range(dim):
            for ind_two in range(4):
                if arr[arr_ind][ind_two, ind_one] != all_[ind][ind_two, ind_one]:
                    all_equal = False
                    break
            if all_equal == False:
                break
        if all_equal:
            account[arr_ind] += sign
            exist_ = True
            break
    if not exist_:
        arr.append(all_[ind])
        account.append(sign)
val = 0
for ind in range(len(account)):
    val += account[ind]
print(val)

