
import csv
from pathlib import Path
import matplotlib.pyplot as plt

root = Path(__file__).resolve().parent.parent
output = root / "docs" / "figures"
output.mkdir(parents=True, exist_ok=True)

with (root / "results" / "benchmark.csv").open(newline="") as file:
    rows = [
        {k.strip(): v.strip() for k, v in row.items()}
        for row in csv.DictReader(file)
    ]

def get_rows(name):
    return [r for r in rows if r["experiment"] == name]

fig, ax = plt.subplots(figsize=(8, 5))

for algorithm, label in [
    ("Stoer-Wagner", "Stoer-Wagner"),
    ("Karger", "Karger x20")
]:
    data = sorted(
        [r for r in get_rows("size_scaling") if r["algorithm"] == algorithm],
        key=lambda r: int(r["vertices"])
    )
    ax.plot(
        [int(r["vertices"]) for r in data],
        [float(r["median_time_ns"]) / 1_000_000 for r in data],
        marker="o",
        label=label
    )

ax.set(
    title="Runtime versus Graph Size",
    xlabel="Number of vertices",
    ylabel="Median runtime (ms)"
)
ax.legend()
ax.grid(alpha=0.3)
fig.tight_layout()
fig.savefig(output / "runtime_scaling.png", dpi=200)
plt.close(fig)

data = sorted(
    get_rows("density_multi_graph"),
    key=lambda r: float(r["density"])
)

fig, ax = plt.subplots(figsize=(8, 5))
ax.plot(
    [float(r["density"]) for r in data],
    [float(r["success_rate"]) * 100 for r in data],
    marker="o"
)
ax.set(
    title="Karger Reliability versus Density",
    xlabel="Graph density",
    ylabel="Success rate (%)",
    ylim=(0, 100)
)
ax.grid(alpha=0.3)
fig.tight_layout()
fig.savefig(output / "density_reliability.png", dpi=200)
plt.close(fig)

data = get_rows("density_repetitions")
fig, ax = plt.subplots(figsize=(8, 5))

for density in sorted({float(r["density"]) for r in data}):
    selected = sorted(
        [r for r in data if float(r["density"]) == density],
        key=lambda r: int(r["repetitions"])
    )
    ax.plot(
        [int(r["repetitions"]) for r in selected],
        [float(r["success_rate"]) * 100 for r in selected],
        marker="o",
        label=f"Density {density:.2f}"
    )

ax.set(
    title="Karger Reliability versus Repetitions",
    xlabel="Number of repetitions",
    ylabel="Success rate (%)",
    xticks=[1, 10, 20],
    ylim=(0, 105)
)
ax.legend()
ax.grid(alpha=0.3)
fig.tight_layout()
fig.savefig(output / "repetition_reliability.png", dpi=200)
plt.close(fig)

print("Generated 3 graphs in docs/figures/")
