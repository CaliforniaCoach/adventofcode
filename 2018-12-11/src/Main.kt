const val GRID_SN = 7315

fun main() {
    val grid = IntArray(300 * 300)
    for (y in 0..<300) {
        for (x in 0..<300) {
            grid[y * 300 + x] = powerLevel(x + 1, y + 1)
        }
    }

    val (maxx, maxy, maxPowerLevelSum) = calcMaxSquare(grid, 3)

    println("Max power level $maxPowerLevelSum with size 3 at $maxx,$maxy")

    var maxPower = Int.MIN_VALUE
    var x = 0
    var y = 0
    var maxSize = 0
    for (size in 1..300) {
        val (xx, yy, power) = calcMaxSquare(grid, size)
        if (power > maxPower) {
            x = xx
            y = yy
            maxSize = size
            maxPower = power
        }
    }

    println("Max power level $maxPower with variable size at $x,$y,$maxSize")
}


fun powerLevel(x: Int, y: Int): Int {
    var level = x + 10
    level *= y
    level += GRID_SN
    level *= x + 10
    level = (level / 100) % 10
    level -= 5

    return level
}


fun calcMaxSquare(grid: IntArray, squareSize: Int): Triple<Int, Int, Int> {
    var maxPowerLevelSum = Int.MIN_VALUE
    var maxx = 0
    var maxy = 0
    for (y in 0..300 - squareSize) {
        for (x in 0..300 - squareSize) {
            var powerLevelSum = 0
            for (dy in 0..<squareSize) {
                for (dx in 0..<squareSize) {
                    powerLevelSum += grid[(y + dy) * 300 + x + dx]
                }
            }
            if (powerLevelSum > maxPowerLevelSum) {
                maxPowerLevelSum = powerLevelSum
                maxx = x + 1
                maxy = y + 1
            }
        }
    }
    return Triple(maxx, maxy, maxPowerLevelSum)
}
