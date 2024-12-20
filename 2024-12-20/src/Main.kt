import java.io.File
import kotlin.math.absoluteValue

fun main() {
    val maze = File("input.txt").readLines()
    val pathFinder = PathFinder(maze)
    val cheatPositions = pathFinder.getCheatPositions()
    println("Cheats max 2 picoseconds saving at least 100 picoseconds: ${cheatPositions.count { it.value >= 100 }}")
    val cheatPositions2 = pathFinder.getCheatsRule2()
    println("Cheats max 20 picoseconds saving at least 100 picoseconds: ${cheatPositions2.count { it.value >= 100 }}")
}

class PathFinder(val maze: List<String>) {
    val startY = maze.indexOfFirst { it.contains('S') }
    val startX = maze[startY].indexOf('S')
    val endY = maze.indexOfFirst { it.contains('E') }
    val endX = maze[startY].indexOf('E')

    val distmap = Array(maze.size) { IntArray(maze.first().length) { Int.MAX_VALUE } }

    init {
        next(startX, startY, 0)
    }

    fun next(x: Int, y: Int, stepsSoFar: Int) {
        if ((x == endX && y == endY) || maze[y][x] == '#' || stepsSoFar >= distmap[y][x]) {
            return
        }
        distmap[y][x] = stepsSoFar
        for ((dx, dy) in listOf(Pair(1, 0), Pair(-1, 0), Pair(0, 1), Pair(0, -1))) {
            next(x + dx, y + dy, stepsSoFar + 1)
        }
    }

    fun getCheatPositions(): Map<Pair<Int, Int>, Int> {
        val cheatPositions: MutableMap<Pair<Int, Int>, Int> = mutableMapOf()
        for (y in 1..<maze.size - 1) {
            for (x in 1..<maze[y].length - 1) {
                if (maze[y][x] == '#') {
                    if (maze[y + 1][x] != '#' && maze[y - 1][x] != '#') {
                        cheatPositions[Pair(x, y)] = (distmap[y + 1][x] - distmap[y - 1][x]).absoluteValue - 2
                    }
                    if (maze[y][x + 1] != '#' && maze[y][x - 1] != '#') {
                        cheatPositions[Pair(x, y)] = (distmap[y][x + 1] - distmap[y][x - 1]).absoluteValue - 2
                    }
                }
            }
        }
        return cheatPositions
    }

    fun getCheatsRule2(): Map<Pair<Pair<Int, Int>, Pair<Int, Int>>, Int> {
        val cheatPositions: MutableMap<Pair<Pair<Int, Int>, Pair<Int, Int>>, Int> = mutableMapOf()
        for (y1 in 1..<maze.size - 1) {
            for (x1 in 1..<maze[y1].length - 1) {
                if (maze[y1][x1] != '#') {
                    for (cheatDist in 2..20) {
                        for (dy in 0..cheatDist) {
                            val y2 = y1 + dy
                            if (y2 >= maze.size) {
                                continue
                            }
                            for (dx in if (dy == 0) listOf(cheatDist) else listOf(-(cheatDist - dy), cheatDist - dy)) {
                                val x2 = x1 + dx
                                if (x2 < 0 || x2 >= maze[y2].length) {
                                    continue
                                }
                                if (maze[y2][x2] != '#') {
                                    cheatPositions[Pair(Pair(x1, y1), Pair(x2, y2))] = (distmap[y1][x1] - distmap[y2][x2]).absoluteValue - cheatDist
                                }
                            }
                        }
                    }
                }
            }
        }
        return cheatPositions
    }
}
