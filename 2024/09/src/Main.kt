import java.io.File

fun main() {
    var disk: MutableList<Int> = mutableListOf()
    for (line in File("input.txt").readLines()) {
        for (num in line.toCharArray().withIndex()) {
            for (i in 0..<num.value.code - '0'.code) {
                disk.add(if (num.index and 1 == 0) num.index / 2 else -1)
            }
        }
    }
    val origDisk = disk.toList()

    while (true) {
        val firstEmpty = disk.indexOf(-1)
        val lastNonEmpty = disk.indexOfLast { it >= 0 }
        if (firstEmpty >= lastNonEmpty) {
            break
        }
        disk[firstEmpty] = disk[lastNonEmpty]
        disk[lastNonEmpty] = -1
    }

    println("Checksum after defragmentation (individual blocks): ${checksum(disk)}")

    disk = origDisk.toMutableList()
    var id = disk.findLast { it >= 0 }!!
    while (id >= 0) {
        val idxFileStart = disk.indexOf(id)
        val idxFileEnd = disk.lastIndexOf(id)
        val fileLen = idxFileEnd - idxFileStart + 1
        var idxGapStart = disk.indexOf(-1)
        while (idxGapStart < idxFileStart) {
            var idxGapEnd = idxGapStart + 1
            while (disk[idxGapEnd] == -1) {
                idxGapEnd++
            }
            idxGapEnd--
            val gapLen = idxGapEnd - idxGapStart + 1
            if (gapLen >= fileLen) {
                for (i in 0..<fileLen) {
                    disk[idxGapStart + i] = id
                    disk[idxFileStart + i] = -1
                }
                break
            }
            idxGapStart = idxGapEnd + 1
            while (disk[idxGapStart] >= 0) {
                idxGapStart++
            }
        }
        id--
    }

    println("Checksum after defragmentation (whole files): ${checksum(disk)}")
}

fun checksum(disk: List<Int>): Long {
    var checksum = 0L
    for (i in disk.indices) {
        if (disk[i] >= 0) {
            checksum += i * disk[i]
        }
    }
    return checksum
}
