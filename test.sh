#!/bin/bash
assert() {
#将输入的两个参数赋值给本地变量 expected 和 input。
  expected="$1"
  input="$2"
# 调用当前目录下的 chibicc 可执行文件，并将 $input 作为参数传递给它。将 chibicc 的标准输出重定向到名为 tmp.s 的文件中。如果执行出错（返回值非 0），则退出脚本。
  ./chibicc "$input" > tmp.s || exit
# 调用 gcc 编译器，将名为 tmp.s 的汇编代码文件作为输入文件，并生成名为 tmp 的静态可执行文件。
  gcc -static -o tmp tmp.s
# 运行 tmp 可执行文件，将其输出结果存储在本地变量 actual 中
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 0
assert 42 42 #digit 
assert 21 '5+20-4' # sub,add
assert 41 ' 12 + 34 - 5 ' # space
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
assert 10 '-10+20'
assert 10 '- -10'
assert 10 '- - +10'

echo OK