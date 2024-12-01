import java.io.File
import kotlin.math.abs
import kotlin.math.max
import kotlin.math.min

fun main() {
    val seeds = readSeeds("input.txt")
    val (field, width) = calcField(seeds)
    var area = biggestArea(field, width)
    println("Greatest area: $area")

    area = regionDistanceSumSmallerThan(seeds, 10000)
    println("Close region: $area")
}


fun printField(field: IntArray, width: Int) {
    for ((i, value) in field.withIndex()) {
        print(if (value >= 0) value else '.')
        if (i % width == width - 1) {
            println()
        }
    }
}


fun calcField(seeds: List<Pair<Int, Int>>): Pair<IntArray, Int> {
    var minx = Int.MAX_VALUE
    var miny = Int.MAX_VALUE
    var maxx = Int.MIN_VALUE
    var maxy = Int.MIN_VALUE
    for ((x, y) in seeds) {
        minx = min(minx, x)
        miny = min(miny, y)
        maxx = max(maxx, x)
        maxy = max(maxy, y)
    }
    val offsetx = minx - 1
    val offsety = miny - 1
    val width = maxx - minx + 3
    val height = maxy - miny + 3
    val field = IntArray(width * height)
    for (y in miny - 1..maxy + 1) {
        for (x in minx - 1..maxx + 1) {
            var shortestSeed = 0
            var shortestDist = Int.MAX_VALUE
            seeds.forEachIndexed { seed, coord ->
                val sx = coord.first
                val sy = coord.second
                val dist = abs(x - sx) + abs(y - sy)
                if (dist < shortestDist) {
                    shortestSeed = seed
                    shortestDist = dist
                } else if (dist == shortestDist) {
                    shortestSeed = -1
                }
            }
            field[(y - offsety) * width + (x - offsetx)] = shortestSeed
        }
    }
    return Pair(field, width)
}


fun biggestArea(field: IntArray, width: Int): Int {
    val height = field.size / width
    val ignore: MutableSet<Int> = mutableSetOf()
    for (x in 0..<width) {
        ignore.add(field[x])
        ignore.add(field[(height - 1) * width + x])
    }
    for (y in 0..<height) {
        ignore.add(field[y * width])
        ignore.add(field[(y + 1) * width - 1])
    }

    var maxArea = 0
    var maxSeed = 0
    var curSeed = 0
    while (true) {
        if (!ignore.contains(curSeed)) {
            var count = 0
            for (value in field) {
                if (value == curSeed) {
                    count++
                }
            }
            if (count > maxArea) {
                maxArea = count
                maxSeed = curSeed
            } else if (count == 0) {
                break
            }
        }
        curSeed++
    }
    return maxArea
}


fun regionDistanceSumSmallerThan(seeds: List<Pair<Int, Int>>, maxTotalDistance: Int): Int {
    var minx = Int.MAX_VALUE
    var miny = Int.MAX_VALUE
    var maxx = Int.MIN_VALUE
    var maxy = Int.MIN_VALUE
    for ((x, y) in seeds) {
        minx = min(minx, x)
        miny = min(miny, y)
        maxx = max(maxx, x)
        maxy = max(maxy, y)
    }

    var count = 0
    var y = 0
    while (y <= maxy) {
        var totalDist = 0
        var x = 0
        while (x <= maxx || totalDist < maxTotalDistance) {
            totalDist = 0
            for (seed in seeds) {
                totalDist += abs(x - seed.first) + abs(y - seed.second)
            }
            if (totalDist < maxTotalDistance) {
                count++
            }
            x++
        }
        y++
    }
    return count
}


fun readSeeds(filename: String): List<Pair<Int, Int>> {
    val seeds: MutableList<Pair<Int, Int>> = mutableListOf()
    for (line in File(filename).readLines()) {
        val tok = line.split(", ")
        seeds.add(Pair(tok[0].toInt(), tok[1].toInt()))
    }
    return seeds
}
