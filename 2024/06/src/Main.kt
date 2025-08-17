import java.io.File

fun main() {
    val field = File("input.txt").readLines().map { it.toCharArray() }

    var dir = Pair(0, 0)
    var x = 0
    var y = 0
    mainLoop@ for (ty in field.indices) {
        for (tx in field[ty].indices) {
            dir = Pair(0, 0)
            when (field[ty][tx]) {
                '^' -> dir = Pair(0, -1)
                '>' -> dir = Pair(1, 0)
                'v' -> dir = Pair(0, 1)
                '<' -> dir = Pair(-1, 0)
            }
            if (dir != Pair(0, 0)) {
                x = tx
                y = ty
                break@mainLoop
            }
        }
    }
    val (_, count) = analyze(field, x, y, dir)
    println("Guard visited $count distinct positions")

    var blockerCount = 0
    for (oy in field.indices) {
        for (ox in field[oy].indices) {
            if (field[oy][ox] == '.') {
                field[oy][ox] = '#'
                val (loop, _) = analyze(field, x, y, dir)
                if (loop) {
                    blockerCount++
                }
                field[oy][ox] = '.'
            }
        }
    }
    println("$blockerCount obstacles lead to loops")
}

fun analyze(field: List<CharArray>, startX: Int, startY: Int, startDir: Pair<Int, Int>): Pair<Boolean, Int> {
    val path = field.map { it.clone() }.toList()
    val dirs: Array<Array<MutableSet<Pair<Int, Int>>>> = Array(field.size) { Array(field[0].size) { mutableSetOf() } }
    var dir = startDir
    var x = startX
    var y = startY
    var loop = false
    path[y][x] = 'X'
    dirs[y][x].add(dir)
    while (true) {
        val nx = x + dir.first
        val ny = y + dir.second
        if (ny !in path.indices || nx !in path.indices) {
            break
        }
        if (path[ny][nx] == '#') {
            dir = Pair(-dir.second, dir.first)
        } else {
            x = nx
            y = ny
            path[y][x] = 'X'
        }
        if (dir in dirs[y][x]) {
            loop = true
            break
        }
        dirs[y][x].add(dir)
    }
    val count = path.sumOf { it.count { it2 -> it2 == 'X' } }
    return Pair(loop, count)
}
