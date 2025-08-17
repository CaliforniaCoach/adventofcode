import java.io.File

val SIZE = 1000

fun main() {
	val grid = MutableList<Int>(SIZE * SIZE) { 0 }
	val lineFormat = "#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)".toRegex()

	File("input.txt").forEachLine() {
		val matchResult = lineFormat.find(it)!!
		val (_, x, y, w, h) = matchResult.destructured.toList().map { it.toInt() }
		for (dx in 0..<w) {
			for (dy in 0..<h) {
				grid[(y + dy) * SIZE + x + dx]++
			}
		}
	}

	var count = 0
	for (overlaps in grid) {
		if (overlaps >= 2) {
			count++
		}
	}

	println("This many tiles are covered by more than one claim: ${count}")


	var non_overlapping_claim = 0
	File("input.txt").forEachLine() {
		val matchResult = lineFormat.find(it)!!
		val (id, x, y, w, h) = matchResult.destructured.toList().map { it.toInt() }
		for (dx in 0..<w) {
			for (dy in 0..<h) {
				if (grid[(y + dy) * SIZE + x + dx] != 1) {
					return@forEachLine
				}
			}
		}
		non_overlapping_claim = id
	}

	println("Claim ID which does not overlap with others: ${non_overlapping_claim}")
}
