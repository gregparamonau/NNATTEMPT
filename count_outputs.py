if __name__ == "__main__":
    testing_file = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/testing.txt"
    count = 1
    sum_0 = 0
    sum_1 = 0
    with open(testing_file, "r") as f:
        for line in f:
            count += 1
            if (count == 0): continue
            temp = list(map(float, line.split()[1:]))
            sum_0 += temp [len(temp) - 2]
            sum_1 += temp [len(temp) - 1]
    
    print(sum_0 / count, sum_1 / count)
