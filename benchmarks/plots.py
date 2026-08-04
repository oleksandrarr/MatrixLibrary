import csv
import matplotlib.pyplot as plt

names = []
gflops = []

with open("results/csv/benchmark.csv", newline="") as f:

    # Buscar cabecera
    for line in f:
        if line.startswith("name,"):
            header = next(csv.reader([line]))
            break

    reader = csv.DictReader(
        f,
        fieldnames=header
    )

    for row in reader:
        names.append(row["name"])
        gflops.append(float(row["GFLOPS"]))


plt.figure(figsize=(12,6))

plt.bar(names, gflops)

plt.xticks(rotation=90)
plt.ylabel("GFLOPS")
plt.title("Matrix multiplication benchmark")

plt.tight_layout()

plt.savefig("results/plots/gflops.png")
