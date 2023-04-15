# csv2bin
CSV形式のサイバネコード（駅コード）をC言語配列形式またはバイナリ形式に変換します。

## 使用方法

```
$ git clone https://github.com/toyowata/csv2bin
$ cd csv2bin

# C言語配列形式の出力
$ python3 csv2bin.py sc_utf8.csv sc_utf8.h -f hex

# バイナリ形式の出力
$ python3 csv2bin.py sc_utf8.csv sc_utf8.bin -f bin
```
