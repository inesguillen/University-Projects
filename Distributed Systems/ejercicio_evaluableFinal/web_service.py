from flask import Flask, jsonify
from datetime import datetime

app = Flask(__name__)

@app.route('/get_time', methods=['GET'])
def get_time():
    now = datetime.now()
    formatted_time = now.strftime("%d/%m/%Y %H:%M:%S")
    return jsonify({'timestamp': formatted_time})

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5000)
