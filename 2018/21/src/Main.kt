import java.io.File
import kotlin.system.exitProcess

enum class Mnemonic {
    ADDR,
    ADDI,
    MULR,
    MULI,
    BANR,
    BANI,
    BORR,
    BORI,
    SETR,
    SETI,
    GTIR,
    GTRI,
    GTRR,
    EQIR,
    EQRI,
    EQRR
}

data class Instr(val instr: Mnemonic, val a: Long, val b: Long, val c: Long)

fun main() {
    var ipBoundReg = -1
    val prg: MutableList<Instr> = mutableListOf()
    for (line in File("input.txt").readLines()) {
        val tok = line.split(" ")
        if (tok[0] == "#ip") {
            ipBoundReg = tok[1].toInt()
        } else {
            val mnemonic = when (tok[0]) {
                "addi" -> Mnemonic.ADDI
                "addr" -> Mnemonic.ADDR
                "mulr" -> Mnemonic.MULR
                "muli" -> Mnemonic.MULI
                "banr" -> Mnemonic.BANR
                "bani" -> Mnemonic.BANI
                "borr" -> Mnemonic.BORR
                "bori" -> Mnemonic.BORI
                "setr" -> Mnemonic.SETR
                "seti" -> Mnemonic.SETI
                "gtir" -> Mnemonic.GTIR
                "gtri" -> Mnemonic.GTRI
                "gtrr" -> Mnemonic.GTRR
                "eqir" -> Mnemonic.EQIR
                "eqri" -> Mnemonic.EQRI
                "eqrr" -> Mnemonic.EQRR
                else -> exitProcess(1)
            }
            prg.add(Instr(mnemonic, tok[1].toLong(), tok[2].toLong(), tok[3].toLong()))
        }
    }

    disassemble(prg, ipBoundReg)
    println()

    run(prg, ipBoundReg, 0, true)
}


fun run(prg: List<Instr>, ipBoundReg: Int, reg0StartVal: Long, checkCyclicPattern: Boolean): Int {
    val reg = longArrayOf(reg0StartVal, 0, 0, 0, 0, 0)
    var ip = 0L
    var numInstructionsExecuted = 0
    var firstReg0CompVal: Long? = null
    val regStatesVisited: MutableList<List<Long>> = mutableListOf()
    while (ip in prg.indices) {
        reg[ipBoundReg] = ip
        val (instr, a, b, c) = prg[ip.toInt()]
        when (instr) {
            Mnemonic.ADDR -> reg[c.toInt()] = reg[a.toInt()] + reg[b.toInt()]
            Mnemonic.ADDI -> reg[c.toInt()] = reg[a.toInt()] + b
            Mnemonic.MULR -> reg[c.toInt()] = reg[a.toInt()] * reg[b.toInt()]
            Mnemonic.MULI -> reg[c.toInt()] = reg[a.toInt()] * b
            Mnemonic.BANR -> reg[c.toInt()] = reg[a.toInt()] and reg[b.toInt()]
            Mnemonic.BANI -> reg[c.toInt()] = reg[a.toInt()] and b
            Mnemonic.BORR -> reg[c.toInt()] = reg[a.toInt()] or reg[b.toInt()]
            Mnemonic.BORI -> reg[c.toInt()] = reg[a.toInt()] or b
            Mnemonic.SETR -> reg[c.toInt()] = reg[a.toInt()]
            Mnemonic.SETI -> reg[c.toInt()] = a
            Mnemonic.GTIR -> reg[c.toInt()] = if (a > reg[b.toInt()]) 1 else 0
            Mnemonic.GTRI -> reg[c.toInt()] = if (reg[a.toInt()] > b) 1 else 0
            Mnemonic.GTRR -> reg[c.toInt()] = if (reg[a.toInt()] > reg[b.toInt()]) 1 else 0
            Mnemonic.EQIR -> reg[c.toInt()] = if (a == reg[b.toInt()]) 1 else 0
            Mnemonic.EQRI -> reg[c.toInt()] = if (reg[a.toInt()] == b) 1 else 0
            Mnemonic.EQRR -> {
                reg[c.toInt()] = if (reg[a.toInt()] == reg[b.toInt()]) 1 else 0
                if (checkCyclicPattern) {
                    if (a == 0L || b == 0L) {
                        val compReg = if (a == 0L) b.toInt() else a.toInt()
                        val compVal = reg[compReg]
                        if (firstReg0CompVal == null) {
                            firstReg0CompVal = compVal
                            println("Reg 0 value resulting in fewest instruction executions: $compVal")
                        } else {
                            if (reg.toList() in regStatesVisited) {
                                val compValues = regStatesVisited.map { it[compReg] }
                                for ((i, value) in compValues.withIndex().reversed()) {
                                    if (i == compValues.indexOf(value)) {
                                        println("Reg 0 value resulting in most instruction executions: $value")
                                        return -1
                                    }
                                }
                            }
                        }
                        regStatesVisited.add(reg.toList())
                    }
                }
            }
        }
        ip = reg[ipBoundReg]
        ip++
        numInstructionsExecuted++
    }
    return numInstructionsExecuted
}

