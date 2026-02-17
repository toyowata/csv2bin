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

## テストコード（ネイティブビルド）

事前に `sc_utf8.h` を生成してください。

```
$ python3 csv2bin.py sc_utf8.csv sc_utf8.h -f hex
```

ビルド:

```
$ cc -O2 -I. -o test/print_sc_utf8 test/print_sc_utf8.c
```

使い方:

```
# 全駅を表示
$ ./test/print_sc_utf8

# 指定した駅を表示（エリア 路線 駅）
$ ./test/print_sc_utf8 0 239 20

# ヘルプ
$ ./test/print_sc_utf8 --help
```
