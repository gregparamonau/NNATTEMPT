
if __name__ == "__main__":
    read_file_output = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/output.txt"
    write_file_output = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/sampled_output.txt"

    data = []
    count = 0
    with open(read_file_output, "r") as f:
        for line in f:
            if (count % 26 == 0): 
                data.append(line)
            count += 1
    f.close()


    with open (write_file_output, "w") as f:
        for i in data:
            f.write(i)
    f.close()