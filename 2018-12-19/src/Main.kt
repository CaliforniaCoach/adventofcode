import java.io.File
import kotlin.system.exitProcess

data class Instr(val instr: String, val a: Int, val b: Int, val c: Int)

fun main() {
    var ipBoundReg = -1
    val prg: MutableList<Instr> = mutableListOf()
    for (line in File("input.txt").readLines()) {
        val tok = line.split(" ")
        if (tok[0] == "#ip") {
            ipBoundReg = tok[1].toInt()
        } else {
            prg.add(Instr(tok[0], tok[1].toInt(), tok[2].toInt(), tok[3].toInt()))
        }
    }

    disassemble(prg, ipBoundReg)

    println("Registers after running with reg[0] = 0: ${run(prg, ipBoundReg, 0).toList()}")
    var sum = 0
    for (i in 1..10551275) {
        if (10551275 % i == 0) {
            sum += i
        }
    }
    println("Sum of prime factors of 875: $sum")

    println("Registers after running with reg[0] = 1: ${run(prg, ipBoundReg, 1).toList()}")
}


fun run(prg: List<Instr>, ipBoundReg: Int, reg0StartVal: Int): IntArray {
    val reg = intArrayOf(reg0StartVal, 0, 0, 0, 0, 0)
    var ip = 0
    while (ip in prg.indices) {
        reg[ipBoundReg] = ip
        val (instr, a, b, c) = prg[ip]
        when (instr) {
            "addr" -> reg[c] = reg[a] + reg[b]
            "addi" -> reg[c] = reg[a] + b
            "mulr" -> reg[c] = reg[a] * reg[b]
            "muli" -> reg[c] = reg[a] * b
            "banr" -> reg[c] = reg[a] and reg[b]
            "bani" -> reg[c] = reg[a] and b
            "borr" -> reg[c] = reg[a] or reg[b]
            "bori" -> reg[c] = reg[a] or b
            "setr" -> reg[c] = reg[a]
            "seti" -> reg[c] = a
            "gtir" -> reg[c] = if (a > reg[b]) 1 else 0
            "gtri" -> reg[c] = if (reg[a] > b) 1 else 0
            "gtrr" -> reg[c] = if (reg[a] > reg[b]) 1 else 0
            "eqir" -> reg[c] = if (a == reg[b]) 1 else 0
            "eqri" -> reg[c] = if (reg[a] == b) 1 else 0
            "eqrr" -> reg[c] = if (reg[a] == reg[b]) 1 else 0
            else -> {
                println("Mnemonic for $instr not known"); exitProcess(4)
            }
        }
        ip = reg[ipBoundReg]
        ip++
    }
    return reg
}

fun disassemble(prg: List<Instr>, ipBoundReg: Int) {
    for (i in prg.indices) {
        val (instr, a, b, c) = prg[i]

        val humanReadale = if (c != ipBoundReg) {
            when (instr) {
                "addr" -> when {
                    a == ipBoundReg -> "${reg2char(c)} = $i + ${reg2char(b)}"
                    b == ipBoundReg -> "${reg2char(c)} = $i + ${reg2char(a)}"
                    else -> "${reg2char(c)} = ${reg2char(a)} + ${reg2char(b)}"
                }
                "addi" -> "${reg2char(c)} = ${reg2char(a)} + $b"
                "mulr" -> when {
                    a == ipBoundReg -> "${reg2char(c)} = $i * ${reg2char(b)}"
                    b == ipBoundReg -> "${reg2char(c)} = $i * ${reg2char(a)}"
                    else -> "${reg2char(c)} = ${reg2char(a)} * ${reg2char(b)}"
                }
                "muli" -> "${reg2char(c)} = ${reg2char(a)} * $b"
                "banr" -> "${reg2char(c)} = ${reg2char(a)} and ${reg2char(b)}"
                "bani" -> "${reg2char(c)} = ${reg2char(a)} and $b"
                "borr" -> "${reg2char(c)} = ${reg2char(a)} or ${reg2char(b)}"
                "bori" -> "${reg2char(c)} = ${reg2char(a)} or $b"
                "setr" -> when {
                    a == ipBoundReg -> "${reg2char(c)} = $i"
                    else -> "${reg2char(c)} = ${reg2char(a)}"
                }
                "seti" -> when {
                    else -> "${reg2char(c)} = $a"
                }
                "gtir" -> "${reg2char(c)} = if ($a > ${reg2char(b)}) 1 else 0"
                "gtri" -> "${reg2char(c)} = if (${reg2char(a)} > $b) 1 else 0"
                "gtrr" -> "${reg2char(c)} = if (${reg2char(a)} > ${reg2char(b)}) 1 else 0"
                "eqir" -> "${reg2char(c)} = if ($a == ${reg2char(b)}) 1 else 0"
                "eqri" -> "${reg2char(c)} = if (${reg2char(a)} == $b) 1 else 0"
                "eqrr" -> "${reg2char(c)} = if (${reg2char(a)} == ${reg2char(b)}) 1 else 0"
                else -> {
                    println("Mnemonic for $instr not known"); exitProcess(4)
                }
            }
        } else {
            when (instr) {
                "addr" -> when {
                    a == ipBoundReg -> "JMPREL ${reg2char(b)} + 1"
                    b == ipBoundReg -> "JMPREL ${reg2char(a)} + 1"
                    else -> "GOTO ${reg2char(a)} + ${reg2char(b)} + 1"
                }
                "addi" -> if (a == ipBoundReg) "GOTO ${i + b + 1}" else "GOTO ${reg2char(a)} + $b + 1"
                "mulr" -> when {
                    a == ipBoundReg && b == ipBoundReg -> "GOTO ${i * i + 1}"
                    a == ipBoundReg -> "GOTO $i * ${reg2char(b)} + 1}"
                    b == ipBoundReg -> "GOTO $i * ${reg2char(a)} + 1}"
                    else -> "GOTO ${reg2char(a)} * ${reg2char(b)} + 1"
                }
                "muli" -> "GOTO ${reg2char(a)} * $b + 1"
                "banr" -> "GOTO ${reg2char(a)} and ${reg2char(b)} + 1"
                "bani" -> "GOTO ${reg2char(a)} and $b + 1"
                "borr" -> "GOTO ${reg2char(a)} or ${reg2char(b)} + 1"
                "bori" -> "GOTO ${reg2char(a)} or $b + 1"
                "setr" -> "GOTO ${reg2char(a)} + 1"
                "seti" -> "GOTO ${a + 1}"
                "gtir" -> "GOTO if ($a > ${reg2char(b)}) 2 else 1"
                "gtri" -> "GOTO if (${reg2char(a)} > $b) 2 else 1"
                "gtrr" -> "GOTO if (${reg2char(a)} > ${reg2char(b)}) 2 else 1"
                "eqir" -> "GOTO if ($a == ${reg2char(b)}) 2 else 1"
                "eqri" -> "GOTO if (${reg2char(a)} == $b) 2 else 1"
                "eqrr" -> "GOTO if (${reg2char(a)} == ${reg2char(b)}) 2 else 1"
                else -> {
                    println("Mnemonic for $instr not known"); exitProcess(4)
                }
            }
        }
        println("$i: $humanReadale")
    }
}

fun reg2char(reg: Int): Char {
    return ('A'.code + reg).toChar()
}
