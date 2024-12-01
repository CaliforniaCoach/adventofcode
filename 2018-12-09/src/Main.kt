import java.util.*

const val PLAYERS = 468
const val LAST_MARBLE = 71843

fun main() {
    val circle = LinkedList<Int>()
    circle.add(0)
    val playerScore = LongArray(PLAYERS)
    var currentPlayer = 1

    for (marble in 1..LAST_MARBLE * 100) {
        if (marble % 23 != 0) {
            circle.offerLast(circle.pollFirst())
            circle.offerLast(circle.pollFirst())
            circle.offerFirst(marble)
        } else {
            for (i in 0..<7) {
                circle.offerFirst(circle.pollLast())
            }
            playerScore[currentPlayer] += marble + circle.pollFirst()
        }
        currentPlayer = (currentPlayer + 1) % PLAYERS
        if (marble == LAST_MARBLE) {
            println("Max score after $marble moves: ${playerScore.maxOrNull()}")
        }
    }
    println("Max score after ${LAST_MARBLE * 100} moves: ${playerScore.maxOrNull()}")
}
