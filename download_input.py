import os
import sys
from pathlib import Path

import requests
from dotenv import load_dotenv


def load_session_token() -> str | None:
    load_dotenv()
    return os.getenv("SESSION_TOKEN")


def download_input(day_path: str):
    session_token = load_session_token()
    if not session_token:
        print("Error: SESSION_TOKEN not found in .env file")
        sys.exit(1)

    day_dir = Path(day_path)
    day = day_dir.name.split("-")[1]
    year = day_dir.parent.name

    url = f"https://adventofcode.com/{year}/day/{int(day)}/input"
    headers = {"Cookie": f"session={session_token}"}

    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        output_file = day_dir / "input.txt"
        output_file.write_text(response.text)
    else:
        print(f"Error downloading input: {response.status_code}")
        sys.exit(1)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python download_input.py <path/to/day-XX>")
        sys.exit(1)

    download_input(sys.argv[1])
