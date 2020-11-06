import jieba
from pypinyin import lazy_pinyin

def load_valid_Han():
    path = 'data/train/一二级汉字.txt'
    validSet = set()
    with open(path) as f:
        line = f.readline()
        for ch in line:
            validSet.add(ch)
    return validSet

def isHan(ch):
    if '\u4e00' <= ch <= '\u9fff':
        return True
    return False

def convert(pinyin):
    if pinyin == 'nve':
        return 'nue'
    if pinyin == 'lve':
        return 'lue'
    return pinyin

def build_polyphone(corpus_file, poly_map):
    poly_map = {}
    with open(corpus_file) as f:
        for cnt, line in enumerate(f.readlines()):
            #print("Processing line ", cnt)

            # Preprocess line to eliminate invalid chars
            line = "".join([ch for ch in line if isHan(ch)])

            seg_list = jieba.lcut(line)
            pinyin_list = lazy_pinyin(seg_list)

            # dump mismatched line
            if len(line) != len(pinyin_list):
                print("dump line ", cnt)
                continue

            ix = 0
            for ch in line:
                if isHan(ch):
                    pinyin = convert(pinyin_list[ix])
                    if (ch + pinyin) not in poly_map:
                        poly_map[ch + pinyin] = 0 	
                    poly_map[ch + pinyin] += 1
                    ix += 1
    return poly_map

def save2file(fileName, poly_map):
    with open(fileName, 'w') as f:
        for key, value in poly_map.items():
            f.write(key + ' ' + str(value) + '\n')

def load_polymap(path):
    poly_map = {}
    with open(path) as f:
        for line in f.readlines():
                key, value = line.split(' ')
                if key not in poly_map:
                    poly_map[key] = 0
                poly_map[key] += int(value)
    return poly_map

def normalize(input_path, output_path):
    from math import log
    poly_map = {}
    validSet = load_valid_Han()
    with open(input_path) as f:
        for line in f.readlines():
            key, value = line.split(' ')
            han, pinyin = key[0], key[1:]
            if han not in validSet:
                continue
            if han not in poly_map:
                poly_map[han] = {}
            poly_map[han][pinyin] = int(value)
    with open(output_path, 'w') as f:
        for han, pinyin_dict in poly_map.items():
            if len(pinyin_dict) == 1:
                continue
            tot = sum(pinyin_dict.values())
            for pinyin, value in pinyin_dict.items():
                f.write(han + pinyin + ' ' + str(log(value/tot)) + '\n')

if __name__ == "__main__":
    path = 'data/train/'
    corpus_files = ["2016-02", "2016-04", "2016-05", "2016-06", "2016-07", 
                    "2016-08", "2016-09", "2016-10", "2016-11"]

    output_path = 'model/'
    poly_map = {}

    for corpus_file in corpus_files:
        build_polyphone(path + corpus_file + '.txt', poly_map)
        print("Finish processing ", corpus_file + '.txt')
    	save2file(output_path + corpus_file + '.txt', poly_map)
    
    # merge different polyphone maps
    poly_map = {}
    for corpus_file in corpus_files:
        with open(output_path + corpus_file + '.txt') as f:
            for line in f.readlines():
                key, value = line.split(' ')
                if key not in poly_map:
                    poly_map[key] = 0
                poly_map[key] += int(value)

    target_file = 'poly_map.txt'
    save2file(output_path + target_file, poly_map)

    # normalize poly_map
    output_file = 'poly_mapping.txt'
    normalize(output_path + target_file, output_path + output_file)
