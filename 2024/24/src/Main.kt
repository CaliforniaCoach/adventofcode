import java.io.File
import kotlin.system.exitProcess

fun main() {
    val signals: MutableMap<String, Boolean> = mutableMapOf()
    val gates: MutableMap<String, Triple<String, String, (Boolean, Boolean) -> Boolean>> = mutableMapOf()
    val gatesReadable: MutableMap<String, Triple<String, String, String>> = mutableMapOf()
    for (line in File("input.txt").readLines()) {
        if (line.contains(':')) {
            val tok = line.trim().split(": ")
            signals[tok[0]] = tok[1].toInt() != 0
        } else if (line.contains("->")) {
            val tok = line.trim().split(" ")
            gates[tok[4]] = Triple(tok[0], tok[2], when (tok[1]) {
                "AND" -> { a, b -> a && b }
                "OR" -> { a, b -> a || b }
                "XOR" -> { a, b -> a != b }
                else -> exitProcess(1)
            })
            gatesReadable[tok[4]] = Triple(tok[0], tok[2], tok[1])
        }
    }

    var result = 0L
    val dependencies: MutableMap<String, MutableList<String>> = mutableMapOf()
    for (gate in gates.keys) {
        if (gate.startsWith('z')) {
            if (calcSignal(gate, signals, gates)) {
                result = result or (1L shl gate.substring(1, gate.length).toInt())
            }
            dependencies[gate] = mutableListOf()
            getDependencies(gate, dependencies[gate]!!, gates)
        }
    }
    println("Result for z<xx>: $result")
    for ((sig, dep) in dependencies.toSortedMap()) {
        val z = sig.substring(1, sig.length).toInt()
        if (dep.any { it.substring(1, it.length).toInt() > z }) {
            println("$sig -> ${dep.sorted()}")
        }
    }
    showTree("z00", gatesReadable, 0)
    println()
    showTree("z01", gatesReadable, 0)
    println()
    showTree("z02", gatesReadable, 0)
    println()
    showTree("z03", gatesReadable, 0)
    println()

    val swapCandidates: MutableList<String> = mutableListOf()
    val notFullSumOutputs: MutableList<String> = mutableListOf()
    for (i in 0..gates.maxOf { it.key.substring(1, it.key.length).toIntOrNull() ?: 0 } - 1) {
        val sumFull = String.format("z%02d", i)
        val in1 = String.format("x%02d", i)
        val in2 = String.format("y%02d", i)
        val inputs = listOf(in1, in2)
        val gate = gatesReadable[sumFull]!!
        if (gate.third != "XOR") {
            notFullSumOutputs.add(sumFull)
            showTree(sumFull, gatesReadable, 0)
        }
        val halfSumCandidate1 = gate.first
        val halfSumCandidate2 = gate.second
        val gate1 = gatesReadable[halfSumCandidate1]
        val gate2 = gatesReadable[halfSumCandidate2]
        if (gate1 == null)
            swapCandidates.add(halfSumCandidate1)
        if (gate2 == null)
            swapCandidates.add(halfSumCandidate2)
        if (!
            (gate1 != null && gate1.third == "XOR" && gate1.first in inputs && gate1.second in inputs) ||
            (gate2 != null && gate2.third == "XOR" && gate2.first in inputs && gate2.second in inputs)) {
            swapCandidates.add(halfSumCandidate1)
            swapCandidates.add(halfSumCandidate2)
        }
    }
    println("${swapCandidates.size} swap candidates: $swapCandidates")
    println("${notFullSumOutputs.size} cannot be full sum outputs: $notFullSumOutputs")

    val wrongOrInputs: MutableList<String> = mutableListOf()
    val finalOutputSignals: MutableList<String> = mutableListOf()
    val notFinalOutputSignals: MutableList<String> = mutableListOf()
    mainLoop@ for ((sig, gate) in gatesReadable) {
        when (gate.third) {
            "OR" -> {
                val inGate1 = gatesReadable[gate.first]
                val inGate2 = gatesReadable[gate.second]
                if (inGate1 == null || inGate1.third != "AND")
                    wrongOrInputs.add(gate.first)
                if (inGate2 == null || inGate2.third != "AND")
                    wrongOrInputs.add(gate.second)
            }
        }
        if (isFinalOutputSignal(gatesReadable, sig)) {
            if (!sig.startsWith("z")) {
                finalOutputSignals.add(sig)
            }
        } else {
            if (sig.startsWith("z")) {
                notFinalOutputSignals.add(sig)
            }
        }
    }
    println("${wrongOrInputs.size} wrong OR inputs: $wrongOrInputs")
    println("${finalOutputSignals.size} are unexpected final output signals: $finalOutputSignals")
    println("${notFinalOutputSignals.size} are not  final output signals: $finalOutputSignals")

    val wrongHalfSignals: MutableList<String> = mutableListOf()
    var lastFullCarrySignal: String? = null
    for (i in 0..gates.maxOf { it.key.substring(1, it.key.length).toIntOrNull() ?: 0 } - 1) {
        val x = String.format("x%02d", i)
        val y = String.format("y%02d", i)
        if (i == 0) {
            val (sig, _) = findGate(gatesReadable, x, y, "AND")
            lastFullCarrySignal = sig
        } else {
            val (halfCarrySignal, _) = findGate(gatesReadable, x, y, "AND")
            val (halfSumSignal, _) = findGate(gatesReadable, x, y, "XOR")
            val fullCarrySignal = findGate(gatesReadable, halfCarrySignal, "OR")
            if (fullCarrySignal == null) {
                wrongHalfSignals.add(halfCarrySignal)
            } else {
                val carryGate = gatesReadable[fullCarrySignal]!!
                val interCarrySignal = if (carryGate.first == halfCarrySignal) carryGate.second else carryGate.first
                val interCarryGate = gatesReadable[interCarrySignal]!!
                val carryInputSignal = if (interCarryGate.first == halfSumSignal) interCarryGate.second else interCarryGate.first
                if (lastFullCarrySignal != null && carryInputSignal != lastFullCarrySignal) {
                    wrongHalfSignals.add(lastFullCarrySignal)
                }
            }
            val fullSumSignal = findGate(gatesReadable, halfSumSignal, "XOR")
            if (fullSumSignal == null) {
                wrongHalfSignals.add(halfSumSignal)
            } else {
                if (fullSumSignal != String.format("z%02d", i)) {
                    wrongHalfSignals.add(fullSumSignal)
                }
                val sumGate = gatesReadable[fullSumSignal]!!
                val carryInputSignal = if (sumGate.first == halfSumSignal) sumGate.second else sumGate.first
                if (lastFullCarrySignal != null && carryInputSignal != lastFullCarrySignal) {
                    wrongHalfSignals.add(lastFullCarrySignal)
                }
            }
            lastFullCarrySignal = fullCarrySignal
        }
    }
    println("${wrongHalfSignals.size} wrong half result or carry signals: $wrongHalfSignals")

    val swapSignals = (notFullSumOutputs.toSet() + wrongOrInputs.toSet() + wrongHalfSignals.toSet()).toSortedSet()
    println("${swapSignals.size} are certainly wrong: ${swapSignals.joinToString(",")}")
}

