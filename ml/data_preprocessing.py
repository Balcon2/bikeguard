"""Load and validate BikeGuard sensor CSV files for offline ML experiments."""

from __future__ import annotations

from pathlib import Path
from typing import Tuple

import pandas as pd
from sklearn.model_selection import train_test_split

# These are the features produced by the current firmware prototype.
FEATURE_COLUMNS = [
    "accel_x_g",
    "accel_y_g",
    "accel_z_g",
    "gyro_x_dps",
    "gyro_y_dps",
    "gyro_z_dps",
    "speed_kmh",
]
TARGET_COLUMN = "event_label"
REQUIRED_COLUMNS = ["timestamp_ms", *FEATURE_COLUMNS, TARGET_COLUMN]

# Permit files using the shorter names from the original planned schema.
COLUMN_ALIASES = {
    "timestamp": "timestamp_ms",
    "accel_x": "accel_x_g",
    "accel_y": "accel_y_g",
    "accel_z": "accel_z_g",
    "gyro_x": "gyro_x_dps",
    "gyro_y": "gyro_y_dps",
    "gyro_z": "gyro_z_dps",
    "wheel_speed": "speed_kmh",
}


def _normalise_column_names(frame: pd.DataFrame) -> pd.DataFrame:
    """Rename supported legacy/planned column names to the firmware schema."""
    rename_map = {
        column: COLUMN_ALIASES[column]
        for column in frame.columns
        if column in COLUMN_ALIASES
    }
    return frame.rename(columns=rename_map)


def load_dataset(path: str | Path) -> pd.DataFrame:
    """Read a labeled CSV and reject incomplete or non-numeric sensor data.

    The repository currently contains no dataset. Callers must provide a real
    recording that has been manually labeled; this function never creates
    sample rows or guesses labels.
    """
    csv_path = Path(path)
    if not csv_path.is_file():
        raise FileNotFoundError(f"Dataset does not exist: {csv_path}")

    frame = _normalise_column_names(pd.read_csv(csv_path))
    missing = sorted(set(REQUIRED_COLUMNS) - set(frame.columns))
    if missing:
        raise ValueError(
            "Dataset is missing required columns: " + ", ".join(missing)
        )

    frame = frame[REQUIRED_COLUMNS].copy()
    frame[TARGET_COLUMN] = frame[TARGET_COLUMN].astype("string").str.strip()
    frame = frame[frame[TARGET_COLUMN].notna() & (frame[TARGET_COLUMN] != "")]
    if frame.empty:
        raise ValueError("Dataset contains no labeled rows.")

    for column in ["timestamp_ms", *FEATURE_COLUMNS]:
        frame[column] = pd.to_numeric(frame[column], errors="coerce")
    frame = frame.dropna(subset=["timestamp_ms", *FEATURE_COLUMNS])
    if frame.empty:
        raise ValueError("Dataset contains no complete numeric sensor rows.")

    return frame.reset_index(drop=True)


def split_features_and_target(frame: pd.DataFrame) -> Tuple[pd.DataFrame, pd.Series]:
    """Return model inputs and labels from a validated frame."""
    return frame[FEATURE_COLUMNS], frame[TARGET_COLUMN]


def split_dataset(
    frame: pd.DataFrame,
    test_size: float = 0.2,
    random_state: int = 42,
) -> Tuple[pd.DataFrame, pd.DataFrame, pd.Series, pd.Series]:
    """Create a reproducible stratified holdout split.

    Stratification is used when every class has at least two rows. For tiny
    exploratory recordings, a clear non-stratified split is preferable to a
    cryptic sklearn error.
    """
    features, target = split_features_and_target(frame)
    counts = target.value_counts()
    stratify = target if len(counts) > 1 and counts.min() >= 2 else None
    return train_test_split(
        features,
        target,
        test_size=test_size,
        random_state=random_state,
        stratify=stratify,
    )
