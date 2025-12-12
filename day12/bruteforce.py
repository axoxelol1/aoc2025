import sys
import numpy as np
import numpy.typing as npt

# This does not even finish running for the third example


def parse(file: str):
    with open(file, "r") as f:
        *shapes_input, trees_input = f.read().split("\n\n")
    return (
        [
            np.array(
                [
                    [1 if c == "#" else 0 for c in r]
                    for r in shape_input.splitlines()[1:4]
                ]
            )
            for shape_input in shapes_input
        ],
        [
            ([int(d) for d in dim[:-1].split("x")] + [int(x) for x in counts])
            for dim, *counts in [
                tree_input.split() for tree_input in trees_input.splitlines()
            ]
        ],
    )


def generate_variations(matrix):
    unique_variations = set()
    for k in range(4):
        rotated_matrix = np.rot90(matrix, k)
        unique_variations.add(tuple(map(tuple, rotated_matrix)))
        flipped_matrix = np.flip(rotated_matrix, axis=1)
        unique_variations.add(tuple(map(tuple, flipped_matrix)))
    return [np.array(t) for t in unique_variations]


def can_fit(grid: npt.NDArray[np.int64], presents_left: list[int], variations):
    if all([c == 0 for c in presents_left]):
        return True
    nrows, ncols = grid.shape
    first_present_idx = next(i for i, x in enumerate(presents_left) if x > 0)
    new_presents_left = presents_left.copy()
    new_presents_left[first_present_idx] -= 1
    for i in range(nrows - 2):
        for j in range(ncols - 2):
            for variant in variations[first_present_idx]:
                if np.any(grid[i : i + 3, j : j + 3] + variant > 1):
                    continue
                new_grid = grid.copy()
                new_grid[i : i + 3, j : j + 3] += variant
                if can_fit(new_grid, new_presents_left, variations):
                    return True
    return False


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Give file")
        exit()
    file = sys.argv[1]
    shapes, trees = parse(file)
    all_variations = [generate_variations(s) for s in shapes]
    for w, h, *presents_left in trees:
        print(can_fit(np.zeros((h, w), dtype=int), presents_left, all_variations))
