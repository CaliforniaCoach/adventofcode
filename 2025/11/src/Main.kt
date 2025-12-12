import java.io.File

class Device(listEntry: String, val devices: Map<String, Device>) {
    val name = listEntry.split(": ").first()
    val outputDevices = listEntry.split(": ").last().split(" ").toMutableList()
    var completed = false
    // 0 is subsequent paths containing neither, 1 containing "dac", 2 containing "fft", 3 containing both
    var numSubsequentPaths = arrayOf(0L, 0L, 0L, 0L)

    fun findPaths(): Array<Long> {
        if (completed) {
            return numSubsequentPaths
        }
        for (outputDevice in outputDevices) {
            if (outputDevice == "out") {
                numSubsequentPaths[0]++
            } else {
                val result = devices[outputDevice]!!.findPaths()
                numSubsequentPaths = numSubsequentPaths.zip(result).map { it.first + it.second }.toTypedArray()
            }
        }
        if (name == "dac") {
            numSubsequentPaths[3] = numSubsequentPaths[2]
            numSubsequentPaths[1] = numSubsequentPaths[0]
        } else if (name == "fft") {
            numSubsequentPaths[3] = numSubsequentPaths[1]
            numSubsequentPaths[2] = numSubsequentPaths[0]
        }
        completed = true
        return numSubsequentPaths
    }
}

fun main() {
    val devices = mutableMapOf<String, Device>()
    File("input.txt").forEachLine() {
        devices[it.split(": ").first()] = Device(it.toString(), devices)
    }
    println(devices["you"]!!.findPaths().sum())
    println(devices["svr"]!!.findPaths()[3])
}
