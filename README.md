# pl0c
## 🚧 Loadmap

- [x] 剰余演算子の実装
- [x] 累乗演算子の実装
- [x] for文の実装
- [x] goto/label文の実装
- [x] case文の実装
- [x] 配列の実装
- [x] 条件演算子の実装
- [x] インクリメント/デクリメントの実装
- [ ] レポートかく
- [ ] リファクタリング

## 🔧 Environment
```
$ lex -V
flex 2.5.35 Apple(flex-31)

$ yacc -V
bison (GNU Bison) 2.3
Written by Robert Corbett and Richard Stallman.

$ gcc -v
Apple LLVM version 10.0.1 (clang-1001.0.46.4)
```

## 🌴 Tree
```
├── compiler // cmm compiler src
├── examples // cmm, pl0 examples
├── report // mid report
├── test // test scripts
└── vm // pl0 vm src
```

## 📔 Memo
### 演算子
`(OPR, 0, a)`
- 0: 手続きから戻る
- 1: 単項-
- 2: +
- 3: -
- 4: *
- 5: /
- 6: val % 2 == 1 ? 1 : 0
- 7: %
- 8: =
- 9: !=
- 10: <
- 11: >=
- 12: >
- 13: <=
// ここから拡張
- 14: &&
- 15: ||
- 16: !
- 17: ^

### if while
for 文はないので `while` に変換
```
if <cond> then begin
  <stmts>
end
```

```
while <cond> do begin
  <stmts>
end
```

### 剰余
~~剰余演算子 `%` は ないので `x % y` は `x - y * (x / y)` に変換.~~
-> **VMを拡張した**

### スタック
スコープ毎に予約スタックが3つあるので `STO ( 0, 3 )` から始める.
スタック確保も `INT ( 0, 3 + n )`.

### 引数
最後の引数が `baseptr - 1` 次が `baseptr - 2` ... というように入っている.
