def extendedEuclid(a, b):
    if b == 0:
        return (a, 1, 0)
    else:
        res = extendedEuclid(b, a % b)
        return res[0], res[2], res[1] - (a // b) * res[2]

def solve(a, b, c):
    (gcd, s, t) = extendedEuclid(a, b)
    if c % gcd != 0:
        return None
    x0 = s * c // gcd
    y0 = t * c // gcd
    cycleX = b // gcd
    cycleY = a // gcd
    return (x0, y0, cycleX, cycleY)

# 
# fun positiveModulo(a: Long, b: Long): Long {
#     return (a % b + b) % b
# }
# 
# fun ceilDiv(a: Long, b: Long): Long {
#     return if (a >= 0L) (a + b - 1) / b else a / b
# }
# 

btnAX = 94
btnBX = 22
btnAY = 34
btnBY = 67

prizeX = 10000000008400
prizeY = 10000000005400

(ax0, bx0, cax, cbx) = solve(btnAX, btnBX, prizeX)
(ay0, by0, cay, cby) = solve(btnAY, btnBY, prizeY)

# cycleAX = 11
# cycleBX = 47
# cycleAY = 67
# cycleBY = 34
# ax0 = 20000000016800
# bx0 = -85000000071400
# ay0 = 20000000010800
# by0 = -10000000005400

print(solve(cax - cbx, cby - cay, ay0 + by0 - ax0 - bx0))
