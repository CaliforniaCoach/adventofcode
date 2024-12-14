import java.io.File
import kotlin.math.ceil
import kotlin.math.floor
import kotlin.math.min

fun main() {
    val reBtnA = "Button A: X\\+(\\d+), Y\\+(\\d+)".toRegex()
    val reBtnB = "Button B: X\\+(\\d+), Y\\+(\\d+)".toRegex()
    val rePrize = "Prize: X=(\\d+), Y=(\\d+)".toRegex()

    val reader = File("test.txt").bufferedReader()
    var tokensNeeded = 0L
    var tokensNeeded2 = 0L
    while (reader.ready()) {
        var m = reBtnA.matchEntire(reader.readLine())
        val (btnAX, btnAY) = m!!.groupValues.subList(1, 3).map { it.toLong() }
        m = reBtnB.matchEntire(reader.readLine())
        val (btnBX, btnBY) = m!!.groupValues.subList(1, 3).map { it.toLong() }
        m = rePrize.matchEntire(reader.readLine())
        val (prizeX, prizeY) = m!!.groupValues.subList(1, 3).map { it.toLong() }
        reader.readLine()

        print("btnAX $btnAX, btnAY $btnAY, btnBX $btnBX, btnBY $btnBY, prizeX $prizeX, prizeY $prizeY -> ")
        tokensNeeded += calcButtonPresses(btnAX, btnAY, btnBX, btnBY, prizeX, prizeY)
        tokensNeeded2 += calcButtonPresses(btnAX, btnAY, btnBX, btnBY, prizeX + 10000000000000L, prizeY + 10000000000000L)
        println("So far: $tokensNeeded, $tokensNeeded2")
    }
    println("Minimum tokens needed: $tokensNeeded")
}

fun calcButtonPresses(btnAX: Long, btnAY: Long, btnBX: Long, btnBY: Long, prizeX: Long, prizeY: Long): Long {
    val retvalX = solve(btnAX, btnBX, prizeX)
    val retvalY = solve(btnAY, btnBY, prizeY)
    if (retvalX == null || retvalY == null) {
        return 0
    }
    val (solutionAX, solutionBX) = retvalX.first
    val (cycleAX, cycleBX) = retvalX.second
    val (solutionAY, solutionBY) = retvalY.first
    val (cycleAY, cycleBY) = retvalY.second

    val (gcdX, baseAX, baseBX) = extendedEuclid(btnAX, btnBX)
    val (gcdY, baseAY, baseBY) = extendedEuclid(btnAY, btnBY)

    val retval = solve(cycleAX, -cycleAY, solutionAY - solutionAX)
    if (retval == null) {
        return 0
    }
    val (i0, k0) = retval.first
    var (iCycle, kCycle) = retval.second
    if (iCycle < 0) {
        iCycle = -iCycle
        kCycle = -kCycle
    }
    var i = 0L
    var k = 0L
    if (iCycle != 0L) {
        // Choose smallest possible start value for i so that <a> is just >= 0...
        i = ceilDiv(-solutionAX, cycleAX)
        // ... and so that it is in solution space for <i> and <k>
        i += positiveModulo(i0 - i, iCycle)
        k = k0 - (i - i0) / iCycle * kCycle
    } else {
        i = i0
    }
    while (true) {
        val a = solutionAX + i * cycleAX
        val b = solutionBX - i * cycleBX
        print("a = $a, b = $b -> ")
        if (b < 0) {
            break
        }
        return 3 * a + b
        if ((a - solutionAY) % (btnBY / gcdY) == 0L) {
            val k = k0 - (i - i0) / iCycle * kCycle
            if (b == solutionBY - k * btnAY / gcdY) {
                return 3 * a + b
            }
        }
        i += iCycle
    }
    return 0
}

fun extendedEuclid(a: Long, b: Long): LongArray {
    return if (b == 0L) {
        longArrayOf(a, 1L, 0L)
    } else {
        val res = extendedEuclid(b, a.rem(b))
        longArrayOf(res[0], res[2], res[1] - (a / b) * res[2])
    }
}

/**
 * Solves ax + by = c for given a, b, c
 */
fun solve(a: Long, b: Long, c: Long): Pair<Pair<Long, Long>, Pair<Long, Long>>? {
    val (gcd, s, t) = extendedEuclid(a, b)
    if (c % gcd != 0L) {
        return null
    }
    val x0 = s * c / gcd
    val y0 = t * c / gcd
    val cycleX = b / gcd
    val cycleY = a / gcd
    return Pair(Pair(x0, y0), Pair(cycleX, cycleY))
}

fun positiveModulo(a: Long, b: Long): Long {
    return (a % b + b) % b
}

fun ceilDiv(a: Long, b: Long): Long {
    return if (a >= 0L) (a + b - 1) / b else a / b
}
