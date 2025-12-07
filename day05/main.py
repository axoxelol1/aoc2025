# Limited time today so solved in python, will do c++ during the weekend
import time

start_time = time.perf_counter()
with open("input.txt") as f:
    input_ranges, ids = f.read().split("\n\n")
    input_ranges = [
        (int(r[0]), int(r[1]))
        for r in [line.split("-") for line in input_ranges.splitlines()]
    ]
    ids = [int(id) for id in ids.splitlines()]

print(len(input_ranges), len(ids))
input_ranges.sort(key=lambda x: x[0])
curr_range = input_ranges[0]
merged_ranges = []
for i in range(1, len(input_ranges)):
    next_range = input_ranges[i]
    if curr_range[1] >= next_range[0]:
        curr_range = (curr_range[0], max(curr_range[1], next_range[1]))
        if i == len(input_ranges) - 1:
            merged_ranges.append(curr_range)
    else:
        merged_ranges.append(curr_range)
        curr_range = next_range

fresh = 0
for id in ids:
    for r in merged_ranges:
        if r[0] <= id <= r[1]:
            fresh += 1
            break

fresh_p2 = 0
for range in merged_ranges:
    fresh_p2 += range[1] - range[0] + 1

end_time = time.perf_counter()

print(f"Part 1: {fresh}")
print(f"Part 2: {fresh_p2}")
print(f"Execution time: {(end_time - start_time) * 1000:.4f} ms")
