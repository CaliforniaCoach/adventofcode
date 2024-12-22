import java.io.File
import kotlin.system.exitProcess

fun main() {
    val lines = File("input.txt").readLines()

    println("Sum of complexities with 2 intermediate DirPads: ${processAllLines(lines, 2)}")
    println("Sum of complexities with 3 intermediate DirPads: ${processAllLines(lines, 3)}")
    println("Sum of complexities with 25 intermediate DirPads: ${processAllLines(lines, 25)}")
}

fun processAllLines(lines: List<String>, numDirKeypads: Int): Long {
    var sum = 0L
    for (line in lines) {
        sum += processCascade(line, numDirKeypads)
    }
    return sum
}

fun processCascade(targetSequence: String, numDirKeypads: Int): Long {

    val paths = NumKeypad().findShortestPathCombinations(targetSequence)
    val dirKeypad = DirKeypad()

    val minLen = paths.minOf { dirKeypad.getLen(it, numDirKeypads - 1) }
    return minLen * targetSequence.substring(0, targetSequence.length - 1).toLong()
}

abstract class Keypad {
    abstract val shortestPaths: Map<Pair<Char, Char>, List<String>>

    fun findShortestPaths(start: Char, target: Char): List<String> {
        return if (start == target)
            listOf("")
        else shortestPaths[Pair(start, target)] ?: shortestPaths[Pair(target, start)]!!.map {
            it.map { it2 -> when(it2) {
                '^' -> 'v'
                '<' -> '>'
                '>' -> '<'
                else -> exitProcess(1)
            } }.reversed().joinToString("")
        }
    }

    fun findShortestPathCombinations(sequence: String): Set<String> {
        val sequenceWithStart = "A$sequence"
        var combis = setOf("")
        for (i in 0..<sequenceWithStart.length - 1) {
            val paths = findShortestPaths(sequenceWithStart[i], sequenceWithStart[i + 1])
            val newCombis: MutableSet<String> = mutableSetOf()
            for (oldCombi in combis) {
                for (path in paths) {
                    newCombis.add(oldCombi + path + "A")
                }
            }
            combis = newCombis
        }
        return combis
    }
}

class NumKeypad: Keypad() {
    override val shortestPaths = mapOf(
        Pair('A', '0') to listOf("<"),
        Pair('A', '1') to listOf("<^<", "^<<"),
        Pair('A', '2') to listOf("<^", "^<"),
        Pair('A', '3') to listOf("^"),
        Pair('A', '4') to listOf("<^<^", "<^^<", "^<^<", "^^<<"),
        Pair('A', '5') to listOf("<^^", "^<^", "^^<"),
        Pair('A', '6') to listOf("^^"),
        Pair('A', '7') to listOf("<^<^^", "<^^<^", "^<^<^", "^^<<^", "^^<^<", "^^^<<"),
        Pair('A', '8') to listOf("<^^^", "^<^^", "^^<^", "^^^<"),
        Pair('A', '9') to listOf("^^^"),
        Pair('0', '1') to listOf("^<"),
        Pair('0', '2') to listOf("^"),
        Pair('0', '3') to listOf(">^", "^>"),
        Pair('0', '4') to listOf("^<^", "^^<"),
        Pair('0', '5') to listOf("^^"),
        Pair('0', '6') to listOf("^>^", "^^>", ">^^"),
        Pair('0', '7') to listOf("^<^^", "^^<^", "^^^<"),
        Pair('0', '8') to listOf("^^^"),
        Pair('0', '9') to listOf(">^^^", "^>^^", "^^>^", "^^^>"),
        Pair('1', '2') to listOf(">"),
        Pair('1', '3') to listOf(">>"),
        Pair('1', '4') to listOf("^"),
        Pair('1', '5') to listOf("^>", ">^"),
        Pair('1', '6') to listOf("^>>", ">^>", "^>>"),
        Pair('1', '7') to listOf("^^"),
        Pair('1', '8') to listOf("^^>", "^>^", ">>^"),
        Pair('1', '9') to listOf("^^>>", "^>^>", "^>>^", ">>^^"),
        Pair('2', '3') to listOf(">"),
        Pair('2', '4') to listOf("^<", "<^"),
        Pair('2', '5') to listOf("^"),
        Pair('2', '6') to listOf("^>", ">^"),
        Pair('2', '7') to listOf("^^<", "^<^", "<^^"),
        Pair('2', '8') to listOf("^^"),
        Pair('2', '9') to listOf("^^>", "^>^", ">^^"),
        Pair('3', '4') to listOf("^<<", "<^<", "<<^"),
        Pair('3', '5') to listOf("^<", "<^"),
        Pair('3', '6') to listOf("^"),
        Pair('3', '7') to listOf("<<^^", "<^<^", "<^^<", "^<^<", "^<<^", "^^<<"),
        Pair('3', '8') to listOf("<^^", "^<^", "^^<"),
        Pair('3', '9') to listOf("^^"),
        Pair('4', '5') to listOf(">"),
        Pair('4', '6') to listOf(">>"),
        Pair('4', '7') to listOf("^"),
        Pair('4', '8') to listOf("^>", ">^"),
        Pair('4', '9') to listOf(">>^", ">^>", "^>>"),
        Pair('5', '6') to listOf(">"),
        Pair('5', '7') to listOf("<^", "^<"),
        Pair('5', '8') to listOf("^"),
        Pair('5', '9') to listOf(">^", "^>"),
        Pair('6', '7') to listOf("<<^", "<^<", "^<<"),
        Pair('6', '8') to listOf("<^", "^<"),
        Pair('6', '9') to listOf("^"),
        Pair('7', '8') to listOf(">"),
        Pair('7', '9') to listOf(">>"),
        Pair('8', '9') to listOf(">"),
    )
}

