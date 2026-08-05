import csv
import re
import sys
import matplotlib.pyplot as plt
from collections import defaultdict

data = defaultdict(lambda: {"size": [], "gflops": []})
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

            # separar implementación y tamaño
            match = re.match(r"(.+)/(\d+)", name)

            if match:
                implementation = match.group(1)
                size = int(match.group(2))

                gflops = float(row["GFLOPS"].replace("/s", ""))

                data[implementation]["size"].append(size)
                data[implementation]["gflops"].append(gflops)


plt.figure(figsize=(10, 6))

for impl, values in data.items():
    sizes, gflops = zip(*sorted(zip(values["size"], values["gflops"])))

    if impl == "Naive (IJK)":
        plt.plot(
            sizes,
            gflops,
            linewidth=3,
            marker="o",
            label=impl
        )
    else:
        plt.plot(
            sizes,
            gflops,
            marker="o",
            alpha=0.7,
            label=impl
        )


plt.xlabel("Matrix size (N x N)")
plt.ylabel("GFLOPS")
plt.title("Matrix multiplication performance")

plt.xscale("log", base=2)
plt.grid(True)
plt.legend()

plt.tight_layout()

plt.savefig("results/plots/gflops_scaling.png", dpi=300)
plt.show()
