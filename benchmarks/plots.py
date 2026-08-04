import csv
import re
import matplotlib.pyplot as plt
from collections import defaultdict

data = defaultdict(lambda: {"size": [], "gflops": []})

with open("results/csv/benchmark.csv", newline="") as f:
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
    # ordenar por tamaño
    sizes, gflops = zip(*sorted(
        zip(values["size"], values["gflops"])
    ))

    plt.plot(
        sizes,
        gflops,
        marker="o",
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
