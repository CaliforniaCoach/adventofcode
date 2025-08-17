import java.io.File

fun main() {
    val content = File("input.txt").readText().split("\n\n")
    val rulesString = content[0].trim()
    val prints = content[1].trim()

    val rules: MutableList<Pair<Int, Int>> = mutableListOf()
    for (rule in rulesString.lines()) {
        val tok = rule.split('|')
        val num1 = tok[0].toInt()
        val num2 = tok[1].toInt()
        rules.add(Pair(num1, num2))
    }

    var sum = 0
    var sum2 = 0
    mainLoop@ for (line in prints.lines()) {
        val pages = line.split(',').map { it.toInt() }.toMutableList()
        for (before in 0..<pages.size - 1) {
            for (after in before + 1..<pages.size) {
                if (rules.contains(Pair(pages[after], pages[before]))) {
                    pages.sortWith { v1, v2 ->
                        if (rules.contains(Pair(v1, v2)))  -1
                        else if (rules.contains(Pair(v2, v1))) 1
                        else 0
                    }
                    sum2 += pages[pages.size / 2]
                    continue@mainLoop
                }
            }
        }
        sum += pages[pages.size / 2]
    }
    println("Sum of middle pages of correctly ordered pages: $sum")
    println("Sum of middle pages of incorrectly ordered pages: $sum2")
}
