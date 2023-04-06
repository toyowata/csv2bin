# csv2bin
CSV形式のサイバネコード（駅コード）をバイナリ形式に変換します。

## 使用方法

```
$ git clone https://github.com/toyowata/csv2bin
$ cd csv2bin
$ python3 csv2bin.py
```
## C言語配列への変換

```
$ xxd -i -n sc_utf8 sc_utf8.bin > sc_utf8.c
```
