import java.io.File
import kotlin.math.absoluteValue
import kotlin.math.min
import kotlin.system.exitProcess


class Cave(val tx: Int, val ty: Int, val depth: Int) {
    private val cave: MutableList<MutableList<Int>> = mutableListOf(mutableListOf())
    private val minMinutes: MutableList<MutableList<IntArray>> = mutableListOf(mutableListOf())
    private var minMinutesToTarget = (tx + ty) * 8 + 7

    private fun growIfNecessary(x: Int, y: Int) {
        // Extend in x direction, calculate all values for new columns so we have a rectangle with all values calculated
        for (ix in cave[0].size..x) {
            for ((iy, row) in cave.withIndex()) {
                row.add(calcErosionLevel(ix, iy))
                val minMinutesVal = calcMinMinutes(ix, iy)
                minMinutes[iy].add(IntArray(3) { minMinutesVal })
            }
        }

        // Extend in y direction, calculate all values for new rows so we have a rectangle with all values calculated
        for (iy in cave.size..y) {
            cave.add(mutableListOf())
            minMinutes.add(mutableListOf())
            for (ix in cave[0].indices) {
                cave[iy].add(calcErosionLevel(ix, iy))
                val minMinutesVal = calcMinMinutes(ix, iy)
                minMinutes[iy].add(IntArray(3) { minMinutesVal })
            }
        }
    }

    fun set(x: Int, y: Int, erosionLevel: Int) {
        cave[y][x] = erosionLevel
    }

    fun get(x: Int, y: Int): Int {
        growIfNecessary(x, y)
        return cave[y][x]
    }

    fun calcErosionLevel(x: Int, y: Int): Int {
        return (when {
            x == tx && y == ty -> 0
            y == 0 -> x * 16807
            x == 0 -> y * 48271
            else -> cave[y - 1][x] * cave[y][x - 1]
        } + depth) % 20183
    }

    fun calcMinMinutes(x: Int, y: Int): Int {
        val dist = (x - tx).absoluteValue + (y - ty).absoluteValue
        return minMinutesToTarget - dist
    }

    fun getRiskValue(x: Int, y: Int): Int {
        return get(x, y) % 3
    }

    fun findPath(): Int {
        moveToNext(0, 0, 0, true, false)
        return minMinutesToTarget
    }

    private fun moveToNext(x: Int, y: Int, minutesSoFar: Int, torch: Boolean, gear: Boolean) {
        if (x < 0 || y < 0) {
            return
        }
        if (minutesSoFar >= minMinutesToTarget) {
            return
        }
        val riskLevel = getRiskValue(x, y)
        if (!isEquipmentValid(torch, gear, riskLevel)) {
            return
        }
        when {
            torch && gear -> exitProcess(1)
            torch -> if (minutesSoFar >= minMinutes[y][x][0]) return
            gear -> if (minutesSoFar >= minMinutes[y][x][1]) return
            else -> if (minutesSoFar >= minMinutes[y][x][2]) return
        }
        when {
            torch -> minMinutes[y][x][0] = minutesSoFar
            gear -> minMinutes[y][x][1] = minutesSoFar
            else -> minMinutes[y][x][2] = minutesSoFar
        }
        if (x == tx && y == ty) {
            minMinutesToTarget = min(minMinutesToTarget, minutesSoFar + if (torch) 0 else 7)
            return
        }

        goEveryDirection(x, y, minutesSoFar + 1, torch, gear)
        if (torch) {
            moveToNext(x, y, minutesSoFar + 7, false, false)
            moveToNext(x, y, minutesSoFar + 7, false, true)
        } else {
            moveToNext(x, y, minutesSoFar + 7, false, !gear)
            moveToNext(x, y, minutesSoFar + 7, true, false)
        }
    }

    private fun goEveryDirection(x: Int, y: Int, minutesSoFar: Int, torch: Boolean, gear: Boolean) {
        for ((dx, dy) in listOf(Pair(0, 1), Pair(-1, 0), Pair(1, 0), Pair(0, -1))) {
            moveToNext(x + dx, y + dy, minutesSoFar, torch, gear)
        }
    }

    private fun isEquipmentValid(torch: Boolean, gear: Boolean, riskLevel: Int): Boolean {
        return when (riskLevel) {
            0 -> torch || gear
            1 -> !torch
            2 -> !gear
            else -> exitProcess(1)
        }
    }

    fun print() {
        for (row in cave) {
            for (reg in row) {
                print(when(reg % 3) {
                    0 -> '.'
                    1 -> '='
                    2 -> '|'
                    else -> exitProcess(1)
                })
            }
            println()
        }
    }
}

fun main() {
    val content = File("input.txt").readLines()
    val depth = content[0].substringAfter("depth: ").toInt()
    val (tx, ty) = content[1].substringAfter("target: ").split(",").map { it.toInt() }

    val cave = Cave(tx, ty, depth)

    var totalRiskLevel = 0
    for (y in 0..ty) {
        for (x in 0..tx) {
            totalRiskLevel += cave.getRiskValue(x, y)
        }
    }

    for (i in 1..5) {
        cave.getRiskValue(tx + i, ty + i)
    }
    cave.print()

    println("Total risk level: $totalRiskLevel")
    println("Min minutes to reach target: ${cave.findPath()}")
}
