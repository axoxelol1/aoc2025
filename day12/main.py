import sys


def parse(file: str):
    with open(file, "r") as f:
        *shapes_input, trees_input = f.read().split("\n\n")
    return (
        [
            sum([sum([c == "#" for c in r]) for r in shape_input.splitlines()[1:4]])
            for shape_input in shapes_input
        ],
        [
            ([int(d) for d in dim[:-1].split("x")] + [int(x) for x in counts])
            for dim, *counts in [
                tree_input.split() for tree_input in trees_input.splitlines()
            ]
        ],
    )


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Give file")
        exit()
    file = sys.argv[1]
    shape_counts, trees = parse(file)
    p1 = 0
    yes = 0
    maybe = 0
    no = 0
    for w, h, *counts in trees:
        spaces = sum([shape_counts[i] * c for i, c in enumerate(counts)])
        per_row = w // 3
        n_rows = h // 3
        min_spaces = 9 * per_row * n_rows
        if min_spaces >= sum(counts) * 9:
            yes += 1
        elif min_spaces < spaces and spaces < w * h:
            maybe += 1
        else:
            no += 1
    print(f"Part 1: {yes}, {maybe}, {no}")
