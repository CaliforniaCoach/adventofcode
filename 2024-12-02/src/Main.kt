import java.io.File
import kotlin.math.absoluteValue
import kotlin.math.sign

fun main() {
    val reports = File("input.txt").readLines().map { it.split("\\s+".toRegex()).map { it.toInt() } }
    var safeCount1 = 0
    var safeCount2 = 0
    for (report in reports) {
        if (isSafe(report)) {
            safeCount1++
            safeCount2++
        } else {
            for (i in report.indices) {
                if (isSafe(report.filterIndexed { index, _ -> index != i })) {
                    safeCount2++
                    break
                }
            }
        }
    }
    println("Safe reports without correction: $safeCount1")
    println("Safe reports with removing max 1 level: $safeCount2")
}

fun isSafe(report: List<Int>): Boolean {
    val sign = (report[1] - report[0]).sign
    for (i in 1..<report.size) {
        val diff = report[i] - report[i - 1]
        if (diff.sign != sign || diff.absoluteValue > 3) {
            return false
        }
    }
    return true
}
