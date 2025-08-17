import java.io.File
import kotlin.math.absoluteValue
import kotlin.math.max
import kotlin.math.min

data class OverlapRange(val minX: Int, val maxX: Int, val minY: Int, val maxY: Int, val minZ: Int, val maxZ: Int,
                        val overlaps: Int)

fun calcOverlap1dimension(bot1coord: Int, bot1range: Int, bot2coord: Int, bot2range: Int): Pair<Int, Int>? {
    val min1 = bot1coord - bot1range
    val max1 = bot1coord + bot1range
    val min2 = bot2coord - bot2range
    val max2 = bot2coord + bot2range

    return if (max1 < min2 || max2 < min1) null else Pair(min(min1, min2), max(max1, max2))
}

fun calcOverlap(bot1: List<Int>, bot2: List<Int>): OverlapRange? {
    val rangeX1 = bot1[3] - (bot1[1] - bot2[1]).absoluteValue - (bot1[2] - bot2[2]).absoluteValue
    val rangeY1 = bot1[3] - (bot1[0] - bot2[0]).absoluteValue - (bot1[2] - bot2[2]).absoluteValue
    val rangeZ1 = bot1[3] - (bot1[0] - bot2[0]).absoluteValue - (bot1[1] - bot2[1]).absoluteValue

    val rangeX2 = bot2[3] - (bot1[1] - bot2[1]).absoluteValue - (bot1[2] - bot2[2]).absoluteValue
    val rangeY2 = bot2[3] - (bot1[0] - bot2[0]).absoluteValue - (bot1[2] - bot2[2]).absoluteValue
    val rangeZ2 = bot2[3] - (bot1[0] - bot2[0]).absoluteValue - (bot1[1] - bot2[1]).absoluteValue

    if (rangeX1 <= 0 || rangeX2 <= 0 || rangeY1 <= 0 ||rangeY2 <= 0 ||rangeZ1 <= 0 || rangeZ2 <= 0) {
        return null
    }

    val overlapX = calcOverlap1dimension(bot1[0], rangeX1, bot2[0], rangeX2)
    val overlapY = calcOverlap1dimension(bot1[1], rangeY1, bot2[1], rangeX2)
    val overlapZ = calcOverlap1dimension(bot1[2], rangeZ1, bot2[2], rangeX2)

    return OverlapRange(overlapX!!.first, overlapX.second,
        overlapY!!.first, overlapY.second, overlapZ!!.first, overlapZ.second, 2)
}


fun main() {
    // Part 1
    val bots = File("input.txt").readLines()
        .map { it.split("pos=<|,|>, r=".toRegex()).filter { it2 -> !it2.isEmpty() }.map { it2 -> it2.toInt() } }

    val strongestBot = bots.maxBy { it[3] }
    val count = botsInRange(bots, strongestBot)

    println("Bots in range of strongest bot: $count")

    // Part 2
    val test1 = calcOverlap(listOf(10, 0, 0, 5), listOf(16, 0, 0, 3))
    val test2 = calcOverlap(listOf(10, 0, 0, 5), listOf(16, 4, 0, 3))
    val test3 = calcOverlap(listOf(10, 0, 0, 5), listOf(16, 2, 0, 3))

    return

    val botsInRangeOfOtherBots = bots.map { it to botsInRange(bots, it) }

    val maxBotsInRange = botsInRangeOfOtherBots.maxBy { it.second }
    println("Most bots in range of one bot: ${maxBotsInRange.second}")

    val bot = maxBotsInRange.first
    val highestReception = nextBotsInRange(bots, bot[0], bot[1], bot[2], 0, mutableListOf())
    println("Highest reception: $highestReception")
}


fun botsInRange(bots: List<List<Int>>, bot: List<Int>): Int {
    return bots.count {
        (it[0] - bot[0]).absoluteValue +
                (it[1] - bot[1]).absoluteValue +
                (it[2] - bot[2]).absoluteValue <= bot[3]
    }
}


fun nextBotsInRange(bots: List<List<Int>>, x: Int, y: Int, z: Int, lastBotsInRange: Int, visited: MutableList<List<Int>>): Int {
    if (listOf(x, y, z) in visited) {
        return lastBotsInRange
    }
    visited.add(listOf(x, y, z))
    var maxBotsInRange = lastBotsInRange
    for (delta in listOf(Triple(-1, 0, 0), Triple(1, 0, 0), Triple(0, -1, 0),
                          Triple(0, 1, 0), Triple(0, 0, -1), Triple(0, 0, 1))) {
        val nextX = x + delta.first
        val nextY = y + delta.second
        val nextZ = z + delta.third
        val botsInRange = bots.count {
            (it[0] - x).absoluteValue + (it[1] - y).absoluteValue + (it[2] - z).absoluteValue <= it[3]
        }
        if (botsInRange >= maxBotsInRange) {
            maxBotsInRange = max(maxBotsInRange, nextBotsInRange(bots, nextX, nextY, nextZ, botsInRange, visited))
        }
    }
    return maxBotsInRange
}
