import java.io.File
import kotlin.math.max
import kotlin.math.min
import kotlin.system.exitProcess

fun main() {
    val cave = Cave()
    for (line in File("input.txt").readLines()) {
        cave.parse(line)
    }
    cave.flowDown(500)
    println("Water reaches tiles: ${cave.countTiles('~', '|')}")
    println("Water at rest tiles: ${cave.countTiles('~')}")
}

class Cave {
    val cave: MutableList<CharArray> = mutableListOf()
    var minX = 1000
    var maxX = 0
    var minY = Int.MAX_VALUE
    var maxY = 0

    fun set(x: Int, y: Int, chr: Char) {
        while (y >= cave.size) {
            cave.add(CharArray(1000) { '.' })
        }
        cave[y][x] = chr
        minX = min(minX, x)
        maxX = max(maxX, x)
        minY = min(minY, y)
        maxY = max(maxY, y)
    }

    fun get(x: Int, y: Int): Char {
        return cave[y][x]
    }

    fun parse(line: String) {
        val tok = line.split("[, ]+".toRegex()).map { it.split('=') }
        val tok2 = tok[1][1].split("..")
        for (i in tok2[0].toInt()..tok2[1].toInt()) {
            if (tok[0][0] == "x") {
                set(tok[0][1].toInt(), i, '#')
            } else {
                set(i, tok[0][1].toInt(), '#')
            }
        }
    }

    fun flowDown(x: Int) {
        flowDown(x, minY)
    }

    private fun flowDown(x: Int, y: Int): Boolean {
        set(x, y, '|')
        if (y + 1 > maxY) {
            return true
        }
        if (cave[y + 1][x] == '.') {
            if (!flowDown(x, y + 1)) {
                return flowHorizontal(x, y)
            }
            return true
        } else if (cave[y + 1][x] == '|') {
            return true
        } else {
            return flowHorizontal(x, y)
        }
    }

    private fun flowHorizontal(x: Int, y: Int): Boolean {
        var lx = x - 1
        var flowLeft = false
        var flowRight = false
        while (cave[y][lx] == '.') {
            cave[y][lx] = '|'
            if (cave[y + 1][lx] == '.') {
                flowLeft = flowDown(lx, y + 1)
                if (flowLeft) {
                    break
                }
            }
            if (lx < 300) {
                print()
                exitProcess(1)
            }
            lx--
        }
        var rx = x + 1
        while (cave[y][rx] == '.') {
            cave[y][rx] = '|'
            if (cave[y + 1][rx] == '.') {
                flowRight = flowDown(rx, y + 1)
                if (flowRight) {
                    break
                }
            }
            rx++
        }
        if (!flowLeft && !flowRight) {
            for (xx in lx + 1..rx - 1) {
                cave[y][xx] = '~'
            }
            return false
        } else {
            return true
        }
    }

    fun countTiles(vararg tilesToCount: Char): Int {
        var count = 0
        for (row in cave) {
            for (chr in row) {
                if (tilesToCount.contains(chr)) {
                    count++
                }
            }
        }
        return count
    }

    fun print() {
        for (y in 0..maxY) {
            for (x in minX..maxX) {
                print(cave[y][x])
            }
            println()
        }
    }
}
