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

    println("Registers after running with reg[0] = 0: ${run(prg, ipBoundReg, 0).toList()}")
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
