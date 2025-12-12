import java.io.File

class Finder(val targetLights: String, val buttons: List<List<Int>>, val targetJoltage: List<Int>) {
    val lights = targetLights.map { it == '#' }.toMutableList()
    var minPresses = Int.MAX_VALUE
    var maxPressesToTry = 0
    val lightCombinationsVisited: MutableList<List<Boolean>> = mutableListOf()

    val currentJoltage = MutableList<Int>(targetJoltage.size) { 0 }
    var minPresses2 = Int.MAX_VALUE

    fun find(): Int {
        do {
            minPresses = Int.MAX_VALUE
            maxPressesToTry++
            find(0, -1)
        } while (minPresses >= maxPressesToTry)
        return minPresses
    }

    fun find(numButtonsPressed: Int, lastButtonPressed: Int) {
        if (lights in lightCombinationsVisited || numButtonsPressed >= minPresses || numButtonsPressed >= maxPressesToTry) {
            return
        }
        if (lights.all { !it }) {
            minPresses = numButtonsPressed
            return
        }
        lightCombinationsVisited.add(lights.toList())
        for (button in 0..<buttons.size) {
            if (button != lastButtonPressed) {
                press(button)
                find(numButtonsPressed + 1, button)
                press(button)
            }
        }
        lightCombinationsVisited.removeLast()
    }

    fun press(button: Int) {
        for (light in buttons[button]) {
            lights[light] = !lights[light]
        }
    }

    fun find2(): Int {
        val matrix =MutableList<MutableList<Int>>(targetJoltage.size) { MutableList<Int>(buttons.size) { 0 } }
        for ((i, button) in buttons.withIndex()) {
            for (j in button) {
                matrix[j][i] = 1
            }
        }
        val rightside = targetJoltage.toMutableList()
        for (i in 0..<matrix.size) {
            var k = i
            while (matrix[k][i] != 1 && matrix[k][i] != -1) {
                k++
            }
            matrix[i] = matrix[k].also { matrix[k] = matrix[i] }
            rightside[i] = rightside[k].also { rightside[k] = rightside[i] }
            val divider = matrix[i][i]
            for (col in 0..<matrix[i].size) {
                matrix[i][col] /= divider
            }
            rightside[k] /= divider
            for (k in i + 1..<matrix.size) {
                val multiplier = -matrix[k][i]
                for (col in 0..<matrix[k].size) {
                    matrix[k][col] += multiplier * matrix[i][col]
                }
                rightside[k] += multiplier * rightside[i]
            }
        }
        return minPresses2
    }
}

fun main() {
    var totalButtonPresses1 = 0
    var totalButtonPresses2 = 0
    File("input.txt").readLines().forEach {
        val tokens = it.split(" ")
        val targetLights = tokens.first().substring(1, tokens.first().length - 1)
        val buttons = tokens.subList(1, tokens.size - 1).map {
            btn -> btn.substring(1, btn.length - 1).split(",").map { num -> num.toInt() }
        }
        val targetJoltage = tokens.last().substring(1, tokens.last().length - 1).split(",").map { num -> num.toInt() }
        val finder = Finder(targetLights, buttons, targetJoltage)
        totalButtonPresses1 += finder.find()
        // totalButtonPresses2 += finder.find2()
    }
    println(totalButtonPresses1)
    println(totalButtonPresses2)
}
