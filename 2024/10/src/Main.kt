import java.io.File

fun main() {
    val mountain = File("input.txt").readLines().map { it.toCharArray().map { it2 -> it2.code - '0'.code } }
    var scoreSum = 0
    var ratingSum = 0
    for (y in mountain.indices) {
        for (x in mountain[y].indices) {
            if (mountain[y][x] == 0) {
                val pathFinder = PathFinder(mountain, x, y)
                scoreSum += pathFinder.summits.size
                ratingSum += pathFinder.paths
            }
        }
    }
    println("Sum of scores of all trailheads: $scoreSum")
    println("Sum of ratings of all trailheads: $ratingSum")
}

class PathFinder(private val mountain: List<List<Int>>, startX: Int, startY: Int) {
    val summits: MutableSet<Pair<Int, Int>> = mutableSetOf()
    var paths = 0

    init {
        next(startX, startY, 0)
    }

    fun next(x: Int, y: Int, altitude: Int) {
        if (y !in mountain.indices || x !in mountain[y].indices) {
            return
        }
        if (mountain[y][x] != altitude) {
            return
        }
        if (altitude == 9) {
            summits.add(Pair(x, y))
            paths++
            return
        }
        for (d in listOf(Pair(1, 0), Pair(-1, 0), Pair(0, 1), Pair(0, -1))) {
            val (nextX, nextY) = Pair(x + d.first, y + d.second)
            next(nextX, nextY, altitude + 1)
        }
    }
}
