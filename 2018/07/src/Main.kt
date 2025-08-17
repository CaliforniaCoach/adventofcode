import java.io.File
import kotlin.system.exitProcess


fun main() {
    val regex = "Step (\\w) must be finished before step (\\w) can begin\\.".toRegex()

    val allSteps: MutableSet<Char> = mutableSetOf()
    val dep: MutableMap<Char, MutableSet<Char>> = mutableMapOf()
    for (line in File("input.txt").readLines()) {
        val m = regex.matchEntire(line)
        if (m != null) {
            val depending = m.groupValues[2][0]
            val depends = m.groupValues[1][0]
            dep[depending] = dep.getOrDefault(depending, mutableSetOf()).plus(depends).toMutableSet()
            allSteps.add(depending)
            allSteps.add(depends)
        } else {
            println("Unexpected line: '$line'")
            exitProcess(1)
        }
    }
    var allStepsSorted = allSteps.toSortedSet()

    val steps: MutableList<Char> = mutableListOf()
    main2@ while (steps.size < allSteps.size) {
        step@ for (step in allStepsSorted) {
            val dependencies = dep[step] ?: setOf()
            for (dependency in dependencies) {
                if (!steps.contains(dependency)) {
                    continue@step
                }
            }
            steps.addLast(step)
            allStepsSorted.remove(step)
            continue@main2
        }
        println("Deadlock reached; steps so far: $steps")
        exitProcess(2)
    }
    println("Steps to be executed in this order: ${steps.joinToString("")}")


    // Part 2
    allStepsSorted = allSteps.toSortedSet()
    val stepsCompleted: MutableList<Char> = mutableListOf()
    val workers: MutableMap<Char, Int> = mutableMapOf()
    var secondsElapsed = 0
    main@ while (stepsCompleted.size < allSteps.size) {
        step@ for (step in allStepsSorted) {
            val dependencies = dep[step] ?: setOf()
            for (dependency in dependencies) {
                if (!stepsCompleted.contains(dependency)) {
                    continue@step
                }
            }
            if (workers.size < 5) {
                workers[step] = 61 + (step - 'A')
            }
        }
        val workersToRemove: MutableSet<Char> = mutableSetOf()
        for (worker in workers) {
            allStepsSorted.remove(worker.key)
            worker.setValue(worker.value - 1)
            if (worker.value <= 0) {
                workersToRemove.add(worker.key)
            }
        }
        for (workerToRemove in workersToRemove) {
            workers.remove(workerToRemove)
            stepsCompleted.add(workerToRemove)
        }
        secondsElapsed++
    }
    println("Seconds elapsed: $secondsElapsed")
}
