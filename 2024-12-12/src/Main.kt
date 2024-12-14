import java.io.File

fun main() {
    val field = File("input.txt").readLines().map { it.toCharArray() }
    var totalPrice = 0
    var totalPrice2 = 0
    for (y in field.indices) {
        for (x in field[y].indices) {
            if (field[y][x] <= 'Z') {
                val analyzer = Analyzer(field, x, y)
                totalPrice += analyzer.area * analyzer.edges.size
                totalPrice2 += analyzer.area * analyzer.countSides()
            }
        }
    }
    println("Total price with perimeters: $totalPrice")
    println("Total price with num of sides: $totalPrice2")
}

class Analyzer(private val field: List<CharArray>, x0: Int, y0: Int) {
    var area = 0
    var edges: MutableSet<Pair<Pair<Int, Int>, Pair<Int, Int>>> = mutableSetOf()

    init {
        follow(x0, y0, 0, 0, field[y0][x0])
    }

    private fun follow(x: Int, y: Int, dx: Int, dy: Int, searchChar: Char) {
        if (y !in field.indices || x !in field[y].indices) {
            edges.add(Pair(Pair(x, y), Pair(dx, dy)))
            return
        }
        if (field[y][x] != searchChar) {
            if (field[y][x] != searchChar.lowercaseChar()) {
                edges.add(Pair(Pair(x, y), Pair(dx, dy)))
            }
            return
        }
        field[y][x] = searchChar.lowercaseChar()
        area++
        for (d in listOf(Pair(1, 0), Pair(-1, 0), Pair(0, 1), Pair(0, -1))) {
            follow(x + d.first, y + d.second, d.first, d.second, searchChar)
        }
        return
    }

    fun countSides(): Int {
        val directionalEdges: MutableMap<Triple<Int, Int, Int>, MutableSet<Int>> = mutableMapOf()
        for (edge in edges) {
            val (x, y) = edge.first
            val (dx, dy) = edge.second
            if (dx == 0) {
                directionalEdges.getOrPut(Triple(dx, dy, y)) { mutableSetOf() }.add(x)
            } else {
                directionalEdges.getOrPut(Triple(dx, dy, x)) { mutableSetOf() }.add(y)
            }
        }
        var sides = 0
        for (directionalEdge in directionalEdges) {
            val coordSorted = directionalEdge.value.sorted()
            sides++
            for (i in 1..<coordSorted.size) {
                if (coordSorted[i] - coordSorted[i - 1] > 1) {
                    sides++
                }
            }
        }
        return sides
    }
}

// Can't use the following function because it will not count sides *inside* of the area
@Suppress("unused")
fun analyze2(field: List<CharArray>, x: Int, y: Int): Int {
    var cx = x
    var cy = y
    var dx = 1
    var dy = 0
    var sides = 0
    val searchChar = field[y][x]
    while (cx != x || cy != y || dx != 1 || dy != 0 || sides <= 1) {
        // Can we turn left?
        val lx = cx + dy
        val ly = cy - dx
        if (ly in field.indices && lx in field[ly].indices && field[ly][lx] == searchChar) {
            // ... then turn left!
            dx = dy.also { dy = -dx }
            sides++
            cx += dx
            cy += dy
        } else {
            // Continue going current direction
            val nx = cx + dx
            val ny = cy + dy
            if (ny in field.indices && nx in field[ny].indices && field[ny][nx] == searchChar) {
                cx = nx
                cy = ny
            } else {
                // Turn right in next turn (but do not move yet)
                dx = -dy.also { dy = dx }
                sides++
            }
        }
    }
    return sides
}
