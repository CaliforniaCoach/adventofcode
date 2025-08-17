import java.io.File
import kotlin.system.exitProcess
import kotlin.math.min
import kotlin.math.max


fun main() {
    val maze = Maze(File("input.txt").readText().trim())
    println("Farthest room is this many doors away: ${maze.farthestRoom}")
    println("This many rooms are 1000 or more doors away: ${maze.countRoomsMoreThanNDoorsAway(1000)}")
}

class Maze(regexp: String) {
    val doorsBottom: MutableSet<Pair<Int, Int>> = mutableSetOf()
    val doorsRight: MutableSet<Pair<Int, Int>> = mutableSetOf()

    init {
        processRegexp(0, 0, regexp.substring(1))
    }

    val minX = min(doorsBottom.minOf { it.first }, doorsRight.minOf { it.first })
    val maxX = max(doorsBottom.maxOf { it.first }, doorsRight.maxOf { it.first })
    val minY = min(doorsBottom.minOf { it.second }, doorsRight.minOf { it.second })
    val maxY = max(doorsBottom.maxOf { it.second }, doorsRight.maxOf { it.second })
    val distmap = Array(maxY - minY + 1) { IntArray(maxX - minX + 1) { Int.MAX_VALUE } }

    val farthestRoom = next(0, 0, 0, 0, 0)

    fun processRegexp(currentX: Int, currentY: Int, regexPart: String) {
        println("Processing $regexPart")
        val stack: MutableList<Pair<Int, Int>> = mutableListOf()
        var i = 0
        var x = currentX
        var y = currentY
        while (i < regexPart.length) {
            when (regexPart[i]) {
                '$' -> return
                '(' -> stack.add(Pair(x, y))
                '|' -> {x = stack.last().first; y = stack.last().second}
                ')' -> {x = stack.last().first; y = stack.last().second; stack.removeLast()}
                'N' -> doorsBottom.add(Pair(x, --y))
                'E' -> doorsRight.add(Pair(x++, y))
                'S' -> doorsBottom.add(Pair(x, y++))
                'W' -> doorsRight.add(Pair(--x, y))
                else -> exitProcess(1)
            }
            i++
        }
    }

    fun next(x: Int, y: Int, dx: Int, dy: Int, stepsSoFar: Int): Int {
        when {
            dx > 0 -> if (!doorsRight.contains(Pair(x, y))) return 0
            dx < 0 -> if (!doorsRight.contains(Pair(x - 1, y))) return 0
            dy > 0 -> if (!doorsBottom.contains(Pair(x, y))) return 0
            dy < 0 -> if (!doorsBottom.contains(Pair(x, y - 1))) return 0
        }
        val nextX = x + dx
        val nextY = y + dy
        if (stepsSoFar >= distmap[nextY - minY][nextX - minX]) {
            return 0
        }
        distmap[nextY - minY][nextX - minX] = stepsSoFar
        var farthestDistance = stepsSoFar
        for ((nextDX, nextDY) in listOf(Pair(1, 0), Pair(-1, 0), Pair(0, 1), Pair(0, -1))) {
            farthestDistance = max(farthestDistance, next(nextX, nextY, nextDX, nextDY, stepsSoFar + 1))
        }
        return farthestDistance
    }

    fun countRoomsMoreThanNDoorsAway(n: Int): Int {
        return distmap.sumOf { it.count { it2 -> it2 in n..<Int.MAX_VALUE } }
    }

    fun print() {
        val minX = min(doorsBottom.minOf { it.first }, doorsRight.minOf { it.first })
        val maxX = max(doorsBottom.maxOf { it.first }, doorsRight.maxOf { it.first })
        val minY = min(doorsBottom.minOf { it.second }, doorsRight.minOf { it.second })
        val maxY = max(doorsBottom.maxOf { it.second }, doorsRight.maxOf { it.second })
        for (i in minX..maxX) {
            print("##")
        }
        println('#')
        for (y in minY..maxY) {
            for (x in minX..maxX) {
                print(if (doorsRight.contains(Pair(x - 1, y))) '|' else '#')
                print(if (x != 0 || y != 0) '.' else 'X')
            }
            println('#')
            for (x in minX..maxX) {
                print('#')
                print(if (doorsBottom.contains(Pair(x, y))) '-' else '#')
            }
            println('#')
        }
    }
}
