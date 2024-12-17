import java.io.File
import kotlin.system.exitProcess

fun main() {
    val content = File("input.txt").readLines()
    val reg = content.subList(0, 3).map { it.substringAfter(": ").toLong() }.toLongArray()
    val prg = content[4].substringAfter(": ").split(",").map { it.toLong() }.toLongArray()

    println("Program output: ${run(prg, reg).joinToString(",")}")

    // Part 2
    solveForNextDigit(prg, 0L)
}

fun run(prg: LongArray, reg: LongArray): List<Long> {
    var pc = 0
    val out: MutableList<Long> = mutableListOf()
    while (pc < prg.size) {
        val lit = prg[pc + 1]
        val cmb = if (lit in 0..3) lit else if (lit in 4..6) reg[(lit - 4).toInt()] else exitProcess(1)
        when (prg[pc]) {
            0L -> reg[0] = reg[0] ushr cmb.toInt()
            1L -> reg[1] = reg[1] xor lit
            2L -> reg[1] = cmb and 7
            3L -> if (reg[0] != 0L) pc = (lit - 2).toInt()
            4L -> reg[1] = reg[1] xor reg[2]
            5L -> out.add(cmb and 7L)
            6L -> reg[1] = reg[0] ushr cmb.toInt()
            7L -> reg[2] = reg[0] ushr cmb.toInt()
        }
        pc += 2
    }
    return out
}

fun solveForNextDigit(prg: LongArray, aStart: Long): Boolean {
    val a = aStart shl 3
    for (aTest in a..a + 7) {
        val output = run(prg, longArrayOf(aTest, 0, 0))
        if (output.first() == prg[prg.size - output.size]) {
            if (output.size == prg.size) {
                println("Solution found which produces an output which is copy of program itself: A = $aTest")
                return true
            }
            if (solveForNextDigit(prg, aTest)) {
                return true
            }
        }
    }
    return false
}
