#! /usr/bin/python3

########################################################################################################################
# Imports
########################################################################################################################

from subprocess import run, PIPE
from locale import setlocale, atoi, LC_ALL
from os import remove
from random import choice, randrange
from string import ascii_letters
from json import dumps
from datetime import datetime

########################################################################################################################
# Static vars
########################################################################################################################

INCLUDE_FILES_IN_JSON = False  # If the infile and outfile should be included in the json output

WRITE_LOG = True  # If a log file should be written to disk

ITERATIVE_TEST = True  # If you want to do one or multiple tests
NUMBER_OF_TEST = 5  # Number of tests to run

CHARS_SIZE_START = 10  # Number of chars to start with
CHARS_SIZE_INC = 8   # How much we should increase the number of chars in the infile each tests
CHARS_SIZE_RANDOM = True  # If we should randomize the number of chars each iteration
CHARS_INC_MULTIPLY = True  # True if we should multiply to increase each iteration, false to add

BUFFER_SIZE_START = 10  # Starting size of the buffer
BUFFER_SIZE_INC = 5  # How much we should multiply the buffer size by each iteration
BUFFER_SIZE_RANDOM = False  # If we should randomize the buffer size
BUFFER_INC_MULTIPLY = True  # True if we should multiply to increase each iteration, false to add

setlocale(LC_ALL, 'en_US.UTF-8')

########################################################################################################################
# Utility functions
########################################################################################################################


def encode(msg, key):  # Encodes the message with the specified key
    low_a = ord('a')
    cap_a = ord('A')
    low_z = ord('z')
    cap_z = ord('Z')

    encrypt = ''
    for c in msg:
        c = ord(c)
        if low_a <= c <= low_z:
            c += key
            if c > low_z:
                c = c - low_z + low_a - 1
        elif cap_a <= c <= cap_z:
            c += key
            if c > cap_z:
                c = c - cap_z + cap_a - 1
        encrypt += chr(c)
    return encrypt


def decode(msg, key):  # Decodes the message with the specified key
    low_a = ord('a')
    cap_a = ord('A')
    low_z = ord('z')
    cap_z = ord('Z')

    decrypt = ''
    for c in msg:
        c = ord(c)
        if low_a <= c <= low_z:
            c -= key
            if c < low_a:
                c = c + low_z - low_a + 1
        elif cap_a <= c <= cap_z:
            c -= key
            if c < cap_a:
                c = c + cap_z - cap_a + 1
        decrypt += chr(c)
    return decrypt


def count_chars(chars):
    cts = {}
    for c in chars.upper():
        if c in cts:
            cts[c] += 1
        else:
            cts[c] = 1
    return cts


def compare_file_to_count(chars, counts):
    for c in counts.keys():
        if c in chars:
            chars[c] -= counts[c]
        else:
            chars[c] = -1
    rtn = {}
    for c in chars.keys():
        if chars[c] != 0:
            rtn[c] = chars[c]
    return rtn


def count_file(chars):
    rtn = {}
    for c in chars.upper():
        if c in rtn:
            rtn[c] += 1
        else:
            rtn[c] = 1
    return rtn


def verify(program):
    comp = len(program['infile']['decoded']) - len(program['outfile']['encoded'])
    rtn = {
        'infile': {
            'bigger': comp > 0,
            'counts': count_file(program['infile']['decoded']),
            'matches': program['infile']['encoded'] == program['outfile']['encoded']
        },
        'outfile': {
            'bigger': comp < 0,
            'counts': count_file(program['outfile']['encoded']),
            'matches': program['outfile']['decoded'] == program['infile']['decoded']
        },
        'success': False
    }

    for io in (('infile', 'in'), ('outfile', 'out')):
        for cts in program['counts']:
            rtn[io[0]]['counts'] = compare_file_to_count(rtn[io[0]]['counts'], cts['letters'][io[1]])

    rtn['success'] = not rtn['infile']['bigger'] and not rtn['outfile']['bigger']
    rtn['success'] = rtn['success'] and rtn['infile']['matches'] and rtn['outfile']['matches']
    rtn['success'] = rtn['success'] and len(rtn['infile']['counts']) == 0
    rtn['success'] = rtn['success'] and len(rtn['outfile']['counts']) == 0
    return rtn