fun calcSignal(sig: String, signals: Map<String, Boolean>,
               gates: Map<String, Triple<String, String, (Boolean, Boolean) -> Boolean>>): Boolean {
    val gate = gates[sig]
    return signals[sig] ?: gate!!.third(calcSignal(gate.first, signals, gates), calcSignal(gate.second, signals, gates))
}

fun getDependencies(sig: String, dependencies: MutableList<String>,
                    gates: Map<String, Triple<String, String, (Boolean, Boolean) -> Boolean>>) {
    if (sig.startsWith('x') || sig.startsWith('y')) {
        dependencies.add(sig)
    } else {
        val gate = gates[sig]!!
        getDependencies(gate.first, dependencies, gates)
        getDependencies(gate.second, dependencies, gates)
    }
}

fun showTree(sig: String, gates: Map<String, Triple<String, String, String>>, depth: Int) {
    print("-- $sig")
    if (sig.first() !in listOf('x', 'y')) {
        val gate = gates[sig]!!
        print(" -- ${gate.third} ")
        showTree(gate.first, gates, depth + 1)
        repeat(depth) { print("              ") }
        print("           L--")
        showTree(gate.second, gates, depth + 1)
    } else {
        println()
    }
}

fun findGate(gates: Map<String, Triple<String, String, String>>, in1: String, in2: String, type: String): Pair<String, Triple<String, String, String>> {
    val found: MutableList<Pair<String, Triple<String, String, String>>> = mutableListOf()
    for ((out, gate) in gates) {
        if (((gate.first == in1 && gate.second == in2) || (gate.first == in2 && gate.second == in1)) && gate.third == type) {
            found.add(Pair(out, gate))
        }
    }
    if (found.size == 0) {
        println("No suitable gate found for $in1, $in2 -> $type")
        exitProcess(1)
    } else if (found.size > 1) {
        println("Multiple gates found for $in1, $in2 -> $type: $found")
        exitProcess(1)
    } else {
        return found.first()
    }
}

fun findGate(gates: Map<String, Triple<String, String, String>>, in1: String, type: String): String? {
    val found: MutableList<String> = mutableListOf()
    for ((out, gate) in gates) {
        if (gate.third == type && (gate.first == in1 || gate.second == in1)) {
            found.add(out)
        }
    }
    if (found.size == 0) {
        println("No suitable gate found for $in1 -> $type")
        return null
    } else if (found.size > 1) {
        println("Multiple gates found for $in1 -> $type")
        exitProcess(1)
    } else {
        return found.first()
    }
}

fun isFinalOutputSignal(gates: Map<String, Triple<String, String, String>>, signal: String): Boolean {
    for ((_, gate) in gates) {
        if (signal == gate.first || signal == gate.second) {
            return false
        }
    }
    return true
}
