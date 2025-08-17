import java.io.File

data class Sequence(val diff1: Int, val diff2: Int, val diff3: Int, val diff4: Int)

fun main() {
    val secrets = mutableListOf(File("input.txt").readLines().map { it.toInt() })
    for (i in 0..<2000) {
        secrets.add(secrets.last().map { calcNextSecretNumber(it) })
    }
    println("Sum of 2000th secret numbers: ${secrets.last().sumOf { it.toLong() }} ")

    // Part 2
    val sequencesPerBuyer = Array<MutableMap<Sequence, Int>>(secrets.first().size) { mutableMapOf() }
    val totalBananas: MutableMap<Sequence, Int> = mutableMapOf()
    for (buyer in sequencesPerBuyer.indices) {
        for (i in 4..<secrets.size) {
            val diffSequence = Sequence(
                secrets[i - 3][buyer] % 10 - secrets[i - 4][buyer] % 10,
                secrets[i - 2][buyer] % 10 - secrets[i - 3][buyer] % 10,
                secrets[i - 1][buyer] % 10 - secrets[i - 2][buyer] % 10,
                secrets[i - 0][buyer] % 10 - secrets[i - 1][buyer] % 10
            )
            if (diffSequence !in sequencesPerBuyer[buyer]) {
                sequencesPerBuyer[buyer][diffSequence] = secrets[i][buyer] % 10
                totalBananas[diffSequence] = 0
            }
        }
    }
    for (diffSequence in totalBananas.keys) {
        totalBananas[diffSequence] = sequencesPerBuyer.sumOf { it[diffSequence] ?: 0 }
    }
    println("Maximum number of bananas to earn: ${totalBananas.maxOf { it.value }}")
}

fun calcNextSecretNumber(secret: Int): Int {
    var nextSecret = (secret xor (secret shl 6)) and 0xffffff
    nextSecret = (nextSecret xor (nextSecret shr 5)) and 0xffffff
    nextSecret = (nextSecret xor (nextSecret shl 11)) and 0xffffff
    return nextSecret
}