def run_program(buffer_size, input_size):
    in_text = ''.join([choice(ascii_letters) for _ in range(input_size)])

    with open('infile', "w") as infile:
        infile.write(in_text)

    command = ['valgrind', './encrypt352', 'infile', 'outfile']
    buffer_size = str(buffer_size)
    encrypt352 = run(command, stdout=PIPE, input=str.encode(buffer_size), stderr=PIPE)

    err = encrypt352.stderr.decode('utf-8')

    err_clean = ''

    valgrind_out = {
        'heap_summary': {
            'in_use': {},
            'total_usage': {}
        },
        'leak_summary': {
            'lost': {
                'definitely': {},
                'indirectly': {},
                'possibly': {}
            },
            'reachable': {},
            'suppressed': {}
        }
    }

    finished = False
    valgrind_output = ''
    for line in err.split('\n'):
        if '==' in line:
            valgrind_output += line + '\n'
            if finished:
                continue
            elif 'in use at exit:' in line:
                line = line.split()
                valgrind_out['heap_summary']['in_use']['bytes'] = atoi(line[5])
                valgrind_out['heap_summary']['in_use']['blocks'] = atoi(line[8])
            elif 'total heap usage:' in line:
                line = line.split()
                valgrind_out['heap_summary']['total_usage']['allocs'] = atoi(line[4])
                valgrind_out['heap_summary']['total_usage']['frees'] = atoi(line[6])
                valgrind_out['heap_summary']['total_usage']['allocated'] = atoi(line[8])
            elif 'definitely lost:' in line:
                line = line.split()
                valgrind_out['leak_summary']['lost']['definitely']['bytes'] = atoi(line[3])
                valgrind_out['leak_summary']['lost']['definitely']['blocks'] = atoi(line[6])
            elif 'indirectly lost:' in line:
                line = line.split()
                valgrind_out['leak_summary']['lost']['indirectly']['bytes'] = atoi(line[3])
                valgrind_out['leak_summary']['lost']['indirectly']['blocks'] = atoi(line[6])
            elif 'possibly lost:' in line:
                line = line.split()
                valgrind_out['leak_summary']['lost']['possibly']['bytes'] = atoi(line[3])
                valgrind_out['leak_summary']['lost']['possibly']['blocks'] = atoi(line[6])
            elif 'still reachable:' in line:
                line = line.split()
                valgrind_out['leak_summary']['reachable']['bytes'] = atoi(line[3])
                valgrind_out['leak_summary']['reachable']['blocks'] = atoi(line[6])
            elif 'suppressed:' in line:
                line = line.split()
                valgrind_out['leak_summary']['suppressed']['bytes'] = atoi(line[2])
                valgrind_out['leak_summary']['suppressed']['blocks'] = atoi(line[5])
                finished = True
        elif line != '':
            err_clean += line + '\n'

    if err_clean != '':
        return {
            'error': err_clean,
            'code': encrypt352.returncode
        }

    out = encrypt352.stdout.decode('utf-8')

    out_fixed = ''
    for line in out.split('\n'):
        if 'Key' in line:
            continue
        elif 'buffer size' in line:
            line += buffer_size
        out_fixed += line + '\u21B2\n'

    with open('outfile', "r") as outfile:
        out_text = outfile.read()

    remove('infile')
    remove('outfile')

    counts = []
    valgrind_out['output'] = valgrind_output
    rtn = {
        'input_size': input_size,
        'buffer_size': int(buffer_size),
        'infile': {
            'count': count_chars(in_text),
            'decoded': in_text,
            'encoded': ''
        },
        'outfile': {
            'count': count_chars(out_text),
            'encoded': out_text,
            'decoded': ''
        },
        'output': out,
        'program_output': out_fixed,
        'resets': -1,
        'valgrind': valgrind_out
    }

    which_count = 'in'
    msg_pos_in = 0
    msg_pos_out = 0
    letters_counted_in = 0
    letters_counted_out = 0
    for line in out.split('\n'):
        if line == 'Enter buffer size: ' or line == '' or '==' in line:
            continue
        elif line == 'Reset finished' or line == 'End of file reached':
            rtn['resets'] += 1

            encoded = encode(rtn['infile']['decoded'][msg_pos_in:letters_counted_in], counts[-1]['key'])
            rtn['infile']['encoded'] += encoded
            msg_pos_in = letters_counted_in

            decoded = decode(rtn['outfile']['encoded'][msg_pos_out:letters_counted_out], counts[-1]['key'])
            rtn['outfile']['decoded'] += decoded
            msg_pos_out = letters_counted_out
        elif line == 'Input file contains':
            which_count = 'in'
        elif line == 'Output file contains':
            which_count = 'out'
        elif 'Key' in line:
            counts.append({
                'key': int(line.split(':')[1]),
                'letters': {
                    'in': {},
                    'out': {}
                }
            })
        else:
            for letter in line.split():
                letter = letter.split(":")
                count = int(letter[1])
                letter = letter[0]

                if letter[0] not in counts[-1]['letters'][which_count]:
                    counts[-1]['letters'][which_count][letter] = 0

                counts[-1]['letters'][which_count][letter] += count
                if which_count == 'in':
                    letters_counted_in += count
                elif which_count == 'out':
                    letters_counted_out += count

    rtn['counts'] = counts
    rtn['verify'] = verify(rtn)

    if not INCLUDE_FILES_IN_JSON:
        del rtn['infile']['encoded']
        del rtn['infile']['decoded']
        del rtn['outfile']['encoded']
        del rtn['outfile']['decoded']

    return rtn


