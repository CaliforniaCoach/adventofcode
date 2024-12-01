import java.io.File

fun main() {

	val lines = mutableListOf<String>()
	File("input.txt").forEachLine {
		lines.add(it)
	}

	var two = 0
	var three = 0

	for (line in lines) {
		val chars = mutableMapOf<Char, Int>()
		for (chr in line) {
			chars[chr] = chars.getOrDefault(chr, 0) + 1
		}
		for ((key, value) in chars) {
			if (value == 2) {
				two++
				break
			}
		}
		for ((key, value) in chars) {
			if (value == 3) {
				three++
				break
			}
		}
	}

	println(two * three)


	for (i in 0..lines.size - 2) {
		val line1 = lines[i]
		for (k in (i + 1)..lines.size - 1) {
			val line2 = lines[k]
			var different = 0
			var common = ""
			for (c in 0..line1.length - 1) {
				if (line1[c] != line2[c]) {
					different++
				} else {
					common += line1[c]
				}
			}
			if (different == 1) {
				println(common)
			}
		}
	}
}
