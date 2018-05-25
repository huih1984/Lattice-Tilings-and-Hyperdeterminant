import itertools

num_arr = list(range(1,9))
print(num_arr)
permu = list(itertools.permutations(num_arr,8))
print(permu)