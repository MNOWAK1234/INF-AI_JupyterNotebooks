import sqlite3
from datetime import datetime
import RPi.GPIO as GPIO
import time
import adafruit_fingerprint
import serial

uart = serial.Serial("/dev/ttyS0", baudrate=57600, timeout=1)

finger = adafruit_fingerprint.Adafruit_Fingerprint(uart)

DB_NAME = 'workers.db'

workers = {
    1: 'thumb',
    2: 'pointing finger',
    3: 'middle finger',
    4: 'ring finger',
    5: 'pinky finger',
    6: 'Brother\'s thumb'
}

def create_connection():
    return sqlite3.connect(DB_NAME)

def create_table(conn):
    cursor = conn.cursor()
    cursor.execute(f'DROP TABLE IF EXISTS access_logs')
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS access_logs (
            worker_id INTEGER,
            worker_name TEXT,
            timestamp TEXT,
            status TEXT  -- Adding status column to track entry/exit
        )
    ''')
    conn.commit()

def insert_sample_records(conn):
    cursor = conn.cursor()
    sample_data = [
        (1, 'Worker1', str(datetime.now()), 'ENTER'),
        (2, 'Worker2', str(datetime.now()), 'ENTER'),
        (3, 'Worker3', str(datetime.now()), 'ENTER'),
        (4, 'Worker4', str(datetime.now()), 'ENTER'),
        (5, 'Worker5', str(datetime.now()), 'ENTER'),
        (1, 'Worker1', str(datetime.now()), 'LEAVE'),
        (2, 'Worker2', str(datetime.now()), 'LEAVE'),
        (3, 'Worker3', str(datetime.now()), 'LEAVE'),
        (4, 'Worker4', str(datetime.now()), 'LEAVE'),
        (5, 'Worker5', str(datetime.now()), 'LEAVE')
    ]
    cursor.executemany('INSERT INTO access_logs (worker_id, worker_name, timestamp, status) VALUES (?, ?, ?, ?)', sample_data)
    conn.commit()

def open_close_door():
    # Set GPIO numbering mode
    GPIO.setmode(GPIO.BOARD)

    # Set pin 11 as an output, and set servo1 as pin 11 as PWM
    GPIO.setup(11, GPIO.OUT)
    servo1 = GPIO.PWM(11, 50)  # Note 11 is pin, 50 = 50Hz pulse

    # Start PWM running, but with value of 0 (pulse off)
    servo1.start(0)
    time.sleep(2)

    # Rotate 90 degrees
    servo1.ChangeDutyCycle(7)  # Adjust duty cycle for 90 degrees
    time.sleep(1)  # Give Servo time to rotate

    # Wait for 3 seconds for the worker to get in
    time.sleep(3)

    # Turn back to 0 degrees
    servo1.ChangeDutyCycle(2)
    time.sleep(1)  # Give Servo time to rotate
    servo1.ChangeDutyCycle(0) #Assure Servo rotated back to its starting position

    # Clean things up at the end
    servo1.stop()
    GPIO.cleanup()

def get_fingerprint():
    """Get a finger print image, template it, and see if it matches!"""
    print("Waiting for image...")
    while finger.get_image() != adafruit_fingerprint.OK:
        pass
    print("Templating...")
    if finger.image_2_tz(1) != adafruit_fingerprint.OK:
        return False
    print("Searching...")
    if finger.finger_search() != adafruit_fingerprint.OK:
        return False
    return True

def insert_entry(conn):
    cursor = conn.cursor()
    if get_fingerprint():
        print("Detected #", finger.finger_id, "with confidence", finger.confidence)
        # Prepare data
        record = (finger.finger_id, workers[finger.finger_id], str(datetime.now()), 'ENTER')
        # Insert data
        cursor.execute('INSERT INTO access_logs (worker_id, worker_name, timestamp, status) VALUES (?, ?, ?, ?)', record)
        conn.commit()
        # Call the function to open and close the door
        open_close_door()
    else:
        print("Finger not found")

def fetch_all_workers(conn):
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM access_logs')
    rows = cursor.fetchall()
    for row in rows:
        print(row)

def clear_database(conn):
    cursor = conn.cursor()
    cursor.execute('DELETE FROM access_logs')
    conn.commit()
    print("Database cleared successfully!")

if __name__ == "__main__":
    #Connect to database
    while True:
        connection = create_connection()
        #clear_database(connection)
        #create_table(connection)
        #insert_sample_records(connection)
        fetch_all_workers(connection)  # Retrieve and print all workers
        if connection:
            break
    while True:
        insert_entry(connection)
        fetch_all_workers(connection)  # Retrieve and print all workers
    
    connection.close()