class DirKeypad: Keypad() {
    override val shortestPaths = mapOf(
        Pair('<', 'v') to listOf(">"),
        Pair('<', '>') to listOf(">>"),
        Pair('<', '^') to listOf(">^"),
        Pair('<', 'A') to listOf(">>^", ">^>"),
        Pair('v', '>') to listOf(">"),
        Pair('v', '^') to listOf("^"),
        Pair('v', 'A') to listOf(">^", "^>"),
        Pair('>', '^') to listOf("^<", "<^"),
        Pair('>', 'A') to listOf("^"),
        Pair('^', 'A') to listOf(">"),
    )

    private val lenMap: MutableMap<Pair<String, Int>, Long> = mutableMapOf()

    fun getLen(path: String, depth: Int): Long {
        if (lenMap.contains(Pair(path, depth))) {
            return lenMap[Pair(path, depth)]!!
        }
        var minLen = 0L
        val pathStartingFromA = "A$path"
        for (i in 1..<pathStartingFromA.length) {
            val paths = findShortestPaths(pathStartingFromA[i - 1], pathStartingFromA[i])
            if (depth > 0) {
                minLen += paths.minOf { getLen(it + 'A', depth - 1) }
            } else {
                minLen += paths.minOf { it.length } + 1
            }
        }
        lenMap[Pair(path, depth)] = minLen
        return minLen
    }

    @Suppress("unused", "MemberVisibilityCanBePrivate")
    fun getLenWithPath(path: String, depth: Int): Pair<Long, String> {
        var minLen = 0L
        val newPath = StringBuilder()
        val pathStartingFromA = "A$path"
        for (i in 1..<pathStartingFromA.length) {
            val paths = findShortestPaths(pathStartingFromA[i - 1], pathStartingFromA[i])
            if (depth > 0) {
                minLen += paths.minOf { getLenWithPath(it + 'A', depth - 1).first }
                val test = paths.minBy { getLenWithPath(it + 'A', depth - 1).first }
                newPath.append(getLenWithPath(test + 'A', depth - 1).second)
            } else {
                minLen += paths.minOf { it.length } + 1
                val test = paths.minBy { it.length }
                newPath.append(test).append('A')
            }
        }
        lenMap[Pair(path, depth)] = minLen
        return Pair(minLen, newPath.toString())
    }
}
