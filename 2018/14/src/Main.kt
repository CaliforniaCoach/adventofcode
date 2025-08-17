const val PUZZLE_INPUT = 170641

fun main() {
    solveFor(9, 0, false)
    solveFor(5, 0, false)
    solveFor(18, 0, false)
    solveFor(2018, 0, false)
    solveFor(PUZZLE_INPUT, 0, false)

    println()

    solveFor(51589, 5, true)
    solveFor(1245, 5, true)
    solveFor(92510, 5, true)
    solveFor(59414, 5, true)
    solveFor(PUZZLE_INPUT, 6, true)
}


fun solveFor(inputParam: Int, len: Int, part2: Boolean) {
    val recipes = mutableListOf(3, 7)
    var elf1pos = 0
    var elf2pos = 1

    var compareList: MutableList<Int> = mutableListOf()
    if (part2) {
        for (i in (0..<len).reversed()) {
            var digit = inputParam
            for (k in 0..<i) {
                digit /= 10
            }
            compareList.add(digit % 10)
        }
    }

    while (true) {
        val newScore = recipes[elf1pos] + recipes[elf2pos]
        if (newScore >= 10) {
            recipes.add(newScore / 10)
        }
        recipes.add(newScore % 10)
        elf1pos = (elf1pos + 1 + recipes[elf1pos]) % recipes.size
        elf2pos = (elf2pos + 1 + recipes[elf2pos]) % recipes.size

        if (!part2) {
            if (recipes.size >= inputParam + 10) {
                print("Next ten recipes after $inputParam recipes: ")
                for (i in inputParam..<inputParam + 10) {
                    print(recipes[i])
                }
                println()
                break
            }
        } else if (recipes.size > len) {
            val lastTen = recipes.subList(recipes.size - len, recipes.size)
            val lastTenWithoutLast = recipes.subList(recipes.size - len - 1, recipes.size - 1)
            if (lastTen == compareList) {
                println("$inputParam first appears after ${recipes.size - len} recipes")
                break
            } else if (lastTenWithoutLast == compareList) {
                println("$inputParam first appears after ${recipes.size - len - 1} recipes")
                break
            }
        }
    }
}
