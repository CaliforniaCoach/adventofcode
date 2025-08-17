import java.io.File
import kotlin.math.absoluteValue

fun main() {
    val list1: MutableList<Int> = mutableListOf()
    val list2: MutableList<Int> = mutableListOf()
    for (line in File("input.txt").readLines()) {
        val tok = line.split(' ')
        list1.add(tok.first().toInt())
        list2.add(tok.last().toInt())
    }
    list1.sort()
    list2.sort()
    var dist = 0
    for ((x1, x2) in list1.zip(list2)) {
        dist += (x1 - x2).absoluteValue
    }
    println("Total distance: $dist")

    var similarity = 0
    for (x1 in list1) {
        similarity += x1 * list2.count { it == x1 }
    }
    println("Similarity: $similarity")
}
