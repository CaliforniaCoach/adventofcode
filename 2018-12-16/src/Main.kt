import java.io.File
import kotlin.system.exitProcess

fun main() {
    val reBefore = "Before: \\[(\\d+), (\\d+), (\\d+), (\\d+)]".toRegex()
    val reInstruction = "(\\d+) (\\d+) (\\d+) (\\d+)".toRegex()
    val reAfter = "After: +\\[(\\d+), (\\d+), (\\d+), (\\d+)]".toRegex()
    val lines = File("input.txt").readLines()

    var i = 0
    var threeOrMore = 0
    val mnemonics = Array(16) { mutableSetOf(
        "addr",
        "addi",
        "mulr",
        "muli",
        "banr",
        "bani",
        "borr",
        "bori",
        "setr",
        "seti",
        "gtir",
        "gtri",
        "gtrr",
        "eqir",
        "eqri",
        "eqrr"
    )}
    val actualPrg: MutableList<IntArray> = mutableListOf()
    while (i < lines.size) {
        if (lines[i].isEmpty()) {
            i++
            continue
        }
        val mBefore = reBefore.matchEntire(lines[i])
        if (mBefore != null) {
            val regBefore = mBefore.groupValues.subList(1, 5).map { it.toInt() }
            i++
            val mInstruction = reInstruction.matchEntire(lines[i++])
            if (mInstruction == null) {
                println("Unexpected line after 'Before' line in line $i")
                exitProcess(1)
            } else {
                val instr = mInstruction.groupValues[1].toInt()
                val a = mInstruction.groupValues[2].toInt()
                val b = mInstruction.groupValues[3].toInt()
                val c = mInstruction.groupValues[4].toInt()
                if (a >= 4 || b >= 4 || c >= 4) {
                    println("Operand >= 4 found, I am not prepared for this")
                    exitProcess(2)
                }
                val mAfter = reAfter.matchEntire(lines[i++])
                if (mAfter == null) {
                    println("Unexpected line after 'Before' line in line $i")
                    exitProcess(3)
                }
                val regAfter = mAfter.groupValues.subList(1, 5).map { it.toInt() }

                val mnemonicsPossible: MutableMap<String, Boolean> = mutableMapOf()
                var onlyOutputModified = true
                for (k in 0..<4) {
                    onlyOutputModified = onlyOutputModified && (regBefore[k] == regAfter[k] || k == c)
                }
                mnemonicsPossible["addr"] = onlyOutputModified && regAfter[c] == regBefore[a] + regBefore[b]
                mnemonicsPossible["addi"] = onlyOutputModified && regAfter[c] == regBefore[a] + b
                mnemonicsPossible["mulr"] = onlyOutputModified && regAfter[c] == regBefore[a] * regBefore[b]
                mnemonicsPossible["muli"] = onlyOutputModified && regAfter[c] == regBefore[a] * b
                mnemonicsPossible["banr"] = onlyOutputModified && regAfter[c] == regBefore[a] and regBefore[b]
                mnemonicsPossible["bani"] = onlyOutputModified && regAfter[c] == regBefore[a] and b
                mnemonicsPossible["borr"] = onlyOutputModified && regAfter[c] == regBefore[a] or regBefore[b]
                mnemonicsPossible["bori"] = onlyOutputModified && regAfter[c] == regBefore[a] or b
                mnemonicsPossible["setr"] = onlyOutputModified && regAfter[c] == regBefore[a]
                mnemonicsPossible["seti"] = onlyOutputModified && regAfter[c] == a
                mnemonicsPossible["gtir"] = onlyOutputModified && regAfter[c] == if (a > regBefore[b]) 1 else 0
                mnemonicsPossible["gtri"] = onlyOutputModified && regAfter[c] == if (regBefore[a] > b) 1 else 0
                mnemonicsPossible["gtrr"] = onlyOutputModified && regAfter[c] == if (regBefore[a] > regBefore[b]) 1 else 0
                mnemonicsPossible["eqir"] = onlyOutputModified && regAfter[c] == if (a == regBefore[b]) 1 else 0
                mnemonicsPossible["eqri"] = onlyOutputModified && regAfter[c] == if (regBefore[a] == b) 1 else 0
                mnemonicsPossible["eqrr"] = onlyOutputModified && regAfter[c] == if (regBefore[a] == regBefore[b]) 1 else 0

                val numPossible = mnemonicsPossible.count { it.value }
                if (numPossible >= 3) {
                    threeOrMore++
                } else if (numPossible == 0) {
                    println("*** Warning: no possibilities for sequence ending with line $i")
                }
                mnemonicsPossible.forEach {
                    if (!it.value) {
                        mnemonics[instr].remove(it.key)
                    }
                }
            }
        } else {
            val mInstruction = reInstruction.matchEntire(lines[i++])
            if (mInstruction == null) {
                println("Unexpected line after 'Before' line in line $i")
                exitProcess(1)
            } else {
                actualPrg.add(mInstruction.groupValues.subList(1, 5).map { it.toInt() }.toIntArray())
            }
        }
    }
    println("Samples behaving like three or more opcodes: $threeOrMore")


    // Part 2

    // If an instruction number has exactly one unique mnemonic mapped to it, remove that mnemonic
    // from all other instruction number mappings
    var somethingChanged = true
    while (somethingChanged) {
        somethingChanged = false
        for (instructionNum in mnemonics.indices) {
            if (mnemonics[instructionNum].size == 1) {
                for (k in mnemonics.indices) {
                    if (k != instructionNum) {
                        somethingChanged = somethingChanged || mnemonics[k].remove(mnemonics[instructionNum].first())
                    }
                }
            }
        }
    }
    val mnemonic = mnemonics.map {
        if (it.size == 1) {
            it.first()
        } else {
            println("Mnemonic $it not unique")
            exitProcess(5)
        }
    }
    val reg = IntArray(4) { 0 }
    for (instr in actualPrg) {
        when (mnemonic[instr[0]]) {
            "addr" -> reg[instr[3]] = reg[instr[1]] + reg[instr[2]]
            "addi" -> reg[instr[3]] = reg[instr[1]] + instr[2]
            "mulr" -> reg[instr[3]] = reg[instr[1]] * reg[instr[2]]
            "muli" -> reg[instr[3]] = reg[instr[1]] * instr[2]
            "banr" -> reg[instr[3]] = reg[instr[1]] and reg[instr[2]]
            "bani" -> reg[instr[3]] = reg[instr[1]] and instr[2]
            "borr" -> reg[instr[3]] = reg[instr[1]] or reg[instr[2]]
            "bori" -> reg[instr[3]] = reg[instr[1]] or instr[2]
            "setr" -> reg[instr[3]] = reg[instr[1]]
            "seti" -> reg[instr[3]] = instr[1]
            "gtir" -> reg[instr[3]] = if (instr[1] > reg[instr[2]]) 1 else 0
            "gtri" -> reg[instr[3]] = if (reg[instr[1]] > instr[2]) 1 else 0
            "gtrr" -> reg[instr[3]] = if (reg[instr[1]] > reg[instr[2]]) 1 else 0
            "eqir" -> reg[instr[3]] = if (instr[1] == reg[instr[2]]) 1 else 0
            "eqri" -> reg[instr[3]] = if (reg[instr[1]] == instr[2]) 1 else 0
            "eqrr" -> reg[instr[3]] = if (reg[instr[1]] == reg[instr[2]]) 1 else 0
            else -> { println("Mnemonic for ${instr[0]} not known"); exitProcess(4) }
        }
    }
    println("Registers after running actual program: ${reg.toList()}")
}
