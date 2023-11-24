import random

import client
from http.server import BaseHTTPRequestHandler, HTTPServer



def __getShortUrl():
    result = ''
    for i in range(6):
        r = random.random()
        if (r<=0.2):
            result += chr(int(random.random()*1000 % 10 + 48))
        elif (r>0.2 and r<=0.6):
            result += chr(int(random.random()*1000 % 26 + 65))
        else:
            result += chr(int(random.random()*1000 % 26 + 97))

    return result

def getShortUrl(longUrl : str):
    shortUrl = __getShortUrl()
    counter =0 
    print(client.client("HGET hash " + shortUrl))
    while ((not ("is not member" in client.client("HGET hash " + shortUrl))) and
                not (longUrl in client.client("HGET hash " + shortUrl) ) ):
        shortUrl = __getShortUrl()
        counter+=1
        if (counter==100):
            return ''
    
    client.client("HSET hash " + shortUrl +" " + longUrl)

    return shortUrl

class MyRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        print('get')
        strhashmap = (client.client("STALL hash").split(';'))
        if (strhashmap[0] == ''):
            strhashmap = strhashmap[1:]
        if (strhashmap[-1] == ''):
            strhashmap = strhashmap[:-1]
            
        allLinks = {}
        for el in strhashmap:
            shortUrl = el.split(':')[0]
            longUrl = el.split(':')[1]
            allLinks[shortUrl] = longUrl

        print(allLinks)
        print(strhashmap)
        searched=False
        for link in allLinks:
            print(link, longUrl)
            if self.path == '/' + link:
                longUrl = allLinks[link]
                self.send_response(302)
                self.send_header('Location', "http://" + longUrl)
                self.end_headers()
                searched = True
        if (not searched):
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b'404 Not Found')

    def do_POST(self):
        content_length = int(self.headers['Content-Length']) 
        body = self.rfile.read(content_length).decode()
        longUrl = body[4:]
        shortUrl = getShortUrl(longUrl)
        self.send_response(200)
        self.end_headers()
        self.wfile.write(shortUrl.encode())


def run():
    host = '127.0.0.1'
    port = 80
    server = HTTPServer((host, port), MyRequestHandler)
    print(f'Server running on {host}:{port}')
    server.serve_forever()

if __name__ == '__main__':
    run()