fun disassemble(prg: List<Instr>, ipBoundReg: Int) {
    @Suppress("NAME_SHADOWING")
    val ipBoundReg = ipBoundReg.toLong()
    for (i in prg.indices) {
        val (instr, a, b, c) = prg[i]

        val humanReadale = if (c != ipBoundReg) {
            when (instr) {
                Mnemonic.ADDR -> when {
                    a == ipBoundReg -> "${reg2char(c)} = $i + ${reg2char(b)}"
                    b == ipBoundReg -> "${reg2char(c)} = $i + ${reg2char(a)}"
                    else -> "${reg2char(c)} = ${reg2char(a)} + ${reg2char(b)}"
                }
                Mnemonic.ADDI -> "${reg2char(c)} = ${reg2char(a)} + $b"
                Mnemonic.MULR -> when {
                    a == ipBoundReg -> "${reg2char(c)} = $i * ${reg2char(b)}"
                    b == ipBoundReg -> "${reg2char(c)} = $i * ${reg2char(a)}"
                    else -> "${reg2char(c)} = ${reg2char(a)} * ${reg2char(b)}"
                }
                Mnemonic.MULI -> "${reg2char(c)} = ${reg2char(a)} * $b"
                Mnemonic.BANR -> "${reg2char(c)} = ${reg2char(a)} and ${reg2char(b)}"
                Mnemonic.BANI -> "${reg2char(c)} = ${reg2char(a)} and $b"
                Mnemonic.BORR -> "${reg2char(c)} = ${reg2char(a)} or ${reg2char(b)}"
                Mnemonic.BORI -> "${reg2char(c)} = ${reg2char(a)} or $b"
                Mnemonic.SETR -> when {
                    a == ipBoundReg -> "${reg2char(c)} = $i"
                    else -> "${reg2char(c)} = ${reg2char(a)}"
                }
                Mnemonic.SETI -> when {
                    else -> "${reg2char(c)} = $a"
                }
                Mnemonic.GTIR -> "${reg2char(c)} = if ($a > ${reg2char(b)}) 1 else 0"
                Mnemonic.GTRI -> "${reg2char(c)} = if (${reg2char(a)} > $b) 1 else 0"
                Mnemonic.GTRR -> "${reg2char(c)} = if (${reg2char(a)} > ${reg2char(b)}) 1 else 0"
                Mnemonic.EQIR -> "${reg2char(c)} = if ($a == ${reg2char(b)}) 1 else 0"
                Mnemonic.EQRI -> "${reg2char(c)} = if (${reg2char(a)} == $b) 1 else 0"
                Mnemonic.EQRR -> "${reg2char(c)} = if (${reg2char(a)} == ${reg2char(b)}) 1 else 0"
            }
        } else {
            when (instr) {
                Mnemonic.ADDR -> when {
                    a == ipBoundReg -> "JMPREL ${reg2char(b)} + 1"
                    b == ipBoundReg -> "JMPREL ${reg2char(a)} + 1"
                    else -> "GOTO ${reg2char(a)} + ${reg2char(b)} + 1"
                }
                Mnemonic.ADDI -> if (a == ipBoundReg) "GOTO ${i + b + 1}" else "GOTO ${reg2char(a)} + $b + 1"
                Mnemonic.MULR -> when {
                    a == ipBoundReg && b == ipBoundReg -> "GOTO ${i * i + 1}"
                    a == ipBoundReg -> "GOTO $i * ${reg2char(b)} + 1}"
                    b == ipBoundReg -> "GOTO $i * ${reg2char(a)} + 1}"
                    else -> "GOTO ${reg2char(a)} * ${reg2char(b)} + 1"
                }
                Mnemonic.MULI -> "GOTO ${reg2char(a)} * $b + 1"
                Mnemonic.BANR -> "GOTO ${reg2char(a)} and ${reg2char(b)} + 1"
                Mnemonic.BANI -> "GOTO ${reg2char(a)} and $b + 1"
                Mnemonic.BORR -> "GOTO ${reg2char(a)} or ${reg2char(b)} + 1"
                Mnemonic.BORI -> "GOTO ${reg2char(a)} or $b + 1"
                Mnemonic.SETR -> "GOTO ${reg2char(a)} + 1"
                Mnemonic.SETI -> "GOTO ${a + 1}"
                Mnemonic.GTIR -> "GOTO if ($a > ${reg2char(b)}) 2 else 1"
                Mnemonic.GTRI -> "GOTO if (${reg2char(a)} > $b) 2 else 1"
                Mnemonic.GTRR -> "GOTO if (${reg2char(a)} > ${reg2char(b)}) 2 else 1"
                Mnemonic.EQIR -> "GOTO if ($a == ${reg2char(b)}) 2 else 1"
                Mnemonic.EQRI -> "GOTO if (${reg2char(a)} == $b) 2 else 1"
                Mnemonic.EQRR -> "GOTO if (${reg2char(a)} == ${reg2char(b)}) 2 else 1"
            }
        }
        println("$i: $humanReadale")
    }
}

fun reg2char(reg: Long): Char {
    return ('A'.code + reg).toInt().toChar()
}
