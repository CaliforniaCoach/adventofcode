import java.io.File

fun main() {
    // val tokens = "2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2".split(' ').map { it.toInt() }
    val tokens = File("input.txt").readText().trim().split(' ').map { it.toInt() }
    val retval = rek(tokens)
    println("Metadata sum: ${retval.second}")
    println("Root node value: ${retval.third}")
}


fun rek(tokens: List<Int>): Triple<List<Int>, Int, Int> {
    val childNodes = tokens[0]
    val metaEntries = tokens[1]
    var restList = tokens.subList(2, tokens.size)
    var metaDataSum = 0
    val childNodeValues: MutableList<Int> = mutableListOf()
    for (i in 0..<childNodes) {
        val retval = rek(restList)
        restList = retval.first
        metaDataSum += retval.second
        childNodeValues.add(retval.third)
    }
    var nodeValue = 0
    for (i in 0..<metaEntries) {
        val metaDate = restList.first()
        metaDataSum += metaDate
        restList = restList.subList(1, restList.size)
        if (childNodes != 0) {
            if (metaDate >= 1 && metaDate <= childNodeValues.size) {
                nodeValue += childNodeValues[metaDate - 1]
            }
        } else {
            nodeValue += metaDate
        }
    }
    return Triple(restList, metaDataSum, nodeValue)
}
