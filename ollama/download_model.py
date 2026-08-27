# rev-a7b31e-20260827 download_model.py
#!/usr/bin/env python3
"""
Download DeepSeek V4 Flash Local 0731 GGUF model from HuggingFace.

Usage:
    python download_model.py --quant q4_k_m
    python download_model.py --quant q8_0 --output ./models
"""

import argparse
import subprocess
import sys
from pathlib import Path

MODEL_ID = "deepseek-ai/DeepSeek-V4-Flash-0731-GGUF"

QUANTIZATIONS = {
    "q4_k_m": "deepseek-v4-flash-0731-Q4_K_M.gguf",
    "q5_k_m": "deepseek-v4-flash-0731-Q5_K_M.gguf",
    "q6_k": "deepseek-v4-flash-0731-Q6_K.gguf",
    "q8_0": "deepseek-v4-flash-0731-Q8_0.gguf",
    "f16": "deepseek-v4-flash-0731-F16.gguf",
}


def main():
    parser = argparse.ArgumentParser(description="Download DeepSeek V4 Flash Local 0731")
    parser.add_argument("--quant", choices=list(QUANTIZATIONS.keys()),
                        default="q4_k_m", help="Quantization level")
    parser.add_argument("--output", default="./models", help="Output directory")
    args = parser.parse_args()

    filename = QUANTIZATIONS[args.quant]
    output_dir = Path(args.output)
    output_dir.mkdir(parents=True, exist_ok=True)

    print(f"Downloading DeepSeek V4 Flash Local 0731 ({args.quant})...")
    print(f"Model: {MODEL_ID}")
    print(f"File: {filename}")
    print(f"Output: {output_dir}")

    try:
        from huggingface_hub import hf_hub_download
    except ImportError:
        subprocess.check_call([sys.executable, "-m", "pip", "install", "huggingface_hub"])
        from huggingface_hub import hf_hub_download

    path = hf_hub_download(
        repo_id=MODEL_ID,
        filename=filename,
        local_dir=str(output_dir),
    )

    print(f"\nDownloaded to: {path}")
    print(f"Load this file in the DeepSeek V4 Flash Local desktop app.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
