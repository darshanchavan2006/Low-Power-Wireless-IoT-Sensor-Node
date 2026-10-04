"""
RF Link Characterization Analysis
---------------------------------
Reads rf_characterization_sample_data.csv and produces:
1. Distance vs average RSSI
2. Distance vs Packet Delivery Ratio (PDR)
3. Distance vs average latency
4. A summary CSV

IMPORTANT:
The included CSV contains representative SAMPLE data for testing the
analysis pipeline. Replace it with measurements from your NodeMCU setup
before using the results in a report/resume.
"""

import pandas as pd
import matplotlib.pyplot as plt

INPUT_FILE = "rf_characterization_sample_data.csv"

df = pd.read_csv(INPUT_FILE)

# Basic validation
required = {"distance_m", "packet", "rssi_dbm", "latency_ms", "status"}
missing = required - set(df.columns)

if missing:
    raise ValueError(f"Missing columns: {sorted(missing)}")

# Convert status into 1/0 for PDR calculation
df["received"] = (df["status"].str.upper() == "RECEIVED").astype(int)

# Summary by test distance
summary = (
    df.groupby("distance_m")
      .agg(
          packets_sent=("packet", "count"),
          packets_received=("received", "sum"),
          average_rssi_dbm=("rssi_dbm", "mean"),
          average_latency_ms=("latency_ms", "mean")
      )
      .reset_index()
)

summary["packets_lost"] = (
    summary["packets_sent"] - summary["packets_received"]
)

summary["pdr_percent"] = (
    summary["packets_received"] / summary["packets_sent"] * 100
)

summary = summary[
    [
        "distance_m",
        "packets_sent",
        "packets_received",
        "packets_lost",
        "pdr_percent",
        "average_rssi_dbm",
        "average_latency_ms"
    ]
]

print("\nRF LINK CHARACTERIZATION SUMMARY")
print("=" * 70)
print(summary.round(2).to_string(index=False))

summary.to_csv("rf_characterization_summary.csv", index=False)

# 1. Distance vs RSSI
plt.figure(figsize=(8, 5))
plt.plot(
    summary["distance_m"],
    summary["average_rssi_dbm"],
    marker="o"
)
plt.xlabel("Distance (m)")
plt.ylabel("Average RSSI (dBm)")
plt.title("Distance vs Average RSSI")
plt.grid(True)
plt.tight_layout()
plt.savefig("distance_vs_rssi.png", dpi=300)
plt.show()

# 2. Distance vs PDR
plt.figure(figsize=(8, 5))
plt.plot(
    summary["distance_m"],
    summary["pdr_percent"],
    marker="o"
)
plt.xlabel("Distance (m)")
plt.ylabel("Packet Delivery Ratio (%)")
plt.title("Distance vs Packet Delivery Ratio")
plt.ylim(0, 105)
plt.grid(True)
plt.tight_layout()
plt.savefig("distance_vs_pdr.png", dpi=300)
plt.show()

# 3. Distance vs latency
plt.figure(figsize=(8, 5))
plt.plot(
    summary["distance_m"],
    summary["average_latency_ms"],
    marker="o"
)
plt.xlabel("Distance (m)")
plt.ylabel("Average Latency (ms)")
plt.title("Distance vs Average Latency")
plt.grid(True)
plt.tight_layout()
plt.savefig("distance_vs_latency.png", dpi=300)
plt.show()

print("\nAnalysis complete.")
print("Generated:")
print("- rf_characterization_summary.csv")
print("- distance_vs_rssi.png")
print("- distance_vs_pdr.png")
print("- distance_vs_latency.png")
