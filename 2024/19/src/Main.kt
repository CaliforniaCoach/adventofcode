import java.io.File
import kotlin.math.min

fun main() {
    val content = File("input.txt").readLines()
    val towels = content[0].split(", ")
    val towelsByLength = Array<MutableList<String>>(10) { mutableListOf() }
    val towelsByFirstLetter = Array<MutableList<String>>(26) { mutableListOf() }
    for (towel in towels) {
        towelsByLength[towel.length].add(towel)
        towelsByFirstLetter[towel.first().code - 'a'.code].add(towel)
    }

    var designsPossible = 0
    var combinationsPossible = 0L
    for (design in content.subList(2, content.size)) {
        println("Checking $design...")
        val possibilities = CombinationCalculator(design, towelsByLength).combinations
        if (possibilities > 0) {
            designsPossible++
        }
        combinationsPossible += possibilities
        println("Possible designs: $designsPossible")
        println("Possible combinations: $combinationsPossible")
    }
}

class CombinationCalculator(targetDesign: String, val availableTowels: Array<MutableList<String>>) {
    val visited: MutableMap<String, Long> = mutableMapOf()
    val combinations = tryNext(targetDesign)

    fun tryNext(restDesign: String): Long {
        if (restDesign.isEmpty()) {
            return 1
        }
        val lookedUpPossibilities = visited[restDesign]
        if (lookedUpPossibilities != null) {
            return lookedUpPossibilities
        }
        var possibilities = 0L
        for (len in 1..min(9, restDesign.length)) {
            if (restDesign.substring(0, len) in availableTowels[len]) {
                possibilities += tryNext(restDesign.substring(len))
            }
        }
        visited[restDesign] = possibilities
        return possibilities
    }
}
