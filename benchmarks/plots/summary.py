import csv
import re
import sys
from collections import defaultdict

data = defaultdict(dict)

benchmarks = sys.argv[1:]
print("ARGS:", benchmarks)

for bench in benchmarks:
    filename = f"results/csv/{bench}_benchmark.csv"
    print(f"Reading {filename}")

    with open(filename, newline="") as f:
        for line in f:
            if line.startswith("name,"):
                header = next(csv.reader([line]))
                break

        reader = csv.DictReader(f, fieldnames=header)

        for row in reader:
            name = row["name"]

            match = re.match(r"(.+)/(\d+)", name)
            if not match:
                continue

            implementation = match.group(1)
            size = int(match.group(2))

            gflops = float(row["GFLOPS"])

            data[size][implementation] = gflops


for bench in benchmarks:

    out_file = f"results/csv/{bench}_summary.csv"

    with open(out_file, "w", newline="") as f:
        writer = csv.writer(f)

        writer.writerow([
            "Size",
            "Implementation",
            "GFLOPS",
            "Speedup",
            "Improvement (%)"
        ])

        for size in sorted(data):

            baseline = data[size]["Naive (IJK)"]

            for impl, perf in sorted(data[size].items()):

                speedup = perf / baseline
                improvement = (perf - baseline) / baseline * 100.0

                writer.writerow([
                size,
                impl,
                f"{perf:.3f}".replace(".", ","),
                f"{speedup:.3f}".replace(".", ","),
                f"{improvement:.1f}".replace(".", ",")
            ])

print("Summary generated in /results/csv.")
