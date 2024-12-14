import java.io.File
import java.lang.Math.floorMod
import kotlin.math.min

const val FILE = "input.txt"
const val SIZEX = 101
const val SIZEY = 103

/*
const val FILE = "test.txt"
const val SIZEX = 11
const val SIZEY = 7
*/

fun main() {
    println("Safety factor after 100 seconds: ${solve(100).first}")
    var seconds = 0
    while (true) {
        val retval = solve(++seconds)
        if (retval.second || retval.first < 100000000) {
            printField(retval.third)
            println("--> after $seconds seconds")
            println("--------------------------------------------------------------------------------------")

            Thread.sleep(500)
        }
    }
}

fun solve(seconds: Int): Triple<Int, Boolean, Array<IntArray>> {
    val re = "p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)".toRegex()
    val robots = File(FILE).readLines().map {
        re.matchEntire(it)!!.groupValues.subList(1, 5).map {
            it2 -> it2.toInt()
        }.toMutableList()
    }

    val field = Array(SIZEX) { IntArray(SIZEY) { 0 } }
    val quadrant = Array(2) { IntArray(2) { 0 } }
    for (robot in robots) {
        val x = floorMod(robot[0] + robot[2] * seconds, SIZEX)
        val y = floorMod(robot[1] + robot[3] * seconds, SIZEY)
        field[x][y]++
        if (x != SIZEX / 2 && y != SIZEY / 2) {
            quadrant[min(x / (SIZEX / 2), 1)][min(y / (SIZEY / 2), 1)]++
        }
    }
    val safetyFactor = quadrant[0][0] * quadrant[0][1] * quadrant[1][0] * quadrant[1][1]
    return Triple(safetyFactor, quadrant[0][0] == quadrant[1][0] && quadrant[0][1] == quadrant[1][1], field)
}

fun printField(field: Array<IntArray>) {
    println()
    for (y in 0..<SIZEY) {
        for (x in 0..<SIZEX) {
            val value = field[x][y]
            print(when(value) {0 -> ' '; 1 -> 'x'; else -> '*'})
        }
        println()
    }
}
