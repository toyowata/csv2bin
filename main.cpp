/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "BlockDevice.h"
#include <stdio.h>
#include <errno.h>

// Maximum number of elements in buffer
#define BUFFER_MAX_LEN 120

// This will take the system's default block device
BlockDevice *bd = BlockDevice::get_default_instance();

#include "FATFileSystem.h"
FATFileSystem fs("fs");

char found[BUFFER_MAX_LEN], tmp[BUFFER_MAX_LEN];
static int max_char = 0;

int findStation(int area, int line, int station, char *buf)
{
    int idx = 0, num;
    char *endptr, *ptr;
    static int nchar;

    // 文字列を全てをコピー
    strcpy(tmp, buf);

    // カンマを区切りに文字列を分割
    // 1回目
    ptr = strtok(tmp, ",");
    // エリアコード
    num = strtol(ptr, &endptr, 10);
    if (num != area) {
        return 0;
    }
    idx++;
    
    // 2回目以降
    while(ptr != NULL) {
        // strtok関数により変更されたNULLのポインタが先頭
        ptr = strtok(NULL, ",");
        
        // ptrがNULLの場合エラーが発生するので対処
        if(ptr != NULL) {
            num = strtol(ptr, &endptr, 10);
            // 路線コード
            if (idx == 1) {
                if (num != line) {
                    return 0;
                }
            }
            // 駅コード
            if (idx == 2) {
                if (num != station) {
                    return 0;
                }
            }
            // 路線名
            if (idx == 3) {
                sprintf(found, "%s線 ", ptr);
                /*
                nchar = strlen(ptr);
                if (max_char < nchar) {
                    max_char = nchar;
                    printf("%s\n", ptr);
                }
                */
            }
            // 駅名
            if (idx == 4) {
                //printf("%d %d %d %s %s\n", area, line, station, found, ptr);
                strcat(found, ptr);
                return 1;
            }
            idx++;
        }
    }

    return 0;
}

int csv2bin(char *buf)
{
    int idx = 0, num;
    char *endptr, *ptr;
    int len;

    // 文字列を全てをコピー
    strcpy(tmp, buf);

    // カンマを区切りに文字列を分割
    // 1回目
    ptr = strtok(tmp, ",");
    // エリアコード
    num = strtol(ptr, &endptr, 10);
    printf("%1d ", num);
    idx++;
    
    // 2回目以降
    while(ptr != NULL) {
        // strtok関数により変更されたNULLのポインタが先頭
        ptr = strtok(NULL, ",");
        
        // ptrがNULLの場合エラーが発生するので対処
        if(ptr != NULL) {
            if (idx == 1 || idx == 2) {
                num = strtol(ptr, &endptr, 10);
                printf("%03d ", num);
            }

            // 路線名
            if (idx == 3) {
                sprintf(found, "%s線", ptr);
                len = strlen(found);
                for (int i = len; i < 40; i++) {
                    strcat(found, " ");
                }
                printf("%02d %s", len, found);                
            }

            // 駅名
            if (idx == 4) {
                printf("%s\n", ptr);
            }

            idx++;
        }
        if(idx > 4) {
            break;
        }
    }

    return 0;
}


int main() {

    int err = fs.mount(bd);
    if (err) {
        // Reformat if we can't mount the filesystem
        // this should only happen on the first boot
        printf("No filesystem found, formatting... ");
        fflush(stdout);
        err = fs.reformat(bd);
        printf("%s\n", (err ? "Fail :(" : "OK"));
        if (err) {
            error("error: %s (%d)\n", strerror(-err), err);
        }
    }

    // Open the numbers file
    printf("Opening \"/fs/sc2_utf8.csv\"... ");
    fflush(stdout);
    FILE *f = fopen("/fs/sc2_utf8.csv", "r");
    printf("%s\n", (!f ? "Fail :(" : "OK"));

    char buf[BUFFER_MAX_LEN];

    while (fgets(buf, BUFFER_MAX_LEN, f) != NULL) {
#if 1
        csv2bin(buf);
#else
        if (findStation(0, 239, 20, buf)) {
            printf("[%s]\n", found);
            break;
        }
#endif
    }
    // printf("max-char = %d\n", max_char);

    // Close the file which also flushes any cached writes
    err = fclose(f);
    if (err < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    // Tidy up
    err = fs.unmount();
    if (err < 0) {
        error("error: %s (%d)\n", strerror(-err), err);
    }

}

