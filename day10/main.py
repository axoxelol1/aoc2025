import re
from itertools import combinations
import z3

with open("input.txt") as f:
    matches = re.findall(r"^\[([.#]+)\] (.*) {(.*)}$", f.read(), re.MULTILINE)
    machines = [
        (
            [c == "#" for c in match[0]],
            [[int(i) for i in button[1:-1].split(",")] for button in match[1].split()],
            [int(joltage) for joltage in match[2].split(",")],
        )
        for match in matches
    ]


def least_presses_p1(machine):
    goal_lights, buttons, _ = machine
    for clicked in range(1, len(buttons) + 1):
        for pressed_indices in combinations(range(len(buttons)), clicked):
            lights = [False] * len(goal_lights)
            for idx in pressed_indices:
                for light in buttons[idx]:
                    lights[light] = not lights[light]
            if lights == goal_lights:
                return clicked
    return 10000000


def least_presses_p2(machine):
    _, buttons, joltages = machine
    s = z3.Solver()
    bs = [z3.Int("b%s" % i) for i in range(len(buttons))]
    for b in bs:
        s.add(b >= 0)
    for jolt_idx in range(len(joltages)):
        relevant_bs = [bs[i] for i in range(len(bs)) if jolt_idx in buttons[i]]
        s.add(z3.Sum(relevant_bs) == joltages[jolt_idx])
    presses = 1
    while True:
        s.push()
        s.add(z3.Sum(bs) == presses)
        if s.check() == z3.sat:
            return presses
        s.pop()
        presses += 1


print("Part 1: ", sum([least_presses_p1(machine) for machine in machines]))
print("Part 2: ", sum([least_presses_p2(machine) for machine in machines]))
