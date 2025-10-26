let startTime;
let timerInterval;
let currentText = "";

// Start the timer
function startTimer() {
    clearInterval(timerInterval);
    startTime = new Date();
    timerInterval = setInterval(() => {
        const seconds = Math.floor((new Date() - startTime) / 1000);
        document.getElementById("timer").innerText = seconds;
    }, 1000);
}

// Fetch random text from backend
function fetchRandomText() {
    fetch("http://127.0.0.1:5000/random-text")
        .then(res => res.json())
        .then(data => {
            if (!data || data.error || !data.text) {
                console.error("Backend error or no text returned:", data?.error);
                currentText = "The quick brown fox jumps over the lazy dog.";
            } else {
                currentText = data.text;
            }
            startTypingTest();
        })
        .catch(err => {
            console.error("Fetch failed:", err);
            currentText = "The quick brown fox jumps over the lazy dog.";
            startTypingTest();
        });
}

// Initialize typing test with current text
function startTypingTest() {
    document.getElementById("textToType").innerText = currentText;
    document.getElementById("userInput").value = "";
    document.getElementById("wpm").innerText = 0;
    document.getElementById("accuracy").innerText = "";
    
    startTimer();
}

// Listen to user input
document.getElementById("userInput").addEventListener("input", function () {
    const typed = this.value;
    const timeTaken = (new Date() - startTime) / 1000; // in seconds

    // Real-time WPM
    const words = typed.trim().split(/\s+/).filter(Boolean).length;
    const wpm = timeTaken > 0 ? Math.round((words / timeTaken) * 60) : 0;
    document.getElementById("wpm").innerText = wpm;

    // Calculate accuracy
    let correctChars = 0;
    for (let i = 0; i < typed.length; i++) {
        if (typed[i] === currentText[i]) correctChars++;
    }
    const accuracy = typed.length > 0 ? Math.round((correctChars / typed.length) * 100) : 100;
    document.getElementById("accuracy").innerText = `${accuracy}%`;

    // When user finishes typing
    if (typed.trim() === currentText.trim()) {
        clearInterval(timerInterval);
        document.getElementById("accuracy").innerText = "100% - Completed!";

        // Update highest WPM locally
        const highest = parseInt(localStorage.getItem("highestWPM")) || 0;
        if (wpm > highest) localStorage.setItem("highestWPM", wpm);
        document.getElementById("highestWPM").innerText = localStorage.getItem("highestWPM");

        // Automatically start new text after 2 seconds
        setTimeout(fetchRandomText, 2000);
    }
});

// Restart button
document.getElementById("restartBtn").addEventListener("click", fetchRandomText);

// Initialize highest WPM and fetch first text
document.getElementById("highestWPM").innerText = localStorage.getItem("highestWPM") || 0;
fetchRandomText();