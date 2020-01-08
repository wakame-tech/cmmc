# コンパイラ 最終レポート

## 演算子
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

## if while
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

## 剰余
剰余演算子 `%` は ないので `x % y` は `x - y * (x / y)` に変換.

## スタック
スコープ毎に予約スタックが3つあるので `STO ( 0, 3 )` から始める.
スタック確保も `INT ( 0, 3 + n )`.

## 引数
最後の引数が `baseptr - 1` 次が `baseptr - 2` ... というように入っている.

## 剰余演算子 `%`
### 構文
```
<expr> % <expr>
```

### 仕様

`a % b` -> `a - b * (a // b)` であるが二回取得できない？のでVM拡張した。

`( OPR, 0, 7 )` # %

## for文
### 構文　
```
for_stmt ::
  = "for" init ";" cont ";" upd "do" stmts "endfor"
```

### 仕様
```
for <初期化>; <継続条件>; <更新式> do
  <文>;
  ...
endfor
```

## 条件演算子
### 構文
```
cond_op ::
  = expr "&&" expr
  | expr "||" expr
  | "!" expr
```

### 仕様
- `<expr> && <expr>`: かつ  
- `<expr> || <expr>`: または
- `!<expr>`: `<expr>` が Truty な値ならば 0 を、そうでないならば 1 を返す

## インクリメント/デクリメント
### 構文
```
increment_and_decrement ::
  = IDENT "++"
  | "++" IDENT
  | IDENT "--"
  | "--" IDENT
```

### 仕様

## VM拡張
### `DLD`, `DST`
#### dyn.pl0
```
( INT, 0, 3 )
( INT, 0, 2 ) // a[2]
( LIT, 0, 1 )
( STO, 0, 3 ) // a[0] = 1
( LIT, 0, 2 )
( STO, 0, 4 ) // a[1] = 2

( LIT, 0, 1 )
( DLD, 0, 0 ) // a[1]
( CSP, 0, 1 ) // write == 2
( CSP, 0, 2 )

( LIT, 0, 5 )
( LIT, 0, 0 )
( DST, 0, 0 ) // a[0] = 5

( LOD, 0, 3 ) // a[0]
( CSP, 0, 1 ) // write == 5
( CSP, 0, 2 )

( OPR, 0, 0 )
```

## 配列
動的ロード/ストアが出来るようにVM拡張