// ===============================
//  PL/0 VM in TypeScript
//     by wakame_tech 2019/09/19
// ===============================

import { createInterface } from 'readline'

const CMAX = 1000
const STACKSIZE = 500

enum OpeCode {
  O_LIT, O_OPR, O_LOD,
  O_STO, O_CAL, O_INT,
  O_JMP, O_JPC, O_CSP,
  O_LAB, O_BAD, O_RET
}

enum OprCode {
  P_RET, P_NEG, P_ADD, P_SUB, P_MUL, P_DIV, P_ODD, P_DUMMY,
  P_EQ, P_NE, P_LT, P_GE, P_GT, P_LE
}

enum CspCode {
  RDI, WRI, WRL
}

type Instruction = {
  f: OpeCode, l: number, a: number
}

// TODO: refactoring
const linearize = (instructions: Instruction[], length: number): Instruction[] => {
  const code: Instruction[] = Array.from(Array(CMAX), () => ({ f: 0, l: 0, a: 0 }))
  const label: number[] = Array.from(Array(CMAX), () => 0)
  let c1, c2
  let cx = 1
  const cx2 = length + 1

  for (let i = 1; i !== cx2; i++) {
    const { f, l, a } = instructions[i]
    switch (f) {
      case OpeCode.O_LAB: {
        if (!label[a]) {
          label[a] = cx
        } else if (label[a] < 0) {
          c1 = -label[a];
          while (c1 != 0) {
            c2 = code[c1].a
            code[c1].a = cx
            c1 = c2
          }
          label[a] = cx
        } else {
          throw 'label already defined'
        }
        break
      }
      case OpeCode.O_JMP:
      case OpeCode.O_JPC:
      case OpeCode.O_CAL:
        code[cx].f = f
        code[cx].l = l
        if (label[a] <= 0) {
          code[cx].a = -label[a]
          label[a] = -cx
        } else {
          code[cx].a = label[a]
        }
        cx++
        break
      default:
        code[cx] = { f, l, a }
        cx++
        break
    }
  }

  console.log('code table')
  for (let i = 1; i <= cx - 1; i++) {
    const c = code[i]
    const { f, l, a } = c ? c : { f: 0, l: 0, a: 0 }
    console.log(`\t${i} [ ${OpeCode[f]},\t${l},\t ${a} ]`)
  }

  return code
}

const executeVm = async (code: Instruction[]) => {
  const base = (l: number) => {
    let b1 = b
    while (l > 0) {
      b1 = stack[b1];
      l--
    }
    return b1
  }

  console.log('start PL/0')

  const stack: number[] = Array.from(Array(STACKSIZE), () => 0)
  let t = 0, b = 1, p = 1
  stack[1] = stack[2] = stack[3] = 0

  do {
    const { f, l, a } = code[p++]
    // console.log(`${f}, ${l}, ${a}`)

    switch (f) {
      case OpeCode.O_LIT: stack[++t] = a
        break
      case OpeCode.O_OPR:
        switch (a as OprCode) {
          case OprCode.P_RET:
            t = b - 1
            p = stack[t + 3]
            b = stack[t + 2]
            break
          case OprCode.P_NEG:
            stack[t] = -stack[t]
            break
          case OprCode.P_ADD:
            --t
            stack[t] += stack[t + 1]
            break
          case OprCode.P_SUB:
            --t
            stack[t] -= stack[t + 1]
            break
          case OprCode.P_MUL:
            --t
            stack[t] *= stack[t + 1]
            break
          case OprCode.P_DIV:
            --t
            stack[t] /= stack[t + 1]
            break
          case OprCode.P_ODD:
            stack[t] %= 2
            break
          case OprCode.P_EQ:
            --t
            stack[t] = (stack[t] == stack[t + 1]) ? 1 : 0
            break
          case OprCode.P_NE:
            --t
            stack[t] = (stack[t] != stack[t + 1]) ? 1 : 0
            break
          case OprCode.P_LT:
            --t
            stack[t] = (stack[t] < stack[t + 1]) ? 1 : 0
            break
          case OprCode.P_GE:
            --t
            stack[t] = (stack[t] >= stack[t + 1]) ? 1 : 0
            break
          case OprCode.P_GT:
            --t
            stack[t] = (stack[t] > stack[t + 1]) ? 1 : 0
            break
          case OprCode.P_LE:
            --t
            stack[t] = (stack[t] <= stack[t + 1]) ? 1 : 0
            break
        }
        break
      case OpeCode.O_LOD:
        stack[++t] = stack[base(l) + a]
        break
      case OpeCode.O_STO:
        stack[base(l) + a] = stack[t--]
        break
      case OpeCode.O_CAL:
        stack[t + 1] = base(l) /* static link */
        stack[t + 2] = b /* dynamic link */
        stack[t + 3] = p /* ret addr */
        b = t + 1
        p = a
        break
      case OpeCode.O_INT:
        t += a
        break
      case OpeCode.O_RET:
        const tmp = stack[t]
        t = b - 1
        p = stack[t + 3]
        b = stack[t + 2]
        t -= a
        stack[++t] = tmp
        break
      case OpeCode.O_JMP:
        p = a
        break
      case OpeCode.O_JPC:
        if (stack[t--] === 0)
          p = a
        break
      case OpeCode.O_CSP:
        switch(a as CspCode) {
          case CspCode.RDI:
            const rl = createInterface({ input: process.stdin })
            const ait = rl[Symbol.asyncIterator]()
            const i = parseInt((await ait.next()).value)
            rl.close()

            if (!i) {
              console.log('error read')
            }
            stack[++t] = i
            break
          case CspCode.WRI:
            process.stdout.write(`\t${stack[t--]}`)
            break
          case CspCode.WRL:
            process.stdout.write('\n')
            break
        }
        break
    }
  } while (p !== 0)

  console.log('end PL/0')
}

