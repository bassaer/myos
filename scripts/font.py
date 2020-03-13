import argparse


def read_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', help='font file', required=True)
    return parser.parse_args()

def read_file(path):
    with open(path, 'r') as f:
        return f.read()


def main():
    args = read_args()
    print(read_file(args.f))


if __name__ == '__main__':
    main()

