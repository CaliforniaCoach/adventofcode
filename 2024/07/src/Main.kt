import java.io.File

fun main() {
    val equations = File("input.txt").readLines().map {
        val tok = it.split(": ")
        (listOf(tok[0]) + tok[1].split(" ")).map { it2 -> it2.toLong() }
    }

    var totalCalibrationResult = 0L
    for (num in equations) {
        val combinations = pow(2, num.size - 2)
        for (combination in 0..combinations) {
            var result = num[1]
            for (i in 0..<num.size - 2) {
                if (combination / pow(2, i) % 2 == 0) {
                    result += num[i + 2]
                } else {
                    result *= num[i + 2]
                }
            }
            if (result == num[0]) {
                totalCalibrationResult += result
                break
            }
        }
    }
    println("Total calibration result: $totalCalibrationResult")

    var totalCalibrationResult2 = 0L
    for (num in equations) {
        val combinations = pow(3, num.size - 2)
        for (combination in 0..combinations) {
            var result = num[1]
            for (i in 0..<num.size - 2) {
                when (combination / pow(3, i) % 3) {
                    0 -> result += num[i + 2]
                    1 -> result *= num[i + 2]
                    2 -> result = (result.toString() + num[i + 2].toString()).toLong()
                }
            }
            if (result == num[0]) {
                totalCalibrationResult2 += result
                break
            }
        }
    }
    println("Total calibration result: $totalCalibrationResult2")
}

fun pow(x: Int, y: Int): Int {
    var res = 1
    for (i in 0..<y) {
        res *= x
    }
    return res
}
