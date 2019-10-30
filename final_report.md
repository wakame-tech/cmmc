# コンパイラ 最終レポート

やったこと

- [x] 剰余演算子の実装
- [x] for文の実装
- [x] 条件演算子の実装
- [x] インクリメント/デクリメントの実装
- [ ] goto/label文の実装
- [x] 動的ロード/ストアが出来るようにVM拡張
- [ ] 配列の実装
- [ ] 生成コード最適化(定数畳み込み)
- [ ] 関数先読み
- [ ] 末尾再帰最適化

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

( LIT, 0, 4 ) // 3 + 1
( DLD, 0, 0 ) // a[1]
( CSP, 0, 1 ) // write
( CSP, 0, 2 )

( LIT, 0, 5 )
( LIT, 0, 3 )
( DST, 0, 0 ) // a[0] = 5

( LOD, 0, 3 ) // a[0]
( CSP, 0, 1 ) // write
( CSP, 0, 2 )

( OPR, 0, 0 )
```