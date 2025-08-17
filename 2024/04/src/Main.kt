import java.io.File

fun main() {
    val grid = File("input.txt").readLines()
    var count = 0
    var count2 = 0
    for (y in grid.indices) {
        for (x in grid[y].indices) {
            for (dy in -1..1) {
                for (dx in -1..1) {
                    if (dx != 0 || dy != 0) {
                        if (get(grid, x, y, dx, dy) == "XMAS") {
                            count++
                        }
                    }
                }
            }
            val (str1, str2) = get2(grid, x, y)
            if ((str1 == "MAS" || str1.reversed() == "MAS") && (str2 == "MAS" || str2.reversed() == "MAS")) {
                count2++
            }
        }
    }
    println("XMAS found $count times")
    println("X-MAS found $count2 times")
}

fun get(grid: List<String>, x: Int, y: Int, dx: Int, dy: Int): String {
    var retval = StringBuilder()
    for (i in 0..<4) {
        val ty = y + dy * i
        val tx = x + dx * i
        if (ty >= 0 && ty < grid.size && tx >= 0 && tx < grid[ty].length) {
            retval.append(grid[ty][tx])
        }
    }
    return retval.toString()
}

fun get2(grid: List<String>, x: Int, y: Int): Pair<String, String> {
    val str1 = StringBuilder()
    val str2 = StringBuilder()
    for (i in 0..2) {
        val tx = x + i
        val ty = y + i
        if (ty >= 0 && ty < grid.size && tx >= 0 && tx < grid[ty].length) {
            str1.append(grid[ty][tx])
        }
        val ty2 = y + (2 - i)
        if (ty2 >= 0 && ty2 < grid.size && tx >= 0 && tx < grid[ty2].length) {
            str2.append(grid[ty2][tx])
        }
    }
    return Pair(str1.toString(), str2.toString())
}
