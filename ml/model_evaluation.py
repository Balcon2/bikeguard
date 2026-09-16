"""Evaluate a saved BikeGuard model on a labeled holdout split."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any

import joblib
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix

from .data_preprocessing import load_dataset, split_dataset


def evaluate_model(
    model_path: str | Path,
    dataset_path: str | Path,
    test_size: float = 0.2,
    random_state: int = 42,
) -> dict[str, Any]:
    """Return metrics for a reproducible holdout evaluation."""
    model = joblib.load(model_path)
    frame = load_dataset(dataset_path)
    _, x_test, _, y_test = split_dataset(frame, test_size, random_state)
    predictions = model.predict(x_test)
    labels = sorted(set(y_test) | set(predictions))

    return {
        "rows_evaluated": len(y_test),
        "accuracy": float(accuracy_score(y_test, predictions)),
        "labels": labels,
        "confusion_matrix": confusion_matrix(
            y_test, predictions, labels=labels
        ).tolist(),
        "classification_report": classification_report(
            y_test, predictions, labels=labels, zero_division=0, output_dict=True
        ),
    }


def main() -> None:
    """Evaluate a model and print JSON metrics to stdout or a file."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("model", type=Path, help="Saved .joblib model")
    parser.add_argument("dataset", type=Path, help="Labeled sensor CSV")
    parser.add_argument("--output", type=Path, help="Optional JSON output path")
    parser.add_argument("--test-size", type=float, default=0.2)
    parser.add_argument("--random-state", type=int, default=42)
    args = parser.parse_args()
    metrics = evaluate_model(
        args.model, args.dataset, args.test_size, args.random_state
    )
    rendered = json.dumps(metrics, indent=2)
    if args.output:
        args.output.write_text(rendered + "\n", encoding="utf-8")
        print(f"Saved metrics to {args.output}")
    else:
        print(rendered)


if __name__ == "__main__":
    main()
