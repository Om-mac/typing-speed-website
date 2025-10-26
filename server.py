from flask import Flask, request
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)  # allow cross-origin requests from frontend

# Path to C++ backend executable
BACKEND_CGI_PATH = os.path.join(os.path.dirname(__file__), "backend", "main.cgi")


@app.route("/random-text")
def random_text():
    """
    Returns random text from CSV via C++ backend.
    """
    try:
        print("Python server: calling C++ backend for random text...")
        result = subprocess.run(
            [BACKEND_CGI_PATH],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        # Debug logs
        print("Backend stdout:", result.stdout)
        if result.stderr:
            print("Backend stderr:", result.stderr)

        # Remove any CGI Content-type headers
        lines = result.stdout.splitlines()
        json_lines = [line for line in lines if not line.lower().startswith("content-type")]
        json_text = "\n".join(json_lines).strip()

        if not json_text:
            json_text = '{ "error": "No text returned from backend" }'

        return json_text
    except Exception as e:
        print("Exception in /random-text:", e)
        return '{ "error": "Backend call failed" }'


@app.route("/calculate-wpm", methods=["POST"])
def calculate_wpm():
    """
    Receives typed text and time from frontend,
    sends to C++ backend for WPM calculation,
    and returns JSON { "wpm": <calculated_value> }.
    """
    try:
        data = request.get_json()
        typed_text = data.get("typedText", "")
        time_taken = str(data.get("timeTaken", 1))  # seconds

        print(f"Python server: calculating WPM for '{typed_text}' over {time_taken}s...")

        result = subprocess.run(
            [BACKEND_CGI_PATH, typed_text, time_taken],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        # Debug logs
        print("Backend stdout:", result.stdout)
        if result.stderr:
            print("Backend stderr:", result.stderr)

        # Remove any CGI headers
        lines = result.stdout.splitlines()
        json_lines = [line for line in lines if not line.lower().startswith("content-type")]
        json_text = "\n".join(json_lines).strip()

        if not json_text:
            json_text = '{ "error": "WPM calculation failed" }'

        return json_text
    except Exception as e:
        print("Exception in /calculate-wpm:", e)
        return '{ "error": "Server failed" }'


if __name__ == "__main__":
    print("Starting Python Flask server on http://127.0.0.1:5000...")
    app.run(port=5000)