const main = (code: Instruction[]) => {
  const instructions: Instruction[] = Array.from(Array(CMAX), () => ({ f: 0, l: 0, a: 0 }))
  instructions.splice(1, code.length, ...code)

  const linearizedInstructions = linearize(instructions, code.length)

  console.log(linearizedInstructions.slice(0, 40))

  executeVm(linearizedInstructions)
}

const add1_2 = [
  // write 1 + 2
  { f: OpeCode.O_LIT, l: 0, a: 1 } /* 1 */,
  { f: OpeCode.O_LIT, l: 0, a: 2 }, /* 2 */
  { f: OpeCode.O_OPR, l: 0, a: 2 }, /* + */
  { f: OpeCode.O_CSP, l: 0, a: 1 }, /* write */
  { f: OpeCode.O_OPR, l: 0, a: 0 } /* EOF */
]

const fib = [
  { f: OpeCode.O_JMP, l: 0, a:  4 },
  { f: OpeCode.O_LAB, l: 0, a:  1 },
  { f: OpeCode.O_INT, l: 0, a:  3 },
  { f: OpeCode.O_LOD, l: 0, a: -1 },
  { f: OpeCode.O_LIT, l: 0, a:  1 },
  { f: OpeCode.O_OPR, l: 0, a:  8 },
  { f: OpeCode.O_JPC, l: 0, a:  2 },
  { f: OpeCode.O_LIT, l: 0, a:  1 },
  { f: OpeCode.O_RET, l: 0, a:  1 },
  { f: OpeCode.O_LAB, l: 0, a:  2 },
  { f: OpeCode.O_LOD, l: 0, a: -1 },
  { f: OpeCode.O_LIT, l: 0, a:  2 },
  { f: OpeCode.O_OPR, l: 0, a:  8 },
  { f: OpeCode.O_JPC, l: 0, a:  3 },
  { f: OpeCode.O_LIT, l: 0, a:  1 },
  { f: OpeCode.O_RET, l: 0, a:  1 },
  { f: OpeCode.O_LAB, l: 0, a:  3 },
  { f: OpeCode.O_LOD, l: 0, a: -1 },
  { f: OpeCode.O_LIT, l: 0, a:  1 },
  { f: OpeCode.O_OPR, l: 0, a:  3 },
  { f: OpeCode.O_CAL, l: 0, a:  1 },
  { f: OpeCode.O_LOD, l: 0, a: -1 },
  { f: OpeCode.O_LIT, l: 0, a:  2 },
  { f: OpeCode.O_OPR, l: 0, a:  3 },
  { f: OpeCode.O_CAL, l: 0, a:  1 },
  { f: OpeCode.O_OPR, l: 0, a:  2 },
  { f: OpeCode.O_RET, l: 0, a:  1 },
  { f: OpeCode.O_LAB, l: 0, a:  4 },
  { f: OpeCode.O_INT, l: 0, a:  4 },
  { f: OpeCode.O_CSP, l: 0, a:  0 },
  { f: OpeCode.O_STO, l: 0, a:  3 },
  { f: OpeCode.O_LOD, l: 0, a:  3 },
  { f: OpeCode.O_CAL, l: 0, a:  1 },
  { f: OpeCode.O_CSP, l: 0, a:  1 },
  { f: OpeCode.O_CSP, l: 0, a:  2 },
  { f: OpeCode.O_OPR, l: 0, a:  0 }
]

main(fib)
