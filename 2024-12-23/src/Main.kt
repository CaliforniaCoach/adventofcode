import java.io.File

fun main() {
    val pairs = File("input.txt").readLines().map { it.split('-').toSet() }
    var triples: MutableSet<Set<String>> = mutableSetOf()
    for (i1 in pairs.indices) {
        for (i2 in i1 + 1..<pairs.size) {
            if (pairs[i1].any { it in pairs[i2] }) {
                triples.add(pairs[i1].toSet().union(pairs[i2].toSet()))
                assert(triples.last().size == 3)
            }
        }
    }
    triples = triples.filter {
        val l = it.toList()
        listOf(setOf(l[0], l[1]), setOf(l[1], l[2]), setOf(l[0], l[2])).all {
            pair -> pairs.contains(pair)
        }
    }.toMutableSet()
    val numStartWithT = triples.count { it.any { it2 -> it2.startsWith('t') } }
    println("Triples with at least one starting with 't': $numStartWithT")

    val groups: MutableList<MutableSet<String>> = mutableListOf()
    for (pair in pairs.map { it.toList() }) {
        for ((c1, c2) in listOf(Pair(pair.first(), pair.last()), Pair(pair.last(), pair.first()))) {
            for (group in groups) {
                if (c1 in group && c2 !in group) {
                    if (group.all { setOf(it, c2) in pairs }) {
                        group.add(c2)
                    }
                }
            }
            if (!groups.any { it.contains(c1) }) {
                groups.add(mutableSetOf(c1, c2))
            }
        }
    }
    val maxGroupSize = groups.maxOf { it.size }
    val largest = groups.filter { it.size == maxGroupSize }.toSet()
    assert(largest.size == 1)
    println("Password: ${largest.first().sorted().joinToString(",")}")
}
