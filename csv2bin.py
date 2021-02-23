#!/usr/bin/env python3

import csv

MAX_LINE_CHAR = 40
MAX_STATION_CHAR = 40

# Column 1 : Area code (int, 1-byte)
# Column 2 : Route code (int, 1-byte)
# Column 3 : Station code (int, 1-byte)
# Column 4 : Route name (string, max 40-bytes)
# Column 5 : Station name (string, max 40-bytes)

def main():
    f = open('sc2_utf8.csv', 'r')

    reader = csv.reader(f)
    with open("sc2_utf8.bin", "wb") as fout:
        for row in reader:
            area = int(row[0])
            line = int(row[1])
            station = int(row[2])
            line_len = len(row[3].encode('utf-8'))
            station_len = len(row[4].encode('utf-8'))

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
                
            fout.write(bary)

    f.close()

if __name__ == "__main__":
    main()
