import java.io.File

data class Inputs(val outputElementCount: Long, val inputElements: List<Pair<String, Long>>)

fun main() {
    val reactions: MutableMap<String, Inputs> = mutableMapOf()
    File("input.txt").readLines().forEach {
        val formula = it.split("=>").map { it2 -> it2.trim() }

        val out = formula[1].split(" ")
        val outElemCount = out[0].toLong()
        val outElem = out[1]

        val inp = formula[0].split(", ")
        val inputElements = inp.map { it.split(" ") }.map { Pair(it[1], it[0].toLong()) }

        assert(!reactions.containsKey(outElem))
        reactions.put(outElem, Inputs(outElemCount, inputElements))
    }

    println(calcMinOreRequired(reactions, 1))

    // Part 2

    var minFuel = 1L
    var maxFuel = 1_000_000_000_000L
    while (maxFuel - minFuel > 1) {
        val fuel = if (minFuel * 2 >= maxFuel) (maxFuel + minFuel) / 2 else minFuel * 2
        if (calcMinOreRequired(reactions, fuel) <= 1_000_000_000_000) {
            minFuel = fuel
        } else {
            maxFuel = fuel
        }
    }
    println(minFuel)
}

fun calcMinOreRequired(reactionMap: Map<String, Inputs>, fuel: Long): Long {
    val needed: MutableMap<String, Long> = mutableMapOf("FUEL" to fuel)
    while (true) {
        val elemsStillNeeded = needed.filter { it.value > 0 }
        if (elemsStillNeeded.size == 1 && elemsStillNeeded.keys.first() == "ORE") {
            break
        }

        val elemNeeded = elemsStillNeeded.keys.first { it != "ORE" }
        val numElemsNeeded = needed[elemNeeded]!!
        val inputs = reactionMap[elemNeeded]!!
        val numReactions = (numElemsNeeded - 1) / inputs.outputElementCount + 1
        for ((elem: String, num: Long) in inputs.inputElements) {
            needed[elem] = needed.getOrDefault(elem, 0) + num * numReactions
        }
        needed[elemNeeded] = needed[elemNeeded]!! - numReactions * inputs.outputElementCount
    }
    return needed["ORE"]!!
}
