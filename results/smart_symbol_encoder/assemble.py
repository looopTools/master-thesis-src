import os

def read_for_generation_size(gen):
    files = [f for f in os.listdir('.') if os.path.isfile(f)]
    files = [f for f in files if gen in f]
    return files

def create_new_result_file(file_name, files):
    results = list()
    for f in files:
        with file(f) as f1:
            s = (f1.read()).strip()
            results.append(s)
    result_file = open(file_name, 'w')
    for result in results:
        result_file.write('%s\n' % result)


files = read_for_generation_size('8_8')
create_new_result_file('generation_size_8_smart_symbol_encoder', files)

files = read_for_generation_size('8_16')
create_new_result_file('generation_size_16_smart_symbol_encoder', files)

files = read_for_generation_size('8_32')
create_new_result_file('generation_size_32_smart_symbol_encoder', files)

files = read_for_generation_size('8_64')
create_new_result_file('generation_size_64_smart_symbol_encoder', files)

files = read_for_generation_size('8_128')
create_new_result_file('generation_size_128_smart_symbol_encoder', files)

files = read_for_generation_size('8_256')
create_new_result_file('generation_size_256_smart_symbol_encoder', files)
