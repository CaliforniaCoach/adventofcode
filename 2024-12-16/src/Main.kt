import java.io.File

fun main() {
    val maze = File("input.txt").readLines().map { it.toCharArray() }
    val pathFinder = PathFinder(maze)
    pathFinder.print()
    println("Lowest score: ${pathFinder.lowestScore}")
    println("Tiles on at least one of best paths: ${pathFinder.onBestPath.size}")
}

val DIR = arrayOf(Pair(1, 0), Pair(0, 1), Pair(-1, 0), Pair(0, -1))

class PathFinder(val maze: List<CharArray>) {
    val minPoints = Array(maze.size) { Array(maze[0].size) { IntArray(4) { Int.MAX_VALUE } } }
    val onBestPath: MutableSet<Pair<Int, Int>> = mutableSetOf()

    val startY = maze.indexOfFirst { it.contains('S') }
    val startX = maze[startY].indexOfFirst { it == 'S' }
    val endY = maze.indexOfFirst { it.contains('E') }
    val endX = maze[endY].indexOfFirst { it == 'E' }

    var lowestScore = Int.MAX_VALUE

    init {
        next(startX, startY, 0, 0)
    }

    fun next(x: Int, y: Int, dir: Int, scoreSoFar: Int): Boolean {
        if (maze[y][x] == 'E') {
            if (scoreSoFar < lowestScore) {
                onBestPath.clear()
                onBestPath.add(Pair(x, y))
                lowestScore = scoreSoFar
                return true
            } else {
                return false
            }
        }
        if (scoreSoFar >= lowestScore || scoreSoFar > minPoints[y][x][dir] || maze[y][x] == '#')
            return false
        if (scoreSoFar == minPoints[y][x][dir])
            return onBestPath.contains(Pair(x, y))
        minPoints[y][x][dir] = scoreSoFar
        var isOnBestPath = false
        isOnBestPath = next(x, y, (dir + 4 + 1) % 4, scoreSoFar + 1000) || isOnBestPath
        isOnBestPath = next(x, y, (dir + 4 - 1) % 4, scoreSoFar + 1000) || isOnBestPath
        isOnBestPath = next(x + DIR[dir].first, y + DIR[dir].second, dir, scoreSoFar + 1) || isOnBestPath
        if (isOnBestPath)
            onBestPath.add(Pair(x, y))
        return isOnBestPath
    }

    fun print() {
        for (y in maze.indices) {
            for (x in maze[y].indices) {
                print(if (onBestPath.contains(Pair(x, y))) 'O' else maze[y][x])
            }
            println()
        }
    }
}
