import java.io.File

fun main() {
    val antennaGrid = File("input.txt").readLines()

    val frequencies: MutableMap<Char, MutableList<Pair<Int, Int>>> = mutableMapOf()
    val antinodes: MutableSet<Pair<Int, Int>> = mutableSetOf()
    val antinodes2: MutableSet<Pair<Int, Int>> = mutableSetOf()

    for (y in antennaGrid.indices) {
        for (x in antennaGrid[y].indices) {
            val freq = antennaGrid[y][x]
            if (freq != '.') {
                frequencies.getOrPut(freq) { mutableListOf() }.add(Pair(x, y))
            }
        }
    }

    for (freq in frequencies) {
        val antennas = freq.value
        for (i1 in 0..<antennas.size - 1) {
            for (i2 in i1 + 1..<antennas.size) {
                val (x1, y1) = antennas[i1]
                val (x2, y2) = antennas[i2]
                val ax1 = x1 - (x2 - x1)
                val ay1 = y1 - (y2 - y1)
                val ax2 = x2 + (x2 - x1)
                val ay2 = y2 + (y2 - y1)
                if (ay1 in antennaGrid.indices && ax1 in antennaGrid[ay1].indices) {
                    antinodes.add(Pair(ax1, ay1))
                }
                if (ay2 in antennaGrid.indices && ax2 in antennaGrid[ay2].indices) {
                    antinodes.add(Pair(ax2, ay2))
                }
                var (x, y) = Pair(x1, y1)
                while (y in antennaGrid.indices && x in antennaGrid[y].indices) {
                    antinodes2.add(Pair(x, y))
                    x += x2 - x1
                    y += y2 - y1
                }
                x = x1
                y = y1
                while (y in antennaGrid.indices && x in antennaGrid[y].indices) {
                    antinodes2.add(Pair(x, y))
                    x -= x2 - x1
                    y -= y2 - y1
                }
            }
        }
    }

    println("${antinodes.size} antinodes without resonant harmonics found")
    println("${antinodes2.size} antinodes with resonant harmonics found")
}
