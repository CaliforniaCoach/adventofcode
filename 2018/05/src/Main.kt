import java.io.File
import kotlin.math.min

fun main() {
    val polymer = File("input.txt").readText().trim()

    println("Rest length of polymer after all reactions: ${react(polymer)}")


    var minLen = Int.MAX_VALUE
    for (excludeChar in polymer.lowercase().toSet()) {
        val modifiedPolymer = polymer.replace(excludeChar.toString(), "").replace(excludeChar.uppercase(), "")
        minLen = min(minLen, react(modifiedPolymer))
    }
    println("Shortest length of reaction results: $minLen")
}


fun react(polymer: String): Int {
    @Suppress("NAME_SHADOWING")
    var polymer = polymer
    mainLoop@ while (true) {
        for (i in 0..<polymer.length - 1) {
            val ch1 = polymer[i]
            val ch2 = polymer[i + 1]
            if (ch1.lowercaseChar() == ch2.lowercaseChar() && ch1.isLowerCase().xor(ch2.isLowerCase())) {
                polymer = polymer.removeRange(i..i + 1)
                continue@mainLoop
            }
        }
        break
    }
    return polymer.length
}
