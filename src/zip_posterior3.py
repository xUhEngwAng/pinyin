if __name__ == "__main__":
    inputPath  = 'model/posterior3zipzip.txt'
    outputPath = 'model/posterior3zip5.txt'
    posterior3Map = {}
    with open(inputPath) as f:
        for line in f.readlines():
            key, value = line.split(' ')
            if int(value) > 5:
                posterior3Map[key] = int(value)
    
    with open(outputPath, 'w') as f:
        for key, value in posterior3Map.items():
            f.write(key + ' ' + str(value) + '\n') 
