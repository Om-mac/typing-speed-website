g++ main.cpp -o main.cgi
./main.cgi 
cd ~/Desktop/typing-speed-website
python3 server.py        
cd ~/Desktop/typing-speed-website/frontend
python3 -m http.server 8000
http://localhost:8000