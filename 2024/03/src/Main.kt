import java.io.File

fun main() {
    val re = "(do)\\(\\)|(don't)\\(\\)|mul\\((\\d{1,3}),(\\d{1,3})\\)".toRegex()
    var sum = 0
    var conditionalSum = 0
    var enabled = true
    for (m in re.findAll(File("input.txt").readText())) {
        if (m.groupValues[1] == "do") {
            enabled = true
        } else if (m.groupValues[2] == "don't") {
            enabled = false
        } else {
            sum += m.groupValues[3].toInt() * m.groupValues[4].toInt()
            if (enabled) {
                conditionalSum += m.groupValues[3].toInt() * m.groupValues[4].toInt()
            }
        }
    }
    println("Sum of all valid products: $sum")
    println("Conditional sum: $conditionalSum")
}
