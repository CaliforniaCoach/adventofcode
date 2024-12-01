import java.io.File

fun main() {
    var result = Triple(' ', 0, mapOf<Char, Int>())
    var attack = 3
    while (result.first != 'E' || result.third['E'] != 0) {
        result = solveFor(attack)
        println("Elve attack power $attack -> ${result.first}s win, ${result.third['E']} Es died, ${result.third['G']} Gs died, outcome: ${result.second}")
        attack++
    }
}

fun solveFor(elveAttackPower: Int): Triple<Char, Int, Map<Char, Int>> {
    val cave = File("input.txt").readLines().map { it.toCharArray()}
    val hp = Array(cave.size) { IntArray(cave[0].size) {200} }
    var rounds = 0
    val deaths = mutableMapOf('E' to 0, 'G' to 0)

    mainLoop@while (true) {
        val movedAlready = Array(cave.size) { BooleanArray(cave[0].size) {false} }
        for (y in cave.indices) {
            for (x in cave[y].indices) {
                if ((cave[y][x] == 'E' || cave[y][x] == 'G') && !movedAlready[y][x]) {
                    val spf = ShortestPathFinder(cave, x, y)
                    if (spf.enemiesExist) {
                        val nx = x + spf.firstStepOnShortestPath.first
                        val ny = y + spf.firstStepOnShortestPath.second
                        cave[ny][nx] = cave[y][x].also { cave[y][x] = cave[ny][nx] }
                        hp[ny][nx] = hp[y][x]
                        movedAlready[ny][nx] = true
                        var minHp = Int.MAX_VALUE
                        var minPos: Pair<Int, Int>? = null
                        for ((tx, ty) in listOf(Pair(nx, ny - 1), Pair(nx - 1, ny), Pair(nx + 1, ny), Pair(nx, ny + 1))) {
                            if (cave[ty][tx] == spf.enemy) {
                                if (hp[ty][tx] < minHp) {
                                    minHp = hp[ty][tx]
                                    minPos = Pair(tx, ty)
                                }
                            }
                        }
                        if (minPos != null) {
                            val (tx, ty) = minPos
                            hp[ty][tx] -= if (cave[ny][nx] == 'E') elveAttackPower else 3
                            if (hp[ty][tx] <= 0) {
                                deaths[cave[ty][tx]] = deaths[cave[ty][tx]]!! + 1
                                cave[ty][tx] = '.'
                            }
                        }
                    } else {
                        break@mainLoop
                    }
                }
            }
        }
        rounds++
    }
    var outcome = 0
    var winner = ' '
    for (y in cave.indices) {
        for (x in cave[y].indices) {
            if (cave[y][x] == 'E' || cave[y][x] == 'G') {
                outcome += hp[y][x]
                winner = cave[y][x]
            }
        }
    }
    outcome *= rounds
    return Triple(winner, outcome, deaths)
}


class ShortestPathFinder(private val cave: List<CharArray>, x: Int, y: Int) {

    var firstStepOnShortestPath = Pair(0, 0)
    val enemy = if (cave[y][x] == 'E') 'G' else 'E'
    var enemiesExist = doEnemiesExist()

    private var minDistToClosestEnemy = Int.MAX_VALUE
    private val distmap = Array(cave.size) { IntArray(cave[0].size) { Int.MAX_VALUE } }
    private var currentFirstStep = Pair(0, 0)

    init {
        if (enemiesExist) {
            getClosestTargetRangeLocation(x, y)
        }
    }

    private fun doEnemiesExist(): Boolean {
        for (row in cave) {
            for (cell in row) {
                if (cell == enemy) {
                    return true
                }
            }
        }
        return false
    }

    private fun getClosestTargetRangeLocation(x: Int, y: Int) {
        move(x, y, 0, false, Pair(0, 0))
    }

    private fun move(x: Int, y: Int, distSoFar: Int, check: Boolean, lastStep: Pair<Int, Int>) {
        if (distSoFar == 1) {
            currentFirstStep = lastStep
        }
        if (check && (cave[y][x] != '.' || distmap[y][x] <= distSoFar || distSoFar >= minDistToClosestEnemy)) {
            return
        }
        distmap[y][x] = distSoFar
        if (cave[y][x + 1] == enemy || cave[y][x - 1] == enemy || cave[y + 1][x] == enemy || cave[y - 1][x] == enemy) {
            minDistToClosestEnemy = distSoFar
            firstStepOnShortestPath = currentFirstStep
            return
        }
        // Order matters here! So we get the first target spot in *reading order* in case of equally short distances
        move(x, y - 1, distSoFar + 1, true, Pair(0, -1))
        move(x - 1, y, distSoFar + 1, true, Pair(-1, 0))
        move(x + 1, y, distSoFar + 1, true, Pair(1, 0))
        move(x, y + 1, distSoFar + 1, true, Pair(0, 1))
    }

    @Suppress("unused")
    fun printDistMap() {
        for (y in distmap.indices) {
            for (x in distmap[y].indices) {
                when (val cell = distmap[y][x]) {
                    in 0..9 -> print(cell)
                    Int.MAX_VALUE -> print(cave[y][x])
                    else -> print('+')
                }
            }
            println()
        }
        println()
    }
}
