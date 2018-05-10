#!/usr/bin/env python

one_kb = 1024
one_mb = one_kb * one_kb
# ten_mb = 10 * one_mb
# twenty_mb = 2 * ten_mb
# thirdtwo_mb = 32 * one_mb
# six = 64 * one_mb
# oneh = 128 * one_mb
# twof = 256 * one_mb
fivet = 512 * one_mb

one_gb = 1024 * one_mb
two_gb = 2 * one_gb


sizes = [fivet, one_gb, two_gb]

gens = [8, 16, 32, 64, 128, 256]

PATH = './configs/'

def write_config(config, threads, symbols, symbol_size, size_index):

    size = ''

    if size_index == fivet:
        size = '512'
    elif size_index == one_gb:
        size = '1024'
    elif size_index == two_gb:
        size = '2048'

    config_file = '{path}{data_size}-{threads}-{symbols}-{symbol_size}'.format(
        path=PATH, data_size=size, threads = threads, symbols = symbols, symbol_size = symbol_size)
    f = open(config_file, 'w')
    f.write(config)
    f.close()


def generate_config(threads, symbols, symbol_size):
    config = '{threads},{symbols},{symbol_size}'.format(
        threads = threads, symbols = symbols, symbol_size = symbol_size)
    return config

def main():
    threads = 1

    for size in sizes:
        print size
        for gen in gens:
            symbol_size = size / gen
            print(generate_config(threads, gen, symbol_size))
            write_config(generate_config(threads, gen, symbol_size), threads, gen, symbol_size, size)

if __name__ == "__main__":
    main()
