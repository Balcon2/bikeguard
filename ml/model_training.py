"""Train and persist a transparent BikeGuard crash-event baseline model."""

from __future__ import annotations

import argparse
from pathlib import Path

import joblib
from sklearn.ensemble import RandomForestClassifier
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler

from .data_preprocessing import load_dataset, split_dataset


def build_model(random_state: int = 42) -> Pipeline:
    """Build the initial offline baseline.

    A random forest is intentionally used as an interpretable first benchmark,
    not as a claim that it is suitable for TinyML or safety-critical use.
    """
    return Pipeline(
        [
            ("scale", StandardScaler()),
            (
                "classifier",
                RandomForestClassifier(
                    n_estimators=100,
                    random_state=random_state,
                    class_weight="balanced",
                    n_jobs=-1,
                ),
            ),
        ]
    )


def train_model(
    dataset_path: str | Path,
    output_path: str | Path,
    test_size: float = 0.2,
    random_state: int = 42,
) -> None:
    """Train on labeled data and save the complete preprocessing/model pipeline."""
    frame = load_dataset(dataset_path)
    x_train, _, y_train, _ = split_dataset(frame, test_size, random_state)
    model = build_model(random_state)
    model.fit(x_train, y_train)

    destination = Path(output_path)
    destination.parent.mkdir(parents=True, exist_ok=True)
    joblib.dump(model, destination)
    print(f"Saved model to {destination}")
    print(f"Training rows: {len(x_train)}; classes: {sorted(y_train.unique())}")


def main() -> None:
    """Parse command-line arguments for ``python -m ml.model_training``."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("dataset", type=Path, help="Labeled sensor CSV")
    parser.add_argument("output", type=Path, help="Output .joblib model path")
    parser.add_argument("--test-size", type=float, default=0.2)
    parser.add_argument("--random-state", type=int, default=42)
    args = parser.parse_args()
    train_model(args.dataset, args.output, args.test_size, args.random_state)


if __name__ == "__main__":
    main()
