import re;print(sum((I:=[*map(int,re.findall(r"\d+", l))])[0] // 3 * I[1] // 3 >= sum(I[2:]) for l in open("input.txt").read().splitlines()[30:]))
