import java.io.File

fun main() {
    val field = File("input.txt").readLines().map { it.toCharArray() }
    val visitedFields: MutableMap<String, Int> = mutableMapOf()
    var i = 0
    var check = true
    while (true) {
        i++
        iterate(field)
        val trees = field.sumOf { it.count { it2 -> it2 == '|' } }
        val lumberyards = field.sumOf { it.count { it2 -> it2 == '#' } }
        if (i == 10) {
            println("Total resource value after $i minutes is ${trees * lumberyards}")
        }
        val fieldAsString = field.map { it.concatToString() }.toString()
        if (check && fieldAsString in visitedFields) {
            val lastIndex = visitedFields[fieldAsString]!!
            val delta = i - lastIndex
            i += delta * ((1000000000 - i) / delta)
            check = false
        }
        visitedFields[fieldAsString] = i
        if (i == 1000000000) {
            println("Total resource value after $i minutes is ${trees * lumberyards}")
            break
        }
    }
}

fun iterate(field: List<CharArray>) {
    val origField = field.map { it.clone() }
    for (y in field.indices) {
        for (x in field[y].indices) {
            val (_, trees, lumberyards) = getAdjacentFieldCounts(origField, x, y)
            when (field[y][x]) {
                '.' -> if (trees >= 3) field[y][x] = '|'
                '|' -> if (lumberyards >= 3) field[y][x] = '#'
                '#' -> if (lumberyards < 1 || trees < 1) field[y][x] = '.'
            }
        }
    }
}

fun getAdjacentFieldCounts(field: List<CharArray>, x: Int, y: Int): Triple<Int, Int, Int> {
    var open = 0
    var trees = 0
    var lumberyards = 0
    for (dx in -1..1) {
        for (dy in -1..1) {
            if (dx != 0 || dy != 0) {
                if (y + dy in field.indices && x + dx in field[y + dy].indices) {
                    when (field[y + dy][x + dx]) {
                        '.' -> open++
                        '|' -> trees++
                        '#' -> lumberyards++
                    }
                }
            }
        }
    }
    return Triple(open, trees, lumberyards)
}

fun printField(field: List<CharArray>) {
    for (row in field) {
        for (chr in row) {
            print(chr)
        }
        println()
    }
}
