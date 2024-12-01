import java.io.File
import java.util.*

fun main() {
    val regex = "position=<\\s*([\\d-]+),\\s*([\\d-]+)> velocity=<\\s*([\\d-]+),\\s*([\\d-]+)>".toRegex()
    val pos: MutableList<Pair<Int, Int>> = mutableListOf()
    val vel: MutableList<Pair<Int, Int>> = mutableListOf()
    for (line in File("input.txt").readLines()) {
        val m = regex.matchEntire(line)
        if (m != null) {
            pos.add(Pair(m.groupValues[1].toInt(), m.groupValues[2].toInt()))
            vel.add(Pair(m.groupValues[3].toInt(), m.groupValues[4].toInt()))
        } else {
            println("Strange line: '$line'")
        }
    }

    val scanner = Scanner(System.`in`)
    var second = 0
    while (true) {
        if (printPoints(pos)) {
            println("----------> after $second seconds")
            readln()
        }

        for (i in 0..<pos.size) {
            pos[i] = Pair(pos[i].first + vel[i].first, pos[i].second + vel[i].second)
        }
        second++
    }
}


fun printPoints(pos: List<Pair<Int, Int>>): Boolean {
    val minx = pos.minBy { e -> e.first}.first
    val miny = pos.minBy { e -> e.second}.second
    val maxx = pos.maxBy { e -> e.first}.first
    val maxy = pos.maxBy { e -> e.second}.second

    if (maxx - minx > 250) {
        return false
    }
    println()
    for (y in miny..maxy) {
        for (x in minx..maxx) {
            if (pos.contains(Pair(x, y))) {
                print("#")
            } else {
                print(" ")
            }
        }
        println()
    }
    return true
}
