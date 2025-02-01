import time
import doublezeta
import time


if __name__ == '__main__':
    a = doublezeta.CpyMatrix(10000, 10000, 10, 'float')
    b = doublezeta.CpyMatrix(10000, 10000, 10, 'float')
    runs = []
    for _ in range(2):
        start = time.time()
        a.mult(b)
        end = time.time()
        runs.append(end - start)

    print(f"Runtime: {sum(runs) / 2}")
