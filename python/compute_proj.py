import itertools
num_arr = []
for ind in range(4):
    num_arr.append(list(range(ind,16,4)))
for ind in range(4):
    num_arr.append(list(range(ind*4,(ind+1)*4)))

num_arr.append(list(range(0,16,5)))
num_arr.append([3,7,10,12])
print(num_arr)
permu = list(itertools.permutations(num_arr,8))
print(permu)