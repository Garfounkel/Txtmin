import sys
import subprocess as sp
import glob
import os


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def usage():
    print("usage:")
    print("\tpython app-test-suite.py tests.folder ref.app ref.bin mine.app mine.bin")
    exit(64)


def test_cat_echo():
    tmp_file = 'tmp.in'
    with open(tmp_file, 'w') as file:
        file.write("approx 0 test")
    ref_process = sp.run(['cat', tmp_file, '|', f'./{ref}', ref_bin],
                         stdout=sp.PIPE, stderr=sp.DEVNULL)
    mine_process = sp.run(['cat', tmp_file, '|', f'./{mine}', mine_bin],
                          stdout=sp.PIPE, stderr=sp.DEVNULL)
    print_test('cat ' + tmp_file + ' | ./App bin', ref_process, mine_process)
    os.remove(tmp_file)

    echo = '"approx 0 test"'
    ref_process = sp.run(['echo', echo, '|', f'./{ref}', ref_bin],
                         stdout=sp.PIPE, stderr=sp.DEVNULL)
    mine_process = sp.run(['echo', echo, '|', f'./{mine}', mine_bin],
                          stdout=sp.PIPE, stderr=sp.DEVNULL)
    print_test('echo ' + echo + ' | ./App bin', ref_process, mine_process)


def print_test(testname, ref_process, mine_process):
    ref_code = ref_process.returncode
    mine_code = mine_process.returncode

    returncode_ok = ref_code == mine_code
    stdout_ok = ref_process.stdout == mine_process.stdout

    if returncode_ok and stdout_ok:
        print(f"{testname}: {bcolors.OKGREEN}OK{bcolors.ENDC}")
    else:
        print(f"{testname}: {bcolors.FAIL}KO{bcolors.ENDC} ")
        if not returncode_ok:
            print(f"\treturn code: {mine_code}[mine], {ref_code}[ref]")
        if not stdout_ok:
            print("\tstdout differs")
        else:
            print("\tstdout OK")


def run_test(testname, test, ref, ref_bin, mine, mine_bin):
    ref_process = sp.run([f"./{ref}", ref_bin],
                         input=test, stdout=sp.PIPE, stderr=sp.DEVNULL)
    mine_process = sp.run([f"./{mine}", mine_bin],
                          input=test, stdout=sp.PIPE, stderr=sp.DEVNULL)
    print_test(testname, ref_process, mine_process)


if __name__ == '__main__':
    if len(sys.argv) != 6:
        usage()

    testspath = sys.argv[1]
    ref = sys.argv[2]
    ref_bin = sys.argv[3]
    mine = sys.argv[4]
    mine_bin = sys.argv[5]

    test_cat_echo()

    for filename in glob.glob(os.path.join(testspath, '*.in')):
        with open(filename) as file:
            test = file.read().encode()
            run_test(filename, test, ref, ref_bin, mine, mine_bin)
