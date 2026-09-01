#!/usr/bin/env python3
import http.server
import socketserver
import os
import webbrowser
import sys

PORT = 8080

class Handler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Cache-Control', 'no-cache, no-store, must-revalidate')
        self.send_header('Pragma', 'no-cache')
        self.send_header('Expires', '0')
        super().end_headers()

if __name__ == '__main__':
    # Change to root directory of project so both web/ and data/ are accessible
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    os.chdir(project_root)
    
    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        url = f"http://localhost:{PORT}/web/index.html"
        print(f"==================================================")
        print(f" Beirut 8-Bit Transit Map Visualizer")
        print(f" Serving at: {url}")
        print(f" Press Ctrl+C to stop.")
        print(f"==================================================")
        if len(sys.argv) > 1 and sys.argv[1] == '--open':
            webbrowser.open(url)
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped.")
