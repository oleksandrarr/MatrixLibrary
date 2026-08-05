import csv
import re
from collections import defaultdict

import matplotlib.pyplot as plt


data = defaultdict(dict)

# Leer CSV de Google Benchmark
with open("results/csv/benchmark.csv", newline="") as f:

    for line in f:
        if line.startswith("name,"):
            header = next(csv.reader([line]))
            break

    reader = csv.DictReader(f, fieldnames=header)

    for row in reader:
        match = re.match(r"(.+)/(\d+)", row["name"])

        if match:
            impl = match.group(1)
            size = int(match.group(2))
            gflops = float(row["GFLOPS"].replace("/s", ""))

            data[impl][size] = gflops


baseline_name = "Naive (IJK)"

if baseline_name not in data:
    raise RuntimeError(
        f"No se encontró la baseline '{baseline_name}'"
    )


baseline = data[baseline_name]


plt.figure(figsize=(10, 6))


for impl, results in data.items():

    sizes = sorted(results.keys())

    speedups = [
        results[size] / baseline[size]
        for size in sizes
    ]

    if impl == baseline_name:
        plt.plot(
            sizes,
            speedups,
            marker="o",
            linewidth=3,
            label=impl
        )
    else:
        plt.plot(
            sizes,
            speedups,
            marker="o",
            label=impl
        )


# línea base
plt.axhline(
    y=1,
    linestyle=":",
    linewidth=1
)


plt.xscale("log", base=2)

plt.xlabel("Matrix size (N x N)")
plt.ylabel("Normalized speedup")
plt.title(
    "Matrix multiplication speedup normalized to Naive (IJK)"
)

plt.grid(True)
plt.legend()

plt.tight_layout()

plt.savefig(
    "results/plots/speedup_normalized.png",
    dpi=300
)

plt.show()
