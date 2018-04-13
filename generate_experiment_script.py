#!/usr/bin/env python

from os import listdir


def all_dirs(path):
    return [dir for dir in listdir(path)]

def get_by_size(size, dirs):

    return [dir for dir in dirs if dir.startswith(size)]


def generate_command(experiment, config):

    return './build/linux/{experiment} ./configs/{config}'.format(experiment=experiment, config=config)

def generate_command_for_size(experiment, size, configs):
    commands = list()
    commands.append('# Experiment: {} -- Size: {}\n'.format(experiment, size))
    for config in configs:
        commands.append(generate_command(experiment, config))
        commands.append('sleep 10')
    commands.append('')
    return commands


def write_run(commands):
    f = open('RUN_BENCHMARK', 'w')
    for cmd in commands:
        f.write('{}\n'.format(cmd))
    f.close()


def main():
    path = './configs'
    dirs = all_dirs(path)

    half_gig = get_by_size('512', dirs)
    one_gig = get_by_size('1024', dirs)
    two_gig = get_by_size('2048', dirs)

    complex = 'complex_encoder_benchmark'
    smart = 'smart_encoder_benchmark'
    simple = 'simple_encoder_benchmark'

    commands = list()
    commands.append('#!/usr/bin/env bash')
    for cmd in generate_command_for_size(complex, '512', half_gig):
        commands.append(cmd)
    for cmd in generate_command_for_size(smart, '512', half_gig):
        commands.append(cmd)
    for cmd in generate_command_for_size(simple, '512', half_gig):
        commands.append(cmd)

    for cmd in generate_command_for_size(complex, '1024', half_gig):
        commands.append(cmd)
    for cmd in generate_command_for_size(smart, '1024', half_gig):
        commands.append(cmd)
    for cmd in generate_command_for_size(simple, '1024', half_gig):
        commands.append(cmd)

    for cmd in generate_command_for_size(complex, '2048', half_gig):
        commands.append(cmd)
    for cmd in generate_command_for_size(smart, '2048', half_gig):
        commands.append(cmd)
    for cmd in generate_command_for_size(simple, '2048', half_gig):
        commands.append(cmd)

    write_run(commands)

if __name__ == "__main__":
    main()