def get_actual_size(iteration, size_start, inc_multiply, size_inc, size_random):
    size = size_start
    for _ in range(iteration):
        if inc_multiply:
            size *= size_inc
        else:
            size += size_inc

    if size_random and size != 1:
        return randrange(1, size)
    else:
        return size


def get_buffer_size(iteration=0):
    return get_actual_size(iteration, BUFFER_SIZE_START, BUFFER_INC_MULTIPLY, BUFFER_SIZE_INC, BUFFER_SIZE_RANDOM)


def get_infile_size(iteration=0):
    return get_actual_size(iteration, CHARS_SIZE_START, CHARS_INC_MULTIPLY, CHARS_SIZE_INC, CHARS_SIZE_RANDOM)


def log_write(out):
    if not WRITE_LOG:
        return
    filename = datetime.now().strftime('%m-%d-%Y_%H-%M-%S') + '.log'
    with open(filename, 'w') as log:
        log.write(dumps(out, indent=2, sort_keys=True))


########################################################################################################################
# Run tests
########################################################################################################################

if not ITERATIVE_TEST:
    infileSize = get_infile_size()
    bufferSize = get_buffer_size()

    print(f'Using input size {infileSize}')
    print(f'Using buffer size {bufferSize}')

    output = run_program(bufferSize, infileSize)

    resets = output['resets']
    print(f'Total of {resets} resets happened.')

    if output['verify']['infile']['bigger']:
        print('More characters were read than written')
    elif output['verify']['outfile']['bigger']:
        print('More characters were written than read')
    else:
        print('The same number of characters were read and written')

    if len(output['verify']['infile']['counts']) > 0:
        print('Input not correctly counted')
    else:
        print('Input counted correctly')
    if len(output['verify']['outfile']['counts']) > 0:
        print('Output not correctly counted')
    else:
        print('Output counted correctly')

    if output['verify']['infile']['matches'] and output['verify']['infile']['matches']:
        print('Message encoded correctly')
    else:
        print('Message did not encode correctly')

    log_write(output)
else:
    tests = []
    passed = 0
    for i in range(NUMBER_OF_TEST):
        print(f'Starting test {i+1}')

        buf_size = get_buffer_size(iteration=i)
        in_size = get_infile_size(iteration=i)

        print(f'Using buffer size of {buf_size} and input size of {in_size}')
        output = run_program(buf_size, in_size)

        tests.append(output)
        if output['verify']['success']:
            print('Test passed')
            passed += 1
        else:
            print('Test failed')
            #  print(dumps(output, indent=2, sort_keys=True))

    log_write(tests)
    print('', f'{passed} out of {NUMBER_OF_TEST} tests passed!')
