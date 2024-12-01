import java.io.File

fun main() {
    val timestamp = "\\[\\d{4}-\\d{2}-\\d{2} \\d{2}:(\\d{2})\\]"
    val shiftStart = (timestamp + " Guard #(\\d+) begins shift").toRegex()
    val fallAsleep = (timestamp + " falls asleep").toRegex()
    val wakeUp = (timestamp + " wakes up").toRegex()

    val records = File("input.txt").readLines().toMutableList()
    records.sort()

    val totalSleepMinutes = mutableMapOf<Int, Int>()
    val minuteMap = mutableMapOf<Int, IntArray>()

    var currentGuard = 0
    for (record in records) {
        shiftStart.matchEntire(record)?.let {
            currentGuard = it.groupValues[2].toInt()
            if (!minuteMap.containsKey(currentGuard)) {
                minuteMap[currentGuard] = IntArray(60)
                totalSleepMinutes[currentGuard] = 0
            }
        }
        fallAsleep.matchEntire(record)?.let {
            val eventMinute = it.groupValues[1].toInt()
            for (minute in eventMinute..<60) {
                minuteMap[currentGuard]!![minute]++
            }
            totalSleepMinutes[currentGuard] = totalSleepMinutes[currentGuard]!! + (60 - eventMinute)
        }
        wakeUp.matchEntire(record)?.let {
            val eventMinute = it.groupValues[1].toInt()
            for (minute in eventMinute..<60) {
                minuteMap[currentGuard]!![minute]--
            }
            totalSleepMinutes[currentGuard] = totalSleepMinutes[currentGuard]!! - (60 - eventMinute)
        }
    }

    val (id, sleepMinutes) = totalSleepMinutes.maxBy { it.value }
    val maxMinute = minuteMap[id]!!.indices.maxBy { minuteMap[id]!![it]}
    println("Guard #${id} slept ${sleepMinutes} minutes, most of which in minute ${maxMinute} -> answer is: ${id * maxMinute}")

    val (maxIdSleepingInAMinute, _) = minuteMap.map { it.key to it.value.max() }.maxBy { it.second }
    val minute = minuteMap[maxIdSleepingInAMinute]!!.indices.maxBy { minuteMap[maxIdSleepingInAMinute]!![it]}
    println("Guard #${maxIdSleepingInAMinute} sleeps longest time per minute in minute ${minute} -> answer is: ${maxIdSleepingInAMinute * minute}")
}
