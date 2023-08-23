#!/usr/bin/env python3

import csv
import os, argparse

parser = argparse.ArgumentParser(description='Convert station code CVS file to binary file')

parser.add_argument('input', help='input CSV file')
parser.add_argument('out', help='output binary file')
parser.add_argument('-d', '--debug', action='store_true')
parser.add_argument('-f', '--format', choices=['bin', 'hex'])
args = parser.parse_args()

print('input  : '+args.input)
print('output : '+args.out)
if args.debug == True:
    print('debug print enabled')

MAX_LINE_CHAR = 40
MAX_STATION_CHAR = 40


def bytes_to_c_arr(data, lowercase=True):
    return [format(b, '#04x' if lowercase else '#04X') for b in data]

def main():
    nlines = 0
    nbytes = 0

    fin = open(args.input, 'r')
    reader = csv.reader(fin)
    array_name = os.path.basename(args.input).split('.', 1)[0]

    attr = "wb"
    if args.format == 'hex':
        attr = "w"

    with open(args.out, attr) as fout:
        if args.format == 'hex':
            s = "const unsigned char " + array_name + "[] = {\n"
            fout.write(s)

        # Column 1 : Area code (int, 1-byte)
        # Column 2 : Route code (int, 1-byte)
        # Column 3 : Station code (int, 1-byte)
        # Column 4 : Route name (string, max 40-bytes)
        # Column 5 : Station name (string, max 40-bytes)

        for row in reader:
            nlines += 1
            area = int(row[0])
            line = int(row[1])
            station = int(row[2])
            line_len = len(row[3].encode('utf-8'))
            station_len = len(row[4].encode('utf-8'))

            if args.debug == True:
                print(format(area, '02x'), format(line, '02x'), format(station, '02x'), end=" ")
                print(row[3] + '(' + str(line_len) + ')', end=" ")
                print(row[4] + '(' + str(station_len) + ')')

            bary = bytearray([area, line, station])
            bary.extend(row[3].encode('utf-8'))
            for x in range(MAX_LINE_CHAR - line_len):
                bary.append(0)
            bary.extend(row[4].encode('utf-8'))
            for y in range(MAX_STATION_CHAR - station_len):
                bary.append(0)
            
            if MAX_LINE_CHAR < line_len:
                print("*** ERROR:", area, line, station)
            if MAX_STATION_CHAR < station_len:
                print("*** ERROR:", area, line, station)
            
            if args.format == 'hex':
                s = "    " + format(", ".join(bytes_to_c_arr(bary))) + ',\n'
                fout.write(s)
            else:
                fout.write(bary)
            
            nbytes += len(bary)

        if args.format == 'hex':
            s = "};\n\nconst unsigned int " + array_name + "_len = " + str(nbytes) + ";\n"
            fout.write(s)
        
    fin.close()

    print('total lines =', nlines)
    print('bytes =', nbytes)

if __name__ == "__main__":
    main()
