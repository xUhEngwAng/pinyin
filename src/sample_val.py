import numpy as np

def shuffle2d(X, Y):
    for i in range(len(X)):
        pos = int(np.random.random() * len(X))
        tmp = X[pos]
        X[pos] = X[i]
        X[i] = tmp
        tmp = Y[pos]
        Y[pos] = Y[i]
        Y[i] = tmp

if __name__ == "__main__":
    val_input = 'data/val_input.txt'
    val_ans = 'data/val_answer.txt'

    input = []
    ans = []
    with open(val_input) as f:
        for line in f.readlines():
            input.append(line)
    with open(val_ans) as f:
        for line in f.readlines():
            ans.append(line)
    
    shuffle2d(input, ans)

    test_input_path = 'data/test_input.txt'
    test_ans_path   = 'data/test_ans.txt'
    test_input = open(test_input_path, 'w')
    test_ans   = open(test_ans_path, 'w')

    for i in range(1000):
        test_input.write(input[i])
        test_ans.write(ans[i])
