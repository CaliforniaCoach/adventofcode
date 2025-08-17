import java.io.File
import kotlin.system.exitProcess

fun main() {
    val compartments = File("input.txt").readText().split("\n\\s*\n".toRegex())
    val field = compartments[0].lines().map { it.toCharArray() }
    val movements = compartments[1].replace("\n", "")

    val field2 = field.map {
        it.flatMap { it2 ->
            when (it2) {
                '#' -> listOf('#', '#')
                'O' -> listOf('[', ']')
                '.' -> listOf('.', '.')
                '@' -> listOf('@', '.')
                else -> exitProcess(2)
            }
        }.toCharArray()
    }

    solve(field, movements)
    solve(field2, movements)
}

fun solve(field: List<CharArray>, movements: String) {
    var y = field.indexOfFirst { it.contains('@') }
    var x = field[y].indexOf('@')

    for (move in movements) {
        val (dx, dy) = mapOf('^' to Pair(0, -1), '>' to Pair(1, 0), 'v' to Pair(0, 1), '<' to Pair(-1, 0))[move]!!
        if (move(field, x, y, dx, dy)) {
            field[y][x] = '.'
            x += dx
            y += dy
        }
    }

    var sum = 0
    for ((ty, row) in field.withIndex())
        for ((tx, chr) in row.withIndex())
            if (chr in listOf('O', '['))
                sum += ty * 100 + tx
    println("Sum of all boxes' GPS coordinates: $sum")
}

fun move(field: List<CharArray>, x: Int, y: Int, dx: Int, dy: Int, dryRun: Boolean = false): Boolean {
    val nextTile = field[y + dy][x + dx]
    when {
        nextTile == '#' -> return false
        nextTile == '.' -> {
            if (!dryRun)
                field[y + dy][x + dx] = field[y][x]
            return true
        }
        nextTile == 'O' || (nextTile in setOf('[', ']')  && dy == 0) -> {
            val nextMoveSuccessful = move(field, x + dx, y + dy, dx, dy, dryRun)
            if (nextMoveSuccessful && !dryRun) {
                field[y + dy][x + dx] = field[y][x]
            }
            return nextMoveSuccessful
        }
        nextTile in setOf('[', ']') && dy != 0 -> {
            val otherX = if (nextTile == '[') x + 1 else x - 1
            if (move(field, x, y + dy, dx, dy, true) && move(field, otherX, y + dy, dx, dy, true)) {
                if (!dryRun) {
                    move(field, x, y + dy, dx, dy, false)
                    move(field, otherX, y + dy, dx, dy, false)
                    field[y + dy][x] = field[y][x]
                    field[y + dy][otherX] = '.'
                }
                return true
            }
            return false
        }
        else -> exitProcess(1)
    }
}
