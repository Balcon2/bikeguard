# BikeGuard offline ML tools

This folder contains the first offline experimentation scaffold. It is not
firmware, does not run on the ESP32-S3, and must not be connected to an
actuator. No dataset or model is included because the project does not yet
have real labeled sensor recordings.

## Files

- `data_preprocessing.py` validates CSV columns, converts sensor values to
  numeric data, removes incomplete rows, and creates a reproducible holdout
  split.
- `model_training.py` trains a first random-forest classification baseline and
  saves the complete scikit-learn pipeline as a `.joblib` file.
- `model_evaluation.py` evaluates that saved pipeline and writes accuracy,
  confusion-matrix, and per-class metrics as JSON.

The loader accepts both the current firmware names (`accel_x_g`,
`speed_kmh`, `timestamp_ms`) and the shorter planned names documented in
`AGENTS.md`. A usable recording must contain:

```text
timestamp_ms,accel_x_g,accel_y_g,accel_z_g,gyro_x_dps,gyro_y_dps,gyro_z_dps,speed_kmh,event_label
```

`event_label` must be assigned during safe, controlled data collection and
annotation. The scripts do not infer labels or create synthetic rows.

## Usage

Install the planned analysis dependencies in your Python environment:

```bash
python -m pip install pandas scikit-learn joblib
```

After a real labeled CSV exists:

```bash
python -m ml.model_training data/recording.csv models/bikeguard_baseline.joblib
python -m ml.model_evaluation models/bikeguard_baseline.joblib data/recording.csv \
  --output evaluation.json
```

The random split is only a first benchmark. Before drawing conclusions, split
future data by ride/session so samples from the same ride cannot appear in
both training and evaluation. Results must not be presented as crash-protection
reliability or certification evidence.
