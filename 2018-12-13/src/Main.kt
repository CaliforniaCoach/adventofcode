import java.io.File

data class Train(var x: Int, var y: Int, var dx: Int, var dy: Int, var mem: Int)

fun main() {
    val field = File("input.txt").readLines()

    val train: MutableList<Train> = mutableListOf()
    for (y in field.indices) {
        for (x in field[y].indices) {
            val tile = field[y][x]
            var dir: Pair<Int, Int>? = null
            when (tile) {
                '^' -> dir = Pair(0, -1)
                '>' -> dir = Pair(1, 0)
                'v' -> dir = Pair(0, 1)
                '<' -> dir = Pair(-1, 0)
            }
            if (dir != null) {
                train.add(Train(x, y, dir.first, dir.second, 0))
            }
        }
    }

    var firstCrashOccurred = false
    while (train.size > 1) {
        var i = 0
        while (i < train.size) {
            val newTrain = train[i].copy()
            newTrain.x += newTrain.dx
            newTrain.y += newTrain.dy
            val k = train.indexOfFirst { it.x == newTrain.x && it.y == newTrain.y }
            if (k >= 0) {
                if (!firstCrashOccurred) {
                    println("First crash at position: ${newTrain.x},${newTrain.y}")
                    firstCrashOccurred = true
                }
                if (k < i) {
                    train.removeAt(i)
                    train.removeAt(k)
                    i--
                } else {
                    train.removeAt(k)
                    train.removeAt(i)
                }
            } else {
                var newDir = Pair(newTrain.dx, newTrain.dy)
                when (field[newTrain.y][newTrain.x]) {
                    '\\' -> newDir = Pair(newDir.second, newDir.first)
                    '/' -> newDir = Pair(-newDir.second, -newDir.first)
                    '+' -> {
                        when (newTrain.mem) {
                            0 -> newDir = Pair(newDir.second, -newDir.first)
                            2 -> newDir = Pair(-newDir.second, newDir.first)
                        }
                        newTrain.mem = (newTrain.mem + 1) % 3
                    }
                }
                newTrain.dx = newDir.first
                newTrain.dy = newDir.second
                train[i] = newTrain
                i++
            }
        }

        train.sortWith(compareBy<Train> { it.y }.thenBy { it.x })
    }
    println("Last train left at position: ${train[0].x},${train[0].y}")
}
