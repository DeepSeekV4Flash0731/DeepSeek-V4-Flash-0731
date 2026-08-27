# rev-a7b31e-20260827 start_ollama.py
#!/usr/bin/env python3
"""
Start DeepSeek V4 Flash Local 0731 with Ollama backend.

Usage:
    python start_ollama.py
    python start_ollama.py --model deepseek-v4-flash:0731
"""

import argparse
import subprocess
import sys
import time
import urllib.request


def check_ollama_running(url="http://localhost:11434"):
    try:
        urllib.request.urlopen(f"{url}/api/tags", timeout=5)
        return True
    except Exception:
        return False


def main():
    parser = argparse.ArgumentParser(description="Start DeepSeek V4 Flash Local with Ollama")
    parser.add_argument("--model", default="deepseek-v4-flash:0731")
    parser.add_argument("--url", default="http://localhost:11434")
    args = parser.parse_args()

    if not check_ollama_running(args.url):
        print("Starting Ollama...")
        subprocess.Popen(["ollama", "serve"],
                        creationflags=subprocess.CREATE_NEW_CONSOLE)
        time.sleep(3)

    if not check_ollama_running(args.url):
        print("ERROR: Ollama failed to start. Install from https://ollama.com")
        return 1

    print(f"Pulling model: {args.model}")
    subprocess.run(["ollama", "pull", args.model])

    print(f"\nModel ready! Start the DeepSeek V4 Flash Local desktop app")
    print(f"and connect to: {args.url}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
