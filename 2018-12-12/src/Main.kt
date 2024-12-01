import java.io.File
import kotlin.math.floor
import kotlin.system.exitProcess
import kotlin.math.min

fun main() {
    solve(20)
    solve(50000000000)
}


fun solve(numSteps: Long) {
    val title = "initial state: ([.#]+)".toRegex()
    val entry = "([.#]+) => ([.#])".toRegex()

    val lines = File("input.txt").readLines()
    val titleMatch = title.matchEntire(lines[0])

    var pots = patternToLongList(titleMatch!!.groupValues[1])
    var offs = 0

    val map = IntArray(32)
    for (line in lines.subList(2, lines.size)) {
        val m = entry.matchEntire(line)
        if (m!!.groupValues[2] == "#") {
            map[patternToULong(m.groupValues[1]).toInt()] = 1
        }
    }

    var lastSum = 0
    var lastDiff = 0
    var numSameDiffs = 0
    for (step in 0..<numSteps) {
        val sum = calcSum(pots, offs)
        val diff = sum - lastSum
        if (diff == lastDiff) {
            numSameDiffs++
            if (numSameDiffs > 1000) {
                println("After interpolation: Sum is: ${(numSteps - step) * diff + sum}")
                return
            }
        } else {
            lastDiff = diff
            numSameDiffs = 0
        }
        lastSum = sum

        val newPots = MutableList(pots.size) { 0uL }
        var newOffs = offs

        for (i in pots.indices) {
            for (bit in 0..<64) {
                newPots[i] = newPots[i] or (map[extractBits(pots, offs, (i + offs) * 64 + bit).toInt()].toULong() shl bit)
            }
        }

        val twoToLeft = map[extractBits(pots, offs, offs * 64 - 2).toInt()]
        val oneToLeft = map[extractBits(pots, offs, offs * 64 - 1).toInt()]
        val potsLeft = (twoToLeft.toULong() shl 62) or (oneToLeft.toULong() shl 63)
        if (potsLeft != 0uL) {
            newOffs -= 1
            newPots.add(0, potsLeft)
        }
        val twoToRight = map[extractBits(pots, offs, (offs + pots.size) * 64 + 1).toInt()]
        val oneToRight = map[extractBits(pots, offs, (offs + pots.size) * 64 + 0).toInt()]
        val potsRight = (twoToRight.toULong() shl 1) or oneToRight.toULong()
        if (potsRight != 0uL) {
            newPots.add(potsRight)
        }

        pots = newPots
        offs = newOffs

    }
    println("Sum is: ${calcSum(pots, offs)}")
}


fun patternToLongList(pattern: String): List<ULong> {
    val longList: MutableList<ULong> = mutableListOf()
    for (i in 0..(pattern.length - 1) / 64) {
        longList.add(patternToULong(pattern.substring(i * 64, min((i + 1) * 64, pattern.length))))
    }
    return longList
}


fun patternToULong(pattern: String): ULong {
    var num = 0uL
    if (pattern.length > 64) {
        println("Pattern '$pattern' too long")
        exitProcess(1)
    }
    for (i in pattern.indices) {
        if (pattern[i] == '#') {
            num = num or (1uL shl i)
        }
    }
    return num
}


fun extractBits(pots: List<ULong>, offs: Int, middlePot: Int): ULong {
    val leftmostPot = middlePot - 2
    val rightmostPot = middlePot + 2
    var bits: ULong

    val i = floor(leftmostPot.toDouble() / 64 - offs).toInt()
    bits = if (i >= 0) {
        (pots[i] shr (leftmostPot % 64)) and 31uL
    } else {
        0uL
    }

    val k = floor(rightmostPot.toDouble() / 64 - offs).toInt()
    if (k != i && k < pots.size) {
        bits = (bits or (pots[k] shl (5 - rightmostPot % 64 - 1))) and 31uL
    }

    return bits
}


@Suppress("unused")
fun printPots(pots: List<ULong>, offs: Int) {
    for (row in pots) {
        for (bit in 0..<64) {
            if (row and (1uL shl bit) != 0uL) {
                print('#')
            } else {
                print('.')
            }
        }
    }
    println()
    for (i in 0..<-offs) {
        for (k in 0..<64) {
            print(' ')
        }
    }
    println('^')
}


fun calcSum(pots: List<ULong>, offs: Int): Int {
    var sum = 0
    for (i in pots.indices) {
        for (bit in 0..<64) {
            if (pots[i] and (1uL shl bit) != 0uL) {
                sum += (i + offs) * 64 + bit
            }
        }
    }
    return sum
}
