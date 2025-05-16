import sqlite3
from flask import Flask, jsonify
from flask_cors import CORS
from datetime import datetime

app = Flask(__name__)
CORS(app)
DB_NAME = 'workers.db'

# Database connection function
def create_connection():
    return sqlite3.connect(DB_NAME)

@app.route('/get_all_data')
def get_all_data():
    conn = create_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM access_logs ORDER BY timestamp DESC')
    rows = cursor.fetchall()
    conn.close()
    return jsonify(rows)

@app.route('/get_worker_data/<username>')
def get_worker_data(username):
    conn = create_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM access_logs WHERE worker_name = ?', (username,))
    rows = cursor.fetchall()
    conn.close()
    return jsonify(rows)

@app.route('/get_data_by_timestamp/<start_time>/<end_time>')
def get_data_by_timestamp(start_time, end_time):
    if end_time == '':  # Check if end_time is empty
        end_time = str(datetime.now())  # Replace with current timestamp if empty
    
    conn = create_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM access_logs WHERE timestamp >= ? AND timestamp <= ?', (start_time, end_time))
    data_within_range = cursor.fetchall()
    conn.close()

    return jsonify(data_within_range)

@app.route('/get_data_by_worker_and_timestamps/<worker_name>/<start_time>/<end_time>')
def get_data_by_worker_and_timestamps(worker_name, start_time, end_time):
    if end_time == '':  # Check if end_time is empty
        end_time = str(datetime.now())  # Replace with current timestamp if empty
    
    conn = create_connection()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM access_logs WHERE worker_name = ? AND timestamp >= ? AND timestamp <= ?', (worker_name, start_time, end_time))
    data_within_range = cursor.fetchall()
    conn.close()

    return jsonify(data_within_range)

@app.route('/get_workers_at_work/<datestamp>')
def get_workers_at_work(datestamp):
    conn = create_connection()
    cursor = conn.cursor()

    # Extract the date from the datestamp (assuming datestamp is in 'YYYY-MM-DD' format)
    date = datestamp.split()[0] if ' ' in datestamp else datestamp
    # Retrieve workers at work on the specified date (ignoring the time)
    cursor.execute("SELECT DISTINCT worker_name FROM access_logs WHERE SUBSTR(timestamp, 1, 10) LIKE ?", (date,))
    
    workers_at_work = cursor.fetchall()

    conn.close()

    return jsonify(workers_at_work)

@app.route('/count_appearances_by_timestamp/<start_time>/<end_time>')
def count_appearances_by_timestamp(start_time, end_time):
    if end_time == '':  # Check if end_time is empty
        end_time = str(datetime.now())  # Replace with current timestamp if empty
    
    conn = create_connection()
    cursor = conn.cursor()
    
    cursor.execute('''
        SELECT worker_name, COUNT(*) as appearance_count 
        FROM access_logs 
        WHERE timestamp >= ? AND timestamp <= ? AND status = 'ENTER'
        GROUP BY worker_name
        ORDER BY appearance_count DESC
    ''', (start_time, end_time))
    
    worker_appearances = cursor.fetchall()
    conn.close()

    return jsonify(worker_appearances)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
