import java.io.File
import kotlin.system.exitProcess

fun main() {
    val keys: MutableList<IntArray> = mutableListOf()
    val locks: MutableList<IntArray> = mutableListOf()
    val lines = File("input.txt").readLines()
    var i = 0
    while (i < lines.size) {
        val retval = getPinHeights(lines, '#', i)
        if (lines[i].all { it == '#' }) {
            locks.add(retval.first)
        } else if (lines[i].all { it == '.' }) {
            keys.add(retval.first)
        } else {
            exitProcess(1)
        }
        i = retval.second + 1
    }

    var matches = 0
    for (key in keys) {
        for (lock in locks) {
            if (key.zip(lock).all { it.first + it.second <= 5 }) {
                matches++
            }
        }
    }

    println("Matching key/lock pairs: $matches")
}

fun getPinHeights(lines: List<String>, pinChar: Char, startLine: Int): Pair<IntArray, Int> {
    var i = startLine
    val columns = IntArray(lines[i].length) { -1 }
    while (i < lines.size && lines[i].isNotEmpty()) {
        for (k in lines[i].indices) {
            if (lines[i][k] == pinChar) {
                columns[k]++
            }
        }
        i++
    }
    return Pair(columns, i)